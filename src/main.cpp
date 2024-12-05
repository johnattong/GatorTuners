#include "Spotify.h"
#include "Stopwatch.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

using namespace std;

int main() {
    Spotify spotify;
    cout << "Hello, Welcome to GatorTuners!" << endl;

    // Prompt user to select an algorithm type
    cout << "\nPlease choose which type of algorithm you would like to implement:\n"
         << "1. B-Tree Algorithm\n2. Graph Algorithm" << endl;

    int algorithmChoice;
    if (!(cin >> algorithmChoice) || (algorithmChoice != 1 && algorithmChoice != 2)) {
        cerr << "Invalid input. Insert 1 or 2 for the choice of algorithm." << endl;
        return 1;
    }

    // Prompt user to specify the number of entries
    int numOfEntries;
    cout << "\nEnter the number of entries (1-9): " << endl;
    if (!(cin >> numOfEntries) || numOfEntries < 1 || numOfEntries > 9) {
        cerr << "Invalid input. Please enter an integer value between 1 and 9." << endl;
        return 1;
    }

    cin.ignore(); // Clear input buffer

    // Instructions for inserting songs
    cout << "\nInstructions for entries:\n"
         << "To insert a song, type in the song name as normal.\n"
         << "Example: 'Enter Sandman'" << endl;

    vector<Track*> vecSongs;

    for (int i = 0; i < numOfEntries; i++) {
        cout << "Enter a song. The more specific the better, i.e., including the Artist's name." << endl;
        string entryValue;
        getline(cin, entryValue);

        cout << "You entered: " << entryValue << endl;
        vector<Track*> tracks = spotify.searchTrack(entryValue);
        cout << "\nChoose the correct song for your input. Enter a number 1-" << tracks.size() << "." << endl;

        for (int j = 0; j < tracks.size(); j++) {
            cout << j+1 << ". " << tracks[j]->name << " - ";
            tracks[j]->printArtists();
            cout << endl;
        }

        int optionInt;
        if (!(cin >> optionInt) || optionInt < 1 || optionInt > tracks.size()) {
            cerr << "Invalid input. Please enter a number between 1 and " << tracks.size() << "." << endl;
            return 1;
        }
//        if(tracks.empty()){
//            cerr << "No tracks found for your input. Please try again." << endl;
//            return 1;
//        }

        cin.ignore(); // Clear input buffer

        Track* chosenTrack = tracks[optionInt-1];
        cout << "\nYou chose: " << chosenTrack->name << " - ";
        chosenTrack->printArtists();
        cout << endl;
        vecSongs.push_back(chosenTrack);
    }

    Stopwatch stopwatch;
    stopwatch.start();

    if (algorithmChoice == 1) {
        cout << "Executing B-Tree Algorithm..." << endl;
        BTree<Track*, 10> btree;

//        cout << "Starting songs:" << endl;
//        for (auto song : vecSongs) {
//            cout << song->name << " - ";
//            song->printArtists();
//            cout << endl;
//            btree.insert(song);
//        }

        for (auto song : vecSongs) {
            vector<Track*> recommendedSongs = spotify.recommendedTracks(song);

            for (auto recSong : recommendedSongs) {
                if (btree.searchByID(recSong->id) == nullptr) {
                    btree.insert(recSong);
                }
            }
        }

        vector<Track*> knnResult = btree.knnSearch(btree.getRootTrack(), 50);
        if (knnResult.empty()) {
            cout << "No recommendations found." << endl;
            return 0; // Early exit if no results
        }



        unordered_map<string, int> artistSongCount;
        vector<Track*> knnResultFinal;
        for (auto recs : knnResult) {
            if (!recs->artists.empty()) {
                string artistId = recs->artists[0]->id;  // Assuming the first artist is the main artist

                if (artistSongCount[artistId] < 3) {
                    knnResultFinal.push_back(recs);
                    artistSongCount[artistId]++;

                    // If we've reached 3 songs for this artist, remove any excess
                    if (artistSongCount[artistId] == 3) {
                        auto it = remove_if(knnResultFinal.begin(), knnResultFinal.end(),
                                            [&artistId](Track* track) {
                                                return track->artists[0]->id == artistId;
                                            });
                        knnResultFinal.erase(it, knnResultFinal.end());
                    }
                }
            }
        }

        cout << "\nHere is your playlist:" << endl;

        for (auto song : knnResultFinal) {
            cout << song->name << " - ";
            song->printArtists();
            cout << endl;
        }
        stopwatch.pause();
        cout << "\nFinal time to create playlist: " << stopwatch.getElapsedTime() << " seconds" << endl;

        cout << "Total # of songs in the playlist: " << knnResultFinal.size() << endl;

    } else {
        cout << "Executing Graph Algorithm..." << endl;
        Graph<Track*> graph;
        // Initalize graph and print beginning songs.
        cout << "Starting songs:" << endl;

        for (auto it = vecSongs.begin(); it != vecSongs.end(); ++it) {
            cout << (*it)->name << " - ";
            (*it)->printArtists();
            cout << endl;

            auto nextit = next(it);
            if (nextit != vecSongs.end()) {
                if (*nextit == nullptr) {
                    cout << "Warning: nullptr encountered in next element of vecSongs" << endl;
                } else {
                    graph.add_edge(*it, *nextit);
                }
            }
        }
        // Get songs from spotify and push into graph.
        for (auto song : vecSongs) {
            vector<Track*> recommendations = spotify.recommendedTracks(song);
            for (auto rec : recommendations) {
                    graph.add_edge(song, rec);
                }
        }
        // Get a vector of the DFS of the graph.
        vector<Track*> DFS = graph.getDFS();
        // For that vector, if nodes share an artist, add an edge between them
        for (int i = 1; i < DFS.size(); i++) {
            if (i >= DFS.size() || (DFS.size() - i - 1) >= DFS.size()) {
                continue;
            }
            for (int j = 0; j < DFS[i]->artists.size(); j++){
                if (DFS[DFS.size() - i] == nullptr) {
                    continue;
                }
                for (auto& person : DFS[DFS.size()-i]->artists){
                    if (DFS[i]->artists[j]->id == person->id){
                        graph.add_edge(DFS[i], DFS[DFS.size()-1]);
                        break;
                    }
                }
            }
        }
        // Prepare final reccomendations.
        vector<pair<Track*, double>> jaccards;

        for (int i = 0; i < (DFS.size() > 50 ? 50 : DFS.size()); i++){

                pair<Track*, double> p;
                double similarityReport = graph.jaccardSimilarity(DFS[i], DFS[DFS.size() - i]);
                p.first = DFS[i];
                p.second = similarityReport;
                jaccards.emplace_back(p);
        }

        std::sort(jaccards.begin(), jaccards.end(),
                  [](const pair<Track*, double>& a, const pair<Track*, double>& b) {
                      return a.second > b.second;
                  });

        set<Track*> setJaccards;
        unordered_map<string, int> frequency;
        for (Track* track : DFS) {
            if (track->artists.empty()) continue;

            std::string artistId = track->artists[0]->id;
            if (frequency[artistId] < 3) {
                setJaccards.insert(track);
                frequency[artistId]++;
            }
        }
        for (auto& j : setJaccards){
            cout << j->name << " - ";
            j->printArtists();
            cout << endl;
        }

        stopwatch.pause();
        cout << "\nFinal time to create playlist: " << stopwatch.getElapsedTime() << " seconds" << endl;

        cout << "Total # of song recommendations produced: " << setJaccards.size() << endl;
    }

    cout << "Goodbye!" << endl;
    return 0;
}