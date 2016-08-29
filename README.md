# XiamiAPI
A COM like xiami C++ api

## Usage

### Login

```C++
#include "xiamiapi.h"

using xiamiapi::XiamiAPI;
using xiamiapi::HRESULT;

auto api = XiamiAPI::CreateInstance();

HRESULT hr = api->Login("xxx@xxx.com", "password");

//C++
```

## TODO
- [ ] use json pointer to get object
