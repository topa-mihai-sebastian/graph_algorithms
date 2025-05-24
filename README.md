## Problema 1

In primul loop am incercat sa fac cat mai multe lucruir pentru eficienta:
- am citit valoarea v[i], verific daca v[1] != 0(se poate face asta pentru ca este
garantat ca v[1] va fi citit la aceasta operatie), verific daca mai exista un element egal cu 0, iar apoi calculez maximul, care reprezinta adancimea arborelui.

Declar 2 structuri de date:
- vector<vector> layer (int)
- vector<pair> edges (int, int)

prima va retine datele duap inaltimea lor din arbore dupa formula:
 - layer[v[i]].push_back(i);
 de exemplu pentru valoare din cerinta:
 v = 0 1 1 2
incepem de la i =1
 v| 0 1 1 2			i 	  |0   1        2     3 4
 i| 1 2 3 4			layers|1   v[2]		v[4]
								2		4
								v[3]
								3	

