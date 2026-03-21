#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // Для функции сортировки (sort)

using namespace std;

// Класс Track

class Track {
private:
    int id; // 1. ID трека
    string title; // 2. Название
    string artist; // 3. Исполнитель
    string genre; // 4. Жанр
    int duration[2]; // 5. длительность: 0 - мин, 1 - сек
    int qualityKbps; // 6. качество аудио кбит/с

public:
    // Конструктор по умолчанию
    Track();
    
    // Конструктор с параметрами
    Track(int trackId, const string& trackTitle, const string& trackArtist,
          const string& trackGenre, int min, int sec, int quality);
          
    // Деструктор
    ~Track();

    // Геттеры (для получения значений) 
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getArtist() const { return artist; }
    string getGenre() const { return genre; }
    int getMinutes() const { return duration[0]; }
    int getSeconds() const { return duration[1]; }
    int getQuality() const { return qualityKbps; }

    // Сеттеры (для изменения значений)
    void setId(int newId) { id = newId; }
    void setTitle(const string& newTitle) { title = newTitle; }
    void setArtist(const string& newArtist) { artist = newArtist; }
    void setGenre(const string& newGenre) { genre = newGenre; }
    void setDuration(int min, int sec) { duration[0] = min; duration[1] = sec; }
    void setQuality(int q) { qualityKbps = q; }
    
    // Общее время в секундах (полезно для сортировки и задач)
    int getTotalSeconds() const {
        return duration[0] * 60 + duration[1];
    }
};

// Реализация конструкторов
Track::Track() {
    id = 0;
    title = "Unknown";
    artist = "Unknown";
    genre = "Unknown";
    duration[0] = 0; 
    duration[1] = 0;
    qualityKbps = 0;
}

Track::Track(int trackId, const string& trackTitle, const string& trackArtist,
             const string& trackGenre, int min, int sec, int quality) {
    id = trackId;
    title = trackTitle;
    artist = trackArtist;
    genre = trackGenre;
    duration[0] = min;
    duration[1] = sec;
    qualityKbps = quality;
}

Track::~Track() {}

// Класс MusicDatabase (База данных)
class MusicDatabase {
private:
    vector<Track> tracks; 

public:
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);
    void printTable() const;
    void addTrack();
    void removeTrackById();
    void searchById() const;
    void filterByQualityRange() const; 
    void sortById();
    
    // Задачи
    void taskPlaylist(); // Задача 1: Плейлист
    void taskLowQuality(); // Задача 2: Треки для перекодирования
};

// 1. Загрузка из файла
void MusicDatabase::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        return;
    }

    tracks.clear(); 
    string line;
    getline(in, line); // Пропускаем заголовок

    while (getline(in, line)) {
        if (line.empty()) continue; 

        stringstream ss(line);
        string token;

        // Читаем до точки с запятой
        getline(ss, token, ';'); int id = stoi(token); 
        getline(ss, token, ';'); string title = token;
        getline(ss, token, ';'); string artist = token;
        getline(ss, token, ';'); string genre = token;
        getline(ss, token, ';'); int min = stoi(token);
        getline(ss, token, ';'); int sec = stoi(token);
        getline(ss, token, ';'); int quality = stoi(token);

        tracks.emplace_back(id, title, artist, genre, min, sec, quality);
    }
    cout << "Успешно загружено записей: " << tracks.size() << endl;
}

// 2. Сохранение в файл
void MusicDatabase::saveToFile(const string& filename) {
    ofstream out(filename);
    out << "ID;Title;Artist;Genre;Minutes;Seconds;Quality\n";
    
    for (int i = 0; i < tracks.size(); i++) {
        out << tracks[i].getId() << ";"
            << tracks[i].getTitle() << ";"
            << tracks[i].getArtist() << ";"
            << tracks[i].getGenre() << ";"
            << tracks[i].getMinutes() << ";"
            << tracks[i].getSeconds() << ";"
            << tracks[i].getQuality() << "\n";
    }
    cout << "Данные сохранены в файл: " << filename << endl;
}

