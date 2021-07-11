#include "stdafx.h"
#include "rbspimp.h"

//TODO: write importer functionality!
RBspImp::RBspImp()
{
	bspObj = nullptr;
}

RBspImp::~RBspImp()
{
	delete bspObj;
}