//===========================================================================
//===========================================================================
//===========================================================================
//==   Direct_Access_Image_Sample.cpp  ==  Author: Costin-Anton BOIANGIU   ==
//===========================================================================
//===========================================================================
//===========================================================================

//===========================================================================
//===========================================================================
#include "stdafx.h"
#include "Direct_Access_Image.h"
#include<iostream>
#include<stdlib.h> 
//===========================================================================
//===========================================================================

//===========================================================================
//===========================================================================
int _tmain(int argc, _TCHAR* argv[])
{
    //Verify command-line usage correctness
    if (argc != 4)
    {
        _tprintf(_T("Use: %s <Input_Image_File_Name> <Output_Image_File_Name> <Confidence_Image_File_Name>\n"), argv[0]);
        return -1;
    }

    //Buffer for the new file names
    TCHAR strNewFileName[0x100];

    //Load and verify that input image is a True-Color one
    KImage *pImage = new KImage(argv[1]);
    if (pImage == NULL || !pImage->IsValid() || pImage->GetBPP() != 24)
    {
        _tprintf(_T("File %s does is not a valid True-Color image!"), argv[0]);
        return -2;
    }
	
    //Apply a Gaussian Blur with small radius to remove obvious noise
    pImage->GaussianBlur(0.5);
    _stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s_blurred.TIF"), argv[1]);
   // pImage->SaveAs(strNewFileName, SAVE_TIFF_LZW);

    //Convert to grayscale
    KImage *pImageGrayscale = pImage->ConvertToGrayscale();
    //Don't forget to delete the original, now useless image
    delete pImage;

    //Verify conversion success...
    if (pImageGrayscale == NULL || !pImageGrayscale->IsValid() || pImageGrayscale->GetBPP() != 8)
    {
        _tprintf(_T("Conversion to grayscale was not successfull!"));
        return -3;
    }
    //... and save grayscale image
    _stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s"), argv[2]);
   // pImageGrayscale->SaveAs(strNewFileName, SAVE_TIFF_LZW);
    
    //Request direct access to image pixels in raw format
    BYTE **pDataMatrixGrayscale = NULL;
    if (pImageGrayscale->BeginDirectAccess() && (pDataMatrixGrayscale = pImageGrayscale->GetDataMatrix()) != NULL)
    {
        //If direct access is obtained get image attributes and start processing pixels
        int intWidth = pImageGrayscale->GetWidth();
        int intHeight = pImageGrayscale->GetHeight();

		// Calculate histogram
		int histogram[256], ii;
		for(ii=0; ii < 256; ii++ )
			histogram[ii] = 0;
	
		for (int y = intHeight - 1; y >= 0; y--){
			for (int x = intWidth - 1; x >= 0; x--){
				BYTE &PixelAtXY = pDataMatrixGrayscale[y][x];
				int i = 0xFF & PixelAtXY;	//Full-White & Pixel of image
				histogram[i]++;
			}
		}

			float sumB = 0, sum = 0, Max = 0;
			int weightB = 0, weightF = 0, threshold = 0, mB, mF;
			int total = intWidth * intHeight;	//number of pixels in the image

			for (int t = 0; t < 256; t++) 
				sum = sum + t * histogram[t];
			
			for (int t = 0; t < 256; t++){

				weightB = weightB + histogram[t];	// Weight Background
				if (weightB == 0) 
					continue;

				weightF = total - weightB;			// Weight Foreground
				if (weightF == 0) 
					break;

				sumB = sumB + (float) (t * histogram[t]);

				mB = ((int) sumB) / weightB;            // Mean Background
				mF = ((int) (sum - sumB)) / weightF;    // Mean Foreground

   // Calculate Between Class Variance
				float Between = (float)weightB * (float)weightF * (mB - mF) * (mB - mF);

   // Check if new maximum found
				if (Between > Max) {
					Max = Between;
					threshold = t;
				}
			}

        //Create binary image
        KImage *pImageBinary = new KImage(intWidth, intHeight, 1);
		KImage *pImageConf = new KImage(intWidth, intHeight, 8);

        if ( (pImageBinary->BeginDirectAccess()) && (pImageConf->BeginDirectAccess()) ){
            //Apply a threshold at half the grayscale range (0x00 is Full-Black, 0xFF is Full-White, 0x80 is the Middle-Gray)
            for (int y = intHeight - 1; y >= 0; y--){
                for (int x = intWidth - 1; x >= 0; x--){
                    BYTE PixelAtXY = pImageGrayscale->Get8BPPPixel(x, y);
					BYTE diff;
                    if (PixelAtXY < (BYTE) (threshold)){
                        pImageBinary->Put1BPPPixel(x, y, false);	//if closer to black, set to black
						diff = PixelAtXY - 0x00;	//diferenta dintre pixel-grayscale si valoarea binarizata
					}
                    else{
                        pImageBinary->Put1BPPPixel(x, y, true);		//if closer to white, set to white
						diff = 0xFF - PixelAtXY;	//diferenta dintre pixel-grayscale si valoarea binarizata
					}
				
					BYTE PixelAtXYConf = 255 - diff;	//confidenta este = valoarea cea mai sigura - diferenta
				//	printf("%d ", PixelAtXYConf);
					
					pImageConf->Put8BPPPixel(x,y,PixelAtXYConf);
                }
			}

			

            //Close direct access
            pImageBinary->EndDirectAccess();
			pImageConf->EndDirectAccess();
            
            //Save binarized image
            _stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s"), argv[2]);
            pImageBinary->SaveAs(strNewFileName, SAVE_TIFF_CCITTFAX4);
			
			//Save confidence image
			_stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s"), argv[3]);
            pImageConf->SaveAs(strNewFileName, SAVE_TIFF_LZW);

            //Don't forget to delete the binary image
            delete pImageBinary;
			delete pImageConf;
        }
        else
        {
            _tprintf(_T("Unable to obtain direct access in binary image!"));
            return -3;
        }


        //Close direct access
        pImageGrayscale->EndDirectAccess();
    }
    else
    {
        _tprintf(_T("Unable to obtain direct access in grayscale image!"));
        return -4;
    }

    //Don't forget to delete the grayscale image
    delete pImageGrayscale;

    //Return with success
    return 0;
}
//===========================================================================
//===========================================================================
