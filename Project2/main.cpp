#include "TGA.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void getData(string Filepath, TGA& Temp_) {
	ifstream File(Filepath, std::ios::binary);
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;

	if (File.is_open()) {
		char idLength;
		File.read((char*)&idLength, 1);

		char colorMapType;
		File.read((char*)&colorMapType, 1);

		char dataTypeCode;
		File.read((char*)&dataTypeCode, 1);

		short colorMapOrigin;
		File.read((char*)&colorMapOrigin, 2);

		short colorMapLength;
		File.read((char*)&colorMapLength, 2);

		char colorMapDepth;
		File.read((char*)&colorMapDepth, 1);

		short xOrigin;
		File.read((char*)&xOrigin, 2);

		short yOrigin;
		File.read((char*)&yOrigin, 2);

		short width;
		File.read((char*)&width, 2);

		short height;
		File.read((char*)&height, 2);

		char bitsPerPixel;
		File.read((char*)&bitsPerPixel, 1);

		char imageDescriptor;
		File.read((char*)&imageDescriptor, 1);

		Temp_.addHeader(idLength, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth, xOrigin, yOrigin, width, height, bitsPerPixel, imageDescriptor);

		unsigned long int Counter_ = (width * height);
		for (unsigned long int i = 0; i < Counter_; i++) {
			File.read((char*)&Blue, 1);
			File.read((char*)&Green, 1);
			File.read((char*)&Red, 1);

			Temp_.addPixel(Red, Green, Blue);
		}
	}
}

void writeData(string Location, TGA& Temp_) {
	ofstream Out;
	Out.open(Location, std::ios::binary);

	Out.write((const char*)&(Temp_.Head->idLength), 1);
	Out.write((const char*)&(Temp_.Head->colorMapType), 1);
	Out.write((const char*)&(Temp_.Head->dataTypeCode), 1);
	Out.write((const char*)&(Temp_.Head->colorMapOrigin), 2);
	Out.write((const char*)&(Temp_.Head->colorMapLength), 2);
	Out.write((const char*)&(Temp_.Head->colorMapDepth), 1);
	Out.write((const char*)&(Temp_.Head->xOrigin), 2);
	Out.write((const char*)&(Temp_.Head->yOrigin), 2);
	Out.write((const char*)&(Temp_.Head->width), 2);
	Out.write((const char*)&(Temp_.Head->height), 2);
	Out.write((const char*)&(Temp_.Head->bitsPerPixel), 1);
	Out.write((const char*)&(Temp_.Head->imageDescriptor), 1);

	for (unsigned int i = 0; i < (Temp_.Pictures.size()); i++) {
		Out.write((const char*)&(Temp_.Pictures.at(i).Blue), 1);
		Out.write((const char*)&(Temp_.Pictures.at(i).Green), 1);
		Out.write((const char*)&(Temp_.Pictures.at(i).Red), 1);
	}
}

//Multiply bledning mode (for part 1, also for parts 3 & 4)
TGA Multiply(TGA& Top, TGA& Bot) {
	TGA Product = Top;

	for (unsigned int i = 0; i < Top.Pictures.size(); i++) {
		Product.Pictures.at(i).Blue = ((((float)Top.Pictures.at(i).Blue * Bot.Pictures.at(i).Blue) / 255) + .5f);
		Product.Pictures.at(i).Green = ((((float)Top.Pictures.at(i).Green * Bot.Pictures.at(i).Green) / 255) + .5f);
		Product.Pictures.at(i).Red = ((((float)Top.Pictures.at(i).Red * Bot.Pictures.at(i).Red) / 255) + .5f);
	}
	return Product;
}

//subtract blending mode (for part 2, also for part 4)
TGA Subtract(TGA& Top, TGA& Bot) {
	TGA Difference = Top;

	for (unsigned int i = 0; i < Top.Pictures.size(); i++) {
		if (((int)Bot.Pictures.at(i).Blue - (int)Top.Pictures.at(i).Blue) > 255) {
			Difference.Pictures.at(i).Blue = (char)255;
		}
		else if (((int)Bot.Pictures.at(i).Blue - (int)Top.Pictures.at(i).Blue) < 0) {
			Difference.Pictures.at(i).Blue = (char)0;
		}
		else {
			Difference.Pictures.at(i).Blue = (char)((int)Bot.Pictures.at(i).Blue - (int)Top.Pictures.at(i).Blue);
		}

		if (((int)Bot.Pictures.at(i).Green - (int)Top.Pictures.at(i).Green) > 255) {
			Difference.Pictures.at(i).Green = (char)255;
		}
		else if (((int)Bot.Pictures.at(i).Green - (int)Top.Pictures.at(i).Green) < 0) {
			Difference.Pictures.at(i).Green = (char)0;
		}
		else {
			Difference.Pictures.at(i).Green = (char)((int)Bot.Pictures.at(i).Green - (int)Top.Pictures.at(i).Green);
		}

		if (((int)Bot.Pictures.at(i).Red - (int)Top.Pictures.at(i).Red) > 255) {
			Difference.Pictures.at(i).Red = (char)255;
		}
		else if (((int)Bot.Pictures.at(i).Red - (int)Top.Pictures.at(i).Red) < 0) {
			Difference.Pictures.at(i).Red = (char)0;
		}
		else {
			Difference.Pictures.at(i).Red = (char)((int)Bot.Pictures.at(i).Red - (int)Top.Pictures.at(i).Red);
		}
	}
	return Difference;
}

