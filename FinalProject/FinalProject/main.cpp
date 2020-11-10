#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;


string files[16]=
{"CatDog.png","Zebra.png","cat1.jpg","fish.bmp","mouse.jpg",
    "Chebra.png","bicycle.bmp","dog.jpg","marilyn.bmp","submarine.bmp",
    "Cheetah.png","cat.jpg","einstein.bmp","motorcycle.bmp","tiger.jpg"
};

char input;

// below are the variables for my sliders in ImageA
const int MAX = 100;
int sigma = 5;
Mat image1;
Mat image2;
Mat hybrid_image;
Mat low_freq_img;
Mat high_freq_img;

//Mat hybrid_image2;


vector<Mat> read2Images();
Mat updateMag(Mat complex);


void fftShift(Mat magI);
void interface();
void program1a();
void program1b();
void program2();
void process();
void process_B();
static void on_trackbar( int, void* );

Mat highPassFilter(Mat src, Mat filter);
Mat computeDFT(Mat image);
Mat computeIDFT(const cv::Mat &complexImage);

Mat getKernel( int n, double sigma, int ktype )
{
    const int SMALL_GAUSSIAN_SIZE = 7;
    static const float small_gaussian_tab[][SMALL_GAUSSIAN_SIZE] =
    {
        {1.f},
        {0.25f, 0.5f, 0.25f},
        {0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f},
        {0.03125f, 0.109375f, 0.21875f, 0.28125f, 0.21875f, 0.109375f, 0.03125f}
    };

    const float* fixed_kernel = n % 2 == 1 && n <= SMALL_GAUSSIAN_SIZE && sigma <= 0 ?
        small_gaussian_tab[n>>1] : 0;

    CV_Assert( ktype == CV_32F || ktype == CV_64F );
    Mat kernel(n, 1, ktype);
    float* cf = (float*)kernel.data;
    double* cd = (double*)kernel.data;

    double sigmaX = sigma > 0 ? sigma : ((n-1)*0.5 - 1)*0.3 + 0.8;
    double scale2X = -0.5/(sigmaX*sigmaX);
    double sum = 0;

    int i;
    for( i = 0; i < n; i++ )
    {
        double x = i - (n-1)*0.5;
        double t = fixed_kernel ? (double)fixed_kernel[i] : std::exp(scale2X*x*x);
        if( ktype == CV_32F )
        {
            cf[i] = (float)t;
            sum += cf[i];
        }
        else
        {
            cd[i] = t;
            sum += cd[i];
        }
    }

    sum = 1./sum;
    for( i = 0; i < n; i++ )
    {
        if( ktype == CV_32F )
            cf[i] = (float)(cf[i]*sum);
        else
            cd[i] *= sum;
    }
    return kernel;
}

int main(){
    interface();
    return 0;
}

vector<Mat> read2Images(){
    vector<Mat> vec;
    for(int i = 0; i < 15; i++){
        cout<<i+1<<": "<<files[i]<<endl;
    }
    int option_img1;
    int option_img2;
    cout<<"Image 1: ";
    cin>>option_img1;
    cout<<"Image 2: ";
    cin>>option_img2;
    
    image1 = imread("data/"+files[option_img1-1]);
    image2 = imread("data/"+files[option_img2-1]);
    
    // Error checking and make sure read in correctly
    if(!image1.data || !image2.data){
        cout<<"Cannot find the image"<<endl;
        exit(0);
    }else{
        cout<<"Found "<<files[option_img1-1]<<" and "<<files[option_img2-1]<<"!"<<endl;
    }
    
    vec.push_back(image1);
    vec.push_back(image2);
    return vec;
}


void interface(){
    cout<<"Please select which program you would like to run: "<<endl;
    cout<<"1. Type 1 to run Program A - FAST: using filter2D()"<<endl;
    cout<<"2. Type 2 to run Program A - SLOW: using my own high pass filter()"<<endl;
    cout<<"3. Type 3 to run Program B"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        program1a();
    }else if(option == '2'){
        program1b();
    }else if(option == '3'){
        program2();
    }
    else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}


void program1a(){
    cout<<"\nWelcome to Program 1!"<<endl;
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];
    
    image1.convertTo(image1, CV_64FC3);
    image2.convertTo(image2, CV_64FC3);
    
    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    
    namedWindow("Hybrid image", WINDOW_AUTOSIZE);
    createTrackbar( "Blend", "Hybrid image", &sigma, MAX, on_trackbar );
    on_trackbar(sigma, 0 );
    waitKey(0);
    if(input == 's'){
        cout<<"saving image..."<<endl;
        imwrite("hybrid", hybrid_image);
    }
    
    
}

static void on_trackbar( int, void* )
{
    process();
    imshow( "Hybrid image", hybrid_image );
}




void process(){
    Mat low_freq_img1;
    Mat low_freq_img2;
    Mat high_freq_img1;
    Mat high_freq_img2;
    
    int n = sigma * 2 + 1;
    Mat filter = getKernel(n, sigma, CV_64F);
    
    filter2D(image1, low_freq_img1, -1, filter, Point(-1, -1), 0, BORDER_CONSTANT);
    filter2D(image2, low_freq_img2, -1, filter, Point(-1, -1), 0, BORDER_CONSTANT);
    
    high_freq_img2 = image2 - low_freq_img2;
    hybrid_image = low_freq_img1 + high_freq_img2;
    
    high_freq_img2.convertTo(high_freq_img2, CV_8UC3);
    low_freq_img1.convertTo(low_freq_img1, CV_8UC3);
    hybrid_image.convertTo(hybrid_image, CV_8UC3);
    
    imshow("Low frequencies", low_freq_img1);
    moveWindow("Low frequencies", 300, 0);
    imshow("High frequencies", high_freq_img2);
    moveWindow("High frequencies", 600, 0);
}


