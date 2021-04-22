// opencv_resize_crop_rotate.cpp
// This script takes an image and applies several image processing methods.
/* 
argv[0] = image_name
argv[2] and argv[3] = x and y values of the resizeImage function
argv[4] = rotate angle of the rotateImage function
argv[5] and argv[6] are x and y values of the cropImage function.
*/


#include <iostream>
#include "opencv2/opencv.hpp"

cv::Mat resizeImage(cv::Mat image, char* argv[])
{
    // This function is used to resize images 
    // using argv[2] and argv[3]

    cv::Mat resized;
    int newColumn = atoi(argv[2]);
    int newRow = atoi(argv[3]);;

    cv::resize(image, resized, cv::Size(newColumn, newRow));

    return resized;
}

cv::Mat rotateImage(cv::Mat image, char* argv[])
{

    // This function is used to rotate images
    // using argv[4]

    double angle = atoi(argv[4]);

    cv::Point2f center((image.cols - 1.0) / 2.0, (image.rows - 1.0) / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), image.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0, 2) += bbox.width / 2.0 - image.cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - image.rows / 2.0;

    cv::Mat rotated;
    cv::warpAffine(image, rotated, rot, bbox.size());

    return rotated;
}

cv::Mat cropImage(cv::Mat image, char* argv[])
{

    // This function is used to rotate images
    // using argv[5] and argv[6]

    int offset_x = atoi(argv[5]);
    int offset_y = atoi(argv[6]);

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = (image.size().width - offset_x) / 2;
    roi.height = (image.size().height - offset_y) / 2;
    std::cout << image.size();
    cv::Mat cropped = image(roi);

    return cropped;
}


int main(int argc, char* argv[])
{
    std::cout << argc << " pieces elements were entered\n";
    
    for (int i = 0; i < argc; i++)
    {
        std::cout << i << ".th element is: " << argv[i] << " and type of this element is: " << typeid(argv[i]).name() << std::endl;
    }
    
    cv::Mat original = cv::imread(std::string(argv[1]) + std::string(".png"), CV_LOAD_IMAGE_UNCHANGED);

    if (original.empty())
    {
        std::cout << "!!! imread() failed to open target image" << std::endl;
        return -1;
    }

    cv::imshow("Original", original);
    cv::imshow("Resized", resizeImage(original, argv));
    cv::imshow("Rotated", rotateImage(original, argv));
    cv::imshow("Cropped", cropImage(original, argv));
    cv::waitKey();
}
