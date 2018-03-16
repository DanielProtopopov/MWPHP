#include <phpcpp.h>
#include <iostream>
#include <jpeglib.h>
#include "BarcodeScanner.h"
#include "MWParser.h"
#include "MWResult.h"

using namespace std;

uint8_t *read_jpeg_file( char *filename, int *imageWidth, int *imageHeight, int *imageBitplanes)
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1];
	
	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;
	int i = 0;

	/* here we set up the standard libjpeg error handler */
	cinfo.err = jpeg_std_error( &jerr );
	/* setup decompression process and source, then read JPEG header */

	jpeg_create_decompress( &cinfo );

	/* this makes the library read from infile */
	jpeg_stdio_src( &cinfo, infile );

	/* reading the image header which contains image information */
	jpeg_read_header( &cinfo, TRUE );
	/* Uncomment the following to output image information, if needed. */
	/* Start decompression jpeg here */

	jpeg_start_decompress( &cinfo );

	/* allocate memory to hold the uncompressed image */
	uint8_t *raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
	/* now actually read the jpeg into the raw buffer */

	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );

	*imageWidth =  cinfo.output_width;
	*imageHeight =  cinfo.image_height;
    *imageBitplanes = cinfo.num_components;

	/* read one scan line at a time */
	int line = 0;
	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++) 
			raw_image[location++] = row_pointer[0][i];
		line++;
	}


	/* wrap up decompression, destroy objects, free pointers and close open files */
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );

	free( row_pointer[0] );
	fclose( infile );

	/* yup, we succeeded! */
	return raw_image;
}

Php::Value getLibVersion()
{
	unsigned int version = MWB_getLibVersion();
	return Php::Value((int)version);
}

Php::Value getSupportedCodes()
{
	unsigned int codes = MWB_getSupportedCodes();
	return Php::Value((int)codes);
}

Php::Value setScanningRect(Php::Parameters &params)
{
	int codeMask = params[0];
	float left = (double)params[1];
	float top = (double)params[2];
	float width = (double)params[3];
	float height = (double)params[4];
	
	int retScanningRect = MWB_setScanningRect((uint32_t)codeMask, left, top, width, height);
	return Php::Value(retScanningRect == MWB_RT_OK ? true : retScanningRect);
}

Php::Value getScanningRect(Php::Parameters &params)
{
	int codeMask = params[0];
	float left = 0, top = 0, width = 0, height = 0;
	int retScanningRect = MWB_getScanningRect((uint32_t)codeMask, &left, &top, &width, &height);

	Php::Value scanningRect;
	if(retScanningRect == MWB_RT_OK) {
		scanningRect["left"] = (double)left;
		scanningRect["top"] = (double)top;
		scanningRect["width"] = (double)width;
		scanningRect["height"] = (double)height;
	}
	return scanningRect;
}

Php::Value registerSDK(Php::Parameters &params)
{
	std::string key = params[0];
	int registration = MWB_registerSDK(key.c_str());
	return Php::Value(registration == MWB_RT_OK ? true : registration);
}

Php::Value setActiveCodes(Php::Parameters &params)
{
	int codeMask = params[0];
	int setCodes = MWB_setActiveCodes((uint32_t)codeMask);
	return Php::Value(setCodes == MWB_RT_OK ? true : setCodes);
}

Php::Value getActiveCodes()
{
	int getCodes = MWB_getActiveCodes();
	return Php::Value(getCodes);
}

Php::Value enableCode(Php::Parameters &params)
{
	int codeMask = params[0];
	int enableCode = MWB_enableCode((uint32_t)codeMask);
	return Php::Value(enableCode == MWB_RT_OK ? true : enableCode);
}

Php::Value disableCode(Php::Parameters &params)
{
	int codeMask = params[0];
	int disableCode = MWB_disableCode((uint32_t)codeMask);
	return Php::Value(disableCode == MWB_RT_OK ? true : disableCode);
}

Php::Value setActiveSubcodes(Php::Parameters &params)
{
	int codeMask = params[0];
	int subMask = params[1];
	int setActive = MWB_setActiveSubcodes((uint32_t)codeMask, (uint32_t)subMask);
	return Php::Value(setActive == MWB_RT_OK ? true : setActive);
}

Php::Value getActiveSubcodes(Php::Parameters &params)
{
	int codeMask = params[0];
	int getActive = MWB_getActiveSubcodes((uint32_t)codeMask);
	return Php::Value(getActive);
}

