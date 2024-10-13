## Postelnicu Cristina-Marina 313CA 2023-2024

    Programul incepe prin definirea structurilor: RGB_MATRIX si IMAGE.
    Structura IMAGE contine: magic_word, care reprezinta formatul imaginii,
dimensiunile matricei, valorea maxima a pixelilor, o matrice de tip int pentru
imaginile alb-negru si Grayscale si cea de a 2 a structura, RGB_MATRIX, pentru
imaginile color, fiecare element al matricea rgb avand trei valori, cate una
pentru fiecare culoare, rosu, verde si albastru.
    La introducerea de la tastatura a unei comenzi in main se apeleaza functia
corespunzatoare si se citesc in continuare comenzi pana cand se introduce
comanda de iesire, EXIT. Astfel fiecare operatie este realizata printr o
functie.
    Inainte de a prezentarea functiile principale, programul mai contine doua
functii de tip int. Functia CLAMP are rolul de a pastra valorea pixelilor
intre 0 si 255, aceasta fiind folosita la EQUALIZE si APPLY. Functia
ATOI_COORDINATES are rolul de a transforma coordonatele din char in int,
aceasta fiind folosita in functia SELECT, unde este citita cu fgets intreaga
linie a comenzii si coordonatele sunt obtinute cu functia strtok.
    Functiile care alcatuiesc programul sunt:
## LOAD, LOAD_ASCII SI LOAD_BINARY
    Functia LOAD incepe prin eliberarea memoriei matricei imaginii anterioare.
Eliberarea se realizeaza daca variabila photo, care poate fi 0, atunci cand 
nu este incarcata o imagine, si 1, atunci cand este incarcata o imagine. De
asemenea, se verifica care dintre cele doua matrice exista, matrix sau rgb.
    In continuare se citeste numele fisierului in variabila file_name si
se deschide acest fisier. Citirea din fisier incepe prin citirea
magic_word-ului in auxiliara magic_aux. Se verifica daca magic_aux desemneaza
o matrice ascii sau binara si se apeleaza functiile LOAD_ASCII sau LOAD_Binary.
    Functiile LOAD_ASCII si LOAD_BINARY incep prin copierea lui magic_aux
in im->magic_word, pentru ca magic_word ul sa se afle in structura.
Imaginile de tip P1, P2 si P3 au matricile de tip ascii, iar P4, P5 si P6 au
matricele de tip binar. Pentru P1, P2, P3 se foloseste functia LOAD_ASCII si
pentru P4, P5, P6 se foloseste functia LOAD_BINARY. IN functia LOAD_ASCII se
citeste din fisier folosind fscaf, iar pentru LOAD_BINARY se folosste fread
pentru citirea matricei. Dupa incarcarea unui tip de imagine, variabila photo
devinde 1. La finalul celor 2 functii se inchide fisierul.
## SELECT
    Exista doua tipuri de selectie SELECT ALL si SELECT x1 y1 x2 y2. De aceea
se citeste cu fgets intreaga linie si se foloseste functia strtok pentru a
separa cuvintele. Dupa prima utilizare a functiei strtok, se obtine variabila
all, careia i se elimina new line ul. Daca in variabila all se afla cuvantul
"ALL", atunci se face selectia intregii matrice, incepand cu coltul stanga sus.
Daca in variabila all nu se afla "ALL", inseamna ca se selcteaza dupa
coordonate. Continutul variabilei all este copiat in x1 si se efectueaza din
nou strtok pentru a obtine si urmatoarele coordonate in y1, x2 si y2.
Se transforma coordonatele in date de tip int prin functia ATOI_COORDINATES,
dupa care se verifica daca aceastea sunt valide, atat daca x1 si x2 sau y1 si
y2 sunt puse in ordine crescatoare, cat si daca coordonatele sunt egale sau mai
mari decat dimensiunile matricei sau sunt negative. Dupa ce coordonatele sunt
transformate si verficate sunt puse in structura.
## EQUALIZE
    Operatia EQUALIZE nu se realizeaza pentru imagini color, verificandu se
