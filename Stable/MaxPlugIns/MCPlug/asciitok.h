//************************************************************************** 
//* Asciitok.h	- Ascii File Exporter
//* 
//* By Christer Janson
//* Kinetix Development
//*
//* January 20, 1997 CCJ Initial coding
//*
//* File format tokens
//*
//* Copyright (c) 1997, All Rights Reserved. 
//***************************************************************************

#ifndef __ASCIITOK__H
#define __ASCIITOK__H

/**********************************************************************
 This is a list of all output tokens

 Note to translators:
 The strings in this module defines the file format and should not be
 translated.
***********************************************************************/

// Top level category ID's
#define ID_SCENE				_T(L"*SCENE")
#define ID_GEOMETRY				_T(L"*GEOMOBJECT") 
#define ID_SHAPE				_T(L"*SHAPEOBJECT") 
#define ID_CAMERA				_T(L"*CAMERAOBJECT") 
#define ID_LIGHT				_T(L"*LIGHTOBJECT") 
#define ID_HELPER				_T(L"*HELPEROBJECT")
#define ID_MATERIAL_LIST		_T(L"*MATERIAL_LIST")

// Hierarchy
#define ID_GROUP				_T(L"*GROUP")

// Node related ID's
#define ID_NODE_TM				_T(L"*NODE_TM")
#define ID_NODE_NAME			_T(L"*NODE_NAME") 
#define ID_NODE_PARENT			_T(L"*NODE_PARENT")

// Object (node) properties
#define ID_PROP_MOTIONBLUR		_T(L"*PROP_MOTIONBLUR")
#define ID_PROP_CASTSHADOW		_T(L"*PROP_CASTSHADOW")
#define ID_PROP_RECVSHADOW		_T(L"*PROP_RECVSHADOW")

// Mesh related ID's
#define ID_MESH					_T(L"*MESH")
#define ID_MESH_NORMALS			_T(L"*MESH_NORMALS") 
#define ID_MESH_NUMVERTEX		_T(L"*MESH_NUMVERTEX") 
#define ID_MESH_NUMFACES		_T(L"*MESH_NUMFACES") 
#define ID_MESH_VERTEX_LIST		_T(L"*MESH_VERTEX_LIST") 
#define ID_MESH_VERTEX			_T(L"*MESH_VERTEX")
#define ID_MESH_FACE_LIST		_T(L"*MESH_FACE_LIST") 
#define ID_MESH_FACE			_T(L"*MESH_FACE")
#define ID_MESH_SMOOTHING		_T(L"*MESH_SMOOTHING") 
#define ID_MESH_MTLID			_T(L"*MESH_MTLID")

#define ID_MESH_NUMTVERTEX		_T(L"*MESH_NUMTVERTEX") 
#define ID_MESH_NUMTVFACES		_T(L"*MESH_NUMTVFACES") 
#define ID_MESH_TVERTLIST		_T(L"*MESH_TVERTLIST") 
#define ID_MESH_TVERT			_T(L"*MESH_TVERT") 
#define ID_MESH_TFACELIST		_T(L"*MESH_TFACELIST") 
#define ID_MESH_TFACE			_T(L"*MESH_TFACE")

#define ID_MESH_NUMCVERTEX		_T(L"*MESH_NUMCVERTEX")
#define ID_MESH_NUMCVFACES		_T(L"*MESH_NUMCVFACES") 
#define ID_MESH_CVERTLIST		_T(L"*MESH_CVERTLIST")
#define ID_MESH_VERTCOL			_T(L"*MESH_VERTCOL")
#define ID_MESH_CFACELIST		_T(L"*MESH_CFACELIST")
#define ID_MESH_CFACE			_T(L"*MESH_CFACE")

// New for R3 - indicates a block with new map channel information
#define ID_MESH_MAPPINGCHANNEL	_T(L"*MESH_MAPPINGCHANNEL")
// End new

#define ID_MESH_FACEMAPLIST		_T(L"*MESH_FACEMAPLIST") 
#define ID_MESH_FACEMAP			_T(L"*MESH_FACEMAP") 
#define ID_MESH_FACEVERT		_T(L"*MESH_FACEMAPVERT")

