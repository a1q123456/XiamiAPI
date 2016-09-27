#include "xiamiapi_impl.h"
#include "stringutilities.h"
#include "str.h"
#include "GenericArray.h"
#include <algorithm>
#include "keynotfoundexception.h"
#include "rapidjson/pointer.h"

using std::string;
using std::vector;

namespace xiamiapi
{
    cpr::Response NetworkThrowIfFailed(cpr::Response &&result)
    {
        if (result.status_code != 200)
        {
            throw NetworkException(result.status_code, result.error.message);
        }
        return result;
    }

    std::map<string, string> deserialize_cookie(string &cookie_str)
    {
        std::map<string, string> ret;
        vector<string> lines;
        xiamiapi::StringUtilities::split(cookie_str, ';', lines);
        decltype(lines) key_value;
        for (auto i : lines)
        {
            if (i == " " || i == "")
            {
                continue;
            }
            StringUtilities::split(i, '=', key_value);
            ret[key_value[0]] = key_value[1];
        }
        return ret;
    }


    inline void XiamiAPI::updateCookie(const cpr::Cookies &cookie)
    {
        this->cookies = cookie;
        std::fstream cookie_file;
        cookie_file.open(cookie_file_name);
        cookie_file.flush();
        cookie_file << cookie.GetEncoded();
        cookie_file.close();
    }

    inline rapidjson::Document XiamiAPI::GetJson(const string &url, cpr::Header header)
    {
        rapidjson::Document ret;
        cpr::Response json = NetworkThrowIfFailed(cpr::Get(cpr::Url{url}, cookies, header));
        updateCookie(json.cookies);

        ret.Parse(json.text.c_str());
        return ret;
    }

    string XiamiAPI::XiamiHtmlLogin(const string &email, const string &password, const string &validation)
    {
        auto XiamiLoginUrl = "https://login.xiami.com/web/login";
        if (validation == "")
        {
            cpr::Response response = NetworkThrowIfFailed(cpr::Post(cpr::Url{XiamiLoginUrl}, cookies,
                                                                    cpr::Payload{{"email",       email},
                                                                                 {"password",    password},
                                                                                 {"remember",    "1"},
                                                                                 {"LoginButton", "%E7%99%BB%E5%BD%95"}
                                                                    }));
            updateCookie(response.cookies);
            return response.text;
        }
        else
        {
            cpr::Response response = NetworkThrowIfFailed(cpr::Post(cpr::Url{XiamiLoginUrl}, cookies,
                                                                    cpr::Payload{{"email",       email},
                                                                                 {"password",    password},
                                                                                 {"remember",    "1"},
                                                                                 {"LoginButton", "%E7%99%BB%E5%BD%95"},
                                                                                 {"validate",    validation}
                                                                    }));
            updateCookie(response.cookies);
            return response.text;
        }
    }

    bool XiamiAPI::IsLogin_impl()
    {
        auto xiami_url = "http://www.xiami.com/web/profile";
        cpr::Response response = NetworkThrowIfFailed(cpr::Get(cpr::Url{xiami_url}, cookies));
        updateCookie(response.cookies);
        return response.url == xiami_url;
    }


    XiamiAPI::XiamiAPI(const string &cookie_file_name)
    {
        auto xiami_url = "http://www.xiami.com/web/profile";
        this->cookie_file_name = cookie_file_name;
        std::fstream cookie_file;
        cookie_file.open(cookie_file_name, std::ios_base::in);
        if (!cookie_file.is_open())
        {
            cookie_file.clear();
            cookie_file.open(cookie_file_name, std::ios_base::out);
            cookie_file.close();
            cookie_file.clear();
            cookie_file.open(cookie_file_name, std::ios_base::in);
        }
        if (cookie_file.eof())
        {
            cpr::Response r = cpr::Get(cpr::Url{xiami_url});
            cookies = r.cookies;
            cookie_file << r.cookies.GetEncoded();
        }
        else
        {
            cookie_file.seekg(0, cookie_file.end);
            auto size = cookie_file.tellg();
            string str(static_cast<uint64_t>(size), '\0');
            cookie_file.seekg(0);
            cookie_file.read(&str[0], size);
            cookies = cpr::Cookies(deserialize_cookie(str));
        }
        cookie_file.close();
    }

    XiamiUserInfo XiamiAPI::XiamiLogin_impl(const string &email, const string &password, const string &validation)
    {
        if (!IsLogin_impl())
        {
            auto ret = XiamiHtmlLogin(email, password, validation);
            if (ret.find("id=\"validate\"") != string::npos)
            {
                // xml parse
                throw NeedValidationException();
            }
        }

        const rapidjson::Document &json_doc = GetJson("http://www.xiami.com/index/home", {});

        if (json_doc["data"].IsNull() || json_doc["data"]["userInfo"].IsArray())
        {
            throw std::runtime_error("Login failure");
        }

        XiamiUserInfo ret(
                static_cast<uint64_t >(std::atoi(json_doc["data"]["userInfo"]["user_id"].GetString())),
                json_doc["data"]["userInfo"]["nick_name"].GetString(),
                json_doc["data"]["userInfo"]["avatar"].GetString(),
                std::to_string(json_doc["data"]["userInfo"]["isMusician"].GetInt()),
                std::to_string(json_doc["data"]["userInfo"]["isVip"].GetInt()),
                json_doc["data"]["userInfo"]["credits"].GetString(),
                json_doc["data"]["userInfo"]["level"].GetString(),
                std::to_string(json_doc["data"]["userInfo"]["numlevel"].GetInt())
        );
        return ret;
    }

