#ifndef MSGHANDL_H
#define MSGHANDL_H

class Client;
class MessageHandler
{
public:
	MessageHandler( unsigned char msgtype,
					int msglen,
					void (*func)(Client *client, void *msg) );
};
#define MSGLEN_2BYTELEN_DATA -2

#define MESSAGE_HANDLER( type, func ) \
	void (*f_dummy_##type)(Client *, type *) = func; \
	MessageHandler type##_handler( type##_ID, sizeof(type), reinterpret_cast<void (*)(Client *, void *)>(func))

#define MESSAGE_HANDLER_VARLEN( type, func ) \
	void (*f_dummy_##type)(Client *, type *) = func; \
	MessageHandler type##_handler( type##_ID, MSGLEN_2BYTELEN_DATA, (void (*)(Client *, void *)) func)


#endif
