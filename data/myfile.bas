CrLf = chr(13) + chr(10)
LV_EVENT_CLICKED = 6
GlobalVal = 100
print "Hello basic!", CrLf
x = GetMainLvObj()
print "Get main lv obj ", x, CrLf
b1 = LvButtonCreate(x, 32, 64, 160, 48)
print "button 1 ", b1, CrLf
b2 = LvButtonCreate(x, 32, 128, 160, 48)
print "button 2 ", b2, CrLf
lx = LvLabelCreate(x, 32, 192, 128, 32)
print "Create a label into main lv obj", lx, CrLf
lb1 = LvLabelCreate(b1, 8, 8, 128, 32)
print "Create a label into button 1", lb1, crlf
lb2 = LvLabelCreate(b2, 8, 8, 128, 32)
print "Create a label into button 2", lb2, crlf
LvLabelSetText(lx, "Current value:" + str(GlobalVal) )
LvLabelSetText(lb1, "Click to increase")
LvLabelSetText(lb2, "Click to decrease")
def UpdateLabel()
     t = "New value: " + str(GlobalVal)
     LvLabelSetText(lx, t )
enddef
def OnButton1(evt)
  if evt=LV_EVENT_CLICKED then
     GlobalVal = GlobalVal + 10
     UpdateLabel()
  endif
enddef
def OnButton2(evt)
  if evt=LV_EVENT_CLICKED then
     GlobalVal = GlobalVal - 10
     UpdateLabel()
  endif
enddef
SetLvEventHandler(b1, "OnButton1")
SetLvEventHandler(b2, "OnButton2")
t = "Multitasking: "
for i=1 to 10
  GlobalVal = GlobalVal +1
  t = "i = " + str(i) + " GlobalVal = " + str(GlobalVal)
  print t, CrLf
  UpdateLabel()
  delay(2000)
next