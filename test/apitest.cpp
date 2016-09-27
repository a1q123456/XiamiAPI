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
        api->GetArtistHotSong(573, &list);
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            xiamiapi::IXiamiSongInfo * info;
            list->get_element(i)->QueryInterface(list->ElementIID(), (void **)&info);
            auto tmp = info->get_singers();
            info->Release();
        }
        list->Release();
    }
    TEST_F(XiamiAPITest, DoesGetRecommendPlaylist) {
        xiamiapi::IGenericArray * list;
        api->GetRecommendPlaylist();
        for (uint64_t i = 0; i < list->length(); ++i)
        {
            
        }
    })


}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
