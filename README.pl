
  /tmp - tiny/tani mp3 player
  (C) Copyright 2003-2005 Wojtek Kaniewski <wojtekka@irc.pl>

  Za³o¿enia:
  - prosty i tani odtwarzacz mp3,
  - wszystkie elementy
  - brak bajerów typu wy¶wietlacz lcd,
  - zapis plików na kartê osobnym urz±dzeniem.

  Ca³o¶æ zaprojektowa³em pod Linuksem, wiêc u¿ytkownicy AVR Studio i innych
  windowsowych IDE bêd± musieli rêcznie stworzyæ projekt i dodaæ odpowiednie
  pliki. Je¶li to mo¿liwe, proszê o podes³anie tak przygotowanego projektu,
  ¿ebym móg³ go dodaæ do archiwum.

  Do debugowania u¿ywam programowego portu RS232. Dane s± nadawane na wyj¶ciu
  MISO z prêdko¶ci± 9600bps. Nale¿y u¿yæ zewnêtrznego konwertera poziomów.

  Z chêci± przyjmê wszystkie poprawki i usprawnienia. Najlepiej gdyby by³y
  w postaci kodu z #ifdef, który w³±czy poprawki dopiero po umieszczeniu
  odpowiedniej opcji. Dziêki temu pojedynczy kod bêdzie móg³ byæ dostosowany
  do wielu ró¿nych sytuacji, bez utrzymywania kilku osobnych ga³êzi.

