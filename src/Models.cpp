//
// Created by John Attong on 11/13/24.
//

#include "Models.h"

Artist::Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres, int popularity)
        : name(name), id(id), genres(genres), popularity(popularity) {}

Track::Track(const std::string &name, const std::string &id, std::vector<Artist *> &artists, int popularity) :
        name(name), id(id), artists(artists), popularity(popularity) {}

Album::Album(const std::string &name, const std::string &id, int num_tracks, const std::string &date,
             std::vector<Track *> tracks, int popularity) : name(name), id(id), num_tracks(num_tracks), date(date),
             tracks(tracks), popularity(popularity) {}