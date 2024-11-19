//
// Created by John Attong on 11/12/24.
//

#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <unordered_map>
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

    std::unordered_map<std::string, Artist*> ids;

    void getTrackHelper(Track* track);

public:

    Spotify();

    static size_t WriteCallback(void *contents, size_t size, size_t items, void *userp);

    static size_t FileCallback(void *contents, size_t size, size_t items, void *userp);

    Artist* getArtist(const std::string &id);

    Track* getTrack(const std::string &id);

    Album* getAlbum(const std::string &id);

    std::vector<Track*> searchTrack(std::string &query);

    std::string getImage(std::string &id, std::string &name);
};


#endif //GATORTUNERS_SPOTIFY_H