// 3. Вывод таблицы 
void MusicDatabase::printTable() const {
    if (tracks.empty()) {
        cout << "База данных пуста!" << endl;
        return;
    }
    
    cout << "--------------------------------------------------------------------------\n";
    for (int i = 0; i < tracks.size(); i++) {
        cout << "1. " << tracks[i].getId() << " "
             << "2. " << tracks[i].getTitle() << " "
             << "3. " << tracks[i].getArtist() << " "
             << "4. " << tracks[i].getGenre() << " "
             << "5. " << tracks[i].getMinutes() << ":" 
             << (tracks[i].getSeconds() < 10 ? "0" : "") << tracks[i].getSeconds() << " "
             << "6. " << tracks[i].getQuality() << " kbps\n";
    }
    cout << "--------------------------------------------------------------------------\n";
}

// 4. Добавление записи
void MusicDatabase::addTrack() {
    int id, min, sec, q;
    string title, artist, genre;

    cout << "Введите ID: "; cin >> id;
    cin.ignore(); 
    cout << "Название трека: "; getline(cin, title);
    cout << "Исполнитель: "; getline(cin, artist);
    cout << "Жанр: "; getline(cin, genre);
    cout << "Минуты: "; cin >> min;
    cout << "Секунды: "; cin >> sec;
    cout << "Качество (кбит/с): "; cin >> q;

    tracks.emplace_back(id, title, artist, genre, min, sec, q);
    cout << "Запись успешно добавлена!" << endl;
}

// 5. Удаление записи по номеру (ID)
void MusicDatabase::removeTrackById() {
    int id;
    cout << "Введите ID трека для удаления: ";
    cin >> id;

    bool found = false;
    for (int i = 0; i < tracks.size(); i++) {
        if (tracks[i].getId() == id) {
            tracks.erase(tracks.begin() + i);
            cout << "Запись удалена!" << endl;
            found = true;
            break; 
        }
    }
    if (!found) cout << "Трек с таким ID не найден." << endl;
}

// 6. Поиск по ID
void MusicDatabase::searchById() const {
    int id;
    cout << "Введите ID для поиска: ";
    cin >> id;

    for (int i = 0; i < tracks.size(); i++) {
        if (tracks[i].getId() == id) {
            cout << "\nНайден трек:\n";
            cout << "Название: " << tracks[i].getTitle() << endl;
            cout << "Исполнитель: " << tracks[i].getArtist() << endl;
            cout << "Жанр: " << tracks[i].getGenre() << endl;
            cout << "Качество: " << tracks[i].getQuality() << " kbps\n";
            return; 
        }
    }
    cout << "Запись не найдена." << endl;
}

// 7. Выборка по диапазону 
void MusicDatabase::filterByQualityRange() const {
    int minQ, maxQ;
    cout << "Введите минимальное качество: "; cin >> minQ;
    cout << "Введите максимальное качество: "; cin >> maxQ;

    cout << "\nТреки в диапазоне качества [" << minQ << " - " << maxQ << "]:\n";
    bool found = false;
    for (int i = 0; i < tracks.size(); i++) {
        if (tracks[i].getQuality() >= minQ && tracks[i].getQuality() <= maxQ) {
            cout << "- " << tracks[i].getArtist() << " - " << tracks[i].getTitle() 
                 << " (" << tracks[i].getQuality() << " kbps)" << endl;
            found = true;
        }
    }
    if (!found) cout << "Ничего не найдено.\n";
}

// Вспомогательные функции для сортировки
bool compareById(const Track& a, const Track& b) {
    return a.getId() < b.getId();
}

bool compareByDuration(const Track& a, const Track& b) {
    return a.getTotalSeconds() < b.getTotalSeconds();
}

// 8. Сортировка по ID
void MusicDatabase::sortById() {
    sort(tracks.begin(), tracks.end(), compareById);
    cout << "База данных отсортирована по ID." << endl;
}