#define ID_MESH_FACENORMAL		_T(L"*MESH_FACENORMAL") 
#define ID_MESH_VERTEXNORMAL	_T(L"*MESH_VERTEXNORMAL")

#define ID_MESH_ANIMATION		_T(L"*MESH_ANIMATION")

// Shape ID's
#define ID_SHAPE_LINECOUNT		_T(L"*SHAPE_LINECOUNT") 
#define ID_SHAPE_LINE			_T(L"*SHAPE_LINE")
#define ID_SHAPE_VERTEX_KNOT	_T(L"*SHAPE_VERTEX_KNOT") 
#define ID_SHAPE_VERTEX_INTERP	_T(L"*SHAPE_VERTEX_INTERP") 
#define ID_SHAPE_VERTEXCOUNT	_T(L"*SHAPE_VERTEXCOUNT") 
#define ID_SHAPE_CLOSED			_T(L"*SHAPE_CLOSED")

// Light ID's
#define ID_LIGHT_SETTINGS		_T(L"*LIGHT_SETTINGS") 
#define ID_LIGHT_TYPE			_T(L"*LIGHT_TYPE") 
#define ID_LIGHT_COLOR			_T(L"*LIGHT_COLOR") 
#define ID_LIGHT_INTENS			_T(L"*LIGHT_INTENS") 
#define ID_LIGHT_HOTSPOT		_T(L"*LIGHT_HOTSPOT") 
#define ID_LIGHT_FALLOFF		_T(L"*LIGHT_FALLOFF") 
#define ID_LIGHT_ATTNSTART		_T(L"*LIGHT_ATTNSTART") 
#define ID_LIGHT_ATTNEND		_T(L"*LIGHT_ATTNEND")
#define ID_LIGHT_ASPECT			_T(L"*LIGHT_ASPECT")
#define ID_LIGHT_SHADOWS		_T(L"*LIGHT_SHADOWS")
#define ID_LIGHT_USELIGHT		_T(L"*LIGHT_USELIGHT")
#define ID_LIGHT_SPOTSHAPE		_T(L"*LIGHT_SPOTSHAPE")
#define ID_LIGHT_TDIST			_T(L"*LIGHT_TDIST")
#define ID_LIGHT_MAPBIAS		_T(L"*LIGHT_MAPBIAS")
#define ID_LIGHT_MAPRANGE		_T(L"*LIGHT_MAPRANGE")
#define ID_LIGHT_MAPSIZE		_T(L"*LIGHT_MAPSIZE")
#define ID_LIGHT_RAYBIAS		_T(L"*LIGHT_RAYBIAS")
#define ID_LIGHT_USEGLOBAL		_T(L"*LIGHT_USEGLOBAL")
#define ID_LIGHT_ABSMAPBIAS		_T(L"*LIGHT_ABSMAPBIAS")
#define ID_LIGHT_OVERSHOOT		_T(L"*LIGHT_OVERSHOOT")
#define ID_LIGHT_EXCLUSIONLIST	_T(L"*LIGHT_EXCLUDELIST")
#define ID_LIGHT_NUMEXCLUDED	_T(L"*LIGHT_NUMEXCLUDED")
#define ID_LIGHT_EXCLUDED		_T(L"*LIGHT_EXCLUDED")
#define ID_LIGHT_EXCLINCLUDE	_T(L"*LIGHT_EXCLUDED_INCLUDE")
#define ID_LIGHT_EXCL_AFFECT_ILLUM	_T(L"*LIGHT_EXCLUDED_AFFECT_ILLUM")
#define ID_LIGHT_EXCL_AFFECT_SHAD	_T(L"*LIGHT_EXCLUDED_AFFECT_SHADOW")
#define ID_LIGHT_ANIMATION		_T(L"*LIGHT_ANIMATION")

