// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/ImageList.hpp"
#include "Wg/Application.hpp"
#include "Wg/Debug.hpp"
#include "Wg/ResourceException.hpp"
#include "Wg/String.hpp"
#include "Wg/Win32.hpp"

using namespace Wg;

#define GdiObj GdiObject<HIMAGELIST, Win32DestroyImageList>

ImageList::ImageList()
  : SharedPtr<GdiObj>(new GdiObj(ImageList_Create(16, 16, ILC_COLOR32, 0, 1)))
{
}

ImageList::ImageList(const Size& sz)
  : SharedPtr<GdiObj>(new GdiObj(ImageList_Create(sz.w, sz.w, ILC_COLOR32, 0, 1)))
{
  if (getHandle() == nullptr)
    throw ResourceException(L"Can't create the image-list");
}

ImageList::ImageList(HIMAGELIST hImageList)
  : SharedPtr<GdiObj>(new GdiObj(hImageList))
{
}

/**
   Loads a ImageList from a @msdn{BITMAP} resource.
*/
ImageList::ImageList(ResourceId bitmapId, int widthPerIcon, const Color& maskColor)
  : SharedPtr<GdiObj>(new GdiObj())
{
  HIMAGELIST himagelist =
    ImageList_LoadImage(Application::getHandle(),
			bitmapId.toLPTSTR(),
			widthPerIcon,
			1,
			convert_to<COLORREF>(maskColor),
			IMAGE_BITMAP,
			LR_CREATEDIBSECTION);

  if (himagelist == nullptr)
    throw ResourceException(format_string(L"Can't create the image-list resource %d",
					  bitmapId.getId()));

  get()->setHandle(himagelist);
}

/**
   Loads a ImageList from a file.
*/
ImageList::ImageList(const String& fileName, int widthPerIcon, const Color& maskColor)
  : SharedPtr<GdiObj>(new GdiObj())
{
  HIMAGELIST himagelist =
    ImageList_LoadImage(Application::getHandle(),
			fileName.c_str(),
			widthPerIcon,
			1,
			convert_to<COLORREF>(maskColor),
			IMAGE_BITMAP,
			LR_LOADFROMFILE);
  if (himagelist == nullptr)
    throw ResourceException(L"Can't load the image-list from file " + fileName);

  get()->setHandle(himagelist);
}

ImageList::~ImageList()
= default;

/**
   Returns how many images has this image list.

   @return
     A number from 0 to n that specified the size of the list.
*/
int ImageList::getImageCount() const
{
  assert(getHandle());
  return ImageList_GetImageCount(getHandle());
}

Size ImageList::getImageSize() const
{
  assert(getHandle());
  Size sz;
  ImageList_GetIconSize(getHandle(), &sz.w, &sz.h);
  return sz;
}

/**
   Adds a new image in the list.

   @param image
     The image to be added.

   @return
     The index of the new image.
*/
int ImageList::addImage(Image& image) const
{
  assert(getHandle());
  assert(image.getHandle());

  return ImageList_Add(getHandle(), image.getHandle(), nullptr);
}

int ImageList::addImage(Image& image, const Color& maskColor) const
{
  assert(getHandle());
  assert(image.getHandle());

  return ImageList_AddMasked(getHandle(),
			     image.getHandle(),
			     convert_to<COLORREF>(maskColor));
}

void ImageList::removeImage(int index) const
{
  assert(getHandle());
  ImageList_Remove(getHandle(), index);
}

void ImageList::removeAllImages() const
{
  assert(getHandle());
  ImageList_RemoveAll(getHandle());
}

/**
   Returns the @msdn{HIMAGELIST} handle.
*/
HIMAGELIST ImageList::getHandle() const
{
  return get()->getHandle();
}
