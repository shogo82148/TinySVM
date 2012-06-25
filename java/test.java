// file: main.java

// This file illustrates the high level C++ interface
// created by SWIG.  The java class gets mapped onto the c++ class
// and behaves as if it is a java class.

import TinySVM;

public class test {
  static {
    try {
       System.loadLibrary("TinySVM");
    } catch (UnsatisfiedLinkError e) {
       System.err.println("Cannot load the example native code.\nMake sure your LD_LIBRARY_PATH contains \'.\'\n" + e);
       System.exit(1);
    }
  }

  public static void main(String argv[]) 
  {
     Example ex = new Example();
     ex.read("../tests/train.svmdata","r",0);
     Model model = ex.learn("-t1 -d2 -c1");

     System.out.println ("size=" + model.size());
     System.out.println ("Trnum=" + model.getTrainingDataSize());
     System.out.println ("SV=" + model.getSVnum());
     System.out.println ("BSV=" + model.getBSVnum());
     System.out.println ("VC=" + model.estimateVC());
     System.out.println ("Margin=" + model.estimateMargin());
     System.out.println ("Sphere=" + model.estimateSphere());
     System.out.println ("Loss=" + model.getLoss());

     for (int i = 0; i < 5; i++) 
	System.out.println( "Y[" + i + "]=" + model.getY(i) + " X[" + i + "]=" + model.getX(i));

     System.out.println();
     model.remove(0);
       
     for (int i = 0; i < 5; i++) 
       System.out.println( "Y[" + i + "]=" + model.getY(i) + " X[" + i + "]=" + model.getX(i));
     
     model.write("model","w",0);
     System.out.println( model.classify("1:1 2:1 5:1 100:1") );
     System.out.println(model.classify("10:1 20:1 50:1 100:1") );

     Model model2 = new Model();
     model2.read("model","r",0);
     System.out.println(model2.classify("1:1 2:1 5:1 100:1") );
     System.out.println(model2.classify("10:1 20:1 50:1 100:1") );
       
     Model model3 = ex.learn("");
     model3.compress();
     model3.write("model3","w",0);
  }
}
