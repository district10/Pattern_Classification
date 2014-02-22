/*
 * @Author: Gnat TANG,(唐志雄)
 * @Emial: <gnat_tang@yeah.net>
 * How to Compile it? 
 * g++ -ggdb -std=c++0x `pkg-config opencv --cflags --libs` \
 *          kl_transform_imageMultiBand.cpp -o kl_transform_imageMultiBand 
 * /
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <cstdio>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat kl_transform(const Mat&);

int main(int argc, char** argv)
{
  if (argc <= 2) {
    cout << "Error: invalid parameters!" << endl;
    cout << "Usage: " << argv[0] << " <image_files> \n" ;
    exit(-1);
  }


static int rows, cols, bands;
bands = argc - 1;
Mat img[bands];
for (int i = 0; i < bands; ++i) {
  img[i] = imread (argv[i + 1], CV_LOAD_IMAGE_GRAYSCALE);
  assert ( img[i].data != NULL && "Valid Image" );
  assert ( img[i].channels() == 1 && "Each image single band" );
  if( i != 0) {
  assert( img[i].rows == rows && img[i].cols == cols
         && "Same size imges");
  }
  rows = img[i].rows;
  cols = img[i].cols;
}

  cout << "Image: [" << rows << ", " << col << "]." << endl;
  /*
    C++: void Mat::convertTo
    (OutputArray m, int rtype, double alpha=1, double beta=0 ) const
  */





for (int i = 0; i < bands; ++i) {
  static char * win_name[20];
  sprintf(win_name, "band_%2d", i + 1);
  namedWindow(win_name, CV_WINDOW_AUTOSIZE);
  imshow(win_name, img[i]);
}

Mat img_f[bands], Rn[bands];
Mat R = Mat(bands, rows*cols, CV_32FC1);
for (int i = 0; i < bands; ++i) {
  img[i].convertTo(img_f[i], CV_32FC1);
  Rn[i] = Mat(1, rows*cols, CV_32FC1, img_f[i].data);
  Rn[i].row(0).copyTo(R.row(i));
}

  Mat m; // m => mean (row wise)
  reduce (R, m, 1 /* apply to row */, CV_REDUCE_AVG);
  assert (m.rows == R.rows /* ==bands */
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
  
  namedWindow("After", CV_WINDOW_AUTOSIZE);
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
