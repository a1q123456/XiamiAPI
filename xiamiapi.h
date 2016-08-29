#ifndef XIAMIAPI_H
#define XIAMIAPI_H

#include <fstream>
#include <cpr/cpr.h>
#include <rapidjson/document.h>
#include "needvalidationexception.h"
#include "networkexception.h"
#include "iunknown.h"
#include "global.h"

namespace xiamiapi
{

    struct IXiamiUserInfo : public IUnknown
    {
        virtual const char *get_user_id() const = 0;

        virtual const char *get_nick_name() const = 0;

        virtual const char *get_avatar() const = 0;

        virtual const char *get_isMusician() const = 0;

        virtual const char *get_isVip() const = 0;

        virtual const char *get_credits() const = 0;

        virtual const char *get_level() const = 0;

        virtual const char *get_numlevel() const = 0;

        virtual ~IXiamiUserInfo() = 0;
    };

    struct IXiamiPlaylistInfo : public IUnknown
    {
        virtual const char *get_song_id() const = 0;

        virtual const char *get_sub_name() const = 0;

        virtual const char *get_album_id() const = 0;

        virtual const char *get_artist_id() const = 0;

        virtual const char *get_artist() const = 0;

        virtual const char *get_singers() const = 0;

        // Array<const char *> singer_ids;
        virtual const char *get_song_writers() const = 0;

        virtual const char *get_composer() const = 0;

        virtual const char *get_arrangement() const = 0;

        virtual const char *get_title() const = 0;

        virtual const char *get_album_name() const = 0;

        virtual const char *get_sub_title() const = 0;

        virtual const char *get_song_sub_title() const = 0;

        virtual const char *get_album_logo() const = 0;

        virtual const char *get_mv_url() const = 0;

        virtual const char *get_download_status() const = 0;

        virtual const char *get_location() const = 0;

        virtual const char *get_lyric_url() const = 0;

        virtual const char *get_pic() const = 0;

        virtual const char *get_album_pic() const = 0;

        virtual ~IXiamiPlaylistInfo() = 0;
    };

    struct IXiamiSongInfo : public IUnknown
    {
        virtual const char *get_song_id() const = 0;

        virtual const char *get_song_name() const = 0;

        virtual const char *get_album_id() const = 0;

        virtual const char *get_album_name() const = 0;

        virtual const char *get_artist_id() const = 0;

        virtual const char *get_artist_name() const = 0;

        virtual const char *get_singers() const = 0;

        virtual const char *get_listen_file() const = 0;

        virtual bool get_need_pay() const = 0;

        virtual ~IXiamiSongInfo() = 0;
    };

    struct IXiamiCollectionInfo : public IUnknown
    {
        virtual const char *get_list_id() const = 0;

        virtual const char *get_collect_name() const = 0;

        virtual const char *get_logo() const = 0;

        virtual const char *get_user_name() const = 0;

        virtual ~IXiamiCollectionInfo() = 0;
    };

    struct IXiamiArtistInfo : public IUnknown
    {
        virtual const char *get_artist_id() const = 0;

        virtual const char *get_name() const = 0;

        virtual const char *get_logo() const = 0;

        virtual const char *get_count_likes() const = 0;

        virtual ~IXiamiArtistInfo() = 0;
    };

    struct IXiamiArtistCategoryInfo : public IUnknown
    {
        virtual const char *get_artist_class() const = 0;

        virtual const char *get_artist_type() const = 0;

        virtual const char *get_artist_name() const = 0;

        virtual ~IXiamiArtistCategoryInfo() = 0;

    };

    struct IXiamiHotSearchKeyInfo : public IUnknown
    {
        virtual const char *get_key() const = 0;

        virtual const char *get_count() const = 0;
        virtual ~IXiamiHotSearchKeyInfo() = 0;

    };

    struct IXiamiRankInfo : public IUnknown
    {
        virtual const char *get_id() const = 0;

        virtual const char *get_name() const = 0;

        virtual const char *get_logo() const = 0;

        virtual ~IXiamiRankInfo() = 0;
    };

    struct IGenericArray: public IUnknown
    {
        virtual IUnknown * get_element(unsigned long index) = 0;
        virtual unsigned long length() = 0;
        virtual ~IGenericArray() = 0;
    };

    struct IStr: public IUnknown
    {
        virtual const char * c_str() = 0;
        virtual unsigned long length() = 0;
        virtual ~IStr() = 0;
    };

    struct IXiamiAPI : public IUnknown
    {
        static XIAMIAPI_EXPORT IXiamiAPI * CreateInstance();
        virtual HRESULT IsLogin(bool * out) = 0;

        virtual HRESULT XiamiLogin(
                const char *email,                     // [in] email
                const char *password,                  // [in] password
                IXiamiUserInfo **out,                  // [out] user info
                const char *validation = ""            // [in] validation code
        ) = 0;

        virtual HRESULT GetCaptcha(
                IStr ** out                             // [out] png format captcha data
        ) = 0;

        virtual HRESULT GetArtistHotSong(
                const int artist_id,                    // [in] artist id
                IGenericArray ** out                    // [out] array of song info
        ) = 0;

        virtual HRESULT GetPlaylist(const int id,       // [in] playlist id
                const int type,                         // [in] playlist type,
                IGenericArray ** out                // [out] array of song, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT GetUserRecommendPlaylist(
                IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT GetRecommendPlaylist(
                IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT GetCollectionPlaylist(
                const int collection_id,            // [in] collection id
                IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT GetAlbumPlaylist(
                const int album_id,                 // [in] album id
                IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT GetSongPlaylist(
                const int song_id,                  // [in] song id
                IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        ) = 0;

        virtual HRESULT CaserDecode(
                const char *val,                    // [in] original string
                IStr ** out                         // [out] decode result
        ) = 0;

        virtual HRESULT GetRecommendCollection(
                IGenericArray ** out                // [out] array of collection info, type `IXiamiCollectionInfo`
        ) = 0;

        virtual HRESULT DecLocation(
                const char *location,               // [in] original location
                IStr ** out                         // [out] real location
        ) = 0;

        virtual HRESULT Search(
                const char *key,                    // [in] search key utf8 encoded
                IGenericArray ** out,               // [out] array of song info, type `IXiamiSongInfo`
                const int page = 1,                 // [in] page number
                const int limit = 25               // [in] count of search result per page

        ) = 0;

        virtual HRESULT GetSong(
                const char *song_id,                // [in] song id
                IXiamiSongInfo ** out                // [out] song info
        ) = 0;

        virtual HRESULT GetArtistHotList(
                const int artist_class,             // [in] artist class
                const int artist_type,              // [in] artist type
                IGenericArray ** out                // [out] array of artist info, type `IXiamiArtistInfo`
        ) = 0;

        virtual HRESULT GetArtistCategory(
                IGenericArray ** out                // [out] array of category info, type `IXiamiArtistCategoryInfo`
        ) = 0;

        virtual HRESULT GetHotSearchKey(
                IGenericArray ** out                // [out] array of search key info, type `IXiamiHotSearchKeyInfo`
        ) = 0;

        virtual HRESULT GetRankList(
                IGenericArray ** out                // [out] array of rank info, type `IXiamiRankInfo`
        ) = 0;

        virtual ~IXiamiAPI() = 0;

        virtual HRESULT GetFileContentFromUrl(
                const char *url,                   // [in] file url
                IStr **out                         // [out] file content
        ) = 0;
    };
}
#endif // XIAMIAPI_H
