#pragma once
#include <d3dx9.h>

class Model {
public:
	Model(char filename[]);
	virtual ~Model();

	/** Get Filename Pointer
	*
	* Get a Filename pointer.
	*
	* @return char* a Filename pointer.
	*/
	char* GetFileName();

private:

	char* fname;
};