daca magic_word este P3 sau P6 la inceput. Se calculeaza suprafata, inmultind
dimensiunile matricei. Initializam elementele vectorului de frecventa cu 0,
dupa care il construim pentru valorile fiecarui pixel. Parcurgem matricea si
initializam cu 0 suma frecventelor pana la elementul dat, conform formulei.
## CROP, CROP_MATRIX, CROP_RGB
    Functia CROP apeleaza functiile CROP_MATRIX si CROP_RGB in functie de
valoarea magic_word-ului. Functiile CROP_MATRIX si CROP_RGB aloca memorie
pentru o matrice auxiliaza, aux_matrix pentru alb-negru si Grayscale sau
aux_rgb pentru color. Se pun in auxiliara elemnetele selectate din matrice,
luand un for de dimensiunile selectiei. Se elibereaza matricea originala, se
actualizeaza dimensiunile matricei la cele ale selectiei si apoi se realoca
memorie pentru matricea din structura cu noile dimensiuni. Punem elementele din
auxiliara in matricea din structura (matrix sau rgb), dupa care eliberam 
memoria alocata pentru auxiliara.
## APPLY, APPLY_EDGE, APPLY_SHARPEN, APPLY_BLUR, APPLY_GAUSSIAN_BLUR
    Citim toata linia cu fgets si folosim functia strtok pentru a obtine
parametrul lui APPLY. Daca dupa Apply nu urmeaza nimic se considera comanda
invalida, iar daca parametrul este diferit de operatiile existente, parametrul
nu este valid. Operatia APPLY se foloseste pentru matricele color, deci trebuie
verificat magic_word ul.
    Functiile APPLY_EDGE, APPLY_SHARPEN, APPLY_BLUR si APPLY_GAUSSIAN_BLUR sunt
realizate in acelasi fel,fiind diferite doar numerele cu care se inmultesc
vecinii, iar APPLY_BLUR si APPLY_GAUSSIAN_BLUR impart sumele la 9 si respectiv
la 16. In functii se aloca memorie pentru o matrice auxiliara de tip rgb care
are dimensiunile selectiei. Se verifica daca matricea auxiliara se afla sau nu 
pe marginile matricei originale, deoarece marginile nu au vecini si trebuie
copiate. Dupa aceasta verificare se initializeaza cu 0 elementele matricei aux
pentru rosu, verde si albastru, apoi se aduna in aceste elemente inmultirea
elementelor matricei originale aflate in selectie cu elemntele din kernel.
Elementele matricei aux sunt transformate in double si rotunjite, dupa se
foloseste asupra lor functia CLAMP. Elementele obtinute in aux sunt puse apoi
in matricea originala in selectie. La finalul functiilor se elibereaza memoria
alocata pentru aux.
## SAVE, SAVE_ASCII, SAVE_BINARY
    Citim toata linia cu fgets si folosim functia strtok pentru a obtine numele
fisierului care trebuie deschis si tipul de salvare dorit. Daca dupa comanda
SAVE si dupa numele fisierului urmeaza "ascii" se salveaza in format ascii si
daca nu urmeaza nimic se salveaza binar. Dupa ce folisim prima data functia
strtok obtinem numele fisierului, iar dupa folosirea functiei a 2 a oara,
verificam daca file_type are valoarea "ascii". Daca avem "ascii" se apeleaza
functia SAVE_ASCII care printeaza cu fprintf structura in functie de
magic_word. Daca imaginea este de tip binar si trebuie salvata in ascii,
se schimba magic word ul. In cazul functiei SAVE_BINARY se intampla acelasi,
dar se foloseste fwrite.
## EXIT
    Functia EXIT elibereaza memoria alocata pentru matricea de tip int sau
pentru matricea rgb.

