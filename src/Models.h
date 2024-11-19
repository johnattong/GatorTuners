//
// Created by John Attong on 11/13/24.
//

#include <iostream>
#include <vector>


#ifndef GATORTUNERS_MODELS_H
#define GATORTUNERS_MODELS_H

struct Model{
    std::string name;
    std::string id;
    int popularity;
    std::string image_url;

    Model(const std::string &name, const std::string &id, int popularity);

    virtual ~Model() = default;
};

struct Artist : public Model{
    std::vector<std::string> genres;

    Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres, int popularity);

    ~Artist() = default;

};

struct Track : public Model{
    std::vector<Artist*> artists;
    float acousticness;
    float danceability;
    float energy;
    float instrumentalness;
    float loudness;
    float speechiness;
    float tempo;
    float valence;



    Track(const std::string &name, const std::string &id, const std::vector<Artist *> &artists, int popularity);

    ~Track() = default;

};

struct Album : public Model{
    int num_tracks;
    std::string date;
    std::vector<Track*> tracks;

    Album(const std::string &name, const std::string &id, int num_tracks, const std::string &date, std::vector<Track*> &tracks, int popularity);

    ~Album() = default;

};




#endif //GATORTUNERS_MODELS_H