Php::Value enableSubcode(Php::Parameters &params)
{
	int codeMask = params[0];
	int subMask = params[1];
	int enableSubcode = MWB_enableSubcode((uint32_t)codeMask, (uint32_t)subMask);
	return Php::Value(enableSubcode == MWB_RT_OK ? true : enableSubcode);
}

Php::Value disableSubcode(Php::Parameters &params)
{
	int codeMask = params[0];
	int subMask = params[1];
	int disableSubcode = MWB_disableSubcode((uint32_t)codeMask, (uint32_t)subMask);
	return Php::Value(disableSubcode == MWB_RT_OK ? true : disableSubcode);
}

Php::Value setCodePriority(Php::Parameters &params)
{
	int codeMask = params[0];
	int priority = params[1];
	int setPriority = MWB_setCodePriority((uint32_t)codeMask, (uint8_t)priority);
	return Php::Value(setPriority == MWB_RT_OK ? true : setPriority);
}

Php::Value cleanupLib()
{
	int cleanup = MWB_cleanupLib();
	return Php::Value(cleanup);
}

Php::Value getLastType()
{
	int lastType = MWB_getLastType();
	return Php::Value(lastType);
}

Php::Value isLastGS1()
{
	int isLast = MWB_isLastGS1();
	return Php::Value(isLast);
}

Php::Value scanGrayscaleImage(Php::Parameters &params)
{
	std::string filename = params[0];
	
	if(filename.find("jpg") == std::string::npos && filename.find("jpeg") == std::string::npos){
		return Php::Value("This function only works with JPEG/JPG files");
	}
	int parserMask = params[3];
	
	Php::Value Return;

	uint8_t *pData = NULL;

	int width = 0, height = 0, channels = 0;
	uint8_t *bytes = read_jpeg_file(const_cast<char *>(filename.c_str()), &width, &height, &channels);
	
	uint8_t *grayscaleBytes = (uint8_t *)malloc(width * height * sizeof(uint8_t));
	
	//Conversion to grayscale
    int x, y;
	for (y = 0; y < height; y++) {
		int srcOffset = y * width;
		int dstOffset = (y * width) *channels;
		for (x = 0; x < width; x++) {
			grayscaleBytes[srcOffset] = bytes[dstOffset];
			srcOffset++;
			dstOffset += channels;
		}
	}
	
	int retval = MWB_scanGrayscaleImage(grayscaleBytes, width, height, &pData);
	
	free(bytes);
	free(grayscaleBytes);
	if (retval > 0) {

		MWResults *mwResults = new MWResults(pData);
		free(pData);
		MWResult *mwResult = NULL;
		if (mwResults != NULL) {
			mwResult = mwResults->getResults().back();
			if (mwResult) {
				Return["barcode"] = mwResult->text;
				if (mwResult->locationPoints) {
					Php::Value Rect;
					Php::Value p1, p2, p3, p4;
					p1["x"] = mwResult->locationPoints->p1->x;
					p1["y"] = mwResult->locationPoints->p1->y;
					p2["x"] = mwResult->locationPoints->p2->x;
					p2["y"] = mwResult->locationPoints->p2->y;
					p3["x"] = mwResult->locationPoints->p3->x;
					p3["y"] = mwResult->locationPoints->p3->y;
					p4["x"] = mwResult->locationPoints->p4->x;
					p4["y"] = mwResult->locationPoints->p4->y;
					Rect[0] = p1;
					Rect[1] = p2;
					Rect[2] = p3;
					Rect[3] = p4;
					Return["rect"] = Rect;
				}
				if (mwResult->isGS1) {

					uint8_t * parserResult = NULL;
					double parserRes = -1;

					//USE THIS CODE FOR JSONFORMATTED RESULT
					parserRes = MWP_getJSON(parserMask, (unsigned char *)mwResult->parserInput, mwResult->bytesLength, &parserResult);
					Return["json"] = std::string(reinterpret_cast<const char*>(parserResult));
					free(parserResult);
					//use jsonString to get the JSON formatted result
					if (parserRes >= 0) {
						switch (parserMask) {
						case MWP_PARSER_MASK_GS1:
							Return["mask"] = "GS1";
							break;
						case MWP_PARSER_MASK_IUID:
							Return["mask"] = "IUID";
							break;
						case MWP_PARSER_MASK_ISBT:
							Return["mask"] = "ISBT";
							break;
						case MWP_PARSER_MASK_AAMVA:
							Return["mask"] = "AAMVA";
							break;
						case MWP_PARSER_MASK_HIBC:
							Return["mask"] = "HIBC";
							break;
						case MWP_PARSER_MASK_SCM:
							Return["mask"] = "SCM";
							break;
						default:
							Return["mask"] = "unknown";
							break;
						}
					}
				}
			}
			delete mwResults;
		}
	}

	return Return;
}

Php::Value setFlags(Php::Parameters &params)
{
	int codeMask = params[0];
	int flags = params[1];
	int setFlags = MWB_setFlags((uint32_t)codeMask, (uint32_t)flags);
	return Php::Value(setFlags == MWB_RT_OK ? true : setFlags);
}

Php::Value enableFlag(Php::Parameters &params)
{
	int codeMask = params[0];
	int flag = params[1];
	int enableFlag = MWB_enableFlag((uint32_t)codeMask, (uint32_t)flag);
	return Php::Value(enableFlag == MWB_RT_OK ? true : enableFlag);
}

Php::Value disableFlag(Php::Parameters &params)
{
	int codeMask = params[0];
	int flag = params[1];
	int disableFlag = MWB_disableFlag((uint32_t)codeMask, (uint32_t)flag);
	return Php::Value(disableFlag == MWB_RT_OK ? true : disableFlag);
}

Php::Value getFlags(Php::Parameters &params)
{
	int codeMask = params[0];
	int getFlags = MWB_getFlags((uint32_t)codeMask);
	return Php::Value(getFlags);
}

Php::Value setLevel(Php::Parameters &params)
{
	int level = params[0];
	int setLevel = MWB_setLevel(level);
	return Php::Value(setLevel == MWB_RT_OK ? true : setLevel);
}

Php::Value setDirection(Php::Parameters &params)
{
	int direction = params[0];
	int setDirection = MWB_setDirection((uint32_t)direction);
	return Php::Value(setDirection == MWB_RT_OK ? true : setDirection);
}

Php::Value setMinLength(Php::Parameters &params)
{
	int codeMask = params[0];
	int minLength = params[1];

	int setMinLength = MWB_setMinLength((uint32_t)codeMask, (uint32_t)minLength);
	return Php::Value(setMinLength == MWB_RT_OK ? true : setMinLength);	
}

Php::Value setParam(Php::Parameters &params)
{
	int codeMask = params[0];
	int paramId = params[1];
	int paramValue = params[2];

	int setParam = MWB_setParam((uint32_t)codeMask, (uint32_t)paramId, (uint32_t)paramValue);
	return Php::Value(setParam == MWB_RT_OK ? true : setParam);	
}

Php::Value getParam(Php::Parameters &params)
{
	int codeMask = params[0];
	int paramId = params[1];

	int getParam = MWB_getParam((uint32_t)codeMask, (uint32_t)paramId);
	return Php::Value(getParam);
}

Php::Value getDirection()
{
	int retDirection = MWB_getDirection();
	return Php::Value(retDirection);
}

Php::Value getBarcodeLocation()
{
	float *points = new float[8];
	int getLocation = MWB_getBarcodeLocation(points);
	
	Php::Value Rect;
	if(getLocation == MWB_RT_OK) {
		Php::Value p1, p2, p3, p4;
		p1["x"] = points[0];
		p1["y"] = points[1];
		p2["x"] = points[2];
		p2["y"] = points[3];
		p3["x"] = points[4];
		p3["y"] = points[5];
		p4["x"] = points[6];
		p4["y"] = points[7];
		Rect[0] = p1;
		Rect[1] = p2;
		Rect[2] = p3;
		Rect[3] = p4;
	}
	delete points;
	return Rect;
}

Php::Value setResultType(Php::Parameters &params)
{
	int resultType = params[0];
		
	int setResultType = MWB_setResultType((uint32_t)resultType);
	return Php::Value(setResultType == MWB_RT_OK ? true : setResultType);
}

Php::Value getResultType()
{	
	int getResultType = MWB_getResultType();
	return Php::Value(getResultType);
}

Php::Value setDuplicatesTimeout(Php::Parameters &params)
{
	int timeout = params[0];
		
	int setTimeout = MWB_setDuplicatesTimeout((uint32_t)timeout);
	return Php::Value(setTimeout == MWB_RT_OK ? true : setTimeout);
}