#define ID_LIGHT_TYPE_OMNI		_T(L"Omni")
#define ID_LIGHT_TYPE_TARG		_T(L"Target")
#define ID_LIGHT_TYPE_DIR		_T(L"Directional")
#define ID_LIGHT_TYPE_FREE		_T(L"Free")

#define ID_LIGHT_SHAD_OFF		_T(L"Off")
#define ID_LIGHT_SHAD_MAP		_T(L"Mapped")
#define ID_LIGHT_SHAD_RAY		_T(L"Raytraced")

#define ID_LIGHT_SHAPE_CIRC		_T(L"Circle")
#define ID_LIGHT_SHAPE_RECT		_T(L"Rect")

// Camera ID's
#define ID_CAMERA_SETTINGS		_T(L"*CAMERA_SETTINGS")
#define ID_CAMERA_HITHER		_T(L"*CAMERA_HITHER") 
#define ID_CAMERA_YON			_T(L"*CAMERA_YON") 
#define ID_CAMERA_NEAR			_T(L"*CAMERA_NEAR") 
#define ID_CAMERA_FAR			_T(L"*CAMERA_FAR") 
#define ID_CAMERA_FOV			_T(L"*CAMERA_FOV") 
#define ID_CAMERA_TDIST			_T(L"*CAMERA_TDIST")
#define ID_CAMERA_ANIMATION		_T(L"*CAMERA_ANIMATION")

#define ID_CAMERA_TYPE			_T(L"*CAMERA_TYPE")
#define ID_CAMERATYPE_TARGET	_T(L"Target")
#define ID_CAMERATYPE_FREE		_T(L"Free")

// Helper objects
#define ID_HELPER_CLASS			_T(L"*HELPER_CLASS")

// Controller ID's
#define ID_CONTROL_POINT3_TCB		_T(L"*CONTROL_POINT3_TCB") 
#define ID_CONTROL_POINT3_BEZIER	_T(L"*CONTROL_POINT3_BEZIER") 
#define ID_CONTROL_COLOR_BEZIER		_T(L"*CONTROL_COLOR_BEZIER") 
#define ID_CONTROL_POINT3_SAMPLE	_T(L"*CONTROL_POINT3_SAMPLE")

#define ID_CONTROL_FLOAT_TCB	_T(L"*CONTROL_FLOAT_TCB") 
#define ID_CONTROL_FLOAT_BEZIER	_T(L"*CONTROL_FLOAT_BEZIER") 
#define ID_CONTROL_FLOAT_LINEAR	_T(L"*CONTROL_FLOAT_LINEAR") 
#define ID_CONTROL_FLOAT_SAMPLE	_T(L"*CONTROL_FLOAT_SAMPLE")

// "Track" is the identification of a sampled controller
#define ID_POS_TRACK			_T(L"*CONTROL_POS_TRACK")
#define ID_ROT_TRACK			_T(L"*CONTROL_ROT_TRACK") 
#define ID_SCALE_TRACK			_T(L"*CONTROL_SCALE_TRACK")

// Sampled keys
#define ID_POS_SAMPLE			_T(L"*CONTROL_POS_SAMPLE")
#define ID_ROT_SAMPLE			_T(L"*CONTROL_ROT_SAMPLE") 
#define ID_SCALE_SAMPLE			_T(L"*CONTROL_SCALE_SAMPLE")

// Specific controller keys
#define ID_POS_KEY				_T(L"*CONTROL_POS_KEY")
#define ID_ROT_KEY				_T(L"*CONTROL_ROT_KEY") 
#define ID_SCALE_KEY			_T(L"*CONTROL_SCALE_KEY")
#define ID_POINT3_KEY			_T(L"*CONTROL_POINT3_KEY")
#define ID_FLOAT_KEY			_T(L"*CONTROL_FLOAT_KEY")

// TCB Keys have Tens, cont, bias, easeIn, easeOut
#define ID_TCB_POINT3_KEY		_T(L"*CONTROL_TCB_POINT3_KEY")
#define ID_TCB_FLOAT_KEY		_T(L"*CONTROL_TCB_FLOAT_KEY")
#define ID_TCB_POS_KEY			_T(L"*CONTROL_TCB_POS_KEY")
#define ID_TCB_ROT_KEY			_T(L"*CONTROL_TCB_ROT_KEY") 
#define ID_TCB_SCALE_KEY		_T(L"*CONTROL_TCB_SCALE_KEY")

