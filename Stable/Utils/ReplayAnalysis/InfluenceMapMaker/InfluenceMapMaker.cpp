// InfluenceMapMaker.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

using namespace std;
using namespace cimg_library;

// 팔레트 생성 코드
//unsigned int c[3];
//CImg<unsigned int> imgP( 1020, 1, 1, 3 );
//int count = 0;
//for( int i = 0; i <= 255; ++i )
//{
//	c[0]= 0; c[1] = i; c[2] = 255;
//	imgP.draw_point( count++, 0, c );
//}
//for( int i = 255; i >= 0; --i )
//{
//	c[0]= 0; c[1] = 255; c[2] = i;
//	imgP.draw_point( count++, 0, c );
//}
//for( int i = 0; i <= 255; ++i )
//{
//	c[0]= i; c[1] = 255; c[2] = 0;
//	imgP.draw_point( count++, 0, c );
//}
//for( int i = 255; i >= 0; --i )
//{
//	c[0]= 255; c[1] = i; c[2] = 0;
//	imgP.draw_point( count++, 0, c );
//}
//imgP.resize( 256, 1, 1, 3 );
//imgP.save_cpp( "pallete.cpp" );

/* Define image size 256x1x1x3 and type 'unsigned int' */
unsigned int palette[] = { 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57,
  61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121,
  125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177, 181, 185,
  189, 193, 197, 201, 205, 209, 213, 217, 221, 225, 229, 233, 237, 241, 245, 249,
  253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  0, 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59,
  63, 67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123,
  127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179, 183, 187,
  191, 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 254, 250, 246, 242, 238, 234, 230, 226, 222, 218, 214, 210, 206, 202, 198,
  194, 190, 186, 182, 178, 174, 170, 166, 162, 158, 154, 150, 146, 142, 138, 134,
  130, 126, 122, 118, 114, 110, 106, 102, 98, 94, 90, 86, 82, 78, 74, 70,
  66, 62, 58, 54, 50, 46, 42, 38, 34, 30, 26, 22, 18, 14, 10, 6,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 252, 248, 244, 240, 236, 232, 228, 224, 220, 216, 212, 208, 204, 200, 196,
  192, 188, 184, 180, 176, 172, 168, 164, 160, 156, 152, 148, 144, 140, 136, 132,
  128, 124, 120, 116, 112, 108, 104, 100, 96, 92, 88, 84, 80, 76, 72, 68,
  64, 60, 56, 52, 48, 44, 40, 36, 32, 28, 24, 20, 16, 12, 8, 4,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const char* keyCSVFile = "CSV_FILE=";
const char* keyBGImg = "BG_IMG=";
const char* keyResultImg = "RESULT_IMG=";
const char* keyPointPower = "POINT_POWER=";
const char* keyPointSize = "POINT_SIZE=";
const char* keyAlphaMin = "ALPHA_MIN=";
const char* keyAlphaMax = "ALPHA_MAX=";
const char* keyBlurPower = "BLUR_POWER=";
const char* keyCamEyeX = "CAM_EYE_X=";
const char* keyCamEyeY = "CAM_EYE_Y=";
const char* keyCamEyeZ = "CAM_EYE_Z=";
const char* keyCamAtX = "CAM_AT_X=";
const char* keyCamAtY = "CAM_AT_Y=";
const char* keyCamAtZ = "CAM_AT_Z=";
const char* keyFovY = "CAM_FOV_Y=";

bool FindFile( const char* fileName )
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile( fileName, &findFileData );

	if( hFind != INVALID_HANDLE_VALUE )
		return true;

	FindClose( hFind );
	return false;
}

bool SetValue( const string& str, const char* keyString, string& out )
{
	string key = keyString;
	if( str.find( key ) != string::npos )
	{
		out = str.substr( strlen(keyString) );
		return true;
	}

	return false;
}

bool SetValue( const string& str, const char* keyString, unsigned int& out )
{
	string key = keyString;
	if( str.find( key ) != string::npos )
	{
		out = atoi( str.substr( strlen(keyString) ).c_str() );
		return true;
	}

	return false;
}

bool SetValue( const string& str, const char* keyString, float& out )
{
	string key = keyString;
	if( str.find( key ) != string::npos )
	{
		out = static_cast<float>( atof( str.substr( strlen(keyString) ).c_str() ) );
		return true;
	}

	return false;
}

void ConvertPoint( const D3DXMATRIX& matViewProj, const D3DVIEWPORT9& viewPort, D3DXVECTOR3& v )
{
  D3DXVec3TransformCoord ( &v, &v, &matViewProj );

  v.x += 1.0f;
  v.y += 1.0f;
  
  v.x = ( viewPort.Width * v.x ) / 2.0f + viewPort.X;
  v.y = ( viewPort.Height * ( 2.0f - v.y ) ) / 2.0f + viewPort.Y;
}

