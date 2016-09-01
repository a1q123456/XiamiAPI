# XiamiAPI
A most comprehensive xiami c++ api and COM-like 

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
  auto song_info = dynamic_cast<IXiamiSongInfo *>(list->get_element(i));
  song_info->get_song_id();
  song_info->get_song_name();
  song_info->get_listen_file();
  ...
  // Don't call Release() for any list item
}

```

### Get Playlist

```C++
IGenericArray * list;
int playlist_id = 1
api->GetPlaylist(playlist_id, &list);
for (unsigned long i = 0; i < list->length(); i++)
{
  auto song_info = dynamic_cast<IXiamiPlaylistInfo *>(list->get_element(i));
  song_info->get_song_id();
  song_info->get_title();
  song_info->get_location();
  ...
  // Don't call Release() for any list item
}

```

More detail avaliable in [wiki](https://github.com/a1q123456/XiamiAPI/wiki)

## TODO
- [ ] use json pointer to get object
- [ ] implement IUnknow->QueryInterface() and IGenericArray->GetUUID()
