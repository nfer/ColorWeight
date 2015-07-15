
#include <iostream>
#include "opencv/cv.h"
#include "opencv/highgui.h"

extern "C" {
	#include "CountSortList.h"
}

using namespace std;

#define OUTPUT_JS

int main(int argc, char * argv[])
{
	if (argc < 2) {
		printf("Please input %s IMAGE_NAME.\n", argv[0]);
		return 0;
	}

	CountSortList_init();
	// load image with 3-channel color
	IplImage *src_img =cvLoadImage(argv[1],1);
	if (NULL == src_img) {
		printf("cvLoadImage %s failed.\n", argv[1]);
		return 0;
	}

	for(int i=0; i<src_img->height; i++)
	{
		for(int j=0; j<src_img->width; j++)
		{
			int b = cvGet2D(src_img, i, j).val[0];
			int g = cvGet2D(src_img, i, j).val[1];
			int r = cvGet2D(src_img, i, j).val[2];
			// printf("-0x%02X%02X%02X\n", b, g, r);
			// printf("0x%06X\n", (r<<16)+(g<<8)+b);
			CountSortList_add((r<<16)+(g<<8)+b);
		}
	}

	cvReleaseImage(&src_img);

	int sum = 0, others = 0;
	const int outputSize = 20;
	CountSortData outputArray[outputSize];
	CountSortList_output(outputArray, outputSize, &sum);
#ifdef OUTPUT_JS
	printf("var countSum = %d;\n", sum);
	printf("var dataArr = new Array();\n");
	printf("var countArr = new Array();\n");
#else
	printf("sum is %d\n", sum);
#endif
	others = sum;
	for (int i=0; i<outputSize; i++) {
#ifdef OUTPUT_JS
		printf("dataArr[%d]=\"#%06X\";\n", i, outputArray[i].data);
		printf("countArr[%d]=\"%d\";\n", i, outputArray[i].count);
#else
		printf("[%d]data is 0x%06X, percent %f%%, count is %d\n",
			i, outputArray[i].data, (double)outputArray[i].count*100/sum, outputArray[i].count);
#endif
		others -= outputArray[i].count;
	}
	CountSortList_deinit();
#ifdef OUTPUT_JS
	printf("dataArr[%d]=\"#%06X\";\n", outputSize, 0);
	printf("countArr[%d]=\"%d\";\n", outputSize, others);
#else
	printf("[%d] others sumary , percent %f%%, count is %d\n",
			outputSize, (double)others*100/sum, others);
#endif
	return 0;
}