int _tmain( int argc, _TCHAR* argv[] )
{
	if( argc < 2 )
	{
		cout << "InfluenceMapMaker.exe [맵 설정 파일]\n";
		getchar();
		return -1;
	}

	string csvFileName;
	string bgFileName;
	string resultFileName;
	float pointPower = 5.0f;
	float pointSize = 10.0f;
	unsigned int alphaMin = 0;
	unsigned int alphaMax = 255;
	float blurPower = 1.5f;
	D3DXVECTOR3 camEye( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 camAt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 camUp( 0.0f, 1.0f, 0.0f );
	float camFovY = 1.0f;

	{
		if( !FindFile( argv[1] ) )
		{
			cout << "에러: " << argv[1] << "파일이 없습니다.\n";
			return -1;
		}
		ifstream ifile;
		ifile.open( argv[1] );
		ws( ifile );

		char buff[MAX_PATH];
		for( int i = 0; i < 15; ++i )
		{
			ifile.getline( buff, MAX_PATH );

			if( ifile.rdstate() & ios::failbit )
				break;

			string str = buff;

			if( SetValue( str, keyCSVFile, csvFileName ) )
				continue;

			if( SetValue( str, keyBGImg, bgFileName ) )
				continue;

			if( SetValue( str, keyResultImg, resultFileName ) )
				continue;

			if( SetValue( str, keyPointPower, pointPower ) )
				continue;

			if( SetValue( str, keyPointSize, pointSize ) )
				continue;

			if( SetValue( str, keyAlphaMin, alphaMin ) )
				continue;

			if( SetValue( str, keyAlphaMax, alphaMax ) )
				continue;

			if( SetValue( str, keyBlurPower, blurPower ) )
				continue;

			if( SetValue( str, keyCamEyeX, camEye.x ) )
				continue;

			if( SetValue( str, keyCamEyeY, camEye.y ) )
				continue;

			if( SetValue( str, keyCamEyeZ, camEye.z ) )
				continue;

			if( SetValue( str, keyCamAtX, camAt.x ) )
				continue;

			if( SetValue( str, keyCamAtY, camAt.y ) )
				continue;

			if( SetValue( str, keyCamAtZ, camAt.z ) )
				continue;

			if( SetValue( str, keyFovY, camFovY ) )
				continue;
		}
		ifile.close();
	}

	if( !FindFile( bgFileName.c_str() ) )
	{
		cout << "에러: 배경 이미지 파일이 없습니다. - " << bgFileName.c_str() << "\n";
		return -1;
	}

	// CSV 파일 열기
	if( !FindFile( csvFileName.c_str() ) )
	{
		cout << "에러: CSV 파일이 없습니다. - " << csvFileName.c_str() << "\n";
		return -1;
	}

	// 팔레트 이미지 생성
	CImg<unsigned int> imgPalette( palette, 256, 1, 1, 3 );

	// 배경 이미지 로드
	CImg<unsigned int> imgBack;
	imgBack.load( bgFileName.c_str() );

	const int imgWidth = imgBack.dimx();
	const int imgHeight = imgBack.dimy();

	CImg<unsigned int> img( imgWidth, imgHeight, 1, 4 );
	CImgList<unsigned int> colors;
	CImgList<unsigned int> blobs;
//	CImgDisplay disp( img, "", 0 );

	camEye.x = -camEye.x;
	camAt.x = -camAt.x;

	D3DXMATRIX view;
	D3DXMatrixLookAtLH( &view, &camEye, &camAt, &camUp );

	float aspectRatio = (float)imgWidth / (float)imgHeight;
	float fov = D3DXToRadian( camFovY );
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, fov, aspectRatio, 1.0f, 1000.0f );

	D3DXMATRIX viewProj;
	D3DXMatrixMultiply( &viewProj, &view, &proj );

	D3DVIEWPORT9 viewPort;
	viewPort.X = 0;
	viewPort.Y = 0;
	viewPort.MinZ = 0.0f;
	viewPort.MaxZ = 1.0f;
	viewPort.Width = imgWidth;
	viewPort.Height = imgHeight;

	{
		FILE* file = fopen( csvFileName.c_str(), "r" );
		char seps[] = " ,\t\n";
		char* token;
		char buff[MAX_PATH];
		while( fgets( buff, MAX_PATH, file ) )
		{
			token = strtok( buff, seps );
			float time = (float)atof( token );

			token = strtok( NULL, seps );
			float x = (float)atof( token );

			token = strtok( NULL, seps );
			float y = (float)atof( token );

			token = strtok( NULL, seps );
			float z = (float)atof( token );

			D3DXVECTOR3 v( x, y, z );
			ConvertPoint( viewProj, viewPort, v );
			if( v.x >= 0.0f && v.x < imgWidth && v.y >= 0.0f && v.y < imgHeight )
			{
				blobs.insert( CImg<>::vector( v.x, v.y, pointSize ) );
				colors.insert( CImg<>(3).fill(pointPower) );
			}
		}
		fclose( file );
	}

	img.fill( 0 );
	if( blobs )
	{
		float dist = 0;

		cimglist_for( blobs, l )
		{
			const CImg<>& blob = blobs[l];

			const float xb = blob[0];
			const float yb = blob[1];
			const float rb = blob[2];

			const float sigma = (float)(rb*(1+0.05f*std::cos(blob[3])));
			const float sigma2 = 2*sigma*sigma;
			const float precision = 4.5f*sigma2;

			const int tx0 = (int)(xb-3*sigma);
			const int ty0 = (int)(yb-3*sigma);
			const int tx1 = (int)(xb+3*sigma);
			const int ty1 = (int)(yb+3*sigma);

			const unsigned int col1 = colors[l](0);
			const unsigned int col2 = colors[l](1);
			const unsigned int col3 = colors[l](2);
			const unsigned int wh = img.dimx() * img.dimy();
			const unsigned int x0 = tx0 < 0 ? 0 : tx0;
			const unsigned int y0 = ty0 < 0 ? 0 : ty0;
			const unsigned int x1 = tx1 >= img.dimx() ? (img.dimx()-1) : tx1;
			const unsigned int y1 = ty1 >= img.dimy() ? (img.dimy()-1) : ty1;

			float dy = y0 - yb;
			unsigned int *ptr = img.ptr( x0, y0 );
			for( unsigned int y = y0; y <= y1; ++y )
			{
				float dx = x0 - xb;
				for( unsigned int x = x0; x <= x1; ++x )
				{
					float dist = dx * dx + dy * dy;
					if( dist < precision )
					{
						const float val = (float)exp( -dist / sigma2 );
						*ptr += (unsigned int)( val * col1 );
						*(ptr+wh) += (unsigned int)( val * col2 );
						*(ptr+2*wh) += (unsigned int)( val * col3 );
					}
					++dx;
					++ptr;
				}
				ptr += img.dimx() - ( x1 - x0 ) - 1;
				++dy;
			}
		}

		unsigned int *ptr1 = img.ptr(0,0,0,1);
		unsigned int *ptr2 = img.ptr(0,0,0,2);
		unsigned int *ptr3 = img.ptr(0,0,0,3);
		unsigned int *ptr4 = img.ptr(0,0,0,4);
		for( unsigned int off = 0; ptr1 > img.data; ++off )
		{
			unsigned int val = *(--ptr1);
			--ptr2;
			--ptr3;
			--ptr4;

			val = val < 255 ? val : 255;

			if( val == 0 )
			{
				*ptr4 = 0;
			}
			else
			{
				*ptr4 = val + alphaMin;
				*ptr4 = *ptr4 < alphaMax ? *ptr4 : alphaMax;
			}

			unsigned int *palptr1 = imgPalette.ptr(val,0,0,0);
			unsigned int *palptr2 = imgPalette.ptr(val,0,0,1);
			unsigned int *palptr3 = imgPalette.ptr(val,0,0,2);

			*ptr1 = *palptr1;
			*ptr2 = *palptr2;
			*ptr3 = *palptr3;
		}
	}

	img.blur( blurPower );

	unsigned int *ptrSrc1 = img.ptr(0,0,0,1);
	unsigned int *ptrSrc2 = img.ptr(0,0,0,2);
	unsigned int *ptrSrc3 = img.ptr(0,0,0,3);
	unsigned int *ptrSrc4 = img.ptr(0,0,0,4);

	unsigned int *ptrDest1 = imgBack.ptr(0,0,0,1);
	unsigned int *ptrDest2 = imgBack.ptr(0,0,0,2);
	unsigned int *ptrDest3 = imgBack.ptr(0,0,0,3);
	for( unsigned int off = 0; ptrDest1 > imgBack.data; ++off )
	{
		--ptrSrc1;
		--ptrSrc2;
		--ptrSrc3;
		--ptrSrc4;

		--ptrDest1;
		--ptrDest2;
		--ptrDest3;

		const unsigned int alpha = *ptrSrc4;
		const unsigned int alphaInv = 255 - alpha;

		*ptrDest1 = ( *ptrDest1 * alphaInv + *ptrSrc1 * alpha ) / 255;
		*ptrDest2 = ( *ptrDest2 * alphaInv + *ptrSrc2 * alpha ) / 255;
		*ptrDest3 = ( *ptrDest3 * alphaInv + *ptrSrc3 * alpha ) / 255;
	}

//	imgBack.display( disp.wait( 25 ) );
	imgBack.save( resultFileName.c_str() );

//	while( !disp.is_closed && !disp.is_keyESC ) {}
	return 0;
}