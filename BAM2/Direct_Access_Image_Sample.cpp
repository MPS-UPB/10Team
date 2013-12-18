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
//===========================================================================
//===========================================================================

// BYTE matrix -> 256 valori (grayscale)
BYTE** my_byte_alloc(int x, int y) {
	BYTE **rez = (BYTE**)malloc(x * sizeof(BYTE*));
	for (int i = 0; i < x; i++) {
		rez[i] = (BYTE*)malloc(y * sizeof(BYTE));
	}
	return rez;
}

//  bool matrix -> 2 valori (alb-negru)
bool** my_bool_alloc(int x, int y) {
	bool **rez = (bool**)malloc(x * sizeof(bool*));
	for (int i = 0; i < x; i++) {
		rez[i] = (bool*)malloc(y * sizeof(bool));
	}
	return rez;
}

//******************
//* otsu algorithm *
//******************
int get_tresh_value (int min_x, int max_x, int min_y, int max_y, unsigned char **matrix){

	// Number of colors
	const int L = 256;
	float hist[L]={0.0F};	

	// for each color save number of apparitions in hist[color]
	for (int y = max_y - 1; y >= min_y; y--) {
		for (int x = max_x - 1; x >= min_x; x--) {
			int graylevel = matrix[x][y];
			hist[graylevel]+=1;
		}
	}

	// Number of pixels from the current tile
	int N = (max_x - min_x) * (max_y - min_y);
 
	//normalize histogram
	for (int i=0; i < L; i++) {
		hist[i]/=N;
	}
 
 
	float ut = 0;
	for (int i=0; i < L; i++) {
		ut+=i*hist[i];
	}

	int max_k=0;
	int max_sigma_k_=0;
	for (int k=0; k < L;++k) {
				
		float wk = 0;
		for (int i = 0; i <=k;++i) {
			wk += hist[i];
		}

		float uk = 0;
		for (int i = 0; i <=k;++i) {
			uk+= i*hist[i];
		}
 
		float sigma_k = 0;
		if (wk !=0 && wk!=1) {
			sigma_k  = ((ut*wk - uk)*(ut*wk - uk))/(wk*(1-wk));
		}

		if (sigma_k > max_sigma_k_) {
			max_k = k;
			max_sigma_k_ = (int) sigma_k;
		}
	}

	
	return max_k;
}

// media pixelilor pe un anumit tile 
int getAvg(int x_min, int x_max, int y_min, int y_max, bool **matrix) {
	int sum = 0;

	for (int i = x_min; i < x_max; i++) {
		for (int j = y_min; j < y_max; j++) {
			int val;
			if (matrix[i][j] == true) {
				val = 255; // white
			} else {
				val = 0;   // black
			}
			sum = sum + val;
		}
	}
	return (sum / ((x_max - x_min) * (y_max - y_min)));
}


int print_matrix (_TCHAR* filename, bool **matrix, int ox, int oy) {
	TCHAR strNewFileName[0x100];
	// create KImage
	KImage *pImageBinary = new KImage(oy, ox, 1);
	if (pImageBinary->BeginDirectAccess()) {
		for (int i = 0; i < ox; i++) {
			for (int j = 0; j < oy; j++) {
				pImageBinary->Put1BPPPixel(j, i, matrix[i][j]);
			}
		}
        //Close direct access
        pImageBinary->EndDirectAccess();
            
        //Save binarized image
		_stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s"), filename);
		pImageBinary->SaveAs(strNewFileName, SAVE_TIFF_CCITTFAX4);

        //Don't forget to delete the binary image
        delete pImageBinary;
    } else {
        _tprintf(_T("Unable to obtain direct access in binary image!"));
        return -3;
    }
	return 0;
}

//  resize 7->1
void border_matrix(BYTE ***Matrix, int OX, int OY, int border) {
	// top
	for (int i = border - 1; i >= 0; i--) {
		for (int j = 0; j < OY + 2 * border; j++) {
			(*Matrix)[i][j] = (*Matrix)[i + 1][j];
		}
	}
	// down
	for (int i = OX + border; i < OX + 2 * border; i++) {
		for (int j = 0; j < OY + 2 * border; j++) {
			(*Matrix)[i][j] = (*Matrix)[i - 1][j];
		}
	}
	// left
	for (int j = border - 1; j >= 0; j--) {
		for (int i = 0; i < OX + 2 * border; i++) {
			(*Matrix)[i][j] = (*Matrix)[i][j + 1];
		} 
	}
	// right
	for (int j = OY + border; j < OY + 2 * border; j++) {
		for (int i = 0; i < OX + 2 * border; i++) {
			(*Matrix)[i][j] = (*Matrix)[i][j - 1];
		}
	}
}