// Bezier keys have inTan, outTan
#define ID_BEZIER_FLOAT_KEY		_T(L"*CONTROL_BEZIER_FLOAT_KEY")
#define ID_BEZIER_POINT3_KEY	_T(L"*CONTROL_BEZIER_POINT3_KEY")
#define ID_BEZIER_POS_KEY		_T(L"*CONTROL_BEZIER_POS_KEY")
#define ID_BEZIER_SCALE_KEY		_T(L"*CONTROL_BEZIER_SCALE_KEY")


#define ID_CONTROL_POS_LINEAR	_T(L"*CONTROL_POS_LINEAR")
#define ID_CONTROL_POS_TCB		_T(L"*CONTROL_POS_TCB")
#define ID_CONTROL_POS_BEZIER	_T(L"*CONTROL_POS_BEZIER")
#define ID_CONTROL_ROT_LINEAR	_T(L"*CONTROL_ROT_LINEAR")
#define ID_CONTROL_ROT_TCB		_T(L"*CONTROL_ROT_TCB")
#define ID_CONTROL_ROT_BEZIER	_T(L"*CONTROL_ROT_BEZIER")
#define ID_CONTROL_SCALE_LINEAR _T(L"*CONTROL_SCALE_LINEAR")
#define ID_CONTROL_SCALE_TCB	_T(L"*CONTROL_SCALE_TCB")
#define ID_CONTROL_SCALE_BEZIER	_T(L"*CONTROL_SCALE_BEZIER")


// IK Node Info
#define ID_IKTERMINATOR			_T(L"*IK_TERMINATOR")
#define ID_IKROT_PINNED			_T(L"*IK_ROT_PINNED")
#define ID_IKPOS_PINNED			_T(L"*IK_POS_PINNED")

// IK Joints
#define ID_IKJOINT				_T(L"*IK_JOINT")
#define ID_IKTYPE				_T(L"*IK_TYPE")
#define ID_IKDOF				_T(L"*IK_DOF")
#define ID_IKXACTIVE			_T(L"*IK_XACTIVE")
#define ID_IKYACTIVE			_T(L"*IK_YACTIVE")
#define ID_IKZACTIVE			_T(L"*IK_ZACTIVE")
#define ID_IKXLIMITED			_T(L"*IK_XLIMITED")
#define ID_IKYLIMITED			_T(L"*IK_YLIMITED")
#define ID_IKZLIMITED			_T(L"*IK_ZLIMITED")
#define ID_IKXEASE				_T(L"*IK_XEASE")
#define ID_IKYEASE				_T(L"*IK_YEASE")
#define ID_IKZEASE				_T(L"*IK_ZEASE")
#define ID_IKLIMITEXACT			_T(L"*IK_LIMITEXACT")
#define ID_IKJOINTINFO			_T(L"*IK_JOINTINFO")
#define ID_IKTYPEPOS			_T(L"Position")
#define ID_IKTYPEROT			_T(L"Rotation")

// Material / Texture related ID's
#define ID_WIRECOLOR			_T(L"*WIREFRAME_COLOR") 
#define ID_MATERIAL				_T(L"*MATERIAL") 
#define ID_MATERIAL_COUNT		_T(L"*MATERIAL_COUNT") 
#define ID_MATERIAL_REF			_T(L"*MATERIAL_REF")
#define ID_NUMSUBMTLS			_T(L"*NUMSUBMTLS") 
#define ID_SUBMATERIAL			_T(L"*SUBMATERIAL") 
#define ID_MATNAME				_T(L"*MATERIAL_NAME") 
#define ID_MATCLASS				_T(L"*MATERIAL_CLASS")

