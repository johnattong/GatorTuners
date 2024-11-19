//
// Created by John Attong on 11/13/24.
//

#include "Models.h"

Model::Model(const std::string &name, const std::string &id, int popularity) : name(name), id(id), popularity(popularity) {}

Artist::Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres, int popularity)
        : Model(name,id,popularity), genres(genres) {}



Track::Track(const std::string &name, const std::string &id, const std::vector<Artist *> &artists, int popularity) :
        Model(name,id,popularity), artists(artists) {}




Album::Album(const std::string &name, const std::string &id, int num_tracks, const std::string &date,
             std::vector<Track *> &tracks, int popularity) : Model(name,id,popularity), num_tracks(num_tracks), date(date),
             tracks(tracks) {}