// la pixelata
void border_matrix_bin(bool ***Matrix, int OX, int OY, int border) {
	// top
	for (int i = border - 1; i >= 0; i--) {
		for (int j = 0; j < OY + 2 * border; j++) {
			(*Matrix)[i][j] = (*Matrix)[i + 1][j];
		}
	}
	// down
	for (int i = OX + border; i < OX + 2 * border; i++) {
		for (int j = 0; j < OY + 2 * border; j++) {
			(*Matrix)[i][j] = (*Matrix)[i - 1][j];
		}
	}
	// left
	for (int j = border - 1; j >= 0; j--) {
		for (int i = 0; i < OX + 2 * border; i++) {
			(*Matrix)[i][j] = (*Matrix)[i][j + 1];
		} 
	}
	// right
	for (int j = OY + border; j < OY + 2 * border; j++) {
		for (int i = 0; i < OX + 2 * border; i++) {
			(*Matrix)[i][j] = (*Matrix)[i][j - 1];
		}
	}
}

//===========================================================================
//===========================================================================
int _tmain(int argc, _TCHAR* argv[])
{
    //Verify command-line usage correctness
    if (argc != 4)
    {
        _tprintf(_T("Use: %s <Input_Image_File_Name (24BPP True-Color)> \
					<Output_Image_File_Name> <Confidence_Image_File_Name>\n"), argv[0]);
        return -1;
    }

    //Buffer for the new file names
    TCHAR strNewFileName[0x100];

    //Load and verify that input image is a True-Color one
    KImage *pImage = new KImage(argv[1]);
    if (pImage == NULL || !pImage->IsValid())
    {
        _tprintf(_T("File %s does is not a valid True-Color image!"), argv[0]);
        return -2;
    }
//===========================================================================
//===========================================================================

	// number of tiles -> OX and OY
	int x_tiles = 1;
	int y_tiles = 1; 

    //Apply a Gaussian Blur with small radius to remove obvious noise
    pImage->GaussianBlur(0.5);

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
    
    //Request direct access to image pixels in raw format
	// BYTE = grayscale = 256 nuante
	// BOOL = alb-negru = 2   nuante
    BYTE **pDataMatrixGrayscale = NULL;
    if (pImageGrayscale->BeginDirectAccess() && (pDataMatrixGrayscale = pImageGrayscale->GetDataMatrix()) != NULL)
    {
        //If direct access is obtained get image attributes and start processing pixels
        // bigIntOx; bigIntOy -> dimensiunile matricei mari (inainte de resize)
		int bigIntOY = pImageGrayscale->GetWidth();
        int bigIntOX = pImageGrayscale->GetHeight();
		int border = 24;
		int resize = 7;
		// Resize 7->1
		int smallIntOX = bigIntOX / resize;
		int smallIntOY = bigIntOY / resize;
		if (bigIntOX % resize != 0) {
			smallIntOX++;
		}
		if (bigIntOY % resize != 0) {
			smallIntOY++;
		}

		// smallIntOx; smallIntOy -> dimensiunile matricei mici (dupa resize)
		BYTE **smallMatrixGrayscale = my_byte_alloc(smallIntOX + 2 * border, smallIntOY + 2 * border);
		for (int i = border; i < smallIntOX + border; i++) {
			for (int j = border; j < smallIntOY + border; j++) {
				smallMatrixGrayscale[i][j] = pImageGrayscale->Get8BPPPixel((j - border) * resize, (i - border) * resize);
			}
		}
		
		// border the small matrix
		border_matrix(&smallMatrixGrayscale, smallIntOX, smallIntOY, border);
		
		// space for small matrix
		bool **smallMatrixBin = my_bool_alloc(smallIntOX, smallIntOY);
		
		// for each tile from small matrix
		for (int i = border; i < smallIntOX + border; i++) {
			for (int j = border; j < smallIntOY + border; j++) {
				int max_k = get_tresh_value (i - border, i + border, j - border, j + border, smallMatrixGrayscale);
				 
				// set pixel
				BYTE &PixelAtXY = smallMatrixGrayscale[i][j];
				if (PixelAtXY < max_k) {
					//...if closer to black, set to black
					smallMatrixBin[i - border][j - border] = false;
				} else {
					//...if closer to white, set to white
					smallMatrixBin[i - border][j - border] = true;
				}
			}
		}

		// Resize 1->7 
		// aloc spatiu si initializez pixelata (bigMatrixBinResized = pixelata)
		bool **bigMatrixBinResized = my_bool_alloc(bigIntOX + 2 * border, bigIntOY + 2 * border);
		for (int i = border; i < bigIntOX + border; i++) {
			for (int j = border; j < bigIntOY + border; j++) {
				bigMatrixBinResized[i][j] = smallMatrixBin[(i - border)/resize][(j - border)/resize];
			}
		} 
		// border the big resized matrix
		border_matrix_bin(&bigMatrixBinResized, bigIntOX, bigIntOY, border);

		// BIG MATRIX --------------------------------------------------------------
		BYTE **bigMatrixGrayscale = my_byte_alloc(bigIntOX + 2 * border, bigIntOY + 2 * border);
		
		for (int i = border; i < bigIntOX + border; i++) {
			for (int j = border; j < bigIntOY + border; j++) {
				bigMatrixGrayscale[i][j] = pImageGrayscale->Get8BPPPixel(j - border, i - border);
			}
		}
		
		// border the big matrix
		border_matrix(&bigMatrixGrayscale, bigIntOX, bigIntOY, border);
		
		// Check if number of tiles is between 0 and number of pixels (on OX and OY)
		if (x_tiles <= 0 || y_tiles <= 0 || x_tiles >= bigIntOX || y_tiles >= bigIntOY) {
		    _tprintf(_T("Incorrect number of tiles\n"), argv[0]);
			return -1;
		}
		// Get tile size (OX and OY)
		int x_tile_size = bigIntOX / x_tiles;
		int y_tile_size = bigIntOY / y_tiles;
		// check if there are pixels left at the border (add one more tile)
		if (bigIntOX % x_tiles != 0) {
			x_tiles++;
		}
		if (bigIntOY % y_tiles != 0) {
			y_tiles++;
		}

		bool **bigMatrixBin = my_bool_alloc(bigIntOX, bigIntOY);
		KImage *pImageConf = new KImage(bigIntOY, bigIntOX, 8);
		pImageConf->BeginDirectAccess();

		// For each tile
		for (int tile_x = 0; tile_x < x_tiles; tile_x++) {
			for (int tile_y = 0; tile_y < y_tiles; tile_y++) {
				// Set the rectangle containing pixels only from the current tile 
				// (min_x, min_y); (min_x, max_y); (max_x, min_y); (max_x, max_y)
				int min_x = tile_x * x_tile_size;
				int min_y = tile_y * y_tile_size;

				int max_x = min_x + x_tile_size; 
				if (max_x > bigIntOX) {
					max_x = bigIntOX;
				}
				int max_y = min_y + y_tile_size;
				if (max_y > bigIntOY) {
					max_y = bigIntOY;
				}

				// Get tresh value for the current tile

				int max_k = get_tresh_value (min_x, max_x, min_y, max_y, bigMatrixGrayscale);
				
				for (int y = min_y + border; y < max_y + border; y++) {
					for (int x = min_x + border; x < max_x + border; x++) {
						int nrColors = 256;
						// valoare de prag ce depinde tile-ul curent din pixelata
						int max_k_small = getAvg(x - border, x + border, y - border, y + border, bigMatrixBinResized);
						// valoarea de prag finala (combinatie intre cele 2 valori de prag - pixelata si initiala)
						int max_k_new = max_k * 7 / 8 + max_k_small / 8;
						BYTE &PixelAtXY = bigMatrixGrayscale[x][y];
						if (PixelAtXY < max_k_new) {
							//...if closer to black, set to black
							bigMatrixBin[x - border][y - border] = false;
							pImageConf->Put8BPPPixel(y - border, x - border, (max_k_new - PixelAtXY) * 255 / (max_k_new + 1));
						} else {
							//...if closer to white, set to white
							bigMatrixBin[x - border][y - border] = true;
							pImageConf->Put8BPPPixel(y - border, x - border, (PixelAtXY - max_k_new) * 255 / (256 - max_k_new));
						}
					}
				}
			}
		}

		// Median filter:
		int ksize = 3;
		// copy final matrix -> matrix of opencv
		cv::Mat src(bigIntOX, bigIntOY, CV_8UC1);
		for (int i = 0; i < bigIntOX; i++) {
			for (int j = 0; j < bigIntOY; j++) {
				src.at<BYTE>(i, j) = bigMatrixBin[i][j];
			}
		}
		// apply median filter
		cv::Mat dst(bigIntOX, bigIntOY, CV_8UC1);
		cv::medianBlur(src, dst, ksize);
		// copy vack matrix of opencv -> final matrix
		for (int i = 0; i < bigIntOX; i++) {
			for (int j = 0; j < bigIntOY; j++) {
				bigMatrixBin[i][j] = dst.at<BYTE>(i, j);
			}
		}

		// save the final bin image
		int ret_v = print_matrix(argv[2], bigMatrixBin, bigIntOX, bigIntOY);
		if (ret_v != 0) {
			return ret_v;
		}
		// save confidence
		_stprintf_s(strNewFileName, sizeof(strNewFileName) / sizeof(TCHAR), _T("%s"), argv[3]);
		pImageConf->SaveAs(strNewFileName, SAVE_TIFF_LZW);

		//Close direct access
        pImageGrayscale->EndDirectAccess();
		pImageConf->EndDirectAccess();
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
