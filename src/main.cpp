#include "Spotify.h"

using namespace std;

int main(){

    Spotify test;
    std::string qu = "One Dance";
    std::vector<Track*> list = test.searchTrack(qu);
    for (int i = 0; i < list.size(); i++){
        cout << i+1 << ". " << list[i]->name << " by ";
        for (int j = 0; j < list[i]->artists.size(); j++){
            cout << list[i]->artists[j]->name << "  ";
        }
        cout << '\n';
    }

    test.getImage(list[1]->image_url,qu);

    return 0;
}