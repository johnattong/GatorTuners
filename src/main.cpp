#include "Spotify.h"

using namespace std;

int main(){

    Spotify test;

    test.getArtist("4Z8W4fKeB5YxbusRsdQVPb");
    std::string qu = "Whiskey Glasses";
    std::vector<Track*> list = test.searchTrack(qu);
    for (int i = 0; i < list.size(); i++){
        cout << i+1 << ". " << list[i]->name << " by ";
        for (int j = 0; j < list[i]->artists.size(); j++){
            cout << list[i]->artists[j]->name << "  ";
        }
        cout << '\n';
    }

    test.getImage(list[0]->image_url,qu);

    test.recommendedTracks(list[0]);

    return 0;
}