#ifndef IMAGE_H
#define IMAGE_H

class Image {
	public:
		int width, height;
		unsigned char *data;
		Image() {}
		~Image();
		Image(const char *fname);
		void invertY();
};



#endif