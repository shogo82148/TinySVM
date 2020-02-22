/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.chasen.tinysvm;

public class Model extends BaseExample {
  private transient long swigCPtr;

  protected Model(long cPtr, boolean cMemoryOwn) {
    super(TinySVMJNI.Model_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Model obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        TinySVMJNI.delete_Model(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public int read(String arg0, String mode, int offset) {
    return TinySVMJNI.Model_read__SWIG_0(swigCPtr, this, arg0, mode, offset);
  }

  public int read(String arg0, String mode) {
    return TinySVMJNI.Model_read__SWIG_1(swigCPtr, this, arg0, mode);
  }

  public int read(String arg0) {
    return TinySVMJNI.Model_read__SWIG_2(swigCPtr, this, arg0);
  }

  public int write(String arg0, String mode, int offset) {
    return TinySVMJNI.Model_write__SWIG_0(swigCPtr, this, arg0, mode, offset);
  }

  public int write(String arg0, String mode) {
    return TinySVMJNI.Model_write__SWIG_1(swigCPtr, this, arg0, mode);
  }

  public int write(String arg0) {
    return TinySVMJNI.Model_write__SWIG_2(swigCPtr, this, arg0);
  }

  public int clear() {
    return TinySVMJNI.Model_clear(swigCPtr, this);
  }

  public double classify(String arg0) {
    return TinySVMJNI.Model_classify__SWIG_0(swigCPtr, this, arg0);
  }

  public double classify(SWIGTYPE_p_TinySVM__feature_node arg0) {
    return TinySVMJNI.Model_classify__SWIG_1(swigCPtr, this, SWIGTYPE_p_TinySVM__feature_node.getCPtr(arg0));
  }

  public double estimateMargin() {
    return TinySVMJNI.Model_estimateMargin(swigCPtr, this);
  }

  public double estimateSphere() {
    return TinySVMJNI.Model_estimateSphere(swigCPtr, this);
  }

  public double estimateVC() {
    return TinySVMJNI.Model_estimateVC(swigCPtr, this);
  }

  public double estimateXA(double rho) {
    return TinySVMJNI.Model_estimateXA__SWIG_0(swigCPtr, this, rho);
  }

  public double estimateXA() {
    return TinySVMJNI.Model_estimateXA__SWIG_1(swigCPtr, this);
  }

  public int compress() {
    return TinySVMJNI.Model_compress(swigCPtr, this);
  }

  public int getSVnum() {
    return TinySVMJNI.Model_getSVnum(swigCPtr, this);
  }

  public int getBSVnum() {
    return TinySVMJNI.Model_getBSVnum(swigCPtr, this);
  }

  public int getTrainingDataSize() {
    return TinySVMJNI.Model_getTrainingDataSize(swigCPtr, this);
  }

  public double getLoss() {
    return TinySVMJNI.Model_getLoss(swigCPtr, this);
  }

  public Model() {
    this(TinySVMJNI.new_Model__SWIG_0(), true);
  }

  public Model(SWIGTYPE_p_TinySVM__Param arg0) {
    this(TinySVMJNI.new_Model__SWIG_1(SWIGTYPE_p_TinySVM__Param.getCPtr(arg0)), true);
  }

}
