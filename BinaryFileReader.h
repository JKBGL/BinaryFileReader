#pragma once
/*#include <cstdio>
failed c attempt

class FILE_LOADER {
	FILE* filx;
	bool loaded = false;
	errno_t lasterror = 0;

	public:
		FILE_LOADER() { }
		FILE_LOADER(char* fname) { open(fname); }
		~FILE_LOADER() { close(); }

		void open(const char* fname) {
			errno_t er = fopen_s(&filx, fname, "rb"); //open file in readonly binary
			if (er == NULL) {
				loaded = true;
			} else {
				lasterror = er;
				perror("Couldn't open file");
				loaded = false;
			}
		}

		void close() {
			errno_t er = fclose(filx);
			if (er == NULL)
				loaded = false;
			else {
				lasterror = er;
				perror("Couldn't close file");
			}
		}

		unsigned char BinaryRead() {
				unsigned char buffer[1]{};
				errno_t er = fread(buffer, (size_t)1, (size_t)1, filx);
				if (er) {
					printf("Error Reading file.\n");
					return '\0';
				}
				return *buffer;
		}
		void Read(char* ret, size_t size) {
			if (isLoaded())
				fread(ret, size, 1, filx);
		}
		bool isLoaded() { return loaded; }
};
*/

#include <fstream>

class BinaryFileReader {
	std::ifstream filx;
	bool loaded = false;

	public:
		BinaryFileReader() {}
		BinaryFileReader(char* fname) {
			open(fname);
		}
		~BinaryFileReader() { close(); }

		void open(char* fname) {
			filx.open(fname, std::ios::in | std::ios::binary);
			if (filx.is_open())
				loaded = true;
			else
				printf("Error trying to open file.\n");
		}

		void close() { filx.close(); }

		unsigned char ReadByte() {
			if (!loaded && filx.good()) {
				printf("File is not open, can't read.");
				return '\0';
			}
			unsigned char buffer;
			//filx.read((char*)&buffer, sizeof(buffer));
			filx.read((char*)&buffer, 1);
			return buffer;
		}

		unsigned char* ReadBytes(size_t s) {
			unsigned char* b = new unsigned char[s];
			for (int a = 0; a < s; a++)
				b[a] = ReadByte();
			return b;
		}

		//Pray to crap that this doesn't cause an infinite loop
		std::string ReadString() {
			std::string a = "";
			while (true) {
				unsigned char bt = ReadByte();
				if (bt == 0)
					break;
				a += bt;
			}
			a += '\0'; //close string just in case.
			filx.seekg(-1, std::ios_base::cur); //move cursor back 1 byte because we are overreading. (doesn't work)
			return a;
		}

		//Read bytes until hitting a 0x00. Maximum readability: 500 bytes.
		unsigned char* ReadBytesUntilBreak(size_t &return_size) {
			unsigned char* res = new unsigned char[500];
			int ct = 0;
			while (true) {
				unsigned char b = ReadByte();
				if (b == 0)
					break;
				res[ct] = b;
				ct++;
			}
			return_size = ct;
			filx.seekg(-1, std::ios_base::cur); //move cursor back 1 byte because we are overreading.
			return res;
		}

		int32_t ReadInt32() {
			unsigned char* b = ReadBytes(4);
			int32_t a = 0;
			for (size_t i = 0; i < 4; ++i)
				a += b[i] << 8 * i;
			return a;
		}

		int16_t ReadInt16() {
			unsigned char *b = ReadBytes(2);
			int16_t a = 0;
			for (size_t i = 0; i < 2; ++i)
				a += b[i] << 8 * i;
			return a;
		}

		static int ConvertBytesToAnyInt(unsigned char* bytes, size_t s) {
			int a = 0;
			for (size_t i = 0; i < s; ++i)
				a += bytes[i] << 8 * i;
			return a;
		}

		//A backup function if the other one doesn't work due to endian offsets.
		static int ConvertBytesToAnyInt2(unsigned char* bits, size_t s, bool little_endian = true) {
			int result = 0;
			if (little_endian)
				for (int n = s; n >= 0; n--)
					result = (result << 8) + bits[n];
			else
				for (size_t n = 0; n < s; n++)
					result = (result << 8) + bits[n];
			return result;
		}

		static char* ConvertBytesToCharArray(unsigned char* bytes, size_t s) {
			char *res = new char[s];
			for (size_t i = 0; i < s; i++)
				res[i] = bytes[i];
			res[s - 1] = '\0';
			return res;
		}

		static void DisplayBytesHex(unsigned char* bytes, size_t s) {
			for (size_t i = 0; i < s; i++)
				printf("%02x ", bytes[i]);
			printf("\n");
		}

		//Checks if file is loaded successfully.
		bool isLoaded() { return loaded; }

		//Checks End Of File.
		bool isEOF() { return filx.eof(); }

		//Returns the size of the binary file.
		long long size() {
			//save cursor position
			std::streampos currpos = filx.tellg();
			long long s;
			//seek start to end
			filx.seekg(0, std::ios::end);
			//save size
			s = filx.tellg();
			//seek back to cursor pos
			filx.seekg(0, currpos);
			//return size
			return s;
		}
};