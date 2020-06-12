#include "clib/stl_inc.h"

#include <stddef.h>
#include <ctype.h>
#include <wctype.h>

#include "clib/endian.h"
#include "clib/clib.h"
#include "clib/logfile.h"
#include "clib/random.h"
#include "clib/strutil.h"

#include "account.h"
#include "client.h"
#include "listenpt.h"
#include "mkscrobj.h"
#include "msghandl.h"
#include "npc.h"
#include "polcfg.h"
#include "sockio.h"
#include "syshook.h"
#include "textcmd.h"
#include "tildecmd.h"
#include "uvars.h"
#include "umsgtype.h"
#include "ufunc.h"
#include "ufuncstd.h"

#include "nmsgtype.h"

#include "clib/fdump.h"

static char buffer[1024];

// ASCII-ONLY VERSIONS
void pc_spoke( NPC& npc, Character *chr, const char *text, int textlen, u8 texttype ) //DAVE
{
    npc.on_pc_spoke( chr, text, texttype );
}
void ghost_pc_spoke( NPC& npc, Character* chr, const char* text, int textlen, u8 texttype ) //DAVE
{
    npc.on_ghost_pc_spoke( chr, text, texttype );
}

// UNICODE VERSIONS
void pc_spoke( NPC& npc, Character *chr, const char *text, int textlen, u8 texttype, const u16 *wtext, const char lang[4], int wtextlen )
{
	npc.on_pc_spoke( chr, text, texttype, wtext, lang );
}
void ghost_pc_spoke( NPC& npc, Character* chr, const char *text, int textlen, u8 texttype, const u16 *wtext, const char lang[4], int wtextlen )
{
	npc.on_ghost_pc_spoke( chr, text, texttype, wtext, lang );
}

void handle_processed_speech( Client* client,
                              char* textbuf,
                              int textbuflen,
                              char firstchar,
                              MSGXX_TEXTDEF* textdef_ce )
{
	// ENHANCE: if (intextlen+1) != textbuflen, then the input line was 'dirty'.  May want to log this fact.
			
	if (textbuflen == 1)	// WTF?  Ended up with an empty string.
		return;

	// printf( "Spoken text: %d bytes: '%s'\n", textbuflen, textbuf );

	if (textbuf[0] == '.' || textbuf[0] == '=')
	{
		if (!process_command( client, textbuf ))
            send_sysmessage( client, string("Unknown command: ") + textbuf );
        return;
	}

    if (firstchar == '~') // we strip tildes out
    {
        process_tildecommand( client, textbuf );
        return;
    }
    
    if (client->chr->squelched())
        return;

    if (client->chr->hidden())
        client->chr->unhide();

    if (config.show_speech_colors)
    {
        cout << client->chr->name() << " speaking w/ color " 
             << hexint( cfBEu16(textdef_ce->color) ) << endl;
    }

	// validate text color
	u16 textcol = cfBEu16(textdef_ce->color);
	if ( textcol < 2 || textcol > 1001 )
	{
		sprintf(buffer, "Client #%lu (account %s) speaking with out-of-range color 0x%x",
						client->instance_,
						(client->acct != NULL) ? client->acct->name() : "unknown",
						textcol);
		cerr << buffer << endl;
		Log("%s\n", buffer);
		return;
	}

    // send to self
	static MSG1C_TEXT talkmsg;
    static MSG1C_TEXT ghostmsg;
	unsigned short msglen = offsetof( MSG1C_TEXT, text ) + textbuflen;
	talkmsg.msgtype         = MSGOUT_1C_TEXT;
	talkmsg.msglen          = ctBEu16( msglen );
	talkmsg.source_serial   = client->chr->serial_ext;
	talkmsg.source_graphic  = client->chr->graphic_ext;
	talkmsg.textdef.type    = textdef_ce->type; // FIXME authorize
	talkmsg.textdef.color   = textdef_ce->color;
	talkmsg.textdef.font    = textdef_ce->font;
	strzcpy( talkmsg.speaker_name, client->chr->name().c_str(), sizeof talkmsg.speaker_name );
	memcpy( talkmsg.text, textbuf, textbuflen );
	transmit( client, &talkmsg, msglen );

	if (client->chr->dead())
    {
        memcpy( &ghostmsg, &talkmsg, sizeof ghostmsg );
        char* t = ghostmsg.text;
        while (*t)
        {
            if (!isspace(*t))
            {
                if (random_int( 4 ) == 0)
                    *t = 'o';
                else
                    *t = 'O';
            }
            ++t;
        }
    }
	// send to those nearby
	for( unsigned cli = 0; cli < clients.size(); cli++ )
	{
		Client *client2 = clients[cli];
		if (!client2->ready) continue;
		if (client == client2) continue;
        if (!client2->chr->is_visible_to_me( client->chr ))
            continue;
		
        bool rangeok;
        if (textdef_ce->type == TEXTTYPE_WHISPER)
            rangeok = in_whisper_range(  client->chr, client2->chr ); //DAVE changed from hardcoded "2"
        else if (textdef_ce->type == TEXTTYPE_YELL)
            rangeok = in_yell_range( client->chr, client2->chr ); //DAVE changed from hardcoded "25"
        else
            rangeok = in_say_range( client->chr, client2->chr ); //DAVE changed from "visual" range check, should be "say" range check.
        if (rangeok)
        {
            if (!client->chr->dead() || 
                 client2->chr->dead() || 
                 client2->chr->can_hearghosts()  )
            {
    			transmit( client2, &talkmsg, msglen );
            }
            else
            {
                transmit( client2, &ghostmsg, msglen );
            }
        }
	}

    if (!client->chr->dead())
        for_nearby_npcs( pc_spoke, client->chr, textbuf, textbuflen, textdef_ce->type );
    else
        for_nearby_npcs( ghost_pc_spoke, client->chr, textbuf, textbuflen, textdef_ce->type );
    
    sayto_listening_points( client->chr, textbuf, textbuflen, textdef_ce->type );
}

                              