//screen blending mode (for part 3, used with Multiply)
TGA Screen(TGA& Top, TGA& Bot) {
	TGA Result = Top;

	for (unsigned int i = 0; i < Top.Pictures.size(); i++) {
		Result.Pictures.at(i).Blue = (char)(((1 - (1 - ((float)Top.Pictures.at(i).Blue / 255)) * (1 - ((float)Bot.Pictures.at(i).Blue) / 255)) * 255) + 0.5f);
		Result.Pictures.at(i).Green = (char)(((1 - (1 - ((float)Top.Pictures.at(i).Green / 255)) * (1 - ((float)Bot.Pictures.at(i).Green) / 255)) * 255) + 0.5f);
		Result.Pictures.at(i).Red = (char)(((1 - (1 - ((float)Top.Pictures.at(i).Red / 255)) * (1 - ((float)Bot.Pictures.at(i).Red) / 255)) * 255) + 0.5f);
	}
	return Result;
}

//Overlay blending mode (for part 5)
TGA Overlay(TGA& Top, TGA& Bot) {
	TGA Result = Top;

	for (unsigned int i = 0; i < Top.Pictures.size(); i++) {
		if (((float)Bot.Pictures.at(i).Blue) / 255 <= 0.5f) {
			Result.Pictures.at(i).Blue = ((2 * (((float)Top.Pictures.at(i).Blue / 255) * ((float)Bot.Pictures.at(i).Blue) / 255)) * 255 + 0.5f);
		}
		else {
			Result.Pictures.at(i).Blue = (char)(((1 - 2 * (1 - ((float)Top.Pictures.at(i).Blue / 255)) * (1 - ((float)Bot.Pictures.at(i).Blue) / 255)) * 255) + 0.5f);
		}

		if (((float)Bot.Pictures.at(i).Green) / 255 <= 0.5f) {
			Result.Pictures.at(i).Green = ((2 * (((float)Top.Pictures.at(i).Green / 255) * ((float)Bot.Pictures.at(i).Green) / 255)) * 255 + 0.5f);
		}
		else {
			Result.Pictures.at(i).Green = (char)(((1 - 2 * (1 - ((float)Top.Pictures.at(i).Green / 255)) * (1 - ((float)Bot.Pictures.at(i).Green) / 255)) * 255) + 0.5f);
		}

		if (((float)Bot.Pictures.at(i).Red) / 255 <= 0.5f) {
			Result.Pictures.at(i).Red = ((2 * (((float)Top.Pictures.at(i).Red / 255) * ((float)Bot.Pictures.at(i).Red) / 255)) * 255 + 0.5f);
		}
		else {
			Result.Pictures.at(i).Red = (char)(((1 - 2 * (1 - ((float)Top.Pictures.at(i).Red / 255)) * (1 - ((float)Bot.Pictures.at(i).Red) / 255)) * 255) + 0.5f);
		}
	}
	return Result;
}

//adds to color channels (for part 6)
TGA Add(TGA& Temp_, int BlueSize, int GreenSize, int RedSize) {
	TGA Sum = Temp_;

	for (unsigned int i = 0; i < Temp_.Pictures.size(); i++) {
		if (((int)Temp_.Pictures.at(i).Blue + BlueSize > 255)) {
			Sum.Pictures.at(i).Blue = (char)255;
		}
		else {
			Sum.Pictures.at(i).Blue = (char)((int)Temp_.Pictures.at(i).Blue + BlueSize);
		}

		if (((int)Temp_.Pictures.at(i).Green + GreenSize > 255)) {
			Sum.Pictures.at(i).Green = (char)255;
		}
		else {
			Sum.Pictures.at(i).Green = (char)((int)Temp_.Pictures.at(i).Green + GreenSize);
		}

		if (((int)Temp_.Pictures.at(i).Red + RedSize > 255)) {
			Sum.Pictures.at(i).Red = (char)255;
		}
		else {
			Sum.Pictures.at(i).Red = (char)((int)Temp_.Pictures.at(i).Red + RedSize);
		}
	}
	return Sum;
}

