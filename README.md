# XiamiAPI
###A most comprehensive COM style xiami c++ api

## Compile
```shell
git clone https://github.com/a1q123456/XiamiAPI
git submodule update --init --recursive
cd XiamiAPI
mkdir build
cmake ..
make -j4
```

## Usage

copy iunknow.h xiamiapi.h to your include dir and libxiamiapi.so libcurl.so to your lib dir

### Login

```C++
#include "xiamiapi.h"

using xiamiapi::XiamiAPI;
using xiamiapi::HRESULT;

auto api = XiamiAPI::CreateInstance();

HRESULT hr = api->XiamiLogin("xxx@xxx.com", "password");

if (hr == 1002UL)
{
  IStr * captcha_data;
  api->GetCaptcha(&captcha_data);
  fstream fs("captcha.png");
  fs.seekp(0);
  fs.write(captcha_data->c_str(), captcha_data->length());
}
hr = api->XiamiLogin("xxx@xxx.com", "password", "captcha");
```

### Get Artist Hot Song
```C++
IGenericArray * list;
int jonathan_lee_id = 573
api->GetArtistHotSong(jonathan_lee_id, &list);
for (unsigned long i = 0; i < list->length(); i++)
{
  xiamiapi::IXiamiSongInfo* info;
  list->get_element(i)->QueryInterface(list->ElementIID(), (void **)&info);
  info->get_song_id();
  info->get_song_name();
  info->get_listen_file();
  ...
  info->Release();
}
api->Release();

// Or use smart pointer to manage com resource
```

### Get Playlist

```C++
IGenericArray * list;
int playlist_id = 1
api->GetPlaylist(playlist_id, &list);
for (unsigned long i = 0; i < list->length(); i++)
{
  xiamiapi::IXiamiPlaylistInfo* info;
  list->get_element(i)->QueryInterface(list->ElementIID(), (void **)&info);
  info->get_song_id();
  info->get_title();
  info->get_location();
  ...
  info->Release();
}
api->Release();

// Or use smart pointer to manage com resource
```

More detail avaliable in [wiki](https://github.com/a1q123456/XiamiAPI/wiki)

## TODO
- [ ] use json pointer to get object