void SpeechHandler( Client *client, MSG03_SPEECH *msg03 )
{
	int i;
	int intextlen;
	
	char textbuf[ SPEECH_MAX_LEN+1 ];
	int textbuflen;

/*	printf( "PC Speech.  texttype=0x%02.02x, color=0x%04.04, unk6=0x%x, font=0x%x\n",
		      msg03->texttype,
			  cfBEu16(msg03->color),
			  msg03->unk6,
			  msg03->font );
*/	
	intextlen = cfBEu16(msg03->msglen) - offsetof( MSG03_SPEECH, text ) - 1;
//	printf( "incoming intextlen: %d\n", intextlen );
	//	intextlen does not include the null terminator.

	// Preprocess the text into a sanity-checked, printable, null-terminated form in textbuf
	if (intextlen < 0)
		intextlen = 0;
	if (intextlen > SPEECH_MAX_LEN) 
		intextlen = SPEECH_MAX_LEN;	// ENHANCE: May want to log this

	for( i = 0, textbuflen = 0; i < intextlen; i++ )
	{
		char ch = msg03->text[i];

		if (ch == 0) break;		// quit early on embedded nulls
		if (ch == '~') continue;	// skip unprintable tildes.  Probably not a reportable offense.

		if (isprint( ch ))
			textbuf[ textbuflen++ ] = ch;
		// ENHANCE: else report client data error? Just log? 
	}
	textbuf[ textbuflen++ ] = 0;

    handle_processed_speech( client, 
                             textbuf, textbuflen,
                             msg03->text[0],
                             &msg03->textdef );
}

MESSAGE_HANDLER_VARLEN( MSG03_SPEECH, SpeechHandler );

