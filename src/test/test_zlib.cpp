#include <fstream>
#include <iostream>
#include <vector>
#include "fm/zlib/zlib.h"
#include "test_zlib.hpp"

using namespace std;

void printZlibError(int code)
{
	switch(code)
	{
	case Z_MEM_ERROR:
		cout << "E: zlib: not enough memory" << endl;
		break;

	case Z_BUF_ERROR:
		cout << "E: zlib: buffer error" << endl;
		break;

	case Z_STREAM_ERROR:
		cout << "E: zlib: stream error" << endl;
		break;

	case Z_ERRNO:
		cout << "E: zlib: Errno" << endl;
		break;

	case Z_DATA_ERROR:
		cout << "E: zlib: invalid or incomplete deflate data" << endl;
		break;

	case Z_VERSION_ERROR:
		cout << "E: zlib: version mismatch" << endl;
		break;

	default:
		break;
	}
}

void test_compress(string filePath)
{
	// Open file

	ifstream ifs(filePath, ios::binary|ios::in);
	if(!ifs.good())
	{
		cout << "E: can't open file " << filePath << endl;
		return;
	}

	// Read file

	std::string data;

	ifs.seekg(0, ios::end);
	data.resize(ifs.tellg());
	ifs.seekg(0, ios::beg);
	ifs.read(&data[0], data.size());
	ifs.close();

	cout << "Read " << data.size() << endl;

	// Compress data

	unsigned long uncompressedSize = data.size();

	std::vector<unsigned char> compressedData;
	compressedData.resize(compressBound(data.size()));
	cout << "Compress bounds: " << compressedData.size() << endl;

	unsigned long compressedLength = compressedData.size();
	int rc = compress(&compressedData[0], &compressedLength, (unsigned char*)(&data[0]), data.size());

	printZlibError(rc);

	compressedData.resize(compressedLength);
	cout << "Compressed length: " << compressedLength << endl;

	// Decompress data

	rc = uncompress((unsigned char*)(&data[0]), &uncompressedSize, &compressedData[0], compressedLength);
	printZlibError(rc);

	// Write output for further compare
	{
		ofstream ofs("test_zlib_data_out.txt", ios::trunc|ios::out|ios::binary);
		ofs.write(&data[0], data.size());
		ofs.close();
	}
	{
		ofstream ofs("test_zlib_compressed_data_out", ios::trunc|ios::out|ios::binary);
		ofs.write((char*)(&compressedData[0]), compressedData.size());
		ofs.close();
	}
}


