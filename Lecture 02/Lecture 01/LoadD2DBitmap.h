#pragma once


namespace D2DBitmap
{
	// RECT_F를 자주 쓰지만 기본적으로 RECT_U다
	// 나중에 필요하면 변경
	template<typename Rect = D2D_RECT_U>

	// WIC Factory
	// RnederTarget
	// 파일 이름
	// Bitmap (1은 D3D11)
	// Rect (WIN32Rect, RECTF, RECTU) 
	// 비트맵이 몇 x 몇으로 될 것인지
	// (RenderTarget에서 설정해서 쓰는 상황이 더 많음)
	// Flip Rotate
	bool LoadImageFromFile(			  IWICImagingFactory		*pwicFactory								
									, ID2D1RenderTarget			*pd2dRenderTarget							
									, LPCTSTR					pszstrFileName								
									, ID2D1Bitmap1				**ppd2dBitmap								
									, Rect						*pd2drcImage  = nullptr						
									, UINT						nWidth		  = 0							
									, UINT						nHeight		  = 0							
									, WICBitmapTransformOptions	nFlipRotation = WICBitmapTransformRotate0	
	)
	{
		// WIC에서 파일 이름을 받아서 Decode 한다.
		ComPtr<IWICBitmapDecoder> pwicBitmapDecoder;

		if (FAILED(pwicFactory->CreateDecoderFromFilename(	  pszstrFileName
															, NULL
															, GENERIC_READ
															, WICDecodeMetadataCacheOnDemand
															, &pwicBitmapDecoder
		))) return false;
		
		//
		ComPtr<IWICBitmapFrameDecode> pwicBitmapFrameDecode;
		if (FAILED(pwicBitmapDecoder->GetFrame(0, &pwicBitmapFrameDecode))) return false;

		// Bitmap Low 파일
		ComPtr<IWICBitmapSource> pwicSource{ pwicBitmapFrameDecode };

		UINT nImageWidth	{ 0 };
		UINT nImageHeight	{ 0 };

		// Width, Height
		if (FAILED(pwicSource->GetSize(&nImageWidth, &nImageHeight))) return false;

		ComPtr<IWICFormatConverter>		pwicFormatConverter;
		ComPtr<IWICBitmapScaler>		pwicScaler;
		ComPtr<IWICBitmapClipper>		pwicClipper;
		ComPtr<IWICBitmapFlipRotator>	pwicFlipRotator;			// Filp

		if (pd2drcImage)
		{
			if (pd2drcImage->left < 0)	pd2drcImage->left = 0;
			if (pd2drcImage->top < 0)	pd2drcImage->top = 0;
			if (pd2drcImage->right > nImageWidth)	pd2drcImage->right = nImageWidth;
			if (pd2drcImage->bottom > nImageHeight)	pd2drcImage->bottom = nImageHeight;

			WICRect wicRect
			{ 
				  static_cast<INT>(pd2drcImage->left)
				, static_cast<INT>(pd2drcImage->top)
				, static_cast<INT>(pd2drcImage->right - pd2drcImage->left)
				, static_cast<INT>(pd2drcImage->bottom - pd2drcImage->top) 
			};

			// Bitmap 자르는 것
			// 초기화
			if (FAILED(pwicFactory->CreateBitmapClipper(&pwicClipper)))			return false;
			if (FAILED(pwicClipper->Initialize(pwicSource.Get(), &wicRect)))	return false;

			pwicSource = pwicClipper;
		}

		if ((nWidth != 0) || (nHeight != 0))
		{
			if (nWidth == 0) nWidth = UINT(static_cast<float>(nHeight) / static_cast<float>(nImageHeight) * static_cast<float>(nImageWidth));
			if (nHeight == 0) nHeight = UINT(static_cast<float>(nWidth) / static_cast<float>(nImageWidth) * static_cast<float>(nImageHeight));

			if (FAILED(pwicFactory->CreateBitmapScaler(&pwicScaler))) return false;
			if (FAILED(pwicScaler->Initialize(pwicSource.Get(), nWidth, nHeight, WICBitmapInterpolationModeCubic))) return false;
			pwicSource = pwicScaler;
		}

		if (nFlipRotation != WICBitmapTransformRotate0)
		{
			if (FAILED(pwicFactory->CreateBitmapFlipRotator(&pwicFlipRotator)))			return false;
			if (FAILED(pwicFlipRotator->Initialize(pwicSource.Get(), nFlipRotation)))	return false;
			pwicSource = pwicFlipRotator;
		}

		if (FAILED(pwicFactory->CreateFormatConverter(	  &pwicFormatConverter
		))) return false;

		// jpg는 alpha 값이 없다.
		// Fomat을 32bit per pixel PBGRA 로 모든 똑같은 포멧으로 바꿔준다.
		if (FAILED(pwicFormatConverter->Initialize(	  pwicSource.Get()
													, GUID_WICPixelFormat32bppPBGRA
													, WICBitmapDitherTypeNone
													, NULL
													, 0.f
													, WICBitmapPaletteTypeMedianCut
		))) return false;
		
		// 생성!
		ID2D1Bitmap* bmp;
		if (FAILED(pd2dRenderTarget->CreateBitmapFromWicBitmap(	  pwicFormatConverter.Get()
																, NULL
																, &bmp
		))) return false;
		
		bmp->QueryInterface(ppd2dBitmap);
		bmp->Release();

		return true;
	}

}