//
// Created by John Attong on 11/13/24.
//

#include "Models.h"

Model::Model(const std::string &name, const std::string &id) : name(name), id(id) {}

Artist::Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres)
        : Model(name,id), genres(genres) {}



Track::Track(const std::string &name, const std::string &id, const std::vector<Artist *> &artists) :
        Model(name,id), artists(artists) {}




Album::Album(const std::string &name, const std::string &id, const std::string &date,
             std::vector<Track *> &tracks) : Model(name,id), date(date),
             tracks(tracks) {}



