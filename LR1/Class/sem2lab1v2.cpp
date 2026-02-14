#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


// Класс Track 

class Track {
private:
    int id;
    string title; // Название
    string artist; // Исполнитель
    string genre; //жанр
    int duration[2]; // 0 - минуты, 1 - секунды
    int qualityKbps; // Качество трека

public:
    Track();
    Track(int trackId, const string& trackTitle, const string& trackArtist,
          const string& trackGenre, int min, int sec, int quality);
    ~Track();

    // Геттеры 
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getArtist() const { return artist; }
    string getGenre() const { return genre; }
    int getMinutes() const { return duration[0]; }
    int getSeconds() const { return duration[1]; }
    int getQuality() const { return qualityKbps; }

    // Сеттеры
    void setId(int newId) { id = newId; }
    void setTitle(const string& newTitle) { title = newTitle; }
    void setArtist(const string& newArtist) { artist = newArtist; }
    void setGenre(const string& newGenre) { genre = newGenre; }
    void setDuration(int min, int sec) { duration[0] = min; duration[1] = sec; }
    void setQuality(int q) { qualityKbps = q; }
}; 


Track::Track() // Конструктор по умолчанию
    : id(0), title("Unknown"), artist("Unknown"), 
      genre("Unknown"), qualityKbps(0)
{
    duration[0] = 0; duration[1] = 0;
}

Track::Track(int trackId, const string& trackTitle, const string& trackArtist,
             const string& trackGenre, int min, int sec, int quality)
    : id(trackId), title(trackTitle), artist(trackArtist), 
      genre(trackGenre), qualityKbps(quality)
{
    duration[0] = min; duration[1] = sec;
}

Track::~Track() {} // Деструктор

vector<Track> readCSV(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Ошибка: файл " << filename << " не найден!" << endl;
        return {};
    }

    vector<Track> data;
    string line;

    // Пропуск первой строки
    getline(in, line);

    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;

        // Читаем данные через точку с запятой
        getline(ss, token, ';'); int id = stoi(token);
        getline(ss, token, ';'); string title = token;
        getline(ss, token, ';'); string artist = token;
        getline(ss, token, ';'); string genre = token;
        getline(ss, token, ';'); int min = stoi(token);
        getline(ss, token, ';'); int sec = stoi(token);
        getline(ss, token, ';'); int q = stoi(token);

        // Добавление в вектор
        data.emplace_back(id, title, artist, genre, min, sec, q);
    }
    return data;
}

int main() {

    vector<Track> data = readCSV("music.csv");
    cout << "Загружено треков: " << data.size() << "\n\n";

    // вывод на экран
    for (const auto& t : data) {
        cout << "ID: " << t.getId()
             << " | " << t.getTitle()
             << " | " << t.getArtist()
             << " | " << t.getGenre()
             << " | " << t.getMinutes() << ":" << t.getSeconds()
             << " | " << t.getQuality() << " kbps"
             << endl;
    }

    return 0;
}