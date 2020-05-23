#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

using namespace cv;
using namespace std;


float single_convol(Mat a,Mat b){
    /*
    this function will just simply multiply the corresponding element of kernel and image pixel values
    to find resultant. This value will be replaced with the real value of pixel.
    */
    float sum=0;
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++)
            sum+=a.at<float>(i,j)*b.at<float>(i,j);
    }
    return sum;
}


void sobelme(Mat img,Mat kernel,Mat& ans){
    /*
    this function will find the sobel derivative using kernal (first vertical and then horizontal)
    and store the result in ans objec
    */

    for(int i=1;i<img.rows-1;i++){
        for(int j=1;j<img.cols-1;j++){
            // looping in the image pixels

           Mat a(img,Range(i-1,i+2),Range(j-1,j+2));  // it will move the window(3X3) (stating from 0th pixel upto the end
                                                      // pixel) in the image. This is like moving a 3X3 matrix in a nXn matrix.
                                                      // Here image is the nXn matrix and its pixels are the elemnts of matrix

           ans.at<float>(i,j)=abs(single_convol(a,kernel)); // replacing the hook value of matrix with the corresponding
                                                            //  convolution result
        }}
}


void harriscorner(Mat img_gray,Mat& out,int size,double k){

    Mat IX=Mat::zeros(img_gray.size(),CV_32FC1),Iy=Mat::zeros(img_gray.size(),CV_32FC1); // Creating the object to store
                                                                                         // matrix for kernels

    float kx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};     // vertical kernel matrix
    Mat kernel1=Mat(3,3,CV_32FC1,kx);                // making kernal of size 3 using matrix defined above

    float ky[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};       // vertical kernel matrix
    Mat kernel2=Mat(3,3,CV_32FC1,ky);                  // making kernal of size 3 using matrix defined above

    sobelme(img_gray,kernel1,IX);           // applying sobel derivative
    sobelme(img_gray,kernel2,Iy);           // applying sobel derivative

    Mat ixx,iyy,ixy,ixg,iyg,ixyg,x2y2,xy,mtrace;    // mat objects for storing various upcoming results

    pow(IX,2.0,ixx);                                 // ixx = IX*IX i.e. ixx equals to IX square
    pow(Iy,2.0,iyy);                                // iyy = Iy*Iy i.e. iyy equals to Iy square
    multiply(IX,Iy,ixy);                             // ixy = IX*Iy i.e. ixy equals to IX multiply Iy (combined derivative of
                                                    //  vertical and horizontal)

    /*
       to check the working of sobel dervative , you can uncomment the next 4 lines
        and comment out the remaining lines of this function . The first line will
        show the sobel derivative in vertical direction i.e. dx=1 and dy=0 .
        Similarly the second line will show the sobel derivative in horizontal direction and
        3rd line will show thier combined effect.
    */
//    imshow("X",IX);
//    imshow("Y",Iy);
//    imshow("XY",ixy);
//    waitKey(0);

    GaussianBlur(ixx,ixg,Size(size,size),2.0,0.0,BORDER_DEFAULT);    //  gaussian blur method is used to
    GaussianBlur(iyy,iyg,Size(size,size),0.0,2.0,BORDER_DEFAULT);    //  remove the unclear pixels and better
    GaussianBlur(ixy,ixyg,Size(size,size),2.0,2.0,BORDER_DEFAULT);   //  blending of image. you can see the effect of gaussian
                                                                     //  blur br uncommenting the next 3 lines
//    imshow("ixg",ixg);
//    imshow("iyg",iyg);
//    imshow("ixyg",ixyg);


    multiply(ixg,iyg,x2y2);
    multiply(ixyg,ixyg,xy);
    pow((ixg+ iyg),2.0,mtrace);
    out=(x2y2-xy)-k*mtrace;
//    imshow("x2y2",x2y2);
//    imshow("xy",xy);
//    imshow("mtrace",mtrace);
//    imshow("Out",out);
//     waitKey(0);

}



int main(){

    string inp = "/home/sahil/demo/03.jpg";      // taking the image path

    int k=3;                                        //setting the value of size for blurring (must be odd)

    Mat img=imread(path);                        //reading the image from the given path

    imshow("original image",img);               // just verfiying the load image(you can also use if condition to check if image is not
                                                 //    loaded perfectly)


    Mat img2,img_gray;              // making two objects of mat type. first will store the gray transform of the image
                                    // and second will store the gray and then 32-bit float transform of original image


    cvtColor(img,img2,COLOR_BGR2GRAY);      // storing gray transform of orignal image in img2

    img2.convertTo(img_gray,CV_32FC1);       // storing 32-bit transform of gray image in img_gray

    Mat out;  // making object of mat type to store output of image

    harriscorner(img_gray,out,k,0.04);             // applying the harris corner method on image

    Mat out_norm;

    normalize(out, out_norm, 0, 255, NORM_MINMAX,CV_32F);  // normalizing image between 0 to 255
//    imshow("out_norm",out_norm);
//    imshow("out",out);




     for(int i=1;i<img.rows-1;i++){
         for(int j=1;j<img.cols-1;j++)
         {
             // looping inside the image and checking if the hook value (mid vlue) of looping window
             //   and if its intensity is greater then threshold then consider it to be a corner and
             //  draw a circle of radius 5 and color black (0,0,0)
             double max1,min1;
         Mat check(out_norm,Range(i-1,i+2),Range(j-1,j+2));
          minMaxLoc(check,&min1,&max1);
             if(max1==out_norm.at<float>(i,j)&&out_norm.at<float>(i,j)>58){
             circle(img,Point(j,i),5,Scalar(0,0,0),2,8,0);
             } }}
    namedWindow("corner");
    imshow("corner",img);
    waitKey(0);
    return 0;
}
