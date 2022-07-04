/********************************
 * Kamil Zwierzchowski (418510) *
 * & Grzegorz Zaleski (418494)  *
 ********************************/

#include <iostream>
#include <bits/stdc++.h>

#define QMARK '?'

using namespace std;
using LL = long long;
// Krotka do trzymania informacji o aucie.
using car_info = tuple<LL, string, LL, LL, LL, bool, bool>;

// Komparator sortujący drogi.
struct comp_road
{
    bool operator() (const string &a, const string &b) const
    {
        int l1 = 0, l2 = 0;
        for (size_t i = 1; i < a.length(); i++)
        {
            l1 *= 10;
            l1 += a[i] - '0';
        }
        for (size_t i = 1; i < b.length(); i++)
        {
            l2 *= 10;
            l2 += b[i] - '0';
        }
        if (l1 == l2)
        {
            return a[0] < b[0];
        }
        return l1 < l2;
    }
};

// Sprawdzenie czy identyfikator drogi jest zgodny z poleceniem zadania (true = błędne dane).
bool wrong_road(string &potential_road)
{
    if (potential_road.length() < 2 || potential_road.length() > 4)
        return true;

    if (potential_road[0] != 'S' && potential_road[0] != 'A')
        return true;

    for (size_t i=1; i<potential_road.length(); ++i)
        if (isdigit(potential_road[i]) == false)
            return true;

    if (potential_road[1] == '0')
        return true;

    return false;
}

// Sprawdzenie czy identyfikator auta jest zgodny z poleceniem zadania (true = błędne dane).
bool wrong_car(string &potential_car)
{
    if (potential_car.length() < 3 || potential_car.length() > 11)
        return true;

    for (auto sign:potential_car)
        if (isalpha(sign) == false && isdigit(sign) == false)
            return true;

    return false;
}

// Przetworzenie linii na polecenie zapytania (true = błędne dane).
bool analyse_query(string &line, string &index)
{
    size_t i = 1;
    while (i < line.length() && isspace(line[i]))
        i++;

    index = "";
    while (i < line.length() && isspace(line[i]) == false)
    {
        index += line[i];
        i++;
    }

    while (i < line.length() && isspace(line[i]))
        i++;

    return i != line.length() || (wrong_road(index) && wrong_car(index) && index != "");
}

// Sprawdzenie czy identyfikator pozycji auta na drodze
//      jest zgodny z poleceniem zadania (true = błędne dane).
bool wrong_height(LL &height, string height_str)
{
    if (height_str.length() < 3)
        return true;

    if (height_str[height_str.length() - 2] != ',')
        return true;

    if (height_str[0] == '0' && height_str[1] != ',')
        return true;

    height = 0;
    for (size_t i = 0; i < height_str.length(); ++i)
    {
        if (isdigit(height_str[i]) == false)
        {
            if (i != (height_str.length() - 2) || height_str[i] != ',')
                return true;
        }
        else
        {
            height = height*10 + (height_str[i] - '0');
        }
    }

    return false;
}

// Preprocessing analizy wczytanej linii - ucięcie białych znaków z przodu.
string cut_blank_prefix(string line)
{
    string res = "";
    size_t i = 0;
    while (i < line.length() && isspace(line[i]))
        i++;
    while (i < line.length())
        res += line[i++];
    return res;
}

// Przetworzenie linii na polecenie wczytania informacji (true = błędne dane).
bool analyse_data_insertion(string &line, string &car, string &road, LL &height)
{
    size_t i = 0;
    while (i < line.length() && isspace(line[i]))
        i++;

    car = "";
    while (i < line.length() && isspace(line[i]) == false)
    {
        car += line[i];
        i++;
    }

    while (i < line.length() && isspace(line[i]))
        i++;

    road = "";
    while (i < line.length() && isspace(line[i]) == false)
    {
        road += line[i];
        i++;
    }

    while (i < line.length() && isspace(line[i]))
        i++;

    string height_str = "";
    while (i < line.length() && isspace(line[i]) == false)
    {
        height_str += line[i];
        i++;
    }

    while (i < line.length() && isspace(line[i]))
        i++;

    return i != line.length() || wrong_car(car) || wrong_road(road) || wrong_height(height, height_str);
}

