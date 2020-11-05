PRINT "Hello world !"
FOR i=1 to 3
  PRINT i
NEXT i
f = fopen("/spiffs/file.txt", "w+")
  fwrite_line(f, "hello world!") ' Write a line
fclose(f)
PRINT "Yes I'm working!"
t = GetTime()
b = GetBatteryPct()
f = fopen("/spiffs/file.txt", "w+")
fwrite_line(f, "Hello world")
fwrite_line(f,t) 'Write the time
fwrite_line(f, "Battery: ", b)
fwrite_line(f, "Ciaone!")
fclose(f)
PRINT "Now is ", t
PRINT "Battery: ", b
PRINT "Ciao!"