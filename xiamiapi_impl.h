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

cpr::Response NetworkThrowIfFailed(cpr::Response && result);

std::map<string, string> deserialize_cookie(string & cookie_str);

struct XiamiUserInfo: public IXiamiUserInfo
{

    XiamiUserInfo() = default;
    XiamiUserInfo(const string & user_id, const string & nick_name, const string & avatar,
                  const string isMusician, const string isVip, const string & credits,
                  const string & level, const string numlevel) :
        user_id(user_id), nick_name(nick_name), avatar(avatar), isMusician(isMusician),
        isVip(isVip), credits(credits), level(level), numlevel(numlevel)
    {

    }

    string user_id;
    string nick_name;
    string avatar;
    string isMusician;
    string isVip;
    string credits;
    string level;
    string numlevel;

    virtual const char * get_user_id() const;
    virtual const char * get_nick_name() const;
    virtual const char * get_avatar() const;
    virtual const char * get_isMusician() const;
    virtual const char * get_isVip() const;
    virtual const char * get_credits() const;
    virtual const char * get_level() const;
    virtual const char * get_numlevel() const;
    virtual void Release();
    virtual ~XiamiUserInfo();
};

struct XiamiPlaylistInfo: public IXiamiPlaylistInfo
{
    XiamiPlaylistInfo() = default;
    XiamiPlaylistInfo(
            const string & song_id,
            const string & sub_name,
            const string & album_id,
            const string & artist_id,
            const string & artist,
            const string & singers,
            // const vector<string> & singer_ids,
            const string & song_writers,
            const string & composer,
            const string & arrangement,
            const string & title,
            const string & album_name,
            const string & sub_title,
            const string & song_sub_title,
            const string & album_logo,
            const string & mv_url,
            const string & download_status,
            const string & location,
            const string & lyric_url,
            const string & pic,
            const string & album_pic
            ):
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
    { }

    string song_id;
    string sub_name;
    string album_id;
    string artist_id;
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


    virtual const char *get_song_id() const;

    virtual const char *get_sub_name() const;

    virtual const char *get_album_id() const;

    virtual const char *get_artist_id() const;

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

    virtual void Release();

};

struct XiamiSongInfo: public IXiamiSongInfo
{
    XiamiSongInfo() = default;
    XiamiSongInfo(
            const string & song_id,
            const string & song_name,
            const string & album_id,
            const string & album_name,
            const string & artist_id,
            const string & artist_name,
            const string & singers,
            const string & listen_file,
            const bool & need_pay
            ):
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

    string song_id;
    string song_name;
    string album_id;
    string album_name;
    string artist_id;
    string artist_name;
    string singers;
    string listen_file;
    bool need_pay;


    virtual const char *get_song_id() const;

    virtual const char *get_song_name() const;

    virtual const char *get_album_id() const;

    virtual const char *get_album_name() const;

    virtual const char *get_artist_id() const;

    virtual const char *get_artist_name() const;

    virtual const char *get_singers() const;

    virtual const char *get_listen_file() const;

    virtual bool get_need_pay() const;

    virtual void Release();

    virtual ~XiamiSongInfo();
};

struct XiamiCollectionInfo: public IXiamiCollectionInfo
{
    XiamiCollectionInfo() = default;
    XiamiCollectionInfo(
            const string & list_id,
            const string & collect_name,
            const string & logo,
            const string & user_name
            ) :
        list_id(list_id),
        collect_name(collect_name),
        logo(logo),
        user_name(user_name)
    {

    }

    string list_id;
    string collect_name;
    string logo;
    string user_name;

    virtual const char *get_list_id() const;

    virtual const char *get_collect_name() const;

    virtual const char *get_logo() const;

    virtual const char *get_user_name() const;

    virtual void Release();

    virtual ~XiamiCollectionInfo();
};

struct XiamiArtistInfo: public IXiamiArtistInfo
{
    XiamiArtistInfo() = default;
    XiamiArtistInfo(
            const string & artist_id,
            const string & name,
            const string & logo,
            const string & count_likes
            ):
        artist_id(artist_id),
        name(name),
        logo(logo),
        count_likes(count_likes) {}
    string artist_id;
    string name;
    string logo;
    string count_likes;

    virtual const char *get_artist_id() const;

    virtual const char *get_name() const;

    virtual const char *get_logo() const;

    virtual const char *get_count_likes() const;

    virtual void Release();

    virtual ~XiamiArtistInfo();
};

struct XiamiArtistCategoryInfo: public IXiamiArtistCategoryInfo
{
    XiamiArtistCategoryInfo() = default;
    XiamiArtistCategoryInfo(
            const string & artist_class,
            const string & artist_type,
            const string & artist_name
            ):
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

    virtual void Release();

    virtual ~XiamiArtistCategoryInfo();
};

struct XiamiHotSearchKeyInfo: public IXiamiHotSearchKeyInfo
{
    XiamiHotSearchKeyInfo() = default;
    XiamiHotSearchKeyInfo(
            const string & key,
            const string & count
            ):
        key(key),
        count(count)
    {}
    string key;
    string count;

    virtual const char *get_key() const;

    virtual const char *get_count() const;

    virtual void Release();

    virtual ~XiamiHotSearchKeyInfo();
};

struct XiamiRankInfo: public IXiamiRankInfo
{

