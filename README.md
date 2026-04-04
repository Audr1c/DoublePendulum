Mesure de temps 
actuellement une image de 1000 x 1000 avec 3000 dp prends ~2.2 sec 
Total time: 2230.970000ms
        Category 1: 2225.820000ms, (99.8%) (Drawing)
        Category 2: 0.570000ms, (0.0%) (dump image)
        Category 3: 4.580000ms, (0.2%) (simumation)

tout est dans le drawing 
Draw un pendule prends 0.7ms
Total time: 0.730000ms
        Category 1: 0.000333ms, (0.0%) (calcul de position etc)
        Category 2: 0.387667ms, (53.1%) (draw line 1)
        Category 3: 0.342000ms, (46.8%) (draw line 2)


Blend: 
actuelement 
- non commutatif (Blend(a,b) != Blend(b,a))
- non associatif (Blend(a,Blend(b, c)) != Blend(Blend(a, b), c)) (c'est ca qui fait que ca rend moche en multiThread)

Idée pour regler les 2 probleme en 1
- modifier le pixel en 3 int couleur, un int alpha et un int nombre de pose  (au lieu de 4 unit_8)
- a chaque fois que je blend un pixel sur le canva, j'ajoute juste le pixel au canva sans division et on increment le nombre de pose. 
- puis à la fin on fait une moyenne en divisant par le nombre de pose
- Donc le brush est une simple matrice de pixel d'une couleur avec des 0 pour pose la où il y a rien et 1 la où il y a qqch
- optimiser l'addition des matrices pour que ca se fasse sans boucle ?

- Idee d'utiliser OpenCL et repasser sur les tracé de lignes a l'ancienne sans brush voir `kernel.cl`

        

Ets ce que `clSetKernelArg` remplace les parametre ou ajoute a la queue ?