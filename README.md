# pkss-time
system zarządzania czasem
## 1) Na początku symulacji na endpoint każdego modułu (adres_modułu/start)
Metoda PUT z body (JSON):
{
	"speed":100, //wielokrotność normalnego kroku czasowego (int)
	"startTime" : "2019-11-18+18:40:15" //czas rozpoczęcia symulacji (string)
}
## 2) Co 5s na endpoint każdego modułu (adres_modułu/time)
Metoda PUT z body (JSON):
{
	"speed":100, //wielokrotność normalnego kroku czasowego (int)
	"symTime" : "2019-11-18+18:40:15" //aktualny czas symulacji (string)
}
## 3) Uruchomienie symulacli (moduł_czasowy/time/start)
Metoda PUT z body (JSON):
{
	"speed":100
}
## 4) Zmiana prędkości symulacji (moduł_czasowy/time/speed)
Metoda PUT z body (JSON):
{
	"speed":100
}
## 5) Szczegółowe informacje o aktualnym stanie czasu (moduł_czasowy/time/time_details)
Metoda GET, w odpowiedzi dostajemy (JSON)
{
	"speed" : 100 // aktualna prędkość symulacji
	"speed_to_set" : 200 // prędkość do ustawienia w kolejnym kroku
	"startTime" : "2019-11-18+18:40:15" //czas rozpoczęcia symulacji
	"symTime" : "2019-11-18+18:40:15" //aktualny czas symulacji
	"sim_start" : 1 //informacja mówiąca o tym czy rozpoczęła sie symulacja
}
