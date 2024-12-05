//
// Created by John Attong on 11/13/24.
//

#include <iostream>
#include <vector>
#pragma once


#ifndef GATORTUNERS_MODELS_H
#define GATORTUNERS_MODELS_H

struct Model{
    std::string name;
    std::string id;
    int popularity;
    std::string image_url;

    Model(const std::string &name, const std::string &id);

    virtual ~Model() = default;
};

struct Artist : public Model{
    std::vector<std::string> genres;

    Artist(const std::string &name, const std::string &id, std::vector<std::string> &genres);

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

    Track(const std::string &name, const std::string &id, const std::vector<Artist *> &artists);

    void printArtists(){
        for (auto person : artists){
            std::cout << person->name << (((find(artists.begin(), artists.end(), person)) != artists.end()) ? ", " : " ");
        }
    }

    ~Track() = default;

};

struct Album : public Model{
    int num_tracks;
    std::string date;
    std::vector<Track*> tracks;

    Album(const std::string &name, const std::string &id, const std::string &date, std::vector<Track*> &tracks);

    ~Album() = default;

};




#endif //GATORTUNERS_MODELS_H