//scales the color channels (for part 7)
TGA Scale(TGA& Temp_, int BlueSize, int GreenSize, int RedSize) {
	TGA Scaler = Temp_;
	for (unsigned int i = 0; i < Temp_.Pictures.size(); i++) {
		if (((int)Temp_.Pictures.at(i).Blue * BlueSize > 255)) {
			Scaler.Pictures.at(i).Blue = (char)255;
		}
		else {
			Scaler.Pictures.at(i).Blue = (char)((int)Temp_.Pictures.at(i).Blue * BlueSize);
		}

		if (((int)Temp_.Pictures.at(i).Green * GreenSize > 255)) {
			Scaler.Pictures.at(i).Green = (char)255;
		}
		else {
			Scaler.Pictures.at(i).Green = (char)((int)Temp_.Pictures.at(i).Green * GreenSize);
		}

		if (((int)Temp_.Pictures.at(i).Red * RedSize > 255)) {
			Scaler.Pictures.at(i).Red = (char)255;
		}
		else {
			Scaler.Pictures.at(i).Red = (char)((int)Temp_.Pictures.at(i).Red * RedSize);
		}
	}
	return Scaler;
}

//seperates color channels to seperate files (for part 8)
void Seperate(TGA& Name, TGA& Blue, TGA& Green, TGA& Red) {
	for (unsigned int i = 0; i < Name.Pictures.size(); i++) {
		Blue.Pictures.at(i).Blue = Name.Pictures.at(i).Blue;
		Blue.Pictures.at(i).Green = Name.Pictures.at(i).Blue;
		Blue.Pictures.at(i).Red = Name.Pictures.at(i).Blue;

		Green.Pictures.at(i).Green = Name.Pictures.at(i).Green;
		Green.Pictures.at(i).Blue = Name.Pictures.at(i).Green;
		Green.Pictures.at(i).Red = Name.Pictures.at(i).Green;

		Red.Pictures.at(i).Red = Name.Pictures.at(i).Red;
		Red.Pictures.at(i).Green = Name.Pictures.at(i).Red;
		Red.Pictures.at(i).Blue = Name.Pictures.at(i).Red;
	}
}

//combines 3 files into one file (for part 9)
TGA Combine(TGA& Blue, TGA& Green, TGA& Red) {
	TGA Result = Blue;

	for (unsigned int i = 0; i < Result.Pictures.size(); i++) {
		Result.Pictures.at(i).Blue = Blue.Pictures.at(i).Blue;
		Result.Pictures.at(i).Green = Green.Pictures.at(i).Green;
		Result.Pictures.at(i).Red = Red.Pictures.at(i).Red;
	}
	return Result;
}

//flips the data (for part 10)
TGA Flip(TGA& Temp_) {
	TGA Flipped = Temp_;
	unsigned int i = (Flipped.Pictures.size() - 1);

	for (unsigned int j = 0; j < Flipped.Pictures.size(); j++) {
		Flipped.Pictures.at(j).Blue = Temp_.Pictures.at(i).Blue;
		Flipped.Pictures.at(j).Green = Temp_.Pictures.at(i).Green;
		Flipped.Pictures.at(j).Red = Temp_.Pictures.at(i).Red;

		i = i - 1;
	}
	return Flipped;
}

//Tests and checks for matc, if not, fails
bool Test(string Testing, TGA& Check) {
	TGA Test_;
	bool Result = true;
	getData(Testing, Test_);
	
	for (unsigned int i = 0; i < Check.Pictures.size(); i++) {
		if (Check.Pictures.at(i).Blue != Test_.Pictures.at(i).Blue) {
			cout << "Failed for Blue at " << i << endl;
			Result = false;
			break;
		}

		if (Check.Pictures.at(i).Green != Test_.Pictures.at(i).Green) {
			cout << "Failed for Green at " << i << endl;
			Result = false;
			break;
		}

		if (Check.Pictures.at(i).Red != Test_.Pictures.at(i).Red) {
			cout << "Failed for Red at " << i << endl;
			Result = false;
			break;
		}
	}
	return Result;
}

//run all the parts from 1 - 10
void partOne() {
	TGA Top;
	getData("input/layer1.tga", Top);
	TGA Bot;
	getData("input/pattern1.tga", Bot);
	
	TGA Result = Multiply(Top, Bot);
	writeData("output/part1.tga", Result);
	if (Test("examples/EXAMPLE_part1.tga", Result)) {
		cout << "Part 1 Passed!" << endl;
	}
	else {
		cout << "Part 1 Failed..." << endl;
	}
}