void SendUnicodeSpeech(Client *client, MSGAD_UNICODE_SPEECH *msgin, u16* wtext, size_t wtextlen, char* ntext, size_t ntextlen)
{
	using std::wstring;

	if (wtext[0] == ctBEu16(L'.') || wtext[0] == ctBEu16(L'='))
	{
		if (!process_command( client, ntext, wtext, msgin->lang ))
		{
			wstring wtmp(L"Unknown command: ");
			// Needs to be done char-by-char due to linux's 4-byte unicode!
			// Dumbass platform differences!! >_<
			for(size_t i = 0; i < wtextlen; i++)
			wtmp += static_cast<wchar_t>(cfBEu16(wtext[i]));

			// Used to be done like this...
	//		wtmp += wstring(static_cast<wchar_t*>(wtext));
	//		for(size_t i = 17; i < wtmp.length(); i++)
	//			wtmp[i] = cfBEu16(wtmp[i]); // need to convert the chars back >_<

			send_sysmessage( client, wtmp, msgin->lang );
		}
        return;
	}

    if (cfBEu16(msgin->wtext[0]) == L'~') // we strip tildes out
    {
        process_tildecommand( client, wtext );
        return;
    }

	if (client->chr->squelched())
		return;

    if (client->chr->hidden())
        client->chr->unhide();

    if (config.show_speech_colors)
    {
        cout << client->chr->name() << " speaking w/ color " 
             << hexint( cfBEu16(msgin->textdef.color) ) << endl;
    }

	// validate text color
	u16 textcol = cfBEu16(msgin->textdef.color);
	if ( textcol < 2 || textcol > 1001 )
	{
		sprintf(buffer, "Client #%lu (account %s) speaking with out-of-range color 0x%x",
						client->instance_,
						(client->acct != NULL) ? client->acct->name() : "unknown",
						textcol);
		cerr << buffer << endl;
		Log("%s\n", buffer);
		return;
	}

	MSGAE_UNICODE_SPEECH_TO_CLIENT ghostmsg;
	MSGAE_UNICODE_SPEECH_TO_CLIENT talkmsg;
	unsigned short msglen = offsetof( MSGAE_UNICODE_SPEECH_TO_CLIENT, wtext ) + wtextlen*sizeof(talkmsg.wtext[0]);
	talkmsg.msgtype         = MSGAE_UNICODE_SPEECH_TO_CLIENT_ID;
	talkmsg.msglen          = ctBEu16( msglen );
	talkmsg.source_serial   = client->chr->serial_ext;
	talkmsg.source_graphic  = client->chr->graphic_ext;
	talkmsg.textdef.type    = msgin->textdef.type; // FIXME authorize
	talkmsg.textdef.color   = msgin->textdef.color;
	talkmsg.textdef.font    = msgin->textdef.font;
	memcpy( talkmsg.lang, msgin->lang, sizeof talkmsg.lang );
	strzcpy( talkmsg.speaker_name, client->chr->name().c_str(), sizeof talkmsg.speaker_name );
	memcpy( talkmsg.wtext, wtext, wtextlen*sizeof(talkmsg.wtext[0]) );

	transmit( client, &talkmsg, msglen ); // self

	if (client->chr->dead())
	{
		unsigned short msglen = offsetof( MSGAE_UNICODE_SPEECH_TO_CLIENT, wtext ) + wtextlen*sizeof(ghostmsg.wtext[0]);
		ghostmsg.msgtype         = MSGAE_UNICODE_SPEECH_TO_CLIENT_ID;
		ghostmsg.msglen          = ctBEu16( msglen );
		ghostmsg.source_serial   = client->chr->serial_ext;
		ghostmsg.source_graphic  = client->chr->graphic_ext;
		ghostmsg.textdef.type    = msgin->textdef.type; // FIXME authorize
		ghostmsg.textdef.color   = msgin->textdef.color;
		ghostmsg.textdef.font    = msgin->textdef.font;
		memcpy( ghostmsg.lang, msgin->lang, sizeof talkmsg.lang );
		strzcpy( ghostmsg.speaker_name, client->chr->name().c_str(), sizeof ghostmsg.speaker_name );
		memcpy( ghostmsg.wtext, wtext, wtextlen*sizeof(ghostmsg.wtext[0]) );

		u16* pwc = ghostmsg.wtext;
		while (*pwc != 0)
		{
            wchar_t wch = (*pwc);
            if (!iswspace(wch))
			{
                if (random_int( 4 ) == 0)
				{
                    *pwc = ctBEu16(L'o');
				}
				else
				{
                    *pwc = ctBEu16(L'O');
				}
			}
			++pwc;
		}
	}
		// send to those nearby
	for( unsigned cli = 0; cli < clients.size(); cli++ )
	{
		Client *client2 = clients[cli];
		if (!client2->ready) continue;
		if (client == client2) continue;
		if (!client2->chr->is_visible_to_me( client->chr )) 
			continue;

        bool rangeok;
        if (msgin->textdef.type == TEXTTYPE_WHISPER)
            rangeok = in_whisper_range(  client->chr, client2->chr ); //DAVE changed from hardcoded "2"
        else if (msgin->textdef.type == TEXTTYPE_YELL)
            rangeok = in_yell_range( client->chr, client2->chr ); //DAVE changed from hardcoded "25"
        else
            rangeok = in_say_range( client->chr, client2->chr ); //DAVE changed from "visual" range check, should be "say" range check.
        if (rangeok)
		{
            if (!client->chr->dead() || 
				client2->chr->dead() || 
				client2->chr->can_hearghosts()  )
			{
    			transmit( client2, &talkmsg, msglen );
			}
			else
			{
                transmit( client2, &ghostmsg, msglen );
			}
		}
	}

    if (!client->chr->dead())
		for_nearby_npcs( pc_spoke, client->chr, ntext, ntextlen, msgin->textdef.type,
						 wtext, msgin->lang, wtextlen);
	else
		for_nearby_npcs( ghost_pc_spoke, client->chr, ntext, ntextlen, msgin->textdef.type,
						 wtext, msgin->lang, wtextlen);

	sayto_listening_points( client->chr, ntext, ntextlen, msgin->textdef.type,
							wtext, msgin->lang, wtextlen);
}
u16 Get12BitNumber(u8 * thearray, u16 theindex){	u16 theresult = 0;	int thenibble = theindex * 3;	int thebyte = thenibble / 2;	if (thenibble % 2)		theresult = cfBEu16(*((u16 *) (thearray + thebyte))) & 0x0FFF;	else		theresult = cfBEu16(*((u16 *) (thearray + thebyte))) >> 4;  	return theresult;}

