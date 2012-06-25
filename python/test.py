# $Id: test.py,v 1.2 2001/08/29 14:56:32 taku-ku Exp $;

import TinySVM

ex = TinySVM.Example();

ex.read("../tests/train.svmdata")
model = ex.learn()

print "Trnum=", model.getTrainingDataSize()
print "SV=", model.getSVnum()
print "BSV=", model.getBSVnum()
print "VC=", model.estimateVC()
print "Margin=", model.estimateMargin()
print "Sphere=", model.estimateSphere()
print "Loss=", model.getLoss()

for i in (range(5)):
    print "Y[",i,"]=", model.getY(i), " X[",i,"]=", model.getX(i)

print "\n";
model.remove(0)

for i in (range(5)):
    print "Y[",i,"]=", model.getY(i), " X[",i,"]=", model.getX(i)


model.write("model")
print model.classify("1:1 2:1 5:1 100:1")
print model.classify("10:1 20:1 50:1 100:1") 

model2 = TinySVM.Model()
model2.read("model")
print model2.classify("1:1 2:1 5:1 100:1")
print model2.classify("10:1 20:1 50:1 100:1")

model3 = ex.learn()
model3.compress()
model3.write("model3")







