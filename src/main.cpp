/*
 * main.cpp
 *
 *  Created on: 12 nov. 2017
 *      Prénom: Elisha
 *      Nom: Drion
 */

#include "minmaxlist.hpp"
#include "minmaxvect.hpp"
#include <iostream>
#include <string>

template <typename Type>
void printMinMax(const Type& minmax, std::string name){
	std::cout << "Affichage du contenu de " << name << std::endl;
	std::cout << minmax;
	std::cout << "##### \n";
}

void testVectInt(){
	//Test de MinMaxVect
	printf("--- DEBUT DU TEST MINMAXVECT<int>--- \n");
	MinMaxVect<int> vect_int1;
	vect_int1.insert(5);
	vect_int1.insert(3);
	vect_int1.insert(8);
	vect_int1.insert(6);
	vect_int1.insert(6);
	vect_int1.insert(4);
	printMinMax(vect_int1, "vect_int1");
	printf("Construction par copie de vect_int2 en utilisant vect_int1 \n");
	MinMaxVect<int> vect_int2(vect_int1);
	printMinMax(vect_int2, "vect_int2");
	printf("Le maximum de vect_int1 est : %i \n", vect_int1.max());
	printf("Suppression du maximum de vect_int1 \n");
	vect_int1.pop_front();
	printMinMax(vect_int1, "vect_int1");
	printf("--- FIN DU TEST MINMAXVECT<INT>--- \n");
	printf("\n");
}

void testListString(){
	printf("--- DEBUT DU TEST MINMAXLIST<string>--- \n");
	MinMaxList<std::string> list_string1;
	list_string1.insert("a");
	list_string1.insert("z");
	list_string1.insert("h");
	list_string1.insert("p");
	list_string1.insert("t");
	printf("Affichage du contenu de list_string1 : \n");
	std::cout << list_string1;
	printf("##### \n");
	printf("Assignation de list_string1 à list_string2");
	MinMaxList<std::string> list_string2 = list_string1;
	printf("Affichage du contenu de list_string2 : \n");
	std::cout << list_string2;
	printf("##### \n");
	printf("--- FIN DU TEST MINMAXLIST<string>--- \n");
}

int main() {
	testVectInt();
	testListString();
	return 0;

}


