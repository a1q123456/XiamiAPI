#ifndef XIAMIAPI_H
#define XIAMIAPI_H

#include "iunknown.h"

#define XIAMIAPI_EXPORT __attribute__((__visibility__("default")))

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

        constexpr static const char *IID()
        {
            return "0afd9e31-8571-409f-9aba-65ed39b73e20";
        }

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

        constexpr static const char *IID()
        {
            return "56b09282-2e65-4810-ad18-d3bce8b816cb";
        }
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

        constexpr static const char *IID()
        {
            return "a0e558d1-da2e-4a4c-b846-1a2fccaf4cea";
        }
    };

    struct IXiamiCollectionInfo : public IUnknown
    {
        virtual const char *get_list_id() const = 0;

        virtual const char *get_collect_name() const = 0;

        virtual const char *get_logo() const = 0;

        virtual const char *get_user_name() const = 0;

        constexpr static const char *IID()
        {
            return "3d5fdd05-a1c4-4e19-b9f8-f116366111d5";
        }
    };

    struct IXiamiArtistInfo : public IUnknown
    {
        virtual const char *get_artist_id() const = 0;

        virtual const char *get_name() const = 0;

        virtual const char *get_logo() const = 0;

        virtual const char *get_count_likes() const = 0;

        constexpr static const char *IID()
        {
            return "0f120178-6cb7-4669-9e84-13b91dba63ab";
        }
    };

    struct IXiamiArtistCategoryInfo : public IUnknown
    {
        virtual const char *get_artist_class() const = 0;

        virtual const char *get_artist_type() const = 0;

        virtual const char *get_artist_name() const = 0;

        constexpr static const char *IID()
        {
            return "e42a5d80-3b22-4197-95ad-53eedc2204ae";
        }
    };

    struct IXiamiHotSearchKeyInfo : public IUnknown
    {
        virtual const char *get_key() const = 0;

        virtual const char *get_count() const = 0;

        constexpr static const char *IID()
        {
            return "bcecc1f9-3091-4512-9ac0-1f940899dd17";
        }
    };

    struct IXiamiRankInfo : public IUnknown
    {
        virtual const char *get_id() const = 0;

        virtual const char *get_name() const = 0;

        virtual const char *get_logo() const = 0;

        constexpr static const char *IID()
        {
            return "1a8060fc-ffeb-40f3-9ddb-16458f26aeb2";
        }
    };

    struct IGenericArray: public IUnknown
    {
        virtual IUnknown * get_element(unsigned long index) = 0;
        virtual unsigned long length() = 0;
        constexpr static const char *IID()
        {
            return "488d7f3e-4c59-4612-a7cd-bc5f364670fe";
        }
        virtual RIID ElementIID() = 0;
    };

    struct IStr: public IUnknown
    {
        virtual const char * c_str() = 0;
        virtual unsigned long length() = 0;
        constexpr static const char *IID()
        {
            return "b01db63e-6577-4e1d-a001-90572a631c1a";
        }
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

        virtual HRESULT GetFileContentFromUrl(
                const char *url,                   // [in] file url
                IStr **out                         // [out] file content
        ) = 0;

        constexpr static const char *IID()
        {
            return "a4ff86c6-abab-4a8f-9bbc-ebf7e91e9c57";
        }
    };
}
#endif // XIAMIAPI_H
