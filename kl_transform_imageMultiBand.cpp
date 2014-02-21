/*
 * @Author: Gnat TANG,(唐志雄)
 * @Emial: <gnat_tang@yeah.net>
 * How to Compile it? 
 * g++ -ggdb -std=c++0x`pkg-config --cflags opencv` `pkg-config --libs opencv` \
 *               kl_transform_imageMultiBand.cpp -o kl_transform_imageMultiBand 
 * /
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <cstdio>

using namespace std;
using namespace cv;

Mat kl_transform(const Mat&);

int main(int argc, char** argv)
{
  if (argc == 1) {
    cout << "Error: invalid parameters!" << endl;
    cout << "Usage: " << argv[0] << " <image_files> \n" ;
    exit(-1);
  }

Mat img[argc - 1];
for (int i = 1; i < argc; ++i) {
  img[i] = imread (argv[i]);

  assert ( img[i].data != NULL && "Valid Image" );
  assert ( data.channels() == 1 && "Each image single band" );

if( i != 1) {
assert( img[i].rows == row && img[i].cols == col
         && "Same size imges");
}
static int row = img[i].rows;
static int col = img[i].cols;
}
  const int rows = (int)data.rows;
  const int cols = (int)data.cols;
  cout << "data.rows: " << rows << endl
       << "data.cols: " << cols << endl;
  /*
    C++: void Mat::convertTo
    (OutputArray m, int rtype, double alpha=1, double beta=0 ) const
  */

  Mat data_f;
  data.convertTo(data_f, CV_32FC3);

  vector< Mat > data_v;
  split( data_f, data_v );
  assert ( data_v.size() == 3 && "Split [RGB] to vector< Mat > " );


  Mat r, g, b;
  data_v[0].convertTo(b, CV_8UC1);
  data_v[1].convertTo(g, CV_8UC1);
  data_v[2].convertTo(r, CV_8UC1);

  namedWindow("R", CV_WINDOW_AUTOSIZE);
  namedWindow("G", CV_WINDOW_AUTOSIZE);
  namedWindow("B", CV_WINDOW_AUTOSIZE);
  imshow("R", r);
  imshow("G", g);
  imshow("B", b);


  // R1 = Mat(1, rows*cols, CV_32FC1, data_v.at(0).data); // this works
  Mat R1, R2, R3;
  R1 = Mat(1, rows*cols, CV_32FC1, data_v.at(0).data);
  R2 = Mat(1, rows*cols, CV_32FC1, data_v.at(1).data);
  R3 = Mat(1, rows*cols, CV_32FC1, data_v.at(2).data);

  Mat R = Mat(3, rows*cols, CV_32FC1);
  R1.row(0).copyTo(R.row(0));
  R2.row(0).copyTo(R.row(1));
  R3.row(0).copyTo(R.row(2));
  
  Mat m; // m => mean (row wise)
  reduce (R, m, 1 /* apply to row */, CV_REDUCE_AVG);
  assert (m.rows == R.rows /* ==3 */
          && m.cols == 1
	  && "m: mean of all samples");

  Mat RR = R / R.cols * R.t() + m / R.cols * m.t();
  cout << RR << endl;

  Mat TransMat = kl_transform(RR); 
  cout << "The Transforming Matrix is:"
       << TransMat << endl;
  Mat smp_done = TransMat * R;
  Mat output = Mat(rows, cols, CV_32FC1, smp_done.data);
  Mat output_d;
  output.convertTo(output_d, CV_8UC1);
  //assert (output_d.type() == data.type()
  //	  && "Same datatype");
  namedWindow("Origin", CV_WINDOW_AUTOSIZE);
  namedWindow("After", CV_WINDOW_AUTOSIZE);
  imshow("Origin", data);
  imshow("After", output_d);
  
  imwrite("after_KL_transform.bmp", output_d);
  waitKey(0);
  return 0;
}

/// ##################################
/// # IMPLEMENTATION
/// ##################################

Mat kl_transform(const Mat& square)
{
  auto rows = square.rows;
  auto cols = square.cols;
  assert (rows == cols
	  && "kl_transforme take square matrix, ONLY");

  Mat output;

  Mat eigen_values, eigen_vectors;
  // C++: bool eigen(InputArray src, OutputArray eigenvalues, OutputArray eigenvectors=noArray() )
 
  eigen( square, eigen_values, eigen_vectors );
  /*
    cout << "eigen_values: " << endl
    << eigen_values << endl;
    cout << "eigen_vectors: " << endl
    << eigen_vectors << endl;
  */
  
  output = eigen_vectors.row(0);
  
  /*
    C++: void normalize(InputArray src, OutputArray dst, double alpha=1, double beta=0, int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray() )
  */
  normalize(output, output, 1, 0, NORM_L1);
  return output;  
}
