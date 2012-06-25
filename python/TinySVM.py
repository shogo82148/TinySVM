# This file was created automatically by SWIG.
import TinySVMc
class BaseExample:
    def __init__(self,this):
        self.this = this

    def add(*args):
        val = apply(TinySVMc.BaseExample_add,args)
        return val
    def set(*args):
        val = apply(TinySVMc.BaseExample_set,args)
        return val
    def get(*args):
        val = apply(TinySVMc.BaseExample_get,args)
        return val
    def remove(*args):
        val = apply(TinySVMc.BaseExample_remove,args)
        return val
    def clear(*args):
        val = apply(TinySVMc.BaseExample_clear,args)
        return val
    def size(*args):
        val = apply(TinySVMc.BaseExample_size,args)
        return val
    def read(*args):
        val = apply(TinySVMc.BaseExample_read,args)
        return val
    def write(*args):
        val = apply(TinySVMc.BaseExample_write,args)
        return val
    def readSVindex(*args):
        val = apply(TinySVMc.BaseExample_readSVindex,args)
        return val
    def writeSVindex(*args):
        val = apply(TinySVMc.BaseExample_writeSVindex,args)
        return val
    def __del__(self,TinySVMc=TinySVMc):
        if getattr(self,'thisown',0):
            TinySVMc.delete_BaseExample(self)
    def append(*args):
        val = apply(TinySVMc.BaseExample_append,args)
        return val
    def appendSVindex(*args):
        val = apply(TinySVMc.BaseExample_appendSVindex,args)
        return val
    def getDimension(*args):
        val = apply(TinySVMc.BaseExample_getDimension,args)
        return val
    def getNonzeroDimension(*args):
        val = apply(TinySVMc.BaseExample_getNonzeroDimension,args)
        return val
    def getY(*args):
        val = apply(TinySVMc.BaseExample_getY,args)
        return val
    def getX(*args):
        val = apply(TinySVMc.BaseExample_getX,args)
        return val
    def getAlpha(*args):
        val = apply(TinySVMc.BaseExample_getAlpha,args)
        return val
    def getGradient(*args):
        val = apply(TinySVMc.BaseExample_getGradient,args)
        return val
    def getG(*args):
        val = apply(TinySVMc.BaseExample_getG,args)
        return val
    def __repr__(self):
        return "<C BaseExample instance at %s>" % (self.this,)
class BaseExamplePtr(BaseExample):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = BaseExample



class Model(BaseExample):
    def __init__(self,*args):
        self.this = apply(TinySVMc.new_Model,args)
        self.thisown = 1

    def read(*args):
        val = apply(TinySVMc.Model_read,args)
        return val
    def write(*args):
        val = apply(TinySVMc.Model_write,args)
        return val
    def clear(*args):
        val = apply(TinySVMc.Model_clear,args)
        return val
    def classify(*args):
        val = apply(TinySVMc.Model_classify,args)
        return val
    def estimateMargin(*args):
        val = apply(TinySVMc.Model_estimateMargin,args)
        return val
    def estimateSphere(*args):
        val = apply(TinySVMc.Model_estimateSphere,args)
        return val
    def estimateVC(*args):
        val = apply(TinySVMc.Model_estimateVC,args)
        return val
    def estimateXA(*args):
        val = apply(TinySVMc.Model_estimateXA,args)
        return val
    def compress(*args):
        val = apply(TinySVMc.Model_compress,args)
        return val
    def getSVnum(*args):
        val = apply(TinySVMc.Model_getSVnum,args)
        return val
    def getBSVnum(*args):
        val = apply(TinySVMc.Model_getBSVnum,args)
        return val
    def getTrainingDataSize(*args):
        val = apply(TinySVMc.Model_getTrainingDataSize,args)
        return val
    def getLoss(*args):
        val = apply(TinySVMc.Model_getLoss,args)
        return val
    def __del__(self,TinySVMc=TinySVMc):
        if getattr(self,'thisown',0):
            TinySVMc.delete_Model(self)
    def __repr__(self):
        return "<C Model instance at %s>" % (self.this,)
class ModelPtr(Model):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = Model



class Example(BaseExample):
    def __init__(self,*args):
        self.this = apply(TinySVMc.new_Example,args)
        self.thisown = 1

    def read(*args):
        val = apply(TinySVMc.Example_read,args)
        return val
    def write(*args):
        val = apply(TinySVMc.Example_write,args)
        return val
    def rebuildSVindex(*args):
        val = apply(TinySVMc.Example_rebuildSVindex,args)
        return val
    def learn(*args):
        val = apply(TinySVMc.Example_learn,args)
        if val: val = ModelPtr(val) 
        return val
    def __del__(self,TinySVMc=TinySVMc):
        if getattr(self,'thisown',0):
            TinySVMc.delete_Example(self)
    def __repr__(self):
        return "<C Example instance at %s>" % (self.this,)
class ExamplePtr(Example):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
        self.__class__ = Example





#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

