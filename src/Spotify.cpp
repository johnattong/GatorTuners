//
// Created by John Attong on 11/12/24.
//

#include "Spotify.h"
#include <thread>
#include <chrono>

 //auth token constructor
Spotify::Spotify(){
     std::string url = "https://accounts.spotify.com/api/token";
     std::string field = "grant_type=client_credentials&client_id=" + client_id + "&client_secret="
                         + client_secret;
     std::string buffer;

     CURL *curl = curl_easy_init();

     if (curl){
         CURLcode result;
         curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
         curl_easy_setopt(curl,CURLOPT_POSTFIELDS,field.c_str());

         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
         result = curl_easy_perform(curl);
     }

     json parse = json::parse(buffer);

     std::string temp = parse["access_token"].dump();
     temp.pop_back();
     temp.erase(temp.begin());

     auth_token += temp;
}

// curl writeback
size_t Spotify::WriteCallback(void *contents, size_t size, size_t items, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * items);
    return size * items;
}

// curl write to file
size_t Spotify::FileCallback(void *contents, size_t size, size_t items, void *userp) {
    std::ofstream *outfile = (std::ofstream*)userp;


    outfile->write((char*)contents,size*items);

    return size * items;
}


// create ptr to artist
Artist* Spotify::getArtist(const std::string &id) {
    // artist already in program
    if (ids.find(id) != ids.end()) return ids[id];

    std::string url = spotify_url + "/v1/artists/" + id;
    std::string buffer;
    std::string header;

    CURL *curl = curl_easy_init();

    struct curl_slist * list = NULL;

    list = curl_slist_append(list, auth_token.c_str());

    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);

        while (header.find("HTTP/2 429") != std::string::npos){
            int index = header.find("retry-after:");
            index += 12;
            int cooldown = stoi(header.substr(index, header.find('\\')));
            std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
            std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds (cooldown));
            result = curl_easy_perform(curl);
        }
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

    Artist* ptr = new Artist(name, artist_id, genres);

    if (parse["images"].size() != 0) ptr->image_url = parse["images"][0]["url"];

    ids[id] = ptr;

    //std::cout << "Artist found: " << name << "\n\n";

    return ptr;
}

// ptr to track
Track *Spotify::getTrack(const std::string &id) {
    std::string url = spotify_url + "/v1/tracks/" + id;
    std::string buffer;
    std::string header;

    CURL *curl = curl_easy_init();

    struct curl_slist * list = NULL;

    list = curl_slist_append(list, auth_token.c_str());

    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);

        while (header.find("HTTP/2 429") != std::string::npos){
            int index = header.find("retry-after:");
            index += 12;
            int cooldown = stoi(header.substr(index, header.find('\\')));
            std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
            std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds (cooldown));
            result = curl_easy_perform(curl);
        }

        curl_easy_cleanup(curl);
    }

    if (buffer.empty()) return nullptr;

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

    Track* ptr = new Track(name, id,artists);

    ptr->image_url = track["album"]["images"][1]["url"];

    // deprecated function
    //getTrackHelper(ptr);

    //std::cout << "Track found: " << name << "\n\n";

    return ptr;
}

// -------------------DEPRECATED BY SPOTIFY-------------------------
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

    if (buffer.find("error")) return;

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
Album *Spotify::getAlbum(const std::string &id) {
    std::string url = spotify_url + "/v1/albums/" + id;
    std::string buffer;
    std::string header;

    CURL *curl = curl_easy_init();

    struct curl_slist * list = NULL;

    list = curl_slist_append(list, auth_token.c_str());

    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);

        while (header.find("HTTP/2 429") != std::string::npos){
            int index = header.find("retry-after:");
            index += 12;
            int cooldown = stoi(header.substr(index, header.find('\\')));
            std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
            std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds (cooldown));
            result = curl_easy_perform(curl);
        }

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

    Album* ptr = new Album(name, id, date, tracks);

    ptr->image_url = album["images"][1]["url"];

    std::cout << "Album found: " << name << "\n\n";

    return ptr;
}

// vector of 10 tracks from search
std::vector<Track*> Spotify::searchTrack(std::string &query) {
    auto encodeQ = [](std::string &q) {
        while (q.find(' ') != std::string::npos) {
            q[q.find(' ')] = '+';
        }
    };

    std::vector<Track*> tracks;

    //while (true) {
        std::string originalQuery = query;
        encodeQ(query);

        // Construct the API URL
        std::string url = spotify_url + "/v1/search?q=remaster%2520track%3A" + query + "&type=track&limit=10&offset=0";
        std::string buffer;
        std::string header;

        // Initialize CURL
        CURL *curl = curl_easy_init();
        struct curl_slist *list = nullptr;
        list = curl_slist_append(list, auth_token.c_str());

        if (curl) {
            CURLcode result;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            result = curl_easy_perform(curl);

            while (header.find("HTTP/2 429") != std::string::npos){
                int index = header.find("retry-after:");
                index += 12;
                int cooldown = stoi(header.substr(index, header.find('\\')));
                std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
                std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds (cooldown));
                result = curl_easy_perform(curl);
            }

            curl_easy_cleanup(curl);
        }

        json data = json::parse(buffer);

        // Parse JSON response