#define ID_MAT_SHADE_CONST		_T(L"Constant")
#define ID_MAT_SHADE_PHONG		_T(L"Phong")
#define ID_MAT_SHADE_METAL		_T(L"Metal")
#define ID_MAT_SHADE_BLINN		_T(L"Blinn")
#define ID_MAT_SHADE_OTHER		_T(L"Other")

#define ID_MAP_XPTYPE_FLT		_T(L"Filter")
#define ID_MAP_XPTYPE_SUB		_T(L"Subtractive")
#define ID_MAP_XPTYPE_ADD		_T(L"Additive")
#define ID_MAP_XPTYPE_OTH		_T(L"Other")

#define ID_BMP_FILT_PYR			_T(L"Pyramidal")
#define ID_BMP_FILT_SAT			_T(L"SAT")
#define ID_BMP_FILT_NONE		_T(L"None")

#define ID_FALLOFF_OUT			_T(L"Out")
#define ID_FALLOFF_IN			_T(L"In")
								
#define ID_MAPTYPE_EXP			_T(L"Explicit")
#define ID_MAPTYPE_SPH			_T(L"Spherical")
#define ID_MAPTYPE_CYL			_T(L"Cylindrical")
#define ID_MAPTYPE_SHR			_T(L"Shrinkwrap")
#define ID_MAPTYPE_SCR			_T(L"Screen")
								
#define ID_AMBIENT				_T(L"*MATERIAL_AMBIENT") 
#define ID_DIFFUSE				_T(L"*MATERIAL_DIFFUSE") 
#define ID_SPECULAR				_T(L"*MATERIAL_SPECULAR") 
#define ID_SHINE				_T(L"*MATERIAL_SHINE")
#define ID_SHINE_STRENGTH		_T(L"*MATERIAL_SHINESTRENGTH") 
#define ID_TRANSPARENCY			_T(L"*MATERIAL_TRANSPARENCY") 
#define ID_WIRESIZE				_T(L"*MATERIAL_WIRESIZE")
								
#define ID_SHADING				_T(L"*MATERIAL_SHADING") 
#define ID_XP_FALLOFF			_T(L"*MATERIAL_XP_FALLOFF") 
#define ID_SELFILLUM			_T(L"*MATERIAL_SELFILLUM") 
#define ID_TWOSIDED				_T(L"*MATERIAL_TWOSIDED") 
#define ID_WIRE					_T(L"*MATERIAL_WIRE")
#define ID_WIREUNITS			_T(L"*MATERIAL_WIREUNITS") 
#define ID_FALLOFF				_T(L"*MATERIAL_FALLOFF") 
#define ID_FACEMAP				_T(L"*MATERIAL_FACEMAP") 
#define ID_SOFTEN				_T(L"*MATERIAL_SOFTEN") 
#define ID_XP_TYPE				_T(L"*MATERIAL_XP_TYPE")
								
#define ID_TEXNAME				_T(L"*MAP_NAME") 
#define ID_TEXCLASS				_T(L"*MAP_CLASS") 
#define ID_TEXSUBNO				_T(L"*MAP_SUBNO") 
#define ID_TEXAMOUNT			_T(L"*MAP_AMOUNT")
								
#define ID_BITMAP				_T(L"*BITMAP")
#define ID_TEX_INVERT			_T(L"*BITMAP_INVERT") 
#define ID_BMP_FILTER			_T(L"*BITMAP_FILTER")
								
#define ID_MAPTYPE				_T(L"*MAP_TYPE") 
#define ID_U_OFFSET				_T(L"*UVW_U_OFFSET") 
#define ID_V_OFFSET				_T(L"*UVW_V_OFFSET") 
#define ID_U_TILING				_T(L"*UVW_U_TILING") 
#define ID_V_TILING				_T(L"*UVW_V_TILING") 
#define ID_ANGLE				_T(L"*UVW_ANGLE") 
#define ID_BLUR					_T(L"*UVW_BLUR")
#define ID_BLUR_OFFSET			_T(L"*UVW_BLUR_OFFSET") 
#define ID_NOISE_AMT			_T(L"*UVW_NOUSE_AMT") 
#define ID_NOISE_SIZE			_T(L"*UVW_NOISE_SIZE") 
#define ID_NOISE_LEVEL			_T(L"*UVW_NOISE_LEVEL") 
#define ID_NOISE_PHASE			_T(L"*UVW_NOISE_PHASE")
								
