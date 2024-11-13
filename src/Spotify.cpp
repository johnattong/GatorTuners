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
    // artist already in program
    if (ids.find(id) != ids.end()) return ids[id];

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
    int i = 1;

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

    ids[id] = ptr;
    return ptr;
}

// ptr to track
Track *Spotify::getTrack(const std::string id) {
    std::string url = spotify_url + "/v1/tracks/" + id;
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

    json track = json::parse(buffer);


    std::vector<Artist*> artists;
    for (int i = 0; i < track["artists"].size(); i++){
        std::string artist_id = track["artists"][i]["id"].dump();

        artist_id.pop_back();
        artist_id.erase(artist_id.begin());

        artists.push_back(getArtist(artist_id));
    }

    std::string name = track["name"].dump();
    name.pop_back();
    name.erase(name.begin());

    std::string track_id = track["id"].dump();
    track_id.pop_back();
    track_id.erase(track_id.begin());

    Track* ptr = new Track(name, id,artists, std::stoi(track["popularity"].dump()));

    getTrackHelper(ptr);


    return ptr;
}

// updates tracks audio features (i.e. tempo, energy etc.)
void Spotify::getTrackHelper(Track* track) {
    std::string url = spotify_url + "/v1/audio-features/" + track->id;
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
    json features = json::parse(buffer);

    track->acousticness = std::stof(features["acousticness"].dump());
    track->danceability = std::stof(features["danceability"].dump());
    track->energy = std::stof(features["energy"].dump());
    track->instrumentalness = std::stof(features["instrumentalness"].dump());
    track->loudness = std::stof(features["loudness"].dump());
    track->speechiness = std::stof(features["speechiness"].dump());
    track->tempo = std::stof(features["tempo"].dump());
    track->valence = std::stof(features["valence"].dump());


}

// ptr to album
Album *Spotify::getAlbum(const std::string id) {
    std::string url = spotify_url + "/v1/albums/" + id;
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

    json album = json::parse(buffer);

    std::vector<Track*> tracks;
    for (int i = 0; i < album["tracks"]["items"].size(); i++){
        std::string track_id = album["tracks"]["items"][i]["id"].dump();

        track_id.pop_back();
        track_id.erase(track_id.begin());

        tracks.push_back(getTrack(track_id));
    }

    std::string date = album["release_date"].dump();
    date.pop_back();
    date.erase(date.begin());

    std::string name = album["name"].dump();
    name.pop_back();
    name.erase(name.begin());

    Album* ptr = new Album(name, id, std::stoi(album["total_tracks"].dump()), date, tracks, std::stoi(album["popularity"].dump()));

    return ptr;
}