u16 GetNextUTF8(u8 * bytemsg, int i,u16& unicodeChar)
{	u16 result = 0;

	if ((bytemsg[i] & 0x80) == 0)
	{
		unicodeChar = bytemsg[i];
		return i+1;
	}

	if ((bytemsg[i] & 0xE0) == 0xC0)
	{
		// two byte sequence :
		if ((bytemsg[i+1] & 0xC0) == 0x80)
		{
			result = ((bytemsg[i] & 0x1F) << 6) | (bytemsg[i+1] & 0x3F);
			unicodeChar = result;
			return i+2;
		}
	}
	else if ((bytemsg[i] & 0xF0) == 0xE0)
	{
		// three byte sequence
		if (((bytemsg[i+1] & 0xC0) == 0x80) &&
			((bytemsg[i+2] & 0xC0) == 0x80)
		   )
		{
			result = ((bytemsg[i] & 0x0F) << 12) | ((bytemsg[i+1] & 0x3F) < 6) | (bytemsg[i+2] & 0x3F);
			unicodeChar = result;
			return i+3;
		}
	}
	
	// An error occurred in the sequence(or sequence > 16 bits) :
	unicodeChar = 0x20;  // Set unicode char to a "space" character instead"
	return i+1;
}

void UnicodeSpeechHandler( Client *client, MSGAD_UNICODE_SPEECH *msgin )
{
	using std::wcout; // wcout.narrow() function r0x! :-)

	int intextlen;
	int numtokens = 0;
	u16 * themsg = msgin->wtext;
	u8 *  bytemsg = ((u8 *) themsg);
	int bytemsglen;
	int wtextoffset = 0;
	ObjArray* speechtokens = NULL;
    BLong * atoken = NULL;
	int i,j;
	u16 unicodeChar;

	u16 tempbuf[ SPEECH_MAX_LEN+1 ];

    u16 wtextbuf[ SPEECH_MAX_LEN+1 ];
    size_t wtextbuflen;

	char ntextbuf[SPEECH_MAX_LEN+1];
	size_t ntextbuflen;

	if (msgin->textdef.type & 0xc0)
	{
		numtokens = Get12BitNumber((u8 *) (msgin->wtext), 0);
		wtextoffset = ((((numtokens+1)*3)/2) + ((numtokens+1) % 2));		bytemsg = (((u8*) themsg) + wtextoffset);
		bytemsglen = cfBEu16(msgin->msglen) - wtextoffset - offsetof( MSGAD_UNICODE_SPEECH, wtext ) - 1;
        intextlen = 0;

		if(system_hooks.speechmul_hook != NULL)
		{
			for (int i = 0; i < numtokens; i++)
			{
				if (speechtokens == NULL)
					speechtokens = new ObjArray();
				atoken = new BLong(Get12BitNumber((u8 *) (msgin->wtext), i+1));
				speechtokens->addElement(atoken);
			}
			system_hooks.speechmul_hook->call( make_mobileref(client->chr), speechtokens );
		}


		i = 0;
		j = 0;
		while ((i < bytemsglen) && (i < SPEECH_MAX_LEN))
		{
			i = GetNextUTF8(bytemsg, i, unicodeChar);
			tempbuf[j++] = cfBEu16(unicodeChar);
			intextlen++;
		}

		themsg = tempbuf;		msgin->textdef.type &= (~0xC0);  // Client won't accept C0 text type messages, so must set to 0
	}
	else
		intextlen = (cfBEu16(msgin->msglen) - offsetof( MSGAD_UNICODE_SPEECH, wtext ))
					/ sizeof(msgin->wtext[0]) - 1;
	//	intextlen does not include the null terminator.

	// Preprocess the text into a sanity-checked, printable, null-terminated form in textbuf
	if (intextlen < 0)
		intextlen = 0;
	if (intextlen > SPEECH_MAX_LEN) 
		intextlen = SPEECH_MAX_LEN;	// ENHANCE: May want to log this

//	printf( "incoming intextlen: %d\n", intextlen );

	// Preprocess the text into a sanity-checked, printable, null-terminated form
	// in 'wtextbuf' and 'ntextbuf'
	ntextbuflen = 0;
	wtextbuflen = 0;
    for( i = 0; i < intextlen; i++ )
    {
		u16 wc = cfBEu16(themsg[i]);
		if (wc == 0) break;		// quit early on embedded nulls
		if (wc == L'~') continue;	// skip unprintable tildes. 
		wtextbuf[ wtextbuflen++ ] = ctBEu16(wc);
		ntextbuf[ ntextbuflen++ ] = wcout.narrow((wchar_t)wc, '?');
	}
	wtextbuf[ wtextbuflen++ ] = (u16)0;
	ntextbuf[ ntextbuflen++ ] = 0;


    //check for valid command char
	// wtextbuf is used because typically 16-bit unicode values won't decode as
    // 1-byte, so they get stripped off in ntextbuf.
    // this may mean textcommands won't be handled in those alphabets.

/*
    if((ntextbuf[0] == '=') || (ntextbuf[0] == '.') || (ntextbuf[0] == '~'))
	{
		// process as single byte string
		handle_processed_speech( client, 
                             ntextbuf, ntextbuflen,
                             ntextbuf[0],
                             &msgin->textdef,
							 wtextbuf, wtextbuflen);
		return;
    }
*/
	SendUnicodeSpeech(client, msgin, wtextbuf, wtextbuflen, ntextbuf, ntextbuflen);
}

MESSAGE_HANDLER_VARLEN( MSGAD_UNICODE_SPEECH, UnicodeSpeechHandler );