void program1b(){
    cout<<"\nWelcome to Program 1!"<<endl;
    
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];
    
    image1.convertTo(image1, CV_64FC3);
    image2.convertTo(image2, CV_64FC3);
    
    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    cout<<"Converting....This can take a while..."<<endl;
    int n = sigma * 3;
    Mat filter = getKernel(n, sigma, CV_64F);
    filter = filter*filter.t();
    
    low_freq_img = highPassFilter(image1, filter);
    high_freq_img = image2 - highPassFilter(image2, filter);
    hybrid_image = low_freq_img + high_freq_img;
    
    high_freq_img.convertTo(high_freq_img, CV_8UC3);
    low_freq_img.convertTo(low_freq_img, CV_8UC3);
    hybrid_image.convertTo(hybrid_image, CV_8UC3);
    
    imshow("Low frequencies", low_freq_img);
    moveWindow("Low frequencies", 300, 0);
    imshow("High frequencies", high_freq_img);
    moveWindow("High frequencies", 600, 0);
    imshow("Hybrid image", hybrid_image );
    moveWindow("Hybrid image", 900, 0);
    waitKey(0);
    if(input == 's'){
        cout<<"saving image..."<<endl;
        imwrite("hybrid", hybrid_image);
    }
    
}


Mat highPassFilter(Mat src, Mat filter)
{
    Mat dst = src.clone();
    double val = 0.0;
    for (int y= 0; y < dst.rows; y++){
        for (int x= 0; x < dst.cols; x++){
            Vec3f color = Vec3f(0.0, 0.0, 0.0);
            for (int channel = 0; channel < 3; channel ++){
                for (int j= 0; j < filter.rows; j++){
                    for (int i= 0; i < filter.cols; i++){
                        int posX = (j - filter.rows / 2) + x;
                        int poxY = (i - filter.cols / 2) + y;
                        if (posX < 0 || poxY < 0 || posX >= src.cols || poxY >= src.rows){
                            val = 0.0;
                        }else{
                            val = src.at<Vec3d>(poxY, posX)[channel]; // get the pixel from src image
                        }
                        color[channel] = color[channel] + val * filter.at<double>(j, i);
                    }
                }
                dst.at<Vec3d>(y, x)[channel] = color[channel]; // update output image with accumulated color
            }
        }
    }
    return dst;
}



void program2(){
    cout<<"\nWelcome to Program 2!"<<endl;
    
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];

    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    
    cvtColor(image1, image1, COLOR_BGR2GRAY);
    cvtColor(image2, image2, COLOR_BGR2GRAY);

    namedWindow("Hybrid image", WINDOW_AUTOSIZE);
    process_B();
    waitKey(0);
    if(input == 's'){
        cout<<"saving image..."<<endl;
        imwrite("hybrid", hybrid_image);
    }
    
}



void process_B(){
    int M = getOptimalDFTSize( image1.rows );
    int N = getOptimalDFTSize( image1.cols );
    
    Mat padded;
    copyMakeBorder(image1, padded, 0, M - image1.rows, 0, N - image1.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    
    Mat padded2;
    copyMakeBorder(image2, padded2, 0, M - image2.rows, 0, N - image2.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes2[] = {Mat_<float>(padded2), Mat::zeros(padded2.size(), CV_32F)};
    Mat complexImg2;
    merge(planes2, 2, complexImg2);
    dft(complexImg2, complexImg2);
    
    // maginitude function
    split(complexImg, planes);
    split(complexImg2, planes2);
    
    Mat ph;
    Mat ph2;
    Mat mag;
    Mat mag2;
    phase(planes[0], planes[1], ph);
    magnitude(planes[0], planes[1], mag);
    phase(planes2[0], planes2[1], ph2);
    magnitude(planes2[0], planes2[1],mag2);
    
    Mat mat (ph.size(), CV_32FC2);
    Mat mat2 (ph2.size(), CV_32FC2);

    
    // reconstruct mat is not a real image - in freq domain
    for(int row = 0; row < mat.rows; row++){
        for (int col = 0; col < mat.cols; col++){
            mat.at<Vec2f>(row, col)[0] = mag.at<float>(row, col) * cos(ph2.at<float>(row, col));
            mat.at<Vec2f>(row, col)[1] = mag.at<float>(row, col) * sin(ph2.at<float>(row, col));
        }
    }
    
    //idft function - spatial domain
    hybrid_image = computeIDFT(mat);
    imshow("Image-1", image1);
    moveWindow("Image-1", 300, 0);
    imshow("Image-2", image2);
    moveWindow("Image-2", 600, 0);
    imshow("Hybrid", hybrid_image);
    moveWindow("Hybrid", 0, 0);

}

// Compute the Discrete fourier transform
Mat computeDFT(Mat image) {
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complex;
    merge(planes, 2, complex);         // Add to the expanded another plane with zeros
    dft(complex, complex, DFT_COMPLEX_OUTPUT);  // fourier transform
    return complex;
}


// Compute the inverse of the Fourier Transform
Mat computeIDFT(const cv::Mat &complexImage) {
    //calculating the idft
    cv::Mat inverseTransform;
    cv::dft(complexImage, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, NORM_MINMAX);
    return inverseTransform;
}

void fftShift(Mat magI) {

    // crop if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}





