#ifndef XIAMIAPI_IMPL_H
#define XIAMIAPI_IMPL_H

#include <string>
#include <fstream>
#include <cpr/cpr.h>
#include <rapidjson/document.h>
#include "needvalidationexception.h"
#include "networkexception.h"
#include "xiamiapi.h"


using std::vector;
using std::string;

namespace xiamiapi
{

    cpr::Response NetworkThrowIfFailed(cpr::Response &&result);

    std::map<string, string> deserialize_cookie(string &cookie_str);

    struct XiamiUserInfo : public IXiamiUserInfo
    {

        int64_t m_Ref = 1;
        XiamiUserInfo() = default;

        XiamiUserInfo(const uint64_t &user_id, const string &nick_name, const string &avatar,
                      const string isMusician, const string isVip, const string &credits,
                      const string &level, const string numlevel) :
                user_id(user_id), nick_name(nick_name), avatar(avatar), isMusician(isMusician),
                isVip(isVip), credits(credits), level(level), numlevel(numlevel)
        {

        }

        uint64_t user_id;
        string nick_name;
        string avatar;
        string isMusician;
        string isVip;
        string credits;
        string level;
        string numlevel;

        virtual uint64_t get_user_id() const;

        virtual const char *get_nick_name() const;

        virtual const char *get_avatar() const;

        virtual const char *get_isMusician() const;

        virtual const char *get_isVip() const;

        virtual const char *get_credits() const;

        virtual const char *get_level() const;

