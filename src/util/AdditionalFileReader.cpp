#include "util/AdditionalFileReader.h"
using namespace std;
using namespace cv;

Mat loadPFM(const string filePath)
{
    //Open binary file
    ifstream file(filePath.c_str(), ios::in | ios::binary);
    Mat imagePFM;
    //If file correctly openened
    if (file)
    {
        //Read the type of file plus the 0x0a UNIX return character at the end
        char type[3];
        file.read(type, 3 * sizeof(char));

        //Read the width and height
        unsigned int width(0), height(0);
        file >> width >> height;

        //Read the 0x0a UNIX return character at the end
        char endOfLine;
        file.read(&endOfLine, sizeof(char));

        int numberOfComponents(0);
        //The type gets the number of color channels
        if (type[1] == 'F')
        {
            imagePFM = Mat(height, width, CV_32FC3);
            numberOfComponents = 3;
        }
        else if (type[1] == 'f')
        {
            imagePFM = Mat(height, width, CV_32FC1);
            numberOfComponents = 1;
        }

        //TODO Read correctly depending on the endianness
        //Read the endianness plus the 0x0a UNIX return character at the end
        //Byte Order contains -1.0 or 1.0
        char byteOrder[4];
        file.read(byteOrder, 4 * sizeof(char));

        //Find the last line return 0x0a before the pixels of the image
        char findReturn = ' ';
        while (findReturn != 0x0a)
        {
            file.read(&findReturn, sizeof(char));
        }

        //Read each RGB colors as 3 floats and store it in the image.
        float *color = new float[numberOfComponents];
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                file.read((char *)color, numberOfComponents * sizeof(float));

                //In the PFM format the image is upside down
                if (numberOfComponents == 3)
                {
                    //OpenCV stores the color as BGR
                    imagePFM.at<Vec3f>(height - 1 - i, j) = Vec3f(color[2], color[1], color[0]);
                }
                else if (numberOfComponents == 1)
                {
                    //OpenCV stores the color as BGR
                    imagePFM.at<float>(height - 1 - i, j) = color[0];
                }
            }
        }

        delete[] color;

        //Close file
        file.close();
    }
    else
    {
        cerr << "Could not open the file : " << filePath << endl;
    }

    return imagePFM;
}