//        try {
//            json data = json::parse(buffer);
//
//            // Validate the structure of the JSON
//            if (data.contains("tracks") && data["tracks"].contains("items")) {
//                auto items = data["tracks"]["items"];
//                for (const auto &item : items) {
//                    // Ensure the "id" field exists and is a string
//                    if (item.contains("id") && !item["id"].is_null()) {
//                        std::string trackId = item["id"];
//                        tracks.push_back(getTrack(trackId));
//                    } else {
//                        std::cerr << "Skipping item with missing or null 'id'" << std::endl;
//                    }
//                }
//            } else {
//                std::cerr << "No 'tracks' or 'items' found in the response." << std::endl;
//            }
//        } catch (const json::exception &e) {
//            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
//        }
//
//        // Check if tracks were successfully added
//        if (!tracks.empty()) {
//            break;
//        } else {
//            std::cerr << "No songs found with the name \"" << originalQuery << "\". Could you re-enter the name?" << std::endl;
//            std::getline(std::cin, query);
//        }
//    }

        for (int i = 0; i < data["tracks"]["items"].size(); i++){
            tracks.push_back(getTrack(data["tracks"]["items"][i]["id"]));
        }

    return tracks;
}

// returns image path
std::string Spotify::getImage(std::string &url, std::string &name) {

    std::ofstream buffer("../assets/" + name + ".tga",std::ios_base::binary);
    std::string header;

    CURL *curl = curl_easy_init();


    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }


    return "../assets/" + name + ".tga";

}

std::vector<Track*> Spotify::recommendedTracks(Track *track) {
    std::vector<Track*> tracks;

    std::vector<std::string> temp_genres;
    for (auto i : track->artists){
        for (auto j : i->genres){
            temp_genres.push_back(j);
        }
    }

    if (temp_genres.size() == 0){
        std::cout << "Error: No genres to generate songs" << std::endl;
    }

    for (auto genre : temp_genres) {

        while(genre.find(' ') != std::string::npos){
            genre[genre.find(' ')] = '+';
        }

        std::string url = spotify_url + "/v1/search?q=remaster%2520genre%3A" + genre +"&type=artist&limit=3&offset=0";

        std::string buffer;
        std::string header;

        CURL *curl = curl_easy_init();

        struct curl_slist *list = NULL;

        list = curl_slist_append(list, auth_token.c_str());

        if (curl) {
            CURLcode result;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
            curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            result = curl_easy_perform(curl);

            while (header.find("HTTP/2 429") != std::string::npos){
                int index = header.find("retry-after:");
                index += 12;
                int cooldown = stoi(header.substr(index, header.find('\\')));
                std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
                std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds (cooldown));
                result = curl_easy_perform(curl);
            }

            curl_easy_cleanup(curl);
        }

        json data = json::parse(buffer);



        for (int i = 0; i < data["artists"]["items"].size(); i++){
            std::string artist_id = data["artists"]["items"][i]["id"];
 

            recommendedTracksHelper(tracks, artist_id);
        }

    }
    return tracks;
}

// add top tracks from artist to list
void Spotify::recommendedTracksHelper(std::vector<Track *> &tracks, std::string &id) {

    std::string buffer;
    std::string header;
    std::string url = spotify_url + "/v1/artists/" + id + "/top-tracks";

    CURL *curl = curl_easy_init();

    struct curl_slist *list = NULL;

    list = curl_slist_append(list, auth_token.c_str());

    if (curl){
        CURLcode result;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        result = curl_easy_perform(curl);

        while (header.find("HTTP/2 429") != std::string::npos){
            int index = header.find("retry-after:");
            index += 12;
            int cooldown = stoi(header.substr(index, header.find('\\')));
            std::cout << "Spotify cooldown: " << cooldown << " seconds" << endl;
            std::cout << "Rate limiting --- Please kill process or we'll break the API" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds (cooldown));
            result = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }

    json data = json::parse(buffer);

    for (int i = 0; i < data["tracks"].size(); i++){
        tracks.push_back(getTrack(data["tracks"][i]["id"]));
    }
}
