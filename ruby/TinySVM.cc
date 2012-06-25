#include "ruby.h"
#include "tinysvm.h"

/////////////////////////////////////////////////
//
//  Ruby interface for TinySVM
//
//  $Id: TinySVM.cc,v 1.5 2001/01/18 08:50:59 taku-ku Exp $;
//

#ifdef __cplusplus

namespace TinySVM {
		
extern "C" {
  static VALUE ModelClass;
  static VALUE TinySVM_Model_new(int, VALUE *, VALUE);
  static void  TinySVM_Model_destroy(Model *);
  static VALUE TinySVM_Model_read(int, VALUE *, VALUE);
  static VALUE TinySVM_Model_write(VALUE, VALUE);
  static VALUE TinySVM_Model_append(VALUE, VALUE);
  static VALUE TinySVM_Model_add(int, VALUE *, VALUE);
  static VALUE TinySVM_Model_getX(VALUE, VALUE);
  static VALUE TinySVM_Model_getY(VALUE, VALUE);
  static VALUE TinySVM_Model_writeSVindex(VALUE, VALUE);
  static VALUE TinySVM_Model_appendSVindex(VALUE, VALUE);
  static VALUE TinySVM_Model_clear(VALUE);
  static VALUE TinySVM_Model_size(VALUE);
  static VALUE TinySVM_Model_getSVnum(VALUE);
  static VALUE TinySVM_Model_getBSVnum(VALUE);   
  static VALUE TinySVM_Model_getTrainingDataSize(VALUE);
  static VALUE TinySVM_Model_getLoss(VALUE);   
  static VALUE TinySVM_Model_estimateMargin(VALUE);
  static VALUE TinySVM_Model_estimateVC(VALUE);
  static VALUE TinySVM_Model_estimateSphere(VALUE);
  static VALUE TinySVM_Model_estimateXA(int, VALUE *, VALUE);
  static VALUE TinySVM_Model_classify(VALUE, VALUE);

  static VALUE ExampleClass;
  static VALUE TinySVM_Example_new(int, VALUE *, VALUE);
  static void  TinySVM_Example_destroy(Example *);
  static VALUE TinySVM_Example_read(int, VALUE *, VALUE);
  static VALUE TinySVM_Example_write(VALUE, VALUE);
  static VALUE TinySVM_Example_append(VALUE, VALUE);
  static VALUE TinySVM_Example_add(int, VALUE *, VALUE);
  static VALUE TinySVM_Example_getX(VALUE, VALUE);
  static VALUE TinySVM_Example_getY(VALUE, VALUE);
  static VALUE TinySVM_Example_clear(VALUE);
  static VALUE TinySVM_Example_size(VALUE);
  static VALUE TinySVM_Example_learn(int, VALUE *, VALUE);
  
  void Init_TinySVM(void);
}

//////////////////////////////////////////
// 
// Model
//

static VALUE
TinySVM_Model_new(int argc, VALUE *argv, VALUE self)
{
   Model *model = new Model();

  VALUE fn;
  rb_scan_args(argc, argv, "01", &fn);
  if (fn != Qnil) {
    Check_SafeStr(fn);
    if (! model->read(STR2CSTR(fn))) 
      rb_raise(rb_eIOError, "no such file or directory.");
  }

  return Data_Wrap_Struct(self, 0, (void(*)())TinySVM_Model_destroy, model);
}

static void
TinySVM_Model_destroy(Model *model)
{
  delete model;
}

static VALUE
TinySVM_Model_read(int argc, VALUE *argv, VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  VALUE fn, pos;
  rb_scan_args(argc, argv, "11", &fn, &pos);
  int p = (pos == Qnil)?0:NUM2INT(pos);
  Check_SafeStr(fn);

  if (! model->read(STR2CSTR(fn),"r",p)) {
    rb_raise(rb_eIOError, "no such file or directory.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Model_write(VALUE self, VALUE fn)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  Check_SafeStr(fn);

  if (! model->write(STR2CSTR(fn))) {
    rb_raise(rb_eIOError, "permission denied.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Model_append(VALUE self, VALUE fn)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  Check_SafeStr(fn);

  if (! model->write(STR2CSTR(fn),"a")) 
    rb_raise(rb_eIOError, "permission denied.");

  return Qtrue;
}

static VALUE
TinySVM_Model_writeSVindex(VALUE self, VALUE fn)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  Check_SafeStr(fn);

  if (! model->writeSVindex(STR2CSTR(fn))) {
    rb_raise(rb_eIOError, "permission denied.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Model_appendSVindex(VALUE self, VALUE fn)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  Check_SafeStr(fn);

  if (! model->writeSVindex(STR2CSTR(fn),"a")) {
    rb_raise(rb_eIOError, "permission denied.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Model_add(int argc, VALUE *argv, VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  VALUE a1,a2;
  rb_scan_args(argc, argv, "11", &a1, &a2);

  if (a1 != Qnil && a2 == Qnil) {
    Check_SafeStr(a1);
    if (model->add(STR2CSTR(a1))) return Qtrue;
  } else if (a1 != Qnil && a2 != Qnil) {
    double y = NUM2DBL(a1);
    Check_SafeStr(a2);
    if (model->add(y,STR2CSTR(a2))) return Qtrue;
  }

  return Qfalse;
}

static VALUE
TinySVM_Model_getX(VALUE self, VALUE index)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  int i = NUM2INT(index);
  if (i >= 0 && i < model->size()) {
    char tmp[256];
    feature_node *f = model->x[i];
    VALUE ary = rb_ary_new();

    for (int j = 0; f[j].index != -1; j++) {
      sprintf(tmp,"%d:%g",f[j].index,f[j].value);
      rb_ary_push(ary,rb_str_new2(tmp));
    }

    return ary;
  } 

  rb_raise(rb_eIndexError, "Model::getX() -- index is out of range");
  return Qfalse;
}

static VALUE
TinySVM_Model_getY(VALUE self, VALUE index)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  int i = NUM2INT(index);
  if (i >= 0 && i < model->size())
    return rb_float_new(model->y[i]);

  rb_raise(rb_eIndexError, "Model::getY() -- index is out of range");
  return Qfalse;
}

static VALUE 
TinySVM_Model_clear(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  model->clear();
  return self;
}

static VALUE 
TinySVM_Model_size(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return INT2NUM(model->size());
}

static VALUE 
TinySVM_Model_getBSVnum(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return INT2NUM(model->getBSVnum());
}

static VALUE 
TinySVM_Model_getTrainingDataSize(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return INT2NUM(model->getTrainingDataSize());
}

static VALUE 
TinySVM_Model_getLoss(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return rb_float_new(model->getLoss());
}
   
static VALUE
TinySVM_Model_estimateMargin(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return rb_float_new(model->estimateMargin());
}

static VALUE
TinySVM_Model_estimateSphere(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return rb_float_new(model->estimateSphere());
}
   
static VALUE
TinySVM_Model_estimateVC(VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  return rb_float_new(model->estimateVC());
}

static VALUE
TinySVM_Model_estimateXA(int argc, VALUE *argv, VALUE self)
{
  Model *model;
  Data_Get_Struct(self, Model, model);

  VALUE lo;
  rb_scan_args(argc, argv, "01", &lo);
  if (lo == Qnil) return rb_float_new(model->estimateXA());
  return rb_float_new(model->estimateXA(NUM2DBL(lo)));
}

static VALUE
TinySVM_Model_classify(VALUE self, VALUE str)
{
  Model *model;
  Data_Get_Struct(self, Model, model);
  //  Check_SafeStr(str); 
  return rb_float_new(model->classify(STR2CSTR(str)));
}

//////////////////////////////////////////
// 
// Example
//

static VALUE
TinySVM_Example_new(int argc, VALUE *argv, VALUE self)
{
  Example *example = new Example();

  VALUE fn;
  rb_scan_args(argc, argv, "01", &fn);
  if (fn != Qnil) {
    Check_SafeStr(fn);
    if (! example->read(STR2CSTR(fn))) 
      rb_raise(rb_eIOError, "no such file or directory.");
  }

  return Data_Wrap_Struct(self, 0, (void(*)())TinySVM_Example_destroy, example);
}

static void
TinySVM_Example_destroy(Example *example)
{
  delete example;
}

static VALUE
TinySVM_Example_read(int argc, VALUE *argv, VALUE self)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  VALUE fn, pos;
  rb_scan_args(argc, argv, "11", &fn, &pos);
  int p = (pos == Qnil)?0:NUM2INT(pos);
  Check_SafeStr(fn);

  if (! example->read(STR2CSTR(fn),"r",p)) {
    rb_raise(rb_eIOError, "no such file or directory.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Example_write(VALUE self, VALUE fn)
{
  Example *model;
  Data_Get_Struct(self, Example, model);

  Check_SafeStr(fn);

  if (! model->write(STR2CSTR(fn))) {
    rb_raise(rb_eIOError, "permission denied.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Example_append(VALUE self, VALUE fn)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  Check_SafeStr(fn);

  if (! example->write(STR2CSTR(fn),"a")) {
    rb_raise(rb_eIOError, "permission denied.");
    return Qfalse;
  }

  return Qtrue;
}

static VALUE
TinySVM_Example_add(int argc, VALUE *argv, VALUE self)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  VALUE a1,a2;
  rb_scan_args(argc, argv, "11", &a1, &a2);

  if (a1 != Qnil && a2 == Qnil) {
    Check_SafeStr(a1);
    if (example->add(STR2CSTR(a1))) return Qtrue;
  } else if (a1 != Qnil && a2 != Qnil) {
    double y = NUM2DBL(a1);
    Check_SafeStr(a2);
    if (example->add(y,STR2CSTR(a2))) return Qtrue;
  }

  return Qfalse;
}

static VALUE
TinySVM_Example_getX(VALUE self, VALUE index)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  int i = NUM2INT(index);
  if (i >= 0 && i < example->size()) {
    char tmp[256];
    feature_node *f = example->x[i];
    VALUE ary = rb_ary_new();

    for (int j = 0; f[j].index != -1; j++) {
      sprintf(tmp,"%d:%g",f[j].index,f[j].value);
      rb_ary_push(ary,rb_str_new2(tmp));
    }

    return ary;
  } 

  rb_raise(rb_eIndexError, "Example::getX() -- index is out of range");
  return Qfalse;
}

static VALUE
TinySVM_Example_getY(VALUE self, VALUE index)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  int i = NUM2INT(index);
  if (i >= 0 && i < example->size())
    return rb_float_new(example->y[i]);

  rb_raise(rb_eIndexError, "Example::getY() -- index is out of range");
  return Qfalse;
}

static VALUE
TinySVM_Example_clear(VALUE self)
{
  Example *example;
  Data_Get_Struct(self, Example, example);
  example->clear();
  return self;
}

static VALUE TinySVM_Example_size(VALUE self)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  return INT2NUM(example->size());
}

static VALUE
TinySVM_Example_learn(int argc, VALUE *argv, VALUE self)
{
  Example *example;
  Data_Get_Struct(self, Example, example);

  Model *model;
  VALUE param;
  rb_scan_args(argc, argv, "01", &param);
  if (param == Qnil) {
    model = example->learn("");
  } else {
    Check_SafeStr(param);
    model = example->learn(STR2CSTR(param));
  }

  return Data_Wrap_Struct(ModelClass, 0, (void(*)())TinySVM_Model_destroy, model);
}

void
Init_TinySVM(void)
{
  // Definition for Model class
  ModelClass = rb_define_class("Model",rb_cObject);
  rb_define_singleton_method(ModelClass, "new",  (VALUE(*)())TinySVM_Model_new, -1);
  rb_define_method(ModelClass, "read",           (VALUE(*)())TinySVM_Model_read, -1);
  rb_define_method(ModelClass, "write",          (VALUE(*)())TinySVM_Model_write, 1);
  rb_define_method(ModelClass, "append",         (VALUE(*)())TinySVM_Model_append, 1);
  rb_define_method(ModelClass, "add",            (VALUE(*)())TinySVM_Model_add, -1);
  rb_define_method(ModelClass, "getX",           (VALUE(*)())TinySVM_Model_getX, 1);
  rb_define_method(ModelClass, "getY",           (VALUE(*)())TinySVM_Model_getY, 1);
  rb_define_method(ModelClass, "writeSVindex",   (VALUE(*)())TinySVM_Model_writeSVindex, 1);
  rb_define_method(ModelClass, "appendSVindex",  (VALUE(*)())TinySVM_Model_appendSVindex, 1);
  rb_define_method(ModelClass, "clear",          (VALUE(*)())TinySVM_Model_clear, 0);
  rb_define_method(ModelClass, "size",           (VALUE(*)())TinySVM_Model_size, 0);
  rb_define_method(ModelClass, "getSVnum",       (VALUE(*)())TinySVM_Model_size, 0);  
  rb_define_method(ModelClass, "getBSVnum",      (VALUE(*)())TinySVM_Model_getBSVnum, 0);     
  rb_define_method(ModelClass, "getTrainingDataSize", (VALUE(*)())TinySVM_Model_getTrainingDataSize, 0);        
  rb_define_method(ModelClass, "getLoss",        (VALUE(*)())TinySVM_Model_getLoss, 0);
  rb_define_method(ModelClass, "estimateMargin", (VALUE(*)())TinySVM_Model_estimateMargin, 0);
  rb_define_method(ModelClass, "estimateSphere", (VALUE(*)())TinySVM_Model_estimateSphere, 0);
  rb_define_method(ModelClass, "estimateVC",     (VALUE(*)())TinySVM_Model_estimateVC, 0);
  rb_define_method(ModelClass, "estimateXA",     (VALUE(*)())TinySVM_Model_estimateXA, -1);
  rb_define_method(ModelClass, "classify",       (VALUE(*)())TinySVM_Model_classify, 1);

  // Definition for Example class
  ExampleClass = rb_define_class("Example",rb_cObject);
  rb_define_singleton_method(ExampleClass, "new",   (VALUE(*)())TinySVM_Example_new, -1);
  rb_define_method(ExampleClass, "read",     (VALUE(*)())TinySVM_Example_read, -1);
  rb_define_method(ExampleClass, "write",    (VALUE(*)())TinySVM_Example_write, 1);
  rb_define_method(ExampleClass, "append",   (VALUE(*)())TinySVM_Example_append, 1);
  rb_define_method(ExampleClass, "add",      (VALUE(*)())TinySVM_Example_add, -1);
  rb_define_method(ExampleClass, "getX",     (VALUE(*)())TinySVM_Example_getX, 1);
  rb_define_method(ExampleClass, "getY",     (VALUE(*)())TinySVM_Example_getY, 1);
  rb_define_method(ExampleClass, "getAlpha", (VALUE(*)())TinySVM_Example_getY, 1);
  rb_define_method(ExampleClass, "clear",    (VALUE(*)())TinySVM_Example_clear, 0);
  rb_define_method(ExampleClass, "size",     (VALUE(*)())TinySVM_Example_size, 0);
  rb_define_method(ExampleClass, "learn",    (VALUE(*)())TinySVM_Example_learn, -1); 
}

}
#endif
