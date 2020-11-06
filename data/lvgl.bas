CrLf = chr(13) + chr(10)
print "Hello basic!", CrLf
x = GetMainLvObj()
print "Get main lv obj ", x, CrLf
b = LvButtonCreate(x, 32, 64, 160, 48)
print "button 1 ", b, CrLf
b1 = LvButtonCreate(x, 32, 128, 160, 48)
print "button 2 ", b1, CrLf
l = LvLabelCreate(b, 8, 8, 128, 32)
print "Create a label into button 1", l, crlf
l1 = LvLabelCreate(x, 32, 192, 128, 32)
print "Create a label into main lv obj", l1, CrLf
LvLabelSetText(l, "Button 1 caption")
LvLabelSetText(l1, "Label 2 text")
