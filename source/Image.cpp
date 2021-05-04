// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Image.hpp"
#include "VACA/Debug.hpp"
#include "VACA/Graphics.hpp"
#include "VACA/Application.hpp"
#include "VACA/ResourceException.hpp"
#include "VACA/String.hpp"

using namespace Wg;

// ======================================================================

ImageHandle::ImageHandle()
{
  m_hdc = nullptr;
  m_graphics = nullptr;
}

ImageHandle::ImageHandle(HBITMAP handle)
  : GdiObject<HBITMAP>(handle)
{
  m_hdc = nullptr;
  m_graphics = nullptr;
}

ImageHandle::~ImageHandle()
{
  delete m_graphics;
}

// ======================================================================

Image::Image()
  : SharedPtr<ImageHandle>(new ImageHandle())
{
}

/**
   Loads an image from the resource specified by @a imageId. This
   constructor uses Win32 LoadBitmap.
*/
Image::Image(ResourceId imageId)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  // HBITMAP
  HBITMAP hbmp = LoadBitmap(Application::getHandle(), imageId.toLPTSTR());
  if (hbmp == nullptr)
    throw ResourceException(format_string(L"Can't load the image resource %d",
					  imageId.getId()));

  get()->m_hdc = GetDC(GetDesktopWindow());
  get()->setHandle(hbmp);
}

Image::Image(const String& fileName)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  // file name size
  int size = static_cast<int>(fileName.size() + 1);
  Char* lpstr = new Char[size];
  copy_string_to(fileName, lpstr, size);

  // HBITMAP
  auto hbmp = reinterpret_cast<HBITMAP>
    (LoadImage(Application::getHandle(),
	       lpstr,
	       IMAGE_BITMAP,
	       0, 0,
	       LR_LOADFROMFILE));
  delete[] lpstr;

  if (hbmp == nullptr)
    throw ResourceException(L"Can't load the image from file " + fileName);

  get()->m_hdc = GetDC(GetDesktopWindow());
  get()->setHandle(hbmp);
}

Image::Image(const Size& sz)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  init(sz.w, sz.h);
}

Image::Image(int width, int height)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  init(width, height);
}

Image::Image(int width, int height, int depth)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  init(width, height, depth);
}

Image::Image(const Size& sz, int depth)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  init(sz.w, sz.h, depth);
}

Image::Image(const Size& sz, Graphics& g)
  : SharedPtr<ImageHandle>(new ImageHandle())
{
  assert(g.getHandle());
  assert(sz.w > 0 && sz.h > 0);

  get()->m_hdc = g.getHandle();
  get()->setHandle(CreateCompatibleBitmap(get()->m_hdc, sz.w, sz.h));

}

Image::Image(const Image& image) = default;

Image::~Image() = default;

int Image::getWidth() const
{
  return getSize().w;
}

int Image::getHeight() const
{
  return getSize().h;
}

Size Image::getSize() const
{
  BITMAPCOREHEADER bc;
  ZeroMemory(&bc, sizeof(bc));
  bc.bcSize = sizeof(bc);
  GetDIBits(get()->m_hdc, getHandle(), 0, 0, nullptr,
	    reinterpret_cast<BITMAPINFO*>(&bc), 0);
  return Size(bc.bcWidth, bc.bcHeight);
}

int Image::getDepth() const
{
  BITMAPCOREHEADER bc;
  ZeroMemory(&bc, sizeof(bc));
  bc.bcSize = sizeof(bc);
  GetDIBits(get()->m_hdc, getHandle(), 0, 0, nullptr,
	    reinterpret_cast<BITMAPINFO*>(&bc), 0);
  return bc.bcBitCount;
}

/**
   Returns a Graphics context to draw inside the image.
*/
Graphics& Image::getGraphics()
{
  ImageHandle* ptr = get();

  if (ptr->m_graphics == nullptr) {
    assert(ptr->m_hdc != NULL);
    ptr->m_graphics = new Graphics(ptr->m_hdc, *this);
  }

  return *ptr->m_graphics;
}

ImagePixels Image::getPixels() const
{
  BITMAPCOREHEADER bc;
  ZeroMemory(&bc, sizeof(bc));
  bc.bcSize = sizeof(bc);
  GetDIBits(get()->m_hdc, getHandle(), 0, 0, nullptr,
	    reinterpret_cast<BITMAPINFO*>(&bc), 0);

  ImagePixels imagePixels(bc.bcWidth,
			  bc.bcHeight < 0 ? -bc.bcHeight: bc.bcHeight);

  bc.bcBitCount = 32; // TODO is it right? there are alpha channel?

  GetDIBits(get()->m_hdc, getHandle(),
            0, static_cast<UINT>(getHeight()),
            reinterpret_cast<LPVOID>(&imagePixels[0]),
            reinterpret_cast<BITMAPINFO*>(&bc), 0);

  imagePixels.invertScanlines();

  return imagePixels;
}

void Image::setPixels(const ImagePixels& imagePixels)
{
  BITMAPCOREHEADER bc;
  ZeroMemory(&bc, sizeof(bc));
  bc.bcSize = sizeof(bc);
  GetDIBits(get()->m_hdc, getHandle(), 0, 0, nullptr,
	    reinterpret_cast<BITMAPINFO*>(&bc), 0);

  bc.bcBitCount = 32;

  ImagePixels copy = imagePixels.clone();
  copy.invertScanlines();

  SetDIBits(get()->m_hdc,
            getHandle(),
            0, static_cast<UINT>(getHeight()),
            reinterpret_cast<LPVOID>(&copy[0]),
            reinterpret_cast<BITMAPINFO*>(&bc), DIB_RGB_COLORS);
}

HBITMAP Image::getHandle() const
{
  return get()->getHandle();
}

Image& Image::operator=(const Image& image)
{
  SharedPtr<ImageHandle>::operator=(image);
  return *this;
}

Image Image::clone() const
{
  Image image(getSize(), getDepth());
  this->copyTo(image);
  return image;
}

void Image::init(int width, int height)
{
  assert(width > 0 && height > 0);

  get()->m_hdc = GetDC(GetDesktopWindow());
  get()->setHandle(CreateCompatibleBitmap(get()->m_hdc, width, height));

  // TODO handle error
}

void Image::init(int width, int height, int depth)
{
  assert(width > 0 && height > 0);

  BITMAPINFOHEADER bhdr;
  bhdr.biSize = sizeof(BITMAPINFOHEADER);
  bhdr.biWidth = width;
  bhdr.biHeight = -height;
  bhdr.biPlanes = 1;
  bhdr.biBitCount = static_cast<WORD>(depth);
  bhdr.biCompression = BI_RGB;
  bhdr.biSizeImage = 0;
  bhdr.biXPelsPerMeter = 0;
  bhdr.biYPelsPerMeter = 0;
  bhdr.biClrUsed = 0;
  bhdr.biClrImportant = 0;

  BITMAPINFO binf;
  RGBQUAD dummy = { 0, 0, 0, 0 };
  binf.bmiColors[0] = dummy;
  binf.bmiHeader = bhdr;

  char* bits = nullptr;

  get()->m_hdc = GetDC(GetDesktopWindow());
  get()->setHandle(CreateDIBSection(get()->m_hdc, &binf, DIB_RGB_COLORS,
				    reinterpret_cast<void**>(&bits),
				    nullptr, 0));

  // TODO handle error
}

void Image::copyTo(Image& image) const
{
  image.getGraphics().drawImage(*const_cast<Image*>(this), 0, 0);
}
