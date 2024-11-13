//
// Created by John Attong on 11/12/24.
//

#include "Spotify.h"

// auth token constructor
Spotify::Spotify(const std::string &authToken){
    this->auth_token += authToken;
}

// curl writeback
size_t Spotify::WriteCallback(void *contents, size_t size, size_t items, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * items);
    return size * items;
}

// create ptr to artist
Artist* Spotify::getArtist(const std::string &id) {
    std::string url = spotify_url + "/v1/artists/" + id;
    std::string buffer;

    CURL *curl = curl_easy_init();

    struct curl_slist * list = NULL;

    list = curl_slist_append(list, auth_token.c_str());

    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    json parse = json::parse(buffer);

    std::string genres_raw = parse["genres"].dump();
    std::vector<std::string> genres;
    int i = 0;

    // parse genres array
    while (i != genres_raw.length() - 1){
        int start = genres_raw.find('\"', i) + 1;
        i = start;
        int end = genres_raw.find('\"', i);
        std::string temp = genres_raw.substr(start, end - start);
        i = end + 1;
        genres.push_back(temp);
    }

    // remove quotes
    std::string name = parse["name"].dump();
    name.pop_back();
    name.erase(name.begin());

    std::string artist_id = parse["id"].dump();
    artist_id.pop_back();
    artist_id.erase(artist_id.begin());

    Artist* ptr = new Artist(name, artist_id, genres, std::stoi(parse["popularity"].dump()));
    return ptr;
}