// Sub texture types			
#define ID_MAP_GENERIC			_T(L"*MAP_GENERIC") 
#define ID_MAP_AMBIENT			_T(L"*MAP_AMBIENT") 
#define ID_MAP_DIFFUSE			_T(L"*MAP_DIFFUSE") 
#define ID_MAP_SPECULAR			_T(L"*MAP_SPECULAR") 
#define ID_MAP_SHINE			_T(L"*MAP_SHINE")
#define ID_MAP_SHINESTRENGTH	_T(L"*MAP_SHINESTRENGTH") 
#define ID_MAP_SELFILLUM		_T(L"*MAP_SELFILLUM") 
#define ID_MAP_OPACITY			_T(L"*MAP_OPACITY")
#define ID_MAP_FILTERCOLOR		_T(L"*MAP_FILTERCOLOR") 
#define ID_MAP_BUMP				_T(L"*MAP_BUMP")
#define ID_MAP_REFLECT			_T(L"*MAP_REFLECT") 
#define ID_MAP_REFRACT			_T(L"*MAP_REFRACT")
								
// TM related ID's				
#define ID_TM_ROW0				_T(L"*TM_ROW0") 
#define ID_TM_ROW1				_T(L"*TM_ROW1") 
#define ID_TM_ROW2				_T(L"*TM_ROW2") 
#define ID_TM_ROW3				_T(L"*TM_ROW3") 
#define ID_TM_POS				_T(L"*TM_POS")
#define ID_TM_ROTAXIS			_T(L"*TM_ROTAXIS") 
#define ID_TM_ROTANGLE			_T(L"*TM_ROTANGLE") 
#define ID_TM_SCALE				_T(L"*TM_SCALE") 
#define ID_TM_SCALEAXIS			_T(L"*TM_SCALEAXIS")
#define ID_TM_SCALEAXISANG		_T(L"*TM_SCALEAXISANG") 
#define ID_TM_ANIMATION			_T(L"*TM_ANIMATION")

// TM Inheritance flags
#define ID_INHERIT_POS			_T(L"*INHERIT_POS")
#define ID_INHERIT_ROT			_T(L"*INHERIT_ROT")
#define ID_INHERIT_SCL			_T(L"*INHERIT_SCL")

// Scene related ID's			
#define ID_FILENAME				_T(L"*SCENE_FILENAME") 
#define ID_FIRSTFRAME			_T(L"*SCENE_FIRSTFRAME") 
#define ID_LASTFRAME			_T(L"*SCENE_LASTFRAME") 
#define ID_FRAMESPEED			_T(L"*SCENE_FRAMESPEED") 
#define ID_TICKSPERFRAME		_T(L"*SCENE_TICKSPERFRAME") 
#define ID_ENVMAP				_T(L"*SCENE_ENVMAP")
#define ID_STATICBGCOLOR		_T(L"*SCENE_BACKGROUND_STATIC") 
#define ID_ANIMBGCOLOR			_T(L"*SCENE_BACKGROUND_ANIM") 
#define ID_STATICAMBIENT		_T(L"*SCENE_AMBIENT_STATIC") 
#define ID_ANIMAMBIENT			_T(L"*SCENE_AMBIENT_ANIM")

#define ID_VISIBILITY_TRACK		_T(L"*NODE_VISIBILITY_TRACK")

// Generic ID's that can show up here and there 
#define ID_TIMEVALUE			_T(L"*TIMEVALUE")
#define ID_COMMENT				_T(L"*COMMENT")
#define ID_FILEID				_T(L"*3DSMAX_ASCIIEXPORT")
#define ID_BOUNDINGBOX_MIN		_T(L"*BOUNDINGBOX_MIN")
#define ID_BOUNDINGBOX_MAX		_T(L"*BOUNDINGBOX_MAX")

#endif 