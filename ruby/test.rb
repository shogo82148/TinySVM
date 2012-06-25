#!/usr/bin/ruby

require 'TinySVM'

ex = TinySVM::Example.new()
ex.read("../tests/train.svmdata")
model = ex.learn()

print "Trnum=", model.getTrainingDataSize(), "\n";
print "SV=", model.getSVnum(), "\n"
print "BSV=", model.getBSVnum(), "\n"
print "VC=", model.estimateVC(), "\n"
print "Margin=", model.estimateMargin(), "\n"
print "Sphere=", model.estimateSphere(), "\n"
print "Loss=", model.getLoss(), "\n"

i = 0
while i < 1000 do
  print "Y[",i,"]=", model.getY(i), " X[",i,"]=", model.getX(i),"\n"
  i = i + 1
end

print "\n";
model.remove(0)
i = 0
while i < 1000 do
  print "Y[",i,"]=", model.getY(i), " X[",i,"]=", model.getX(i),"\n"
  i = i + 1
end

model.write("model")

print model.classify("1:1 2:1 5:1 100:1") , "\n"
print model.classify("10:1 20:1 50:1 100:1") , "\n"

model2 = TinySVM::Model.new()
model2.read("model")
print model2.classify("1:1 2:1 5:1 100:1") , "\n"
print model2.classify("10:1 20:1 50:1 100:1") , "\n"

model3 = ex.learn()
model3.compress()
model3.write("model3")





