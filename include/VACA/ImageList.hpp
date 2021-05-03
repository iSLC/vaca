// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_IMAGELIST_HPP
#define VACA_IMAGELIST_HPP

#include "VACA/Base.hpp"
#include "VACA/Image.hpp"
#include "VACA/Color.hpp"
#include "VACA/ResourceId.hpp"
#include "VACA/GdiObject.hpp"
#include "VACA/SharedPtr.hpp"

namespace vaca {

/**
   Used to destroy the HIMAGELIST handle from GdiObject.

   @internal
*/
struct Win32DestroyImageList
{
  /**
     @win32
     Calls @msdn{ImageList_Destroy}.
     @endwin32
  */
  static void destroy(HIMAGELIST handle)
  {
    ::ImageList_Destroy(handle);
  }
};

/**
   ImageLists are containers of images and icons that can be used in
   items of a ListView or a TreeView.

   This is a SharedPtr, so if you copy instances of ImageLists they will be
   referencing to the same place.

   @win32
     This is a @msdn{HIMAGELIST} wrapper.
   @endwin32

   @see ListView#setImageList, TreeView#setImageList
*/
class VACA_DLL ImageList : public SharedPtr<GdiObject<HIMAGELIST, Win32DestroyImageList> >
{
public:

  ImageList();
  explicit ImageList(const Size& sz);
  explicit ImageList(HIMAGELIST hImageList);
  ImageList(ResourceId bitmapId, int widthPerIcon, Color maskColor);
  ImageList(const String& fileName, int widthPerIcon, Color maskColor);
  virtual ~ImageList();

  int getImageCount() const;
  Size getImageSize() const;

  int addImage(Image& image);
  int addImage(Image& image, Color maskColor);
  void removeImage(int index);
  void removeAllImages();

  HIMAGELIST getHandle() const;

};

} // namespace vaca

#endif // VACA_IMAGELIST_HPP
