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
#include "BTree.h"
#include "Graph.h"
#pragma once


#ifndef GATORTUNERS_SPOTIFY_H
#define GATORTUNERS_SPOTIFY_H

using json = nlohmann::json;


class Spotify {                                                 //i think both of these are mines - lucas one doesn't work so watch out
    std::string client_id = "20a3f27041914ae9ba74bffbf6d5bac0"; //"b1f6b407124b4f8d87e532ac982e4b79"; "0539b46a746748b79f7f94b45834efb1";
    std::string client_secret = "5ad9860283364a31aa232e20886558c1"; //"63956e64d94b4d418ae98946d01b8d72"; "33531cbeff994248beb7d7f3b833823e";
    std::string spotify_url = "https://api.spotify.com";
    std::string auth_token = "Authorization: Bearer  ";

    std::unordered_map<std::string, Artist*> ids;

    void getTrackHelper(Track* track);

    void recommendedTracksHelper(std::vector<Track*>& tracks, std::string& id);

public:

    Spotify();

    static size_t WriteCallback(void *contents, size_t size, size_t items, void *userp);

    static size_t FileCallback(void *contents, size_t size, size_t items, void *userp);

    Artist* getArtist(const std::string &id);

    Track* getTrack(const std::string &id);

    Album* getAlbum(const std::string &id);

    std::vector<Track*> searchTrack(std::string &query);

    std::string getImage(std::string &url, std::string &name);

    std::vector<Track*> recommendedTracks(Track* track);
};


#endif //GATORTUNERS_SPOTIFY_H
