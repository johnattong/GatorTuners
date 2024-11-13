//
// Created by John Attong on 11/12/24.
//

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "include/nlohmann/json.hpp"
#include "Models.h"


#ifndef GATORTUNERS_SPOTIFY_H
#define GATORTUNERS_SPOTIFY_H

using json = nlohmann::json;


class Spotify {
    std::string client_id = "66a7a7b20586418c91deaaffb6b06993";
    std::string client_secret = "10440d9fad6849478d20dfdacae968ce";
    std::string spotify_url = "https://api.spotify.com";
    std::string auth_token = "Authorization: Bearer  ";

public:

    Spotify(const std::string &authToken);

    static size_t WriteCallback(void *contents, size_t size, size_t items, void *userp);

    Artist* getArtist(const std::string &id);
};


#endif //GATORTUNERS_SPOTIFY_H
