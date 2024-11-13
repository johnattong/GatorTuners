//
// Created by John Attong on 11/13/24.
//

#include <iostream>
#include <vector>


#ifndef GATORTUNERS_MODELS_H
#define GATORTUNERS_MODELS_H

struct Artist{
    std::string name;
    std::string id;
    std::vector<std::string> genres;
    int popularity;

    Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres, int popularity);

};

struct Track{
    std::string name;
    std::string id;
    std::vector<Artist*> artists;
    int popularity;
    float acousticness;
    float danceability;
    float energy;
    float instrumentalness;
    float loudness;
    float speechiness;
    float tempo;
    float valence;



    Track(const std::string &name, const std::string &id, const std::vector<Artist *> &artists, int popularity);

};

struct Album{
    std::string name;
    std::string id;
    int num_tracks;
    std::string date;
    std::vector<Track*> tracks;
    int popularity;


    Album(const std::string &name, const std::string &id, int num_tracks, const std::string &date, std::vector<Track*> tracks, int popularity);


};




#endif //GATORTUNERS_MODELS_H
