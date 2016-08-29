//
// Created by jack on 16-8-13.
//

#include "xiamiapi.h"
#include "xiamiapi_impl.h"
#include <string>

using std::string;

namespace xiamiapi
{
    IXiamiAPI * xiamiapi::IXiamiAPI::CreateInstance()
    {
        return new xiamiapi::XiamiAPI();
    }


    IXiamiUserInfo::~IXiamiUserInfo()
    {}

    IXiamiAPI::~IXiamiAPI()
    {}

    IStr::~IStr()
    {}

    IGenericArray::~IGenericArray()
    {}

    IXiamiArtistCategoryInfo::~IXiamiArtistCategoryInfo()
    {}

    IXiamiRankInfo::~IXiamiRankInfo()
    {}

    IXiamiArtistInfo::~IXiamiArtistInfo()
    {}

    IXiamiCollectionInfo::~IXiamiCollectionInfo(){}

    IXiamiHotSearchKeyInfo::~IXiamiHotSearchKeyInfo()
    {}

    IXiamiSongInfo::~IXiamiSongInfo()
    {}

    IXiamiPlaylistInfo::~IXiamiPlaylistInfo()
    {}
}

