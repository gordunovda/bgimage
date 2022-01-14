#include <iostream>
#include "ImageBMP.h"
#include <algorithm>
#include <string>



using namespace std;


void Brightness(BGR *x, int i) {
	if( i > 0) {
		x->g = (uint8_t)min((int)(x->g + i), (int)255);
		x->r = (uint8_t)min((int)(x->r + i), (int)255);
		x->b = (uint8_t)min((int)(x->b + i), (int)255);
	}
	else
	{
		x->g = (uint8_t)max((int)(x->g + i), (int)0);
		x->r = (uint8_t)max((int)(x->r + i), (int)0);
		x->b = (uint8_t)max((int)(x->b + i), (int)0);
}
}

int main(int argc, char* argv[]) {
	if(argc > 3){
		ImageBMP x;

		try {
			cout << "Loading " << argv[1];
			x.loadFromFile(argv[1]);
			cout << " ... [Ok]" << endl
				<< "Transform color";
			
			x.setTransformColor(Brightness, stoi(argv[3]));

			cout << " ... [Ok]" << endl
				<< "Save to"<< argv[2];


			x.saveToFile(argv[2]);

			cout << " ... [Ok]" <<endl<< "Finish!" << endl;
		}
		catch (exception t) {
			cout << endl << "Oh! Error: " << t.what() << endl;
		}
		catch (...) {
			cout << endl << "Fatal error" << endl;
		}


	}
	else {
		cout << "brimage.exe input-file output-file brightness-level" << endl
			<< "-----------------------------------------------------------" << endl
			<< "brimage test util for change brightness 24/32 bits BMP image" << endl
			<< "\tinput-file - input image file name for processing " << endl
			<< "\toutput-file - output image file name" << endl
			<< "\tbrightness-level - brightness level, interval [(dark)-255...255(light)]" << endl;

	}
}
