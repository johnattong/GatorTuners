//
// Created by John Attong on 11/13/24.
//

#include <iostream>
#include <vector>


#ifndef GATORTUNERS_MODELS_H
#define GATORTUNERS_MODELS_H

class Artist{
    std::string name;
    std::string id;
    std::vector<std::string> genres;
    int popularity;
public:
    Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres, int popularity);

};

class Track{
    std::string name;
    std::string id;
    std::vector<Artist*> artists;
    int popularity;
public:
    Track(const std::string &name, const std::string &id, std::vector<Artist*> &artists, int popularity);
};

class Album{
    std::string name;
    std::string id;
    int num_tracks;
    std::string date;
    std::vector<Track*> tracks;
    int popularity;
public:
    Album(const std::string &name, const std::string &id, int num_tracks, const std::string &date, std::vector<Track*> tracks, int popularity);
};




#endif //GATORTUNERS_MODELS_H