// Wstępna analiza wczytanej linii.
inline bool analyse_line(string &line, string &index, string &car, string &road, LL &height)
{
    if (line[0] == QMARK)
        return analyse_query(line, index);
    else
        return analyse_data_insertion(line, car, road, height);
}

// Wypisanie liczby kilometrów w formacie z zadania.
inline string int_to_string(LL x)
{
    string res = "";
    res += (char)(x % 10 + '0');
    res = ',' + res;
    x /= 10;
    if (x == 0)
    {
        res = '0' + res;
    }
    while (x != 0)
    {
        res = (char)(x % 10 + '0') + res;
        x /= 10;
    }
    return res;
}

// Obsługa polecenia wczytania informacji.
void insert_data(LL line, string car, string road, LL height,
        map<string, car_info*> &car_status, map<string, LL, comp_road> &road_status,
        map <LL, string> &line_db)
{
    if (car_status.find(car) == car_status.end())
    {
        car_info* new_info = new car_info;
        *new_info = make_tuple(0, "", 0, 0, 0, false, false);
        car_status[car] = new_info;
    }
    car_info* wsk = car_status[car];

    if (get<0>(*wsk) > 0 && get<1>(*wsk) != road)
    {
        cerr << "Error in line " << get<0>(*wsk) << ": " << line_db[get<0>(*wsk)] << "\n";
                get<0>(*wsk) = 0;
    }

    if (get<0>(*wsk) > 0 && get<1>(*wsk) == road)
    {
        road_status[road] += abs(height - get<2>(*wsk));
        if (road[0] == 'A')
        {
            get<3>(*wsk) += abs(height - get<2>(*wsk));
            get<5>(*wsk) = true;
        }
        else if (road[0] == 'S')
        {
            get<4>(*wsk) += abs(height - get<2>(*wsk));
            get<6>(*wsk) = true;
        }
        get<0>(*wsk) = 0;
    }
    else
    {
        get<0>(*wsk) = line;
        get<1>(*wsk) = road;
        get<2>(*wsk) = height;
    }
}

// Wypisanie informacji o aucie.
void print_car(string car, car_info new_info)
{
    if (get<5>(new_info) == true || get<6>(new_info) == true)
    {
        cout << car;
        if (get<5>(new_info) == true)
        {
            cout << " A " << int_to_string(get<3>(new_info));
        }
        if (get<6>(new_info) == true)
        {
            cout << " S " << int_to_string(get<4>(new_info));
        }
        cout << endl;
    }
}

// Obsługa polecenia zapytania.
void query(string where, map<string, car_info*> &car_status, map<string, LL, comp_road> &road_status)
{
    if (where == "")
    {
        for (auto p : car_status)
        {
            print_car(p.first, *(p.second));
        }
        for (auto p : road_status)
        {
            cout << p.first << " " << int_to_string(p.second) << endl;
        }
    }
    else
    {
        if (car_status.find(where) != car_status.end())
        {
            print_car(where, *(car_status[where]));
        }
        if (road_status.find(where) != road_status.end())
        {
            cout << where << " " << int_to_string(road_status[where]) << endl;
        }
    }
}

// Zwalnianie zaalokowanej pamięci.
void clear_map(map<string, car_info*> &car_status) {
	for (auto i : car_status) {
		delete i.second;
	}
}

int main()
{
    // Inicjalizacja zmiennych używanych w programie.
    LL height, line_counter = 0;
    string line;
    string index, car, road;
    map <string, car_info*> car_status;
    map <string, LL, comp_road> road_status;
    map <LL, string> line_db;

    while (getline(cin, line))
    {
        string line_cut = cut_blank_prefix(line);
        line_counter++;
        if (line.empty() == false)
        {
            if (analyse_line(line_cut, index, car, road, height) == false)
            {
                if (line_cut[0] == QMARK)
                {
                    query(index, car_status, road_status);
                }
                else
                {
                    line_db[line_counter] = line;
                    insert_data(line_counter, car, road, height, car_status, road_status, line_db);
                }
            }
            else cerr << "Error in line " << line_counter << ": " + line << endl;
        }
    }
	clear_map(car_status);
    return 0;
}


