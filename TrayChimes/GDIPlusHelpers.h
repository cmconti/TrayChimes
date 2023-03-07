#include "pch.h"

#ifdef new
#pragma message("include this file before new is redefined")
#endif

Gdiplus::Bitmap* Resize(Gdiplus::Bitmap& original, int height)
{
    float aspectRatio = (float)original.GetHeight() / (float)original.GetWidth();

    int width = (int)(height / aspectRatio);

    Gdiplus::Bitmap* newBitmap = new Gdiplus::Bitmap(width, height, original.GetPixelFormat());

    auto graphics = Gdiplus::Graphics::FromImage(newBitmap);
    graphics->DrawImage(&original, Gdiplus::Rect(0, 0, width, height));
    delete graphics;

    return newBitmap;
}

//load from resource and resize to given height
Gdiplus::Bitmap* LoadPNGBitmap(int height)
{
    Gdiplus::Bitmap* pBitmap = nullptr;
    auto hinstance = AfxGetApp()->m_hInstance;

    HRSRC rsc = FindResourceW(hinstance, MAKEINTRESOURCEW(IDB_REGULATOR_PNG), L"PNG");
    HGLOBAL grsc = LoadResource(hinstance, rsc);
    DWORD rscsize = SizeofResource(hinstance, rsc);
    void* prsc = LockResource(grsc);

    if (nullptr != prsc)
    {
        auto hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, rscsize);
        if (hBuffer)
        {
            void* pBuffer = ::GlobalLock(hBuffer);
            if (pBuffer)
            {
                CopyMemory(pBuffer, prsc, rscsize);

                IStream* pStream = nullptr;
                if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
                {
                    pBitmap = Gdiplus::Bitmap::FromStream(pStream);
                    pStream->Release();
                    if (pBitmap)
                    {
                        if (pBitmap->GetLastStatus() != Gdiplus::Ok)
                        {
                            delete pBitmap;
                            pBitmap = nullptr;
                        }
                        else
                        {
                            auto pResizedBitmap = Resize(*pBitmap, height);
                            delete pBitmap;
                            pBitmap = pResizedBitmap;
                        }
                    }
                }
                ::GlobalUnlock(hBuffer);
            }
            ::GlobalFree(hBuffer);
            hBuffer = nullptr;
        }
    }

    FreeResource(grsc);

    return pBitmap;
}