// Задача 1: Плейлист
void MusicDatabase::taskPlaylist() {
    int choice;
    string searchStr;
    cout << "Искать по (1 - Жанр, 2 - Исполнитель): ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) cout << "Введите жанр: ";
    else cout << "Введите исполнителя: ";
    getline(cin, searchStr);

    int t_min, m_min;
    cout << "Макс. длительность ОДНОГО трека (Т минут): "; cin >> t_min;
    cout << "Макс. ОБЩАЯ длительность плейлиста (М минут): "; cin >> m_min;

    // Шаг 1: Фильтрация треков во временном векторе
    vector<Track> playlist;
    for (int i = 0; i < tracks.size(); i++) {
        bool match = false;
        if (choice == 1 && tracks[i].getGenre() == searchStr) match = true;
        if (choice == 2 && tracks[i].getArtist() == searchStr) match = true;

        if (match && tracks[i].getMinutes() <= t_min) {
            playlist.push_back(tracks[i]);
        }
    }

    // Шаг 2: Сортировка
    sort(playlist.begin(), playlist.end(), compareByDuration);

    // Шаг 3: Собираем плейлист в рамках лимита M
    cout << "\n--- Финальный плейлист ---\n";
    int currentTotalSeconds = 0;
    int limitSeconds = m_min * 60;

    for (int i = 0; i < playlist.size(); i++) {
        if (currentTotalSeconds + playlist[i].getTotalSeconds() <= limitSeconds) {
            cout << playlist[i].getArtist() << " - " << playlist[i].getTitle() 
                 << " (" << playlist[i].getMinutes() << ":" << playlist[i].getSeconds() << ")\n";
                 
            currentTotalSeconds += playlist[i].getTotalSeconds();
        }
    }
    cout << "Итоговая длительность: " << currentTotalSeconds / 60 << " мин " << currentTotalSeconds % 60 << " сек.\n";
}

// Задача 2: Треки для перекодирования
void MusicDatabase::taskLowQuality() {
    // Находим все треки с плохим качеством, кроме классики
    vector<Track> badQualityTracks;
    for (int i = 0; i < tracks.size(); i++) {
        if (tracks[i].getQuality() < 256 && tracks[i].getGenre() != "Classical" && tracks[i].getGenre() != "Классика") {
            badQualityTracks.push_back(tracks[i]);
        }
    }

    if (badQualityTracks.empty()) {
        cout << "Треков для перекодирования не найдено.\n";
        return;
    }

    // Собираем список уникальных жанров
    vector<string> uniqueGenres;
    for (int i = 0; i < badQualityTracks.size(); i++) {
        bool genreExists = false;
        for (int j = 0; j < uniqueGenres.size(); j++) {
            if (uniqueGenres[j] == badQualityTracks[i].getGenre()) {
                genreExists = true;
                break;
            }
        }
        if (!genreExists) {
            uniqueGenres.push_back(badQualityTracks[i].getGenre());
        }
    }

    // Выводим сгруппировав по жанрам
    cout << "\n--- Треки низкого качества для перекодирования ---\n";
    for (int i = 0; i < uniqueGenres.size(); i++) {
        cout << "[ Жанр: " << uniqueGenres[i] << " ]\n";
        for (int j = 0; j < badQualityTracks.size(); j++) {
            if (badQualityTracks[j].getGenre() == uniqueGenres[i]) {
                cout << "  - ID " << badQualityTracks[j].getId() << " | " 
                     << badQualityTracks[j].getArtist() << " - " << badQualityTracks[j].getTitle() 
                     << " (" << badQualityTracks[j].getQuality() << " kbps)\n";
            }
        }
        cout << endl;
    }
}

// Main
int main() {
    setlocale(LC_ALL, "");
    
    MusicDatabase db;
    string filename = "music_db.csv"; // Файл базы данных
    int choice;

    do {
        cout << "1. Загрузить БД из файла\n";
        cout << "2. Просмотр БД (вывод на экран)\n";
        cout << "3. Добавить запись\n";
        cout << "4. Удалить запись по ID\n";
        cout << "5. Сохранить БД в файл\n";
        cout << "6. Сортировка по ID\n";
        cout << "7. Поиск по ID\n";
        cout << "8. Выборка по диапазону (качество аудио)\n";
        cout << "9. Задача 1. Сгенерировать плейлист\n";
        cout << "10. Задача 2. Список треков для перекодировки\n";
        cout << "0. Выход\n";
        cout << "Выберите пункт меню: ";
        cin >> choice;

        switch (choice) {
            case 1: db.loadFromFile(filename); break;
            case 2: db.printTable(); break;
            case 3: db.addTrack(); break;
            case 4: db.removeTrackById(); break;
            case 5: db.saveToFile(filename); break;
            case 6: db.sortById(); break;
            case 7: db.searchById(); break;
            case 8: db.filterByQualityRange(); break;
            case 9: db.taskPlaylist(); break;
            case 10: db.taskLowQuality(); break;
            case 0: cout << "Завершение работы программы...\n"; break;
            default: cout << "Неверный ввод, попробуйте еще раз.\n";
        }
    } while (choice != 0);

    return 0;
}