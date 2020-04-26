'****************************************************************
'* spawn file renumbering utility
'* use when multiple people have been spawning, and they have overwritten eachother's entries
'* 
'****************************************************************

Const ForReading = 1, ForWriting = 2, ForAppending = 3


dim fso1, f1, ts, ts2 , outfile
Set fso1 = CreateObject("Scripting.FileSystemObject")
Set outfile = fso1.createtextfile("tempspawner.txt")
set f1 = fso1.getfile("spawns.cfg")
set ts = f1.openastextstream(ForReading)


While Not ts.atendofstream
        fileline = ts.readline
        if instr(lcase(fileline), "spawn") > 0 then
                        num_spawns = num_spawns + 1
                        fileline = "Spawn " + cstr(num_spawns)
        end if
        
outfile.writeline fileline

Wend

set outfile = nothing
set f1 = nothing
set ts = nothing
set ts2= nothing

fso1.copyfile "tempspawner.txt", "spawns.cfg"


'Sub Read_Files()
 '   Dim fso As New FileSystemObject, txtfile, _
  '    fil1 As File, ts As TextStream
   ' fso.CreateTextFile "c:\testfile.txt", True
   ' MsgBox "Writing file"
   ' ' Write a line.
   ' Set fil1 = fso.GetFile("c:\testfile.txt")
   ' Set ts = fil1.OpenAsTextStream(ForWriting)
  '  ts.Write "Hello World"
  '  ts.Close
    ' Read the contents of the file.
    