#include "ImageBMP.h"

bool ImageBMP::loadFromFile(const char* filename) {
	ifstream bitmap(filename, ios::binary);
	uint32_t row=0;

	if (!bitmap.is_open())
		throw exception("Input file is not open");

	bitmap.read((char*)&_fileHeader, sizeof(_fileHeader));


	if (_fileHeader.file_type != 0x4D42) 
		throw exception("Signature invalid");
	
	bitmap.read((char*)&_infoHeader, sizeof(_infoHeader));

	if (_infoHeader.size >= sizeof(_infoHeader) + sizeof(_colorHeader)) {
		bitmap.read((char*)&_colorHeader, sizeof(_colorHeader));
		checkColorFormat();
	}
	
	
	if(_infoHeader.height <= 0 || _infoHeader.width <= 0)
		throw exception("Size of image less then 0");

	bitmap.seekg(_fileHeader.offset_data, bitmap.beg);

	_data.resize(_infoHeader.width * _infoHeader.height * _infoHeader.bit_count/8);


	row = _infoHeader.width * _infoHeader.bit_count / 8;
	vector<uint8_t> buf;
	buf.resize((4 - (row & 3))%4);


	if (buf.size()) {
		for (auto i = 0; i < _infoHeader.height; ++i) {
			bitmap.read((char*)(_data.data() + (i * row)), row);
			bitmap.read((char*)(buf.data()), buf.size());
		}
	}
	else {
		bitmap.read((char*)(_data.data()), _data.size());
	}

	bitmap.close();
}


bool ImageBMP::saveToFile(const char* filename) {
	ofstream bitmap(filename, ios::binary );
	uint32_t row = _infoHeader.width * _infoHeader.bit_count / 8;
	
	if (!bitmap.is_open()) 
		throw exception("Output file is not open");
	
	_fileHeader.offset_data = sizeof(_infoHeader) + sizeof(_colorHeader) + sizeof(_fileHeader);
	bitmap.write((const char*)&_fileHeader, sizeof(_fileHeader));
	_infoHeader.size = sizeof(_infoHeader) + sizeof(_colorHeader);
	bitmap.write((const char*)&_infoHeader, sizeof(_infoHeader));
	bitmap.write((const char*)&_colorHeader, sizeof(_colorHeader));
	
	if ((_infoHeader.bit_count == 32) || ((_infoHeader.bit_count == 24) && !(row % 4))) {
		bitmap.write((const char*)_data.data(), _data.size());
	}
	else if (_infoHeader.bit_count == 24) {
				vector<uint8_t> buf;
				buf.resize(4 - (row & 3));

				for (auto y = 0; y < _infoHeader.height; ++y) {
					bitmap.write((const char*)(_data.data() + row * y), row);
					bitmap.write((const char*)(buf.data()), buf.size());
				}
			}
	
	bitmap.close();
}

void ImageBMP::setTransformColor(function<void(BGR*, int)> procfunc, int param){
	if (_infoHeader.bit_count == 24) 
	{
		for (auto i = 0; i < _data.size(); i += (_infoHeader.bit_count / 8)) {
			procfunc((BGR *)&_data[i], param);
		}
	}
	else if(_infoHeader.bit_count == 32) {
		for (auto i = 0; i < _data.size(); i += (_infoHeader.bit_count / 8)) {
			procfunc((BGR*)&_data[i + 1], param);
		}
	}

}


void ImageBMP::checkColorFormat() {
	BMPColorHeader default_color;
	if (default_color.red_mask != _colorHeader.red_mask ||
		default_color.blue_mask != _colorHeader.blue_mask ||
		default_color.green_mask != _colorHeader.green_mask ||
		default_color.alpha_mask != _colorHeader.alpha_mask) {
		throw exception("Unexpected color mask format.");
	}
	if (default_color.color_space_type != _colorHeader.color_space_type) {
		throw exception("Unexpected color space type.");
	}
}