        virtual const char *get_numlevel() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);
        virtual uint64_t AddRef();
    };

    struct XiamiPlaylistInfo : public IXiamiPlaylistInfo
    {
        XiamiPlaylistInfo() = default;

        int64_t m_Ref = 1;
        XiamiPlaylistInfo(
                const uint64_t &song_id,
                const string &sub_name,
                const uint64_t &album_id,
                const uint64_t &artist_id,
                const string &artist,
                const string &singers,
                // const vector<string> & singer_ids,
                const string &song_writers,
                const string &composer,
                const string &arrangement,
                const string &title,
                const string &album_name,
                const string &sub_title,
                const string &song_sub_title,
                const string &album_logo,
                const string &mv_url,
                const string &download_status,
                const string &location,
                const string &lyric_url,
                const string &pic,
                const string &album_pic
        ) :
                song_id(song_id),
                sub_name(sub_name),
                album_id(album_id),
                artist_id(artist_id),
                artist(artist),
                singers(singers),
                // singer_ids(singer_ids),
                song_writers(song_writers),
                composer(composer),
                arrangement(arrangement),
                title(title),
                album_name(album_name),
                sub_title(sub_title),
                song_sub_title(song_sub_title),
                album_logo(album_logo),
                mv_url(mv_url),
                download_status(download_status),
                location(location),
                lyric_url(lyric_url),
                pic(pic),
                album_pic(album_pic)
        {}

        uint64_t song_id;
        string sub_name;
        uint64_t album_id;
        uint64_t artist_id;
        string artist;
        string singers;
        // vector<string> singer_ids;
        string song_writers;
        string composer;
        string arrangement;
        string title;
        string album_name;
        string sub_title;
        string song_sub_title;
        string album_logo;
        string mv_url;
        string download_status;
        string location;
        string lyric_url;
        string pic;
        string album_pic;


        virtual uint64_t get_song_id() const;

        virtual const char *get_sub_name() const;

        virtual uint64_t get_album_id() const;

        virtual uint64_t get_artist_id() const;

        virtual const char *get_artist() const;

        virtual const char *get_singers() const;

        // Array<const char *> singer_ids;
        virtual const char *get_song_writers() const;

        virtual const char *get_composer() const;

        virtual const char *get_arrangement() const;

        virtual const char *get_title() const;

        virtual const char *get_album_name() const;

        virtual const char *get_sub_title() const;

        virtual const char *get_song_sub_title() const;

        virtual const char *get_album_logo() const;

        virtual const char *get_mv_url() const;

        virtual const char *get_download_status() const;

        virtual const char *get_location() const;

        virtual const char *get_lyric_url() const;

        virtual const char *get_pic() const;

        virtual const char *get_album_pic() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();
    };

    struct XiamiSongInfo : public IXiamiSongInfo
    {
        XiamiSongInfo() = default;

        int64_t m_Ref = 1;
        XiamiSongInfo(
                const uint64_t &song_id,
                const string &song_name,
                const uint64_t &album_id,
                const string &album_name,
                const uint64_t &artist_id,
                const string &artist_name,
                const string &singers,
                const string &listen_file,
                const bool &need_pay
        ) :
                song_id(song_id),
                song_name(song_name),
                album_id(album_id),
                album_name(album_name),
                artist_id(artist_id),
                artist_name(artist_name),
                singers(singers),
                listen_file(listen_file),
                need_pay(need_pay)
        {}

        uint64_t song_id;
        string song_name;
        uint64_t album_id;
        string album_name;
        uint64_t artist_id;
        string artist_name;
        string singers;
        string listen_file;
        bool need_pay;


        virtual uint64_t get_song_id() const;

        virtual const char *get_song_name() const;

        virtual uint64_t get_album_id() const;

        virtual const char *get_album_name() const;

        virtual uint64_t get_artist_id() const;

        virtual const char *get_artist_name() const;

        virtual const char *get_singers() const;

        virtual const char *get_listen_file() const;

        virtual bool get_need_pay() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();
    };

    struct XiamiCollectionInfo : public IXiamiCollectionInfo
    {
        XiamiCollectionInfo() = default;

        int64_t m_Ref = 1;
        XiamiCollectionInfo(
                const uint64_t &list_id,
                const string &collect_name,
                const string &logo,
                const string &user_name
        ) :
                list_id(list_id),
                collect_name(collect_name),
                logo(logo),
                user_name(user_name)
        {

        }

        uint64_t list_id;
        string collect_name;
        string user_name;
        string logo;


        virtual uint64_t get_list_id() const;

        virtual const char *get_collect_name() const;

        virtual const char *get_logo() const;

        virtual const char *get_user_name() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();
    };

    struct XiamiCollectionFullInfo : public IXiamiCollectionFullInfo
    {

        int64_t m_Ref = 1;
        XiamiCollectionFullInfo(
                const bool &is_help,
                const bool &is_public,
                const int64_t &list_id,
                const int64_t &user_id,
                const string &collect_name,
                const string &collect_logo,
                const string &original_des,
                const string &song_count,
                const string &play_count,
                const int64_t &gmt_create,
                const int64_t &gmt_modify,
                const string &views,
                const string &downloads,
                const string &favorites,
                const string &recommends,
                const string &user_name,
                const string &author_avatar,
                const bool &avatar_default,
                const bool &is_vip,
                const string &comments,
                const string &tags,
                const string &pinyin,
                const string &collects,
                const string &nick_name,
                const string &logo,
                const string &songs_count,
                const string &description,
                const string &avatar,
                const bool &type,
                const string &name
        ) :
                is_help(is_help),
                is_public(is_public),
                list_id(list_id),
                user_id(user_id),
                collect_name(collect_name),
                collect_logo(collect_logo),
                original_des(original_des),
                song_count(song_count),
                play_count(play_count),
                gmt_create(gmt_create),
                gmt_modify(gmt_modify),
                views(views),
                downloads(downloads),
                favorites(favorites),
                recommends(recommends),
                user_name(user_name),
                author_avatar(author_avatar),
                avatar_default(avatar_default),
                is_vip(is_vip),
                comments(comments),
                tags(tags),
                pinyin(pinyin),
                collects(collects),
                nick_name(nick_name),
                logo(logo),
                songs_count(songs_count),
                description(description),
                avatar(avatar),
                type(type),
                name(name)
        {

        }

        bool is_help;
        bool is_public;
        int64_t list_id;
        int64_t user_id;
        string collect_name;
        string collect_logo;
        string original_des;
        string song_count;
        string play_count;
        int64_t gmt_create;
        int64_t gmt_modify;
        string views;
        string downloads;
        string favorites;
        string recommends;
        string user_name;
        string author_avatar;
        bool avatar_default;
        bool is_vip;
        string comments;
        string tags;
        string pinyin;
        string collects;
        string nick_name;
        string logo;
        string songs_count;
        string description;
        string avatar;
        bool type;
        string name;


        virtual bool get_is_help();

        virtual bool get_is_public();

        virtual int64_t get_list_id();

        virtual int64_t get_user_id();

        virtual const char *get_collect_name();

        virtual const char *get_collect_logo();

        virtual const char *get_original_des();

        virtual const char *get_song_count();

        virtual const char *get_play_count();

        virtual int64_t get_gmt_create();

        virtual int64_t get_gmt_modify();

        virtual const char *get_views();

        virtual const char *get_downloads();

        virtual const char *get_favorites();

        virtual const char *get_recommends();

        virtual const char *get_user_name();

        virtual const char *get_author_avatar();

        virtual bool get_avatar_default();

        virtual bool get_is_vip();

        virtual const char *get_comments();

        virtual const char *get_tags();

        virtual const char *get_pinyin();

        virtual const char *get_collects();

        virtual const char *get_nick_name();

        virtual const char *get_logo();

        virtual const char *get_songs_count();

        virtual const char *get_description();

        virtual const char *get_avatar();

        virtual bool get_type();

        virtual const char *get_name();

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();
    };

    struct XiamiArtistFullInfo: public IXiamiArtistFullInfo
    {
        XiamiArtistFullInfo() = default;

        int64_t m_Ref = 1;
        XiamiArtistFullInfo(
                const int64_t &artist_id,
                const string &artist_name,
                const string &image,
                const string &alias,
                const string &area,
                const string &gender,
                const string &description,
                const string &count_likes
        ):
                artist_id(artist_id),
                artist_name(artist_name),
                image(image),
                alias(alias),
                area(area),
                gender(gender),
                description(description),
                count_likes(count_likes)
                {}
        
        int64_t artist_id;
        string artist_name;
        string image;
        string alias;
        string area;
        string gender;
        string description;
        string count_likes;

        virtual int64_t get_artist_id() const;

        virtual const char *get_artist_name() const;

        virtual const char *get_image() const;

        virtual const char * get_alias() const;

        virtual const char * get_area() const;

        virtual const char * get_gender() const;

        virtual const char * get_description() const;

        virtual const char *get_count_likes() const;

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t Release();
        virtual uint64_t AddRef();
    };

    struct XiamiArtistInfo : public IXiamiArtistInfo
    {
        XiamiArtistInfo() = default;

        int64_t m_Ref = 1;
        XiamiArtistInfo(
                const int64_t &artist_id,
                const string &name,
                const string &logo,
                const string &count_likes
        ) :
                artist_id(artist_id),
                name(name),
                logo(logo),
                count_likes(count_likes)
        {}

        int64_t artist_id;
        string name;
        string logo;
        string count_likes;

        virtual int64_t get_artist_id() const;

        virtual const char *get_name() const;

        virtual const char *get_logo() const;

        virtual const char *get_count_likes() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();
    };

    struct XiamiArtistCategoryInfo : public IXiamiArtistCategoryInfo
    {
        XiamiArtistCategoryInfo() = default;

        int64_t m_Ref = 1;
        XiamiArtistCategoryInfo(
                const string &artist_class,
                const string &artist_type,
                const string &artist_name
        ) :
                artist_class(artist_class),
                artist_type(artist_type),
                artist_name(artist_name)
        {}

        string artist_class;
        string artist_type;
        string artist_name;

        virtual const char *get_artist_class() const;

        virtual const char *get_artist_type() const;

        virtual const char *get_artist_name() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);
        virtual uint64_t AddRef();
    };

    struct XiamiHotSearchKeyInfo : public IXiamiHotSearchKeyInfo
    {
        XiamiHotSearchKeyInfo() = default;

        int64_t m_Ref = 1;
        XiamiHotSearchKeyInfo(
                const string &key,
                const string &count
        ) :
                key(key),
                count(count)
        {}

        string key;
        string count;

        virtual const char *get_key() const;

        virtual const char *get_count() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);
        virtual uint64_t AddRef();
    };

    struct XiamiRankInfo : public IXiamiRankInfo
    {

        XiamiRankInfo() = default;

        int64_t m_Ref = 1;
        XiamiRankInfo(
                const int64_t &id,
                const string &name,
                const string &logo
        ) : id(id), name(name), logo(logo)
        {}

        int64_t id;
        string name;
        string logo;

        virtual int64_t get_id() const;

        virtual const char *get_name() const;

        virtual const char *get_logo() const;

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);
        virtual uint64_t AddRef();
    };

    struct XiamiAlbumInfo : public IXiamiAlbumInfo
    {
        XiamiAlbumInfo() = default;

        int64_t m_Ref = 1;
        XiamiAlbumInfo(
                bool is_check,
                int64_t album_id,
                int64_t artist_id,
                string title,
                string artist_name,
                string album_logo,
                int64_t gmt_publish,
                bool is_play,
                string grade,
                string grade_count,
                int64_t musician,
                int64_t star,
                int64_t album_status,
                int64_t upgrade_role
        ) :
                is_check(is_check),
                album_id(album_id),
                artist_id(artist_id),
                title(title),
                artist_name(artist_name),
                album_logo(album_logo),
                gmt_publish(gmt_publish),
                is_play(is_play),
                grade(grade),
                grade_count(grade_count),
                musician(musician),
                star(star),
                album_status(album_status),
                upgrade_role(upgrade_role)
        {}

        bool is_check;
        int64_t album_id;
        int64_t artist_id;
        string title;
        string artist_name;
        string album_logo;
        int64_t gmt_publish;
        bool is_play;
        string grade;
        string grade_count;
        int64_t musician;
        int64_t star;
        int64_t album_status;
        int64_t upgrade_role;


        virtual bool get_is_check() const;

        virtual int64_t get_album_id() const;

        virtual int64_t get_artist_id() const;

        virtual const char *get_title() const;

        virtual const char *get_artist_name() const;

        virtual const char *get_album_logo() const;

        virtual int64_t get_gmt_publish() const;

        virtual bool get_is_play() const;

        virtual const char *get_grade() const;

        virtual const char *get_grade_count() const;

        virtual int64_t get_musician() const;

        virtual int64_t get_star() const;

        virtual int64_t get_album_status() const;

        virtual int64_t get_upgrade_role() const;

        uint64_t Release();

        HRESULT QueryInterface(RIID riid, void **ppv);
        virtual uint64_t AddRef();
    };

    class XiamiAPI : public IXiamiAPI
    {

    private:

        int64_t m_Ref = 1;
        inline void updateCookie(const cpr::Cookies &cookie);

        inline rapidjson::Document GetJson(const string &url, cpr::Header header);

        string XiamiHtmlLogin(const string &email, const string &password, const string &validation);

        cpr::Cookies cookies;
        string cookie_file_name;
    public:
        bool IsLogin_impl();

        XiamiAPI(const string &cookie_file_name = "cookies.txt");

        XiamiUserInfo XiamiLogin_impl(const string &email, const string &password, const string &validation);

        string GetCaptcha_impl();

        vector<XiamiSongInfo> GetArtistHotSong_impl(const int64_t &artist_id);

        vector<XiamiPlaylistInfo> GetPlaylist_impl(const string &url);

        vector<XiamiPlaylistInfo> GetPlaylist_impl(const int64_t &id, const int64_t &type);

        vector<XiamiPlaylistInfo> GetUserRecommendPlaylist_impl();

        vector<XiamiPlaylistInfo> GetRecommendPlaylist_impl();

        vector<XiamiPlaylistInfo> GetCollectionPlaylist_impl(const int64_t &collection_id);

        vector<XiamiPlaylistInfo> GetAlbumPlaylist_impl(const int64_t &album_id);

        vector<XiamiPlaylistInfo> GetSongPlaylist_impl(const int64_t &song_id);

        string CaserDecode_impl(const string &val);

        vector<XiamiCollectionInfo> GetRecommendCollection_impl();

        string DecLocation_impl(const string &location);

        vector<XiamiSongInfo> SearchSong_impl(const string &key, const int64_t &page = 1, const int64_t &limit = 25);

        XiamiSongInfo GetSong_impl(const uint64_t &song_id);

        vector<XiamiArtistInfo> GetArtistHotList_impl(const int64_t &artist_class, const int64_t &artist_type);

        vector<XiamiArtistCategoryInfo> GetArtistCategory_impl();

        vector<XiamiHotSearchKeyInfo> GetHotSearchKey_impl();

        vector<XiamiRankInfo> GetRankList_impl();

        std::basic_string<char> get_file_from_url_impl(const string &url);

        vector<XiamiCollectionFullInfo> SearchCollection_impl(std::string key, const int64_t &page = 1);

        vector<XiamiAlbumInfo> SearchAlbum_impl(std::string key, const int64_t &page = 1);

        vector<XiamiArtistFullInfo> SearchArtist_impl(const char * key, const int64_t &page = 1);

        virtual HRESULT IsLogin(bool *out);

        virtual HRESULT XiamiLogin(
                const char *email,                     // [in] email
                const char *password,                  // [in] password
                IXiamiUserInfo **out,                  // [out] user info
                const char *validation = ""            // [in] validation code
        );

        virtual HRESULT GetCaptcha(
                IStr **out                             // [out] png format captcha data
        );

        virtual HRESULT GetArtistHotSong(
                const int64_t artist_id,                    // [in] artist id
                IGenericArray **out           // [out] array of song info
        );

        virtual HRESULT GetPlaylist(const int64_t id,       // [in] playlist id
                                    const int64_t type,                         // [in] playlist type,
                                    IGenericArray **out                // [out] array of song
        );

        virtual HRESULT GetUserRecommendPlaylist(
                IGenericArray **out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        );

        virtual HRESULT GetRecommendPlaylist(
                IGenericArray **out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        );

        virtual HRESULT GetCollectionPlaylist(
                const int64_t collection_id,            // [in] collection id
                IGenericArray **out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        );

        virtual HRESULT GetAlbumPlaylist(
                const int64_t album_id,                 // [in] album id
                IGenericArray **out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        );

        virtual HRESULT GetSongPlaylist(
                const int64_t song_id,                  // [in] song id
                IGenericArray **out                // [out] array of playlist, type `IXiamiPlaylistInfo`
        );

        virtual HRESULT CaserDecode(
                const char *val,                    // [in] original string
                IStr **out                         // [out] decode result
        );

        virtual HRESULT GetRecommendCollection(
                IGenericArray **out                // [out] array of collection info, type `IXiamiCollectionInfo`
        );

        virtual HRESULT DecLocation(
                const char *location,               // [in] original location
                IStr **out                         // [out] real location
        );

        virtual HRESULT SearchSong(
                const char *key,                    // [in] search key utf8 encoded
                IGenericArray **out,               // [out] array of song info, type `IXiamiSongInfo`
                const int64_t page = 1,                 // [in] page number
                const int64_t limit = 25               // [in] count of search result per page

        );

        virtual HRESULT GetSong(
                const uint64_t song_id,                // [in] song id
                IXiamiSongInfo **out                // [out] song info
        );

        virtual HRESULT GetArtistHotList(
                const int64_t artist_class,             // [in] artist class
                const int64_t artist_type,              // [in] artist type
                IGenericArray **out                // [out] array of artist info, type `IXiamiArtistInfo`
        );

        virtual HRESULT GetArtistCategory(
                IGenericArray **out                // [out] array of category info, type `IXiamiArtistCategoryInfo`
        );

        virtual HRESULT GetHotSearchKey(
                IGenericArray **out                // [out] array of search key info, type `IXiamiHotSearchKeyInfo`
        );

        virtual HRESULT GetRankList(
                IGenericArray **out                // [out] array of rank info, type `IXiamiRankInfo`
        );

        virtual HRESULT SearchCollection(const char * key,
                                         IGenericArray **out,
                                         const int64_t page = 1);

        virtual HRESULT SearchAlbum(const char * key,
                                    IGenericArray **out,
                                    const int64_t page = 1);

        virtual HRESULT SearchArtist(const char * key,
                                     IGenericArray **out,
                                     const int64_t page = 1);

        virtual HRESULT GetFileContentFromUrl(const char *url, IStr **out);

        virtual uint64_t Release();

        virtual HRESULT QueryInterface(RIID riid, void **ppv);

        virtual uint64_t AddRef();

    };
}
#endif // XIAMIAPI_IMPL_H