    string XiamiAPI::GetCaptcha_impl()
    {
        cpr::Response response = NetworkThrowIfFailed(
                cpr::Get(cpr::Url{"https://login.xiami.com/coop/checkcode?forlogin=1"}, cookies));
        updateCookie(response.cookies);
        return response.text;
    }

    vector<XiamiSongInfo> XiamiAPI::GetArtistHotSong_impl(const int64_t &artist_id)
    {
        vector<XiamiSongInfo> ret;
        auto api_url = StringUtilities::string_format(
                "http://api.xiami.com/web?v=2.0&app_key=1&id={artist_id}&r=artist/hot-songs",
                {{"artist_id", artist_id}});
        const rapidjson::Document &json_doc = GetJson(api_url,
                                                      cpr::Header{{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &hot_song_data = json_doc["data"];
        for (rapidjson::SizeType i = 0; i < hot_song_data.Size(); i++)
        {
            ret.push_back(XiamiSongInfo(
                    static_cast<uint64_t >(hot_song_data[i]["song_id"].GetInt64()),
                    hot_song_data[i]["song_name"].GetString(),
                    0,
                    "",
                    0,
                    "",
                    hot_song_data[i]["singers"].GetString(),
                    "",
                    hot_song_data[i]["need_pay_flag"].GetInt() != 0
            ));
        }

        return ret;
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetPlaylist_impl(const string &url)
    {

        vector<XiamiPlaylistInfo> ret;
        const rapidjson::Document &json_doc = GetJson(url, {});
        if (!(json_doc["message"].IsNull() || (json_doc["message"].IsString() && std::strcmp(json_doc["message"].GetString(), ""))))
        {
            throw std::runtime_error("数据获取错误");
        }
        const rapidjson::Value &track_list = json_doc["data"]["trackList"];
        for (rapidjson::SizeType i = 0; i < track_list.Size(); i++)
        {
            auto info = XiamiPlaylistInfo(
                    static_cast<uint64_t>(std::atoi(track_list[i]["songId"].GetString())),
                    track_list[i]["subName"].GetString(),
                    track_list[i]["albumId"].GetUint64(),
                    track_list[i]["artistId"].GetUint64(),
                    track_list[i]["artist"].GetString(),
                    track_list[i]["singers"].GetString(),
                    // track_list[i]["singerIds"],
                    track_list[i]["songwriters"].GetString(),
                    track_list[i]["composer"].GetString(),
                    track_list[i]["arrangement"].GetString(),
                    track_list[i]["title"].GetString(),
                    track_list[i]["album_name"].GetString(),
                    track_list[i]["sub_title"].GetString(),
                    track_list[i]["song_sub_title"].GetString(),
                    track_list[i]["album_logo"].GetString(),
                    track_list[i]["mvUrl"].GetString(),
                    std::to_string(track_list[i]["downloadstatus"].GetInt()),
                    DecLocation_impl(track_list[i]["location"].GetString()),
                    track_list[i]["lyric_url"].GetString(),
                    track_list[i]["pic"].GetString(),
                    track_list[i]["album_pic"].GetString()
            );
            ret.push_back(info);
        }
        return ret;
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetPlaylist_impl(const int64_t &id, const int64_t &type)
    {
        auto api_url = StringUtilities::string_format("http://www.xiami.com/song/playlist/id/{id}/type/{type}/cat/json",
                                                      {{"id",   id},
                                                       {"type", type}});
        return GetPlaylist_impl(api_url);
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetUserRecommendPlaylist_impl()
    {
        return GetPlaylist_impl(1, 9);
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetRecommendPlaylist_impl()
    {
        return GetPlaylist_impl(1, 15);
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetCollectionPlaylist_impl(const int64_t &collection_id)
    {
        return GetPlaylist_impl(collection_id, 3);
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetAlbumPlaylist_impl(const int64_t &album_id)
    {
        return GetPlaylist_impl(album_id, 1);
    }

    vector<XiamiPlaylistInfo> XiamiAPI::GetSongPlaylist_impl(const int64_t &song_id)
    {
        auto api_url = StringUtilities::string_format(
                "http://www.xiami.com/song/playlist/id/{song_id}/object_name/default/object_id/0/cat/json",
                {{"song_id", song_id}});
        return GetPlaylist_impl(api_url);
    }

    string XiamiAPI::CaserDecode_impl(const string &val)
    {
        auto cipher_len = val.length() - 1;
        int64_t rows = val[0] - '0';
        size_t cols = cipher_len / rows;
        size_t offset_rows = cipher_len % rows;
        auto text = val.substr(1);
        std::string plaintext(text);

        for (std::size_t i = 0; i < cipher_len; i++)
        {
            size_t x = i % rows;
            size_t y = i / rows;
            size_t p = 0;
            if (x <= offset_rows)
            {
                p = x * (cols + 1) + y;
            }
            else
            {
                p = offset_rows * (cols + 1) + (x - offset_rows) * cols + y;
            }
            plaintext[i] = text[p];
        }
        return plaintext;
    }

    vector<XiamiCollectionInfo> XiamiAPI::GetRecommendCollection_impl()
    {
        vector<XiamiCollectionInfo> ret;
        auto api_url = "http://api.xiami.com/web?v=2.0&app_key=1&r=collect/recommend";
        const rapidjson::Document &json_doc = GetJson(api_url,
                                                      cpr::Header{{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &collection_data = json_doc["data"];
        for (rapidjson::SizeType i = 0; i < collection_data.Size(); i++)
        {
            ret.push_back(XiamiCollectionInfo(
                    collection_data[i]["list_id"].GetUint64(),
                    collection_data[i]["collect_name"].GetString(),
                    collection_data[i]["logo"].GetString(),
                    collection_data[i]["user_name"].GetString()
            ));
        }
        return ret;
    }

    string XiamiAPI::DecLocation_impl(const string &location)
    {
        string ret = StringUtilities::url_unescape(CaserDecode_impl(location));
        std::replace(ret.begin(), ret.end(), '^', '0');
        return ret;
    }

    vector<XiamiSongInfo> XiamiAPI::SearchSong_impl(const string &key, const int64_t &page, const int64_t &limit)
    {
        auto escaped_key = StringUtilities::url_escape(key, false);
        auto api_url = StringUtilities::string_format(
                "http://api.xiami.com/web?v=2.0&app_key=1&key={key}&page={page}&limit={limit}&r=search/songs",
                {{"key",   escaped_key},
                 {"page",  page},
                 {"limit", limit}});
        const rapidjson::Document &json_doc = GetJson(api_url, {{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &song_doc = json_doc["data"]["songs"];
        vector<XiamiSongInfo> ret;
        for (rapidjson::SizeType i = 0; i < song_doc.Size(); i++)
        {
            ret.push_back(XiamiSongInfo(
                    song_doc[i]["song_id"].GetUint64(),
                    song_doc[i]["song_name"].GetString(),
                    song_doc[i]["album_id"].GetUint64(),
                    song_doc[i]["album_name"].GetString(),
                    song_doc[i]["artist_id"].GetUint64(),
                    song_doc[i]["artist_name"].GetString(),
                    "",
                    song_doc[i]["listen_file"].GetString(),
                    song_doc[i]["need_pay_flag"].GetInt() != 0
            ));
        }
        return ret;
    }

    XiamiSongInfo XiamiAPI::GetSong_impl(const uint64_t &song_id)
    {
        auto api_url = StringUtilities::string_format(
                "http://api.xiami.com/web?v=2.0&app_key=1&id={song_id}&r=song/detail", {{"song_id", song_id}});
        const rapidjson::Document &json_doc = GetJson(api_url, {{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &song_doc = json_doc["data"]["song"];
        return XiamiSongInfo(
                song_doc["song_id"].GetUint64(),
                song_doc["song_name"].GetString(),
                song_doc["album_id"].GetUint64(),
                song_doc["album_name"].GetString(),
                song_doc["artist_id"].GetUint64(),
                song_doc["artist_name"].GetString(),
                song_doc["singers"].GetString(),
                song_doc["listen_file"].GetString(),
                song_doc["need_pay_flag"].GetInt() != 0
        );
    }

    vector<XiamiArtistInfo> XiamiAPI::GetArtistHotList_impl(const int64_t &artist_class, const int64_t &artist_type)
    {
        auto api_url = StringUtilities::string_format(
                "http://api.xiami.com/web?v=2.0&app_key=1&class={artist_class}&type={artist_type}&r=artist/hot-list",
                {{"artist_class", artist_class},
                 {"artist_type",  artist_type}});
        const rapidjson::Document &json_doc = GetJson(api_url, {{"Referer", "http://h.xiami.com/index.html"}});
        vector<XiamiArtistInfo> ret;
        const rapidjson::Value &artist_data = json_doc["data"]["artists"];
        for (rapidjson::SizeType i = 0; i < artist_data.Size(); i++)
        {
            ret.push_back(XiamiArtistInfo(
                    artist_data[i]["artist_id"].GetInt64(),
                    artist_data[i]["name"].GetString(),
                    artist_data[i]["logo"].GetString(),
                    std::to_string(artist_data[i]["count_likes"].GetInt64())
            ));
        }
        return ret;
    }

    vector<XiamiArtistCategoryInfo> XiamiAPI::GetArtistCategory_impl()
    {
        vector<XiamiArtistCategoryInfo> ret;
        auto api_url = "http://api.xiami.com/web?v=2.0&app_key=1&r=artist/category";
        const rapidjson::Document &json_doc = GetJson(api_url, {{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &category_data = json_doc["data"];
        for (rapidjson::SizeType i = 0; i < category_data.Size(); i++)
        {
            ret.push_back(XiamiArtistCategoryInfo(
                    std::to_string(category_data[i]["class"].GetInt64()),
                    std::to_string(category_data[i]["type"].GetInt64()),
                    category_data[i]["name"].GetString()
            ));
        }
        return ret;
    }

    vector<XiamiHotSearchKeyInfo> XiamiAPI::GetHotSearchKey_impl()
    {
        vector<XiamiHotSearchKeyInfo> ret;
        auto api_url = "http://api.xiami.com/web?v=2.0&app_key=1&r=search/hot";
        const rapidjson::Document &json_doc = GetJson(api_url, {{"Referer", "http://h.xiami.com/index.html"}});
        const rapidjson::Value &key_data = json_doc["data"];
        for (rapidjson::SizeType i = 0; i < key_data.Size(); i++)
        {
            ret.push_back(XiamiHotSearchKeyInfo(
                    key_data[i]["key"].GetString(),
                    std::to_string(key_data[i]["count"].GetInt64())
            ));
        }

        return ret;
    }

    vector<XiamiRankInfo> XiamiAPI::GetRankList_impl()
    {
        static const vector<XiamiRankInfo> ret{
                XiamiRankInfo(101, "虾米音乐榜", "http://gtms01.alicdn.com/tps/i1/T19LocFghXXXXsGF3s-640-640.png"),
                XiamiRankInfo(103, "虾米原创榜", "http://gtms01.alicdn.com/tps/i1/T1qMgSFlxkXXXsGF3s-640-640.png"),
                XiamiRankInfo(1, "Hito中文排行榜", "http://res.xiami.net/static/app/img/tw@2x.png"),
                XiamiRankInfo(2, "香港劲歌金榜", "http://res.xiami.net/static/app/img/hk@2x.png"),
                XiamiRankInfo(3, "英国UK单曲榜", "http://res.xiami.net/static/app/img/uk@2x.png"),
                XiamiRankInfo(4, "Billboard单曲榜", "http://res.xiami.net/static/app/img/us@2x.png"),
                XiamiRankInfo(5, "Oricon公信单曲榜", "http://res.xiami.net/static/app/img/jp@2x.png"),
                XiamiRankInfo(6, "M-net综合数据周榜", "http://res.xiami.net/static/app/img/kr@2x.png"),
                XiamiRankInfo(106, "陌陌试听榜", "http://res.xiami.net/static/app/img/momo.png"),
                XiamiRankInfo(31, "音乐风云榜", "http://img.xiami.net/img/rank/top12.png"),
                XiamiRankInfo(10011, "微信分享榜", "http: //img.xiami.net/img/rank/top13.png"),
                XiamiRankInfo(10012, "微博分享榜", "http: //img.xiami.net/img/rank/top14.png"),
                XiamiRankInfo(10013, "大虾试听榜", "http: //img.xiami.net/img/rank/top15.png"),
                XiamiRankInfo(10014, "精选集收录榜", "http: //img.xiami.net/img/rank/top16.png")
        };
        return ret;
    }

    std::basic_string<char> XiamiAPI::get_file_from_url_impl(const string &url)
    {
        cpr::Response result = NetworkThrowIfFailed(cpr::Get(cpr::Url{url},
                                                             cookies,
                                                             cpr::Header{{"Referer", "http://h.xiami.com/index.html"}}));
        updateCookie(result.cookies);
        return result.text;
    }

    vector<XiamiCollectionFullInfo> XiamiAPI::SearchCollection_impl(std::string key, const int64_t &page)
    {
        vector<XiamiCollectionFullInfo> ret;
        cpr::Response result = NetworkThrowIfFailed(cpr::Post(
                cpr::Url{"http://www.xiami.com/app/xiating/search-collect2"},
                cpr::Payload{{"key",      key},
                             {"callback", "xiami"},
                             {"page",     StringUtilities::to_string(page)}},
                cpr::Cookies{{"_xiamitoken", cookies["_xiamitoken"]},
                             {"av",          "4.5"}}
        ));
        updateCookie(result.cookies);
        rapidjson::Document json;
        json.Parse(result.text.c_str());
        rapidjson::Value & data = json["data"];
        for (rapidjson::SizeType i = 0; i < data.Size(); i++)
        {
            ret.push_back(XiamiCollectionFullInfo(
                    data[i]["is_help"].GetBool(),
                    data[i]["is_public"].GetBool(),
                    data[i]["list_id"].GetInt64(),
                    data[i]["user_id"].GetInt64(),
                    data[i]["collect_name"].GetString(),
                    data[i]["collect_logo"].GetString(),
                    data[i]["original_des"].GetString(),
                    std::to_string(data[i]["song_count"].GetInt64()),
                    std::to_string(data[i]["play_count"].GetInt64()),
                    data[i]["gmt_create"].GetInt64(),
                    data[i]["gmt_modify"].GetInt64(),
                    std::to_string(data[i]["views"].GetInt64()),
                    std::to_string(data[i]["downloads"].GetInt64()),
                    std::to_string(data[i]["favorites"].GetInt64()),
                    std::to_string(data[i]["recommends"].GetInt64()),
                    data[i]["user_name"].GetString(),
                    data[i]["author_avatar"].GetString(),
                    data[i]["avatar_default"].GetBool(),
                    data[i]["is_vip"].GetInt() == 1,
                    std::to_string(data[i]["comments"].GetInt64()),
                    data[i]["tags"].GetString(),
                    data[i]["pinyin"].GetString(),
                    std::to_string(data[i]["collects"].GetInt64()),
                    data[i]["nick_name"].GetString(),
                    data[i]["logo"].GetString(),
                    std::to_string(data[i]["songs_count"].GetInt64()),
                    data[i]["description"].GetString(),
                    data[i]["avatar"].GetString(),
                    data[i]["type"].GetBool(),
                    data[i]["name"].GetString()
            ));
        }
        return ret;
    }

    vector<XiamiAlbumInfo> XiamiAPI::SearchAlbum_impl(std::string key, const int64_t &page)
    {

        vector<XiamiAlbumInfo> ret;
        cpr::Response result = NetworkThrowIfFailed(cpr::Post(
                cpr::Url{"http://www.xiami.com/app/xiating/search-album2"},
                cpr::Payload{{"key",      key},
                             {"callback", "xiami"},
                             {"page",     StringUtilities::to_string(page)}},
                cpr::Cookies{{"_xiamitoken", cookies["_xiamitoken"]},
                             {"av",          "4.5"}}
        ));
        updateCookie(result.cookies);
        rapidjson::Document json;
        json.Parse(result.text.c_str());
        rapidjson::Value & data = json["data"];
        for (rapidjson::SizeType i = 0; i < data.Size(); i++)
        {
            ret.push_back(XiamiAlbumInfo(
                    data[i]["is_check"].GetInt64() == 1,
                    data[i]["album_id"].GetInt64(),
                    data[i]["artist_id"].GetInt64(),
                    data[i]["title"].GetString(),
                    data[i]["artist_name"].GetString(),
                    data[i]["album_logo"].GetString(),
                    data[i]["gmt_publish"].GetInt64(),
                    data[i]["is_play"].GetInt64() == 1,
                    data[i]["grade"].GetString(),
                    data[i]["grade_count"].GetString(),
                    data[i]["musician"].GetInt64(),
                    data[i]["star"].GetInt64(),
                    data[i]["album_status"].GetInt64(),
                    data[i]["upgrade_role"].GetInt64()
            ));
        }
        return ret;
    }

    vector<XiamiArtistFullInfo> XiamiAPI::SearchArtist_impl(const char * key, const int64_t &page)
    {
        vector<XiamiArtistFullInfo> ret;
        cpr::Response result = NetworkThrowIfFailed(cpr::Post(
                cpr::Url{"http://www.xiami.com/app/xiating/search-artist2"},
                cpr::Payload{{"key",      key},
                             {"callback", "xiami"},
                             {"page",     StringUtilities::to_string(page)}},
                cpr::Cookies{{"_xiamitoken", cookies["_xiamitoken"]},
                             {"av",          "4.5"}}
        ));
        updateCookie(result.cookies);
        rapidjson::Document json;
        json.Parse(result.text.c_str());
        rapidjson::Value & data = json["data"];
        for (rapidjson::SizeType i = 0; i < data.Size(); i++)
        {
            ret.push_back(XiamiArtistFullInfo(
                    data[i]["artist_id"].GetInt64(),
                    data[i]["artist_name"].GetString(),
                    data[i]["image_d_o"].IsNull()?"": data[i]["image_d_o"]["path"].GetString(),
                    data[i]["alias"].GetString(),
                    data[i]["area"].GetString(),
                    data[i]["gender"].GetString(),
                    data[i]["description"].GetString(),
                    std::to_string(data[i]["count_likes"].GetInt64())
            ));
        }
        return ret;
    }

    HRESULT XiamiAPI::IsLogin(bool *out)
    {
        try
        {
            *out = IsLogin_impl();
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::XiamiLogin(const char *email, const char *password, IXiamiUserInfo **out, const char *validation)
    {
        try
        {
            *out = new XiamiUserInfo(XiamiLogin_impl(email, password, validation));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (NeedValidationException)
        {
            return 1002UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetCaptcha(IStr **out)
    {
        try
        {
            *out = new Str(GetCaptcha_impl());
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetArtistHotSong(const int64_t artist_id, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetArtistHotSong_impl(artist_id), __uuidof(IXiamiSongInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetPlaylist(const int64_t id, const int64_t type, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetPlaylist_impl(id, type), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetUserRecommendPlaylist(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetUserRecommendPlaylist_impl(), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetRecommendPlaylist(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetRecommendPlaylist_impl(), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetCollectionPlaylist(const int64_t collection_id, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetCollectionPlaylist_impl(collection_id), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetAlbumPlaylist(const int64_t album_id, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetAlbumPlaylist_impl(album_id), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetSongPlaylist(const int64_t song_id, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetSongPlaylist_impl(song_id), __uuidof(IXiamiPlaylistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::CaserDecode(const char *val, IStr **out)
    {
        try
        {
            *out = new Str(CaserDecode_impl(val));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetRecommendCollection(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetRecommendCollection_impl(), __uuidof(IXiamiCollectionInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::DecLocation(const char *location, IStr **out)
    {
        try
        {
            *out = new Str(DecLocation_impl(location));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::SearchSong(const char *key, IGenericArray **out, const int64_t page, const int64_t limit)
    {
        try
        {
            *out = new GenericArray(SearchSong_impl(key, page, limit), __uuidof(IXiamiSongInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetSong(const uint64_t song_id, IXiamiSongInfo **out)
    {
        try
        {
            *out = new XiamiSongInfo(GetSong_impl(song_id));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetArtistHotList(const int64_t artist_class, const int64_t artist_type, IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetArtistHotList_impl(artist_class, artist_type), __uuidof(IXiamiArtistInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetArtistCategory(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetArtistCategory_impl(), __uuidof(IXiamiArtistCategoryInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetHotSearchKey(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetHotSearchKey_impl(), __uuidof(IXiamiHotSearchKeyInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::GetRankList(IGenericArray **out)
    {
        try
        {
            *out = new GenericArray(GetRankList_impl(), __uuidof(IXiamiRankInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    uint64_t XiamiAPI::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }


    HRESULT XiamiAPI::GetFileContentFromUrl(const char *url, IStr **out)
    {
        try
        {
            *out = new Str(get_file_from_url_impl(url));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiAPI))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    HRESULT XiamiAPI::SearchCollection(const char *key, IGenericArray **out, const int64_t page)
    {
        try
        {
            *out = new GenericArray(SearchCollection_impl(key, page), __uuidof(XiamiCollectionFullInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::SearchAlbum(const char *key, IGenericArray **out, const int64_t page)
    {
        try
        {
            *out = new GenericArray(SearchAlbum_impl(key, page), __uuidof(XiamiAlbumInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    HRESULT XiamiAPI::SearchArtist(const char *key, IGenericArray **out, const int64_t page)
    {
        try
        {
            *out = new GenericArray(SearchArtist_impl(key, page), __uuidof(XiamiArtistFullInfo));
            return S_OK;
        }
        catch (NetworkException)
        {
            return 1001UL;
        }
        catch (...)
        {
            return FAIL;
        }
    }

    uint64_t XiamiAPI::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    uint64_t XiamiPlaylistInfo::get_song_id() const
    {
        return song_id;
    }

    const char *XiamiPlaylistInfo::get_sub_name() const
    {
        return sub_name.c_str();
    }

    uint64_t XiamiPlaylistInfo::get_album_id() const
    {
        return album_id;
    }

    uint64_t XiamiPlaylistInfo::get_artist_id() const
    {
        return artist_id;
    }

    const char *XiamiPlaylistInfo::get_artist() const
    {
        return artist.c_str();
    }

    const char *XiamiPlaylistInfo::get_singers() const
    {
        return singers.c_str();
    }

    const char *XiamiPlaylistInfo::get_song_writers() const
    {
        return song_writers.c_str();
    }

    const char *XiamiPlaylistInfo::get_composer() const
    {
        return composer.c_str();
    }

    const char *XiamiPlaylistInfo::get_arrangement() const
    {
        return arrangement.c_str();
    }

    const char *XiamiPlaylistInfo::get_title() const
    {
        return title.c_str();
    }

    const char *XiamiPlaylistInfo::get_album_name() const
    {
        return album_name.c_str();
    }

    const char *XiamiPlaylistInfo::get_sub_title() const
    {
        return sub_title.c_str();
    }

    const char *XiamiPlaylistInfo::get_song_sub_title() const
    {
        return song_sub_title.c_str();
    }

    const char *XiamiPlaylistInfo::get_album_logo() const
    {
        return album_logo.c_str();
    }

    const char *XiamiPlaylistInfo::get_mv_url() const
    {
        return mv_url.c_str();
    }

    const char *XiamiPlaylistInfo::get_download_status() const
    {
        return download_status.c_str();
    }

    const char *XiamiPlaylistInfo::get_location() const
    {
        return location.c_str();
    }

    const char *XiamiPlaylistInfo::get_lyric_url() const
    {
        return lyric_url.c_str();
    }

    const char *XiamiPlaylistInfo::get_pic() const
    {
        return pic.c_str();
    }

    const char *XiamiPlaylistInfo::get_album_pic() const
    {
        return album_pic.c_str();
    }

    uint64_t XiamiPlaylistInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiPlaylistInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiPlaylistInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiPlaylistInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    uint64_t XiamiUserInfo::get_user_id() const
    {
        return user_id;
    }

    const char *XiamiUserInfo::get_nick_name() const
    {
        return nick_name.c_str();
    }

    const char *XiamiUserInfo::get_avatar() const
    {
        return avatar.c_str();
    }

    const char *XiamiUserInfo::get_isMusician() const
    {
        return isMusician.c_str();
    }

    const char *XiamiUserInfo::get_isVip() const
    {
        return isVip.c_str();
    }

    const char *XiamiUserInfo::get_credits() const
    {
        return credits.c_str();
    }

    const char *XiamiUserInfo::get_level() const
    {
        return level.c_str();
    }

    const char *XiamiUserInfo::get_numlevel() const
    {
        return numlevel.c_str();
    }

    uint64_t XiamiUserInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiUserInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiUserInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiUserInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    uint64_t XiamiSongInfo::get_song_id() const
    {
        return song_id;
    }

    const char *XiamiSongInfo::get_song_name() const
    {
        return song_name.c_str();
    }

    uint64_t XiamiSongInfo::get_album_id() const
    {
        return album_id;
    }

    const char *XiamiSongInfo::get_album_name() const
    {
        return album_name.c_str();
    }

    uint64_t XiamiSongInfo::get_artist_id() const
    {
        return artist_id;
    }

    const char *XiamiSongInfo::get_artist_name() const
    {
        return artist_name.c_str();
    }

    const char *XiamiSongInfo::get_singers() const
    {
        return singers.c_str();
    }

    const char *XiamiSongInfo::get_listen_file() const
    {
        return listen_file.c_str();
    }

    bool XiamiSongInfo::get_need_pay() const
    {
        return need_pay;
    }

    uint64_t XiamiSongInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiSongInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiSongInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiSongInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }


    uint64_t XiamiCollectionInfo::get_list_id() const
    {
        return list_id;
    }

    const char *XiamiCollectionInfo::get_collect_name() const
    {
        return collect_name.c_str();
    }

    const char *XiamiCollectionInfo::get_logo() const
    {
        return logo.c_str();
    }

    const char *XiamiCollectionInfo::get_user_name() const
    {
        return user_name.c_str();
    }

    uint64_t XiamiCollectionInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiCollectionInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiCollectionInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiCollectionInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    int64_t XiamiArtistInfo::get_artist_id() const
    {
        return artist_id;
    }

    const char *XiamiArtistInfo::get_name() const
    {
        return name.c_str();
    }

    const char *XiamiArtistInfo::get_logo() const
    {
        return logo.c_str();
    }

    const char *XiamiArtistInfo::get_count_likes() const
    {
        return count_likes.c_str();
    }

    uint64_t XiamiArtistInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiArtistInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiArtistInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiArtistInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }


    const char *XiamiArtistCategoryInfo::get_artist_class() const
    {
        return artist_class.c_str();
    }

    const char *XiamiArtistCategoryInfo::get_artist_type() const
    {
        return artist_type.c_str();
    }

    const char *XiamiArtistCategoryInfo::get_artist_name() const
    {
        return artist_name.c_str();
    }

    uint64_t XiamiArtistCategoryInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiArtistCategoryInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiArtistCategoryInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiArtistCategoryInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    const char *XiamiHotSearchKeyInfo::get_key() const
    {
        return key.c_str();
    }

    const char *XiamiHotSearchKeyInfo::get_count() const
    {
        return count.c_str();
    }

    uint64_t XiamiHotSearchKeyInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiHotSearchKeyInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiHotSearchKeyInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiHotSearchKeyInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    int64_t XiamiRankInfo::get_id() const
    {
        return id;
    }

    const char *XiamiRankInfo::get_name() const
    {
        return name.c_str();
    }

    const char *XiamiRankInfo::get_logo() const
    {
        return logo.c_str();
    }

    uint64_t XiamiRankInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiRankInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(IXiamiRankInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiRankInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    bool XiamiCollectionFullInfo::get_is_help()
    {
        return is_help;
    }

    bool XiamiCollectionFullInfo::get_is_public()
    {
        return is_public;
    }

    int64_t XiamiCollectionFullInfo::get_list_id()
    {
        return list_id;
    }

    int64_t XiamiCollectionFullInfo::get_user_id()
    {
        return user_id;
    }

    const char *XiamiCollectionFullInfo::get_collect_name()
    {
        return collect_name.c_str();
    }

    const char *XiamiCollectionFullInfo::get_collect_logo()
    {
        return collect_logo.c_str();
    }

    const char *XiamiCollectionFullInfo::get_original_des()
    {
        return original_des.c_str();
    }

    const char *XiamiCollectionFullInfo::get_song_count()
    {
        return song_count.c_str();
    }

    const char *XiamiCollectionFullInfo::get_play_count()
    {
        return play_count.c_str();
    }

    int64_t XiamiCollectionFullInfo::get_gmt_create()
    {
        return gmt_create;
    }

    int64_t XiamiCollectionFullInfo::get_gmt_modify()
    {
        return gmt_modify;
    }

    const char *XiamiCollectionFullInfo::get_views()
    {
        return views.c_str();
    }

    const char *XiamiCollectionFullInfo::get_downloads()
    {
        return downloads.c_str();
    }

    const char *XiamiCollectionFullInfo::get_favorites()
    {
        return favorites.c_str();
    }

    const char *XiamiCollectionFullInfo::get_recommends()
    {
        return recommends.c_str();
    }

    const char *XiamiCollectionFullInfo::get_user_name()
    {
        return user_name.c_str();
    }

    const char *XiamiCollectionFullInfo::get_author_avatar()
    {
        return author_avatar.c_str();
    }

    bool XiamiCollectionFullInfo::get_avatar_default()
    {
        return avatar_default;
    }

    bool XiamiCollectionFullInfo::get_is_vip()
    {
        return is_vip;
    }

    const char *XiamiCollectionFullInfo::get_comments()
    {
        return comments.c_str();
    }

    const char *XiamiCollectionFullInfo::get_tags()
    {
        return tags.c_str();
    }

    const char *XiamiCollectionFullInfo::get_pinyin()
    {
        return pinyin.c_str();
    }

    const char *XiamiCollectionFullInfo::get_collects()
    {
        return collects.c_str();
    }

    const char *XiamiCollectionFullInfo::get_nick_name()
    {
        return nick_name.c_str();
    }

    const char *XiamiCollectionFullInfo::get_logo()
    {
        return logo.c_str();
    }

    const char *XiamiCollectionFullInfo::get_songs_count()
    {
        return songs_count.c_str();
    }

    const char *XiamiCollectionFullInfo::get_description()
    {
        return description.c_str();
    }

    const char *XiamiCollectionFullInfo::get_avatar()
    {
        return avatar.c_str();
    }

    bool XiamiCollectionFullInfo::get_type()
    {
        return type;
    }

    const char *XiamiCollectionFullInfo::get_name()
    {
        return name.c_str();
    }

    uint64_t XiamiCollectionFullInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiCollectionFullInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(XiamiCollectionFullInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiCollectionFullInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }


    bool XiamiAlbumInfo::get_is_check() const
    {
        return is_check;
    }

    int64_t XiamiAlbumInfo::get_album_id() const
    {
        return album_id;
    }

    int64_t XiamiAlbumInfo::get_artist_id() const
    {
        return artist_id;
    }

    const char *XiamiAlbumInfo::get_title() const
    {
        return title.c_str();
    }

    const char *XiamiAlbumInfo::get_artist_name() const
    {
        return artist_name.c_str();
    }

    const char *XiamiAlbumInfo::get_album_logo() const
    {
        return album_logo.c_str();
    }

    int64_t XiamiAlbumInfo::get_gmt_publish() const
    {
        return gmt_publish;
    }

    bool XiamiAlbumInfo::get_is_play() const
    {
        return is_play;
    }

    const char *XiamiAlbumInfo::get_grade() const
    {
        return grade.c_str();
    }

    const char *XiamiAlbumInfo::get_grade_count() const
    {
        return grade_count.c_str();
    }

    int64_t XiamiAlbumInfo::get_musician() const
    {
        return musician;
    }

    int64_t XiamiAlbumInfo::get_star() const
    {
        return star;
    }

    int64_t XiamiAlbumInfo::get_album_status() const
    {
        return album_status;
    }

    int64_t XiamiAlbumInfo::get_upgrade_role() const
    {
        return upgrade_role;
    }

    uint64_t XiamiAlbumInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiAlbumInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(XiamiAlbumInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiAlbumInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

    int64_t XiamiArtistFullInfo::get_artist_id() const
    {
        return artist_id;
    }

    const char * XiamiArtistFullInfo::get_artist_name() const 
    {
        return artist_name.c_str();
    }

    const char * XiamiArtistFullInfo::get_image() const 
    {
        return image.c_str();
    }

    const char * XiamiArtistFullInfo::get_alias() const 
    {
        return alias.c_str();
    }

    const char * XiamiArtistFullInfo::get_area() const 
    {
        return area.c_str();
    }

    const char * XiamiArtistFullInfo::get_gender() const 
    {
        return gender.c_str();
    }

    const char * XiamiArtistFullInfo::get_description() const 
    {
        return description.c_str();
    }

    const char * XiamiArtistFullInfo::get_count_likes() const 
    {
        return count_likes.c_str();
    }

    uint64_t XiamiArtistFullInfo::Release()
    {
        if (--m_Ref == 0)
        {
            delete this;
            return 0;
        }
        return 1;
    }

    HRESULT XiamiArtistFullInfo::QueryInterface(RIID riid, void **ppv)
    {
        if (ppv == nullptr)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof(XiamiArtistFullInfo))
        {
            *ppv = (IUnknown *) this;
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    uint64_t XiamiArtistFullInfo::AddRef()
    {
        ++m_Ref;
        return 0;
    }

}



