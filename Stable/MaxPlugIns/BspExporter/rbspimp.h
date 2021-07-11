#pragma once
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"
#include "RBspObject.h"

class RBspImp : public SceneImport
{
private:
	RBspObject* bspObj;
public:
	RBspImp();
	~RBspImp();
};