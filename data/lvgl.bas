x = GetMainLvObj()
print "Getting main Lv obj ", x
b = LvButtonCreate(x, 8, 16, 192, 32)
print "Button created", b
l = LvButtonCreate(b, 8, 8, 180, 16)
print "Label created", l
LvLabelSetText(l, "This is a button")
delay(3000)
PRINT "Fine"