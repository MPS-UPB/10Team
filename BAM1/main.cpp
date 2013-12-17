
#include "stdafx.h"
#include "Direct_Access_Image.h"

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
    // verify command-line usage correctness
    if(argc != 4) {
        _tprintf(_T("Use: <Input_Image> <Output_Image> <Confidence_Image>\n"), argv[0]);
        return -1;
    }
	// filtering
	int height = 0, width = 0;
	char path[0x100];
	std::wcstombs(path, argv[1], 100);
	BYTE **img = noiseFilter(path, &height, &width); 
	
	if(img == NULL) {
		_tprintf(_T("Problem with input image\n"));
		return -2;
	}
	
	// image for confidences
	KImage *confidence = new KImage(width, height, 8);
	if (confidence->BeginDirectAccess() && confidence->HasDirectAccess()) {
		// create binary image
        KImage *pImageBinary = new KImage(width, height, 1);
		int res = binarization(pImageBinary, img, 0, height, 0, width);
		if (res == 0) {
			confidence->SetDataMatrix(img);
			confidence->SaveAs(argv[3], SAVE_TIFF_LZW);
            // save binarized image
			pImageBinary->SaveAs(argv[2], SAVE_TIFF_CCITTFAX4);
            // delete the binary image
            delete pImageBinary;
        } else {
			_tprintf(_T("Problem at binarization\n"));
            return res;
        }
        // close direct access
        confidence->EndDirectAccess();
    } else {
        _tprintf(_T("Unable to obtain direct access in grayscale image!"));
        return -2;
    }

    // free the grayscale image
    delete confidence;
  
    return 0;
}