void partTwo() {
	TGA Top;
	getData("input/layer2.tga", Top);
	TGA Bot;
	getData("input/car.tga", Bot);

	TGA Result = Subtract(Top, Bot);
	writeData("output/part2.tga", Result);
	if (Test("examples/EXAMPLE_part2.tga", Result)) {
		cout << "Part 2 Passed!" << endl;
	}
	else {
		cout << "Part 2 Failed..." << endl;
	}
}

void partThree() {
	TGA Top;
	getData("input/layer1.tga", Top);
	TGA Bot;
	getData("input/pattern2.tga", Bot);

	TGA Product = Multiply(Top, Bot);
	TGA TextTGA;
	getData("input/text.tga", TextTGA);

	TGA Result = Screen(TextTGA, Product);
	writeData("output/part3.tga", Result);
	if (Test("examples/EXAMPLE_part3.tga", Result)) {
		cout << "Part 3 Passed!" << endl;
	}
	else {
		cout << "Part 3 Failed..." << endl;
	}
}

void partFour() {
	TGA Top;
	getData("input/layer2.tga", Top);
	TGA Bot;
	getData("input/circles.tga", Bot);

	TGA Product = Multiply(Top, Bot);
	TGA Pattern2TGA;
	getData("input/pattern2.tga", Pattern2TGA);

	TGA Result = Subtract(Pattern2TGA, Product);
	writeData("output/part4.tga", Result);
	if (Test("examples/EXAMPLE_part4.tga", Result)) {
		cout << "Part 4 Passed!" << endl;
	}
	else {
		cout << "Part 4 Failed..." << endl;
	}
}

void partFive() {
	TGA Top;
	getData("input/layer1.tga", Top);
	TGA Bot;
	getData("input/pattern1.tga", Bot);

	TGA Result = Overlay(Top, Bot);
	writeData("output/part5.tga", Result);
	if (Test("examples/EXAMPLE_part5.tga", Result)) {
		cout << "Part 5 Passed!" << endl;
	}
	else {
		cout << "Part 5 Failed..." << endl;
	}
}

void partSix() {
	TGA Temp_;
	getData("input/car.tga", Temp_);

	TGA Result = Add(Temp_, 0, 200, 0);
	writeData("output/part6.tga", Result);
	if (Test("examples/EXAMPLE_part6.tga", Result)) {
		cout << "Part 6 Passed!" << endl;
	}
	else {
		cout << "Part 6 Failed..." << endl;
	}
}

void partSeven() {
	TGA Temp_;
	getData("input/car.tga", Temp_);

	TGA Result = Scale(Temp_, 0, 1, 4);
	writeData("output/part7.tga", Result);
	if (Test("examples/EXAMPLE_part7.tga", Result)) {
		cout << "Part 7 Passed!" << endl;
	}
	else {
		cout << "Part 7 Failed..." << endl;
	}
}

void partEight() {
	TGA Name;
	getData("input/car.tga", Name);
	TGA Blue = Name;
	TGA Green = Name;
	TGA Red = Name;

	Seperate(Name, Blue, Green, Red);
	writeData("output/part8_b.tga", Blue);
	writeData("output/part8_g.tga", Green);
	writeData("output/part8_r.tga", Red);
	if (Test("examples/EXAMPLE_part8_b.tga", Blue)) {
		cout << "Part 8, BLUE, Passed!" << endl;
	}
	else {
		cout << "Part 8, BLUE, Failed..." << endl;
	}

	if (Test("examples/EXAMPLE_part8_g.tga", Green)) {
		cout << "Part 8, GREEN, Passed!" << endl;
	}
	else {
		cout << "Part 8, GREEN, Failed..." << endl;
	}

	if (Test("examples/EXAMPLE_part8_r.tga", Red)) {
		cout << "Part 8, RED, Passed!" << endl;
	}
	else {
		cout << "Part 8, RED, Failed..." << endl;
	}
}

void partNine() {
	TGA Blue;
	TGA Green;
	TGA Red;
	getData("input/layer_blue.tga", Blue);
	getData("input/layer_green.tga", Green);
	getData("input/layer_red.tga", Red);

	TGA Result = Combine(Blue, Green, Red);
	writeData("output/part9.tga", Result);
	if (Test("examples/EXAMPLE_part9.tga", Result)) {
		cout << "Part 9 Passed!" << endl;
	}
	else {
		cout << "Part 9 Failed..." << endl;
	}
}

void partTen() {
	TGA Temp_;
	getData("input/text2.tga", Temp_);

	TGA Result = Flip(Temp_);
	writeData("output/part10.tga", Result);
	if (Test("examples/EXAMPLE_part10.tga", Result)) {
		cout << "Part 10 Passed!" << endl;
	}
	else {
		cout << "Part 10 Failed..." << endl;
	}
}

int main() {
	partOne();
	partTwo();
	partThree();
	partFour();
	partFive();
	partSix();
	partSeven();
	partEight();
	partNine();
	partTen();
}