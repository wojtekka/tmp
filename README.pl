
  /tmp - tiny/tani mp3 player
  (C) Copyright 2003-2005 Wojtek Kaniewski <wojtekka@irc.pl>

  Za�o�enia:
  - prosty i tani odtwarzacz mp3,
  - wszystkie elementy
  - brak bajer�w typu wy�wietlacz lcd,
  - zapis plik�w na kart� osobnym urz�dzeniem.

  Ca�o�� zaprojektowa�em pod Linuksem, wi�c u�ytkownicy AVR Studio i innych
  windowsowych IDE b�d� musieli r�cznie stworzy� projekt i doda� odpowiednie
  pliki. Je�li to mo�liwe, prosz� o podes�anie tak przygotowanego projektu,
  �ebym m�g� go doda� do archiwum.

  Do debugowania u�ywam programowego portu RS232. Dane s� nadawane na wyj�ciu
  MISO z pr�dko�ci� 9600bps. Nale�y u�y� zewn�trznego konwertera poziom�w.

  Z ch�ci� przyjm� wszystkie poprawki i usprawnienia. Najlepiej gdyby by�y
  w postaci kodu z #ifdef, kt�ry w��czy poprawki dopiero po umieszczeniu
  odpowiedniej opcji. Dzi�ki temu pojedynczy kod b�dzie m�g� by� dostosowany
  do wielu r�nych sytuacji, bez utrzymywania kilku osobnych ga��zi.

