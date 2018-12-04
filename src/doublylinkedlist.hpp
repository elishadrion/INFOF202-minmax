/*
 * DoublyLinkedList.hpp
 *
 *  Created on: 12 nov. 2017
 *      Prénom: Elisha
 *      Nom: Drion
 */

#ifndef DOUBLYLINKEDLIST_HPP_
#define DOUBLYLINKEDLIST_HPP_
#include <iostream>


template <typename Type>
class DoublyLinkedList {

	protected:

		struct Node {
			Type value;
			Node* next;
			Node* previous;
			Node(): value(Type {}), next(nullptr), previous(nullptr) {}
			Node(const Type& _val, Node* _next, Node* _previous): value(_val), next(_next), previous(_next) {}
		};

		Node* head;

	public:
		//Constructeur
		DoublyLinkedList(): head(new Node) {
			//élément bidon
			head->next = head;
			head->previous = head;
		};

		//Destructeur
		~DoublyLinkedList() {
			clear();
			delete head;
		};

		//Désalloue la mémoire de chaque noeud sauf de head, vide la liste.
		void clear(){
			Node* current = head->next;
			while (current and current != head){
				Node* old = current;
				current = current->next;
				deleteNode(old);
			}
		}

		Node* createNode(Type _value) {
			Node* newNode = new Node;
			newNode->value = _value;
			newNode->next = newNode;
			newNode->previous = newNode;
			return newNode;
		};

		void deleteNode(Node* to_delete){
			to_delete->previous->next = to_delete->next;
			to_delete->next->previous = to_delete->previous;
			to_delete->next = nullptr;
			to_delete->previous = nullptr;
			delete to_delete;
		}


		Node* insertNode(Type _value){
			//Insère un nouveau noeud tout en préservant l'ordre.
			Node* node = createNode(_value);
			Node *current = head;
			//Recherche de la position où insérer le nouveau noeud
			while (current->next != head and _value < current->next->value) {
				current = current->next;
			}

			node->next = current->next;
			node->previous = current;
			current->next->previous = node;
			current->next = node;
			return node;

		}

};


#endif /* DOUBLYLINKEDLIST_HPP_ */