    XiamiRankInfo() = default;
    XiamiRankInfo(
            const string & id,
            const string & name,
            const string & logo
            ) : id(id), name(name), logo(logo)
    {}
    string id;
    string name;
    string logo;

    virtual const char *get_id() const;

    virtual const char *get_name() const;

    virtual const char *get_logo() const;

    virtual void Release();

    virtual ~XiamiRankInfo();
};

class XiamiAPI: public IXiamiAPI
{

private:
    inline void updateCookie(const cpr::Cookies & cookie);

    inline rapidjson::Document GetJson(const string & url, cpr::Header header);

    string XiamiHtmlLogin(const string & email, const string & password, const string & validation);

    cpr::Cookies cookies;
    string cookie_file_name;
public:
    bool IsLogin_impl();

    XiamiAPI(const string & cookie_file_name = "cookies.txt");

    XiamiUserInfo XiamiLogin_impl(const string &email, const string &password, const string &validation);

    string GetCaptcha_impl();

    vector<XiamiSongInfo> GetArtistHotSong_impl(const int &artist_id);

    vector<XiamiPlaylistInfo> GetPlaylist_impl(const string &url);

    vector<XiamiPlaylistInfo> GetPlaylist_impl(const int &id, const int &type);

    vector<XiamiPlaylistInfo> GetUserRecommendPlaylist_impl();

    vector<XiamiPlaylistInfo> GetRecommendPlaylist_impl();

    vector<XiamiPlaylistInfo> GetCollectionPlaylist_impl(const int &collection_id);

    vector<XiamiPlaylistInfo> GetAlbumPlaylist_impl(const int &album_id);

    vector<XiamiPlaylistInfo> GetSongPlaylist_impl(const int &song_id);

    string CaserDecode_impl(const string &val);

    vector<XiamiCollectionInfo> GetRecommendCollection_impl();

    string DecLocation_impl(const string &location);

    vector<XiamiSongInfo> Search_impl(const string &key, const int &page = 1, const int &limit = 25);

    XiamiSongInfo GetSong_impl(const string &song_id);

    vector<XiamiArtistInfo> GetArtistHotList_impl(const int &artist_class, const int &artist_type);

    vector<XiamiArtistCategoryInfo> GetArtistCategory_impl();

    vector<XiamiHotSearchKeyInfo> GetHotSearchKey_impl();

    vector<XiamiRankInfo> GetRankList_impl();

    std::basic_string<char> get_file_from_url_impl(const string & url);

    virtual HRESULT IsLogin(bool * out);

    virtual HRESULT XiamiLogin(
            const char *email,                     // [in] email
            const char *password,                  // [in] password
            IXiamiUserInfo **out,                  // [out] user info
            const char *validation = ""            // [in] validation code
    );

    virtual HRESULT GetCaptcha(
            IStr ** out                             // [out] png format captcha data
    );

    virtual HRESULT GetArtistHotSong(
            const int artist_id,                    // [in] artist id
            IGenericArray ** out           // [out] array of song info
    );

    virtual HRESULT GetPlaylist(const int id,       // [in] playlist id
                                const int type,                         // [in] playlist type,
                                IGenericArray ** out                // [out] array of song
    );

    virtual HRESULT GetUserRecommendPlaylist(
            IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
    );

    virtual HRESULT GetRecommendPlaylist(
            IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
    );

    virtual HRESULT GetCollectionPlaylist(
            const int collection_id,            // [in] collection id
            IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
    );

    virtual HRESULT GetAlbumPlaylist(
            const int album_id,                 // [in] album id
            IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
    );

    virtual HRESULT GetSongPlaylist(
            const int song_id,                  // [in] song id
            IGenericArray ** out                // [out] array of playlist, type `IXiamiPlaylistInfo`
    );

    virtual HRESULT CaserDecode(
            const char *val,                    // [in] original string
            IStr ** out                         // [out] decode result
    );

    virtual HRESULT GetRecommendCollection(
            IGenericArray ** out                // [out] array of collection info, type `IXiamiCollectionInfo`
    );

    virtual HRESULT DecLocation(
            const char *location,               // [in] original location
            IStr ** out                         // [out] real location
    );

    virtual HRESULT Search(
            const char *key,                    // [in] search key utf8 encoded
            IGenericArray ** out,               // [out] array of song info, type `IXiamiSongInfo`
            const int page = 1,                 // [in] page number
            const int limit = 25               // [in] count of search result per page

    );

    virtual HRESULT GetSong(
            const char *song_id,                // [in] song id
            IXiamiSongInfo ** out                // [out] song info
    );

    virtual HRESULT GetArtistHotList(
            const int artist_class,             // [in] artist class
            const int artist_type,              // [in] artist type
            IGenericArray ** out                // [out] array of artist info, type `IXiamiArtistInfo`
    );

    virtual HRESULT GetArtistCategory(
            IGenericArray ** out                // [out] array of category info, type `IXiamiArtistCategoryInfo`
    );

    virtual HRESULT GetHotSearchKey(
            IGenericArray ** out                // [out] array of search key info, type `IXiamiHotSearchKeyInfo`
    );

    virtual HRESULT GetRankList(
            IGenericArray ** out                // [out] array of rank info, type `IXiamiRankInfo`
    );

    virtual HRESULT GetFileContentFromUrl(const char *url, IStr **out);

    virtual void Release();

    virtual ~XiamiAPI();
};
}
#endif // XIAMIAPI_IMPL_H
