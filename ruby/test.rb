#!/usr/bin/ruby

require 'TinySVM'

ex = Example.new("../tests/train.svmdata")
#model = ex.learn("-t 1 -d 2 -c 1")
model = ex.learn()
print "Trnum=", model.getTrainingDataSize(), "\n";
print "SV=", model.getSVnum(), "\n"
print "BSV=", model.getBSVnum(), "\n"
print "VC=", model.estimateVC(), "\n"
print "Margin=", model.estimateMargin(), "\n"
print "Sphere=", model.estimateSphere(), "\n"
print "Loss=", model.getLoss(), "\n"

i = 0
while i < 10 do
  print "Y[",i,"]=", model.getY(i), " X[",i,"]=", model.getX(i).join(" "),"\n"
  i = i + 1
end

model.write("model")

print model.classify("1:1 2:1 5:1 100:1") , "\n"
print model.classify("10:1 20:1 50:1 100:1") , "\n"

model2 = Model.new("model")
print model2.classify("1:1 2:1 5:1 100:1") , "\n"
print model2.classify("10:1 20:1 50:1 100:1") , "\n"



