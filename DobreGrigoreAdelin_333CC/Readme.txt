Partea 1:

Timpii rezultatelor pentru:

resize_factor = 2 num_threads = 1
2.718448

resize_factor = 2 num_threads = 2
1.400148

resize_factor = 2 num_threads = 4
0.708691

resize_factor = 2 num_threads = 8
0.353822

resize_factor = 3 num_threads = 1
2.412948

resize_factor = 3 num_threads = 2
1.232205

resize_factor = 3 num_threads = 4
0.624337

resize_factor = 3 num_threads = 8
0.309228

resize_factor = 8 num_threads = 1
2.183845

resize_factor = 8 num_threads = 2
1.096877

resize_factor = 8 num_threads = 4
0.557830

resize_factor = 8 num_threads = 8
0.278958

	Pentru prima parte, am ales sa paralelizez primul for al functiei resize(), cel care parcurge toate liniile ce vor fi scrise in imaginea de output.
Variabilele shared, adica cele care au o singura copie pentru toate threadurile am ales sa fie structurile ce contin imaginile de intrare si iesire, "in" respectiv "out" si "resize_factor".
Variabilele private, cele pentru care se face cate o copie separata pentru fiecare thread sunt 
cele ce se afla in interiorul for-ului paralelizat.
	Timpii de mai sus sunt rezultatul rularii algoritmului pe o imagine cu extensia pnm si cu rezolutia de 10800 X 10800. Se observa ca timpul de rulare se injumatateste odata cu dublarea numarului de thread-uri, demonstrand astfel scalabilitatea algoritmului.

Partea 2:

Timpii rezultatelor pentru:

resolution = 10000 num_threads = 1
5.572826

resolution = 10000 num_threads = 2
2.853122

resolution = 10000 num_threads = 4
1.428762

resolution = 10000 num_threads = 8
0.698963

	Functia render este paralelizata similar cu partea 1, astfel ca am ales
sa paralelizez tot primul for, declarand variabilele private si shared. Am testat pe cluster crearea unei imagini cu rezolutia 10000 X 10000 si se observa din nou scalabilitatea, timpul injumatatindu-se pe masura ce dublez numarul de thread-uri.

	Observatie!
	Pe vmchecker am incarcat ultima versiune a temei pe 1 noiembrie 2015 in jururl orei 14.
Nu sunt sigur daca tema trebuie incarcata si pe cs.curs.pub.ro, dar am ales sa fac asta, desi este
cu o zi intarziere. Sper insa sa nu mi se depuncteze din nota, avand in vedere ca m-am incadrat 
in timpul acordat rezolvarii temei pe vmchecker. 

Dobre Grigore Adelin 333CC