Php::Value setDuplicate(Php::Parameters &params)
{
	uint8_t *barcode = new uint8_t[params[0].size()];
	for (int i = 0; i < params[0].size(); i++)
	{
		int value = params[0][i];
		barcode[i] = (uint8_t)value;
	}
	int length = params[1];
	
	MWB_setDuplicate(barcode, length);
	delete barcode;
	return true;
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

	/**
	 *  Function that is called by PHP right after the PHP process
	 *  has started, and that returns an address of an internal PHP
	 *  structure with all the details and features of your extension
	 *
	 *  @return void*   a pointer to an address that is understood by PHP
	 */
	PHPCPP_EXPORT void *get_module()
	{
		// static(!) Php::Extension object that should stay in memory
		// for the entire duration of the process (that's why it's static)
		static Php::Extension extension("mw_barcode_scanner", "1.2");

		extension.onShutdown([]() {
			MWB_cleanupLib();
		});

		extension.add<getLibVersion>("getLibVersion");
		extension.add<getSupportedCodes>("getSupportedCodes");
		extension.add<setScanningRect>("setScanningRect");
		extension.add<getScanningRect>("getScanningRect");
		extension.add<registerSDK>("registerSDK");
		extension.add<setActiveCodes>("setActiveCodes");
		extension.add<getActiveCodes>("getActiveCodes");
		extension.add<enableCode>("enableCode");
		extension.add<disableCode>("disableCode");
		extension.add<setActiveSubcodes>("setActiveSubcodes");
		extension.add<getActiveSubcodes>("getActiveSubcodes");
		extension.add<enableSubcode>("enableSubCode");
		extension.add<disableSubcode>("disableSubCode");
		extension.add<setCodePriority>("setCodePriority");
		extension.add<cleanupLib>("cleanupLib");
		extension.add<getLastType>("getLastType");
		extension.add<isLastGS1>("isLastGS1");
		extension.add<scanGrayscaleImage>("scanGrayscaleImage");
		extension.add<setFlags>("setFlags");
		extension.add<enableFlag>("enableFlag");
		extension.add<disableFlag>("disableFlag");
		extension.add<getFlags>("getFlags");
		extension.add<setLevel>("setLevel");
		extension.add<setDirection>("setDirection");
		extension.add<setMinLength>("setMinLength");
		extension.add<setParam>("setParam");
		extension.add<getParam>("getParam");
		extension.add<getDirection>("getDirection");
		extension.add<getBarcodeLocation>("getBarcodeLocation");
		extension.add<setResultType>("setResultType");
		extension.add<getResultType>("getResultType");
		extension.add<setDuplicatesTimeout>("setDuplicatesTimeout");
		extension.add<setDuplicate>("setDuplicate");

		extension.add(Php::Constant("MWB_GRAYSCALE_LENX_MIN", 10));
		extension.add(Php::Constant("MWB_GRAYSCALE_LENX_MAX", 3000));
		extension.add(Php::Constant("MWB_GRAYSCALE_LENY_MIN", 10));
		extension.add(Php::Constant("MWB_GRAYSCALE_LENY_MAX", 3000));

		extension.add(Php::Constant("MWB_RT_OK", 0));
		extension.add(Php::Constant("MWB_RT_FAIL", -1));
		extension.add(Php::Constant("MWB_RT_NOT_SUPPORTED", -2));
		extension.add(Php::Constant("MWB_RT_BAD_PARAM", -3));

		extension.add(Php::Constant("MWB_RTREG_OK", 0));
		extension.add(Php::Constant("MWB_RTREG_INVALID_KEY", -1));
		extension.add(Php::Constant("MWB_RTREG_INVALID_CHECKSUM", -2));
		extension.add(Php::Constant("MWB_RTREG_INVALID_APPLICATION", -3));
		extension.add(Php::Constant("MWB_RTREG_INVALID_SDK_VERSION", -4));
		extension.add(Php::Constant("MWB_RTREG_INVALID_KEY_VERSION", -5));
		extension.add(Php::Constant("MWB_RTREG_INVALID_PLATFORM", -6));
		extension.add(Php::Constant("MWB_RTREG_KEY_EXPIRED", -7));
		extension.add(Php::Constant("MWB_RTREG_AIMER_REQUIRED", -8));
		extension.add(Php::Constant("MWB_RTREG_AIMER_NOT_DETECTED", -9));

		extension.add(Php::Constant("MWB_CFG_GLOBAL_HORIZONTAL_SHARPENING", 0x01));
		extension.add(Php::Constant("MWB_CFG_GLOBAL_VERTICAL_SHARPENING", 0x02));
		extension.add(Php::Constant("MWB_CFG_GLOBAL_SHARPENING", 0x03));

		extension.add(Php::Constant("MWB_CFG_GLOBAL_ROTATE90", 0x04));
		extension.add(Php::Constant("MWB_CFG_GLOBAL_ROTATE180", 0x08));

		extension.add(Php::Constant("MWB_CFG_GLOBAL_CALCULATE_1D_LOCATION", 0x10));
		extension.add(Php::Constant("MWB_CFG_GLOBAL_VERIFY_1D_LOCATION", 0x20));

		extension.add(Php::Constant("MWB_CFG_GLOBAL_USE_CENTRIC_SCANNING", 0x40));

		extension.add(Php::Constant("MWB_CFG_GLOBAL_DISABLE_PREPROCESSING", 0x80));

		extension.add(Php::Constant("MWB_CFG_CODE39_REQ_CHKSUM", 0x2));

		extension.add(Php::Constant("MWB_CFG_CODE39_DONT_REQUIRE_STOP", 0x4));

		extension.add(Php::Constant("MWB_CFG_CODE39_EXTENDED_MODE", 0x8));

		extension.add(Php::Constant("MWB_CFG_CODE39_CODE32_ENABLED", 0x10));

		extension.add(Php::Constant("MWB_CFG_CODE39_CODE32_PREFIX", 0x20));

		extension.add(Php::Constant("MWB_CFG_CODE93_EXTENDED_MODE", 0x8));

		extension.add(Php::Constant("MWB_CFG_EANUPC_DISABLE_ADDON", 0x1));

		extension.add(Php::Constant("MWB_CFG_EANUPC_DONT_EXPAND_UPCE", 0x2));

		extension.add(Php::Constant("MWB_CFG_CODE25_REQ_CHKSUM", 0x1));

		extension.add(Php::Constant("MWB_CFG_CODE11_REQ_SINGLE_CHKSUM", 0x1));
		extension.add(Php::Constant("MWB_CFG_CODE11_REQ_DOUBLE_CHKSUM", 0x2));

		extension.add(Php::Constant("MWB_CFG_MSI_REQ_10_CHKSUM", 0x01));
		extension.add(Php::Constant("MWB_CFG_MSI_REQ_1010_CHKSUM", 0x02));
		extension.add(Php::Constant("MWB_CFG_MSI_REQ_11_IBM_CHKSUM", 0x04));
		extension.add(Php::Constant("MWB_CFG_MSI_REQ_11_NCR_CHKSUM", 0x08));
		extension.add(Php::Constant("MWB_CFG_MSI_REQ_1110_IBM_CHKSUM", 0x10));
		extension.add(Php::Constant("MWB_CFG_MSI_REQ_1110_NCR_CHKSUM", 0x20));

		extension.add(Php::Constant("MWB_CFG_CODABAR_INCLUDE_STARTSTOP", 0x1));

		extension.add(Php::Constant("MWB_PAR_ID_ECI_MODE", 0x08));
		extension.add(Php::Constant("MWB_PAR_ID_RESULT_PREFIX", 0x10));
		extension.add(Php::Constant("MWB_PAR_ID_VERIFY_LOCATION", 0x20));

		extension.add(Php::Constant("MWB_PAR_VALUE_ECI_DISABLED", 0x00 ));//default
		extension.add(Php::Constant("MWB_PAR_VALUE_ECI_ENABLED", 0x01));

		extension.add(Php::Constant("MWB_PAR_VALUE_RESULT_PREFIX_NEVER", 0x00 ));// default
		extension.add(Php::Constant("MWB_PAR_VALUE_RESULT_PREFIX_ALWAYS", 0x01));
		extension.add(Php::Constant("MWB_PAR_VALUE_RESULT_PREFIX_DEFAULT", 0x02));

		extension.add(Php::Constant("MWB_PAR_VALUE_VERIFY_LOCATION_OFF", 0x00));
		extension.add(Php::Constant("MWB_PAR_VALUE_VERIFY_LOCATION_ON", 0x01));

		extension.add(Php::Constant("MWB_CODE_MASK_NONE", 0x00000000));
		extension.add(Php::Constant("MWB_CODE_MASK_QR", 0x00000001));
		extension.add(Php::Constant("MWB_CODE_MASK_DM", 0x00000002));
		extension.add(Php::Constant("MWB_CODE_MASK_RSS", 0x00000004));
		extension.add(Php::Constant("MWB_CODE_MASK_39", 0x00000008));
		extension.add(Php::Constant("MWB_CODE_MASK_EANUPC", 0x00000010));
		extension.add(Php::Constant("MWB_CODE_MASK_128", 0x00000020));
		extension.add(Php::Constant("MWB_CODE_MASK_PDF", 0x00000040));
		extension.add(Php::Constant("MWB_CODE_MASK_AZTEC", 0x00000080));
		extension.add(Php::Constant("MWB_CODE_MASK_25", 0x00000100));
		extension.add(Php::Constant("MWB_CODE_MASK_93", 0x00000200));
		extension.add(Php::Constant("MWB_CODE_MASK_CODABAR", 0x00000400));
		extension.add(Php::Constant("MWB_CODE_MASK_DOTCODE", 0x00000800));
		extension.add(Php::Constant("MWB_CODE_MASK_11", 0x00001000));
		extension.add(Php::Constant("MWB_CODE_MASK_MSI", 0x00002000));
		extension.add(Php::Constant("MWB_CODE_MASK_MAXICODE", 0x00004000));
		extension.add(Php::Constant("MWB_CODE_MASK_POSTAL", 0x00008000));
		extension.add(Php::Constant("MWB_CODE_MASK_ALL", 0x00ffffff));

		extension.add(Php::Constant("MWB_SUBC_MASK_RSS_14", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_RSS_14_STACK", 0x00000002));
		extension.add(Php::Constant("MWB_SUBC_MASK_RSS_LIM", 0x00000004));
		extension.add(Php::Constant("MWB_SUBC_MASK_RSS_EXP", 0x00000008));

		extension.add(Php::Constant("MWB_SUBC_MASK_QR_STANDARD", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_QR_MICRO", 0x00000002));

		extension.add(Php::Constant("MWB_SUBC_MASK_PDF_STANDARD", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_PDF_MICRO", 0x00000002));

		extension.add(Php::Constant("MWB_SUBC_MASK_C25_INTERLEAVED", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_STANDARD", 0x00000002));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_ITF14", 0x00000004));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_IATA", 0x00000008));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_MATRIX", 0x00000010));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_COOP", 0x00000020));
		extension.add(Php::Constant("MWB_SUBC_MASK_C25_INVERTED", 0x00000040));

		extension.add(Php::Constant("MWB_SUBC_MASK_POSTAL_POSTNET", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_POSTAL_PLANET", 0x00000002));
		extension.add(Php::Constant("MWB_SUBC_MASK_POSTAL_IM", 0x00000004));
		extension.add(Php::Constant("MWB_SUBC_MASK_POSTAL_ROYAL", 0x00000008));
		extension.add(Php::Constant("MWB_SUBC_MASK_POSTAL_AUSTRALIAN", 0x00000010));

		extension.add(Php::Constant("MWB_SUBC_MASK_EANUPC_EAN_13", 0x00000001));
		extension.add(Php::Constant("MWB_SUBC_MASK_EANUPC_EAN_8", 0x00000002));
		extension.add(Php::Constant("MWB_SUBC_MASK_EANUPC_UPC_A", 0x00000004));
		extension.add(Php::Constant("MWB_SUBC_MASK_EANUPC_UPC_E", 0x00000008));
		extension.add(Php::Constant("MWB_SUBC_MASK_EANUPC_UPC_E1", 0x00000010));

		extension.add(Php::Constant("MWB_SCANDIRECTION_HORIZONTAL", 0x00000001));
		extension.add(Php::Constant("MWB_SCANDIRECTION_VERTICAL", 0x00000002));
		extension.add(Php::Constant("MWB_SCANDIRECTION_OMNI", 0x00000004));
		extension.add(Php::Constant("MWB_SCANDIRECTION_AUTODETECT", 0x00000008));

		extension.add(Php::Constant("MWB_RESULT_TYPE_RAW", 0x00000001));
		extension.add(Php::Constant("MWB_RESULT_TYPE_MW", 0x00000002));

		extension.add(Php::Constant("MWB_RESULT_FT_BYTES", 0x00000001));
		extension.add(Php::Constant("MWB_RESULT_FT_TEXT", 0x00000002));
		extension.add(Php::Constant("MWB_RESULT_FT_TYPE", 0x00000003));
		extension.add(Php::Constant("MWB_RESULT_FT_SUBTYPE", 0x00000004));
		extension.add(Php::Constant("MWB_RESULT_FT_SUCCESS", 0x00000005));
		extension.add(Php::Constant("MWB_RESULT_FT_ISGS1", 0x00000006));
		extension.add(Php::Constant("MWB_RESULT_FT_LOCATION", 0x00000007));
		extension.add(Php::Constant("MWB_RESULT_FT_IMAGE_WIDTH", 0x00000008));
		extension.add(Php::Constant("MWB_RESULT_FT_IMAGE_HEIGHT", 0x00000009));
		extension.add(Php::Constant("MWB_RESULT_FT_PARSER_BYTES", 0x0000000A));

		extension.add(Php::Constant("MWB_RESULT_FT_MODULES_COUNT_X", 0x0000000B));
		extension.add(Php::Constant("MWB_RESULT_FT_MODULES_COUNT_Y", 0x0000000C));
		extension.add(Php::Constant("MWB_RESULT_FT_MODULE_SIZE_X", 0x0000000D));
		extension.add(Php::Constant("MWB_RESULT_FT_MODULE_SIZE_Y", 0x0000000E));
		extension.add(Php::Constant("MWB_RESULT_FT_SKEW", 0x0000000F));
		extension.add(Php::Constant("MWB_RESULT_FT_KANJI", 0x00000010));

		extension.add(Php::Constant("MWB_RESULT_FT_BARCODE_WIDTH", 0x00000011));
		extension.add(Php::Constant("MWB_RESULT_FT_BARCODE_HEIGHT", 0x00000012));

		extension.add(Php::Constant("MWB_RESULT_FT_PDF_ROWS", 0x00000020));
		extension.add(Php::Constant("MWB_RESULT_FT_PDF_COLUMNS", 0x00000021));
		extension.add(Php::Constant("MWB_RESULT_FT_PDF_TRUNCATED", 0x00000022));
		extension.add(Php::Constant("MWB_RESULT_FT_PDF_ECLEVEL", 0x00000023));
		extension.add(Php::Constant("MWB_RESULT_FT_PDF_CODEWORDS", 0x00000024));

		extension.add(Php::Constant("MWB_RESULT_FNAME_BYTES", "Bytes"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_TEXT", "Text"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_TYPE", "Type"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_SUBTYPE", "Subtype"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_SUCCESS", "Success"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_ISGS1", "GS1 compliance"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_KANJI", "Kanji encoding"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_LOCATION", "Location"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_IMAGE_WIDTH", "Image Width"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_IMAGE_HEIGHT", "Image Height"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_PARSER_BYTES", "Parser Input"));

		extension.add(Php::Constant("MWB_RESULT_FNAME_MODULES_COUNT_X", "Modules Count X"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_MODULES_COUNT_Y", "Modules Count Y"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_MODULE_SIZE_X", "Module Size X"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_MODULE_SIZE_Y", "Module Size Y"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_SKEW", "Skew"));

		extension.add(Php::Constant("MWB_RESULT_FNAME_BARCODE_WIDTH", "Barcode Width"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_BARCODE_HEIGHT", "Barcode Height"));

		extension.add(Php::Constant("MWB_RESULT_FNAME_PDF_ROWS", "PDF417 Rows Count"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_PDF_COLUMNS", "PDF417 Columns Count"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_PDF_TRUNCATED", "PDF417 is Truncated"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_PDF_ECLEVEL", "PDF417 EC Level"));
		extension.add(Php::Constant("MWB_RESULT_FNAME_PDF_CODEWORDS", "PDF417 Codewords"));
		
		extension.add(Php::Constant("MWP_RT_OK", 0));
		extension.add(Php::Constant("MWP_RT_FAIL", -1));
		extension.add(Php::Constant("MWP_RT_NOT_SUPPORTED", -2));
		extension.add(Php::Constant("MWP_RT_BAD_PARAM", -3));
		extension.add(Php::Constant("MWP_RT_BAD_INPUT", -4));
	
		extension.add(Php::Constant("MWP_PARSER_MASK_NONE", 0x00000000));
		extension.add(Php::Constant("MWP_PARSER_MASK_GS1", 0x00000001));
		extension.add(Php::Constant("MWP_PARSER_MASK_IUID", 0x00000002));
		extension.add(Php::Constant("MWP_PARSER_MASK_ISBT", 0x00000004));
		extension.add(Php::Constant("MWP_PARSER_MASK_AAMVA", 0x00000008));
		extension.add(Php::Constant("MWP_PARSER_MASK_HIBC", 0x00000010));
		extension.add(Php::Constant("MWP_PARSER_MASK_SCM", 0x00000020));

		//UID PARSER ERROR CODES
		extension.add(Php::Constant("UID_ERROR_INVALID_HEADER", -1));
		extension.add(Php::Constant("UID_ERROR_INVALID_FORMAT", -2));
		extension.add(Php::Constant("UID_ERROR_INVALID_EI", -3.0));
		extension.add(Php::Constant("UID_ERROR_INVALID_CAGE", -3.1));
		extension.add(Php::Constant("UID_ERROR_INVALID_DUNS", -3.2));
		extension.add(Php::Constant("UID_ERROR_INVALID_DODAAC", -3.3));
		extension.add(Php::Constant("UID_ERROR_INVALID_GS1COMP", -3.4));
		extension.add(Php::Constant("UID_ERROR_INVALID_PN", -4));
		extension.add(Php::Constant("UID_ERROR_INVALID_SN", -5));
		extension.add(Php::Constant("UID_ERROR_INVALID_UII", -6));
		extension.add(Php::Constant("UID_ERROR_INVALID_LOT", -7));
		extension.add(Php::Constant("UID_ERROR_GS_MISSING", -8));      				//GS Missing after Header
		extension.add(Php::Constant("UID_ERROR_RS_MISSING", -9));
		extension.add(Php::Constant("UID_ERROR_EOT_MISSING", -10));
		extension.add(Php::Constant("UID_ERROR_NO_SN", -11));
		extension.add(Php::Constant("UID_ERROR_NO_EI", -12));
		extension.add(Php::Constant("UID_ERROR_NO_PN", -13));
		extension.add(Php::Constant("UID_ERROR_NO_LOT", -14));
		extension.add(Php::Constant("UID_ERROR_DUPLICATE_DQ", -15));
		extension.add(Php::Constant("UID_ERROR_DUPLICATE_UII", -16));
		extension.add(Php::Constant("UID_ERROR_DUPLICATE_LOT", -17));
		extension.add(Php::Constant("UID_ERROR_DUPLICATE_SN", -18));
		extension.add(Php::Constant("UID_ERROR_DUPLICATE_EI", -19));
		extension.add(Php::Constant("UID_ERROR_LOT_PN_CONFLICT", -20));
		extension.add(Php::Constant("UID_ERROR_MISSING_REQ", -21));
		extension.add(Php::Constant("UID_ERROR_INVALID_IAC", -22));
		extension.add(Php::Constant("UID_ERROR_INVALID_TEI", -23));
		extension.add(Php::Constant("UID_ERROR_NOT_ENOUGHT_MEMORY", -24));

		//UID PARSER WARNING CODES
		extension.add(Php::Constant("UID_WARN_EXTRA_CHARS", 91));      				//characters after EOT
		extension.add(Php::Constant("UID_WARN_UNNEEDED_DATA", 92));      			//unneeded additional data
		extension.add(Php::Constant("UID_WARN_SPACE_AROUND", 93));      			//space at the beginning or end of the uid
		extension.add(Php::Constant("UID_WARN_UNKNOWN_DQ", 94));
		extension.add(Php::Constant("UID_WARN_OBSOLETE_FORMAT", 95));      			//warning for DD

		//AAMVA PARSER ERROR CODES
		extension.add(Php::Constant("AAMVA_ERROR_INVALID_FORMAT", -1));
		extension.add(Php::Constant("AAMVA_ERROR_INVALID_HEADER", -2));
		extension.add(Php::Constant("AAMVA_ERROR_INVALID_IIN", -3));
		extension.add(Php::Constant("NOT_ENOUGHT_MEMORY", -4));
		extension.add(Php::Constant("AAMVA_ERROR_INVALID_JN", -5));
		extension.add(Php::Constant("AAMVA_ERROR_INVALID_NENTIRES", -6));

		//AAMVA PARSER WARNING CODES
		extension.add(Php::Constant("AAMVA_WARNING_MISSING_MANDATORY_FIELDS", 1));

		//UPS/SCM PARSER ERROR CODES
		extension.add(Php::Constant("SCM_ERROR_INVALID_FORMAT", -1));
		extension.add(Php::Constant("SCM_ERROR_INVALID_CODE", -2));
		extension.add(Php::Constant("SCM_ERROR_ELEMENT_NOT_FOUND", -3));
		extension.add(Php::Constant("SCM_ERROR_CANT_ALLOCATE_MEMORY", -4));

		//UPS/SCM PARSER WARNING CODES
		extension.add(Php::Constant("SCM_WARNING_LENGTH_OUT_OF_BOUNDS", 1));		//possible compression used by UPS
		extension.add(Php::Constant("SCM_WARNING_FIELD_EXCEEDS_MAX_LENGTH", 2));	//possible compression used by UPS
		extension.add(Php::Constant("SCM_WARNING_INVALID_TERMINATOR", 3));			//possible compression used by UPS
		return extension;
	}
}
