//
// Created by jack on 16-9-25.
//

#include "../iunknown.h"
#include "../xiamiapi.h"
#include "gtest/gtest.h"

namespace {

    class XiamiAPITest : public ::testing::Test {
    protected:
        xiamiapi::IXiamiAPI * api;

        XiamiAPITest() {
            api = xiamiapi::IXiamiAPI::CreateInstance();
        }

        virtual ~XiamiAPITest() {
            api->Release();
        }
    };

    TEST_F(XiamiAPITest, DoesGetArtistHotSong) {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetArtistHotSong(573, &list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiSongInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **)&info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }
    TEST_F(XiamiAPITest, DoesGetRecommendPlaylist) {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetRecommendPlaylist(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiPlaylistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetUserRecommendPlaylist)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetUserRecommendPlaylist(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiPlaylistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetCollectionPlaylist)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetCollectionPlaylist(224467248, &list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiPlaylistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetAlbumPlaylist)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetAlbumPlaylist(2100243368, &list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiPlaylistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetSongPlaylist)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetSongPlaylist(2101531, &list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiPlaylistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetRecommendCollection)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetRecommendCollection(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiCollectionInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetArtistHotList)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetArtistHotList(1, 2, &list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiArtistInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetArtistCategory)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetArtistCategory(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiArtistCategoryInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetHotSearchKey)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetHotSearchKey(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiHotSearchKeyInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetRankList)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->GetRankList(&list));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiRankInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesSearchCollection)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->SearchCollection("lee", &list, 1));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiCollectionFullInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesSearchAlbum)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->SearchAlbum("lee", &list, 1));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiAlbumInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesSearchArtist)
    {
        xiamiapi::IGenericArray * list;
        ASSERT_EQ(0, api->SearchArtist("lee", &list, 1));
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiArtistFullInfo * info;
            ASSERT_EQ(0, list->get_element(i)->QueryInterface(list->ElementIID(), (void **) & info));
            ASSERT_EQ(1, info->Release());
        }
        ASSERT_EQ(0, list->Release());
    }

    TEST_F(XiamiAPITest, DoesGetFileContentFromUrl)
    {
        xiamiapi::IStr * s;
        ASSERT_EQ(0, api->GetFileContentFromUrl("http://img.xiami.net/images/album/img58/2072992358/2559881474255988.jpg", &s));
        ASSERT_EQ(0, s->Release());
    }
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
