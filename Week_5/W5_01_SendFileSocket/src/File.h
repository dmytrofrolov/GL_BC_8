


#ifndef _FILE_H_
#define _FILE_H_

class File{
private:
	FILE * file_;

public:
	File();

	int openFile();
	int eof();
	char * readFile();
	char * writeFile();
	int closeFile();

	~File();
};

#endif // _FILE_H_ 