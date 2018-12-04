/*
 * DoublyLinkedList.hpp
 *
 *  Created on: 12 nov. 2017
 *      Prénom: Elisha
 *      Nom: Drion
 */

#ifndef MINMAXLIST_HPP_
#define MINMAXLIST_HPP_

#include "minmax.hpp"
#include "doublylinkedlist.hpp"

#include <stdexcept>


template <typename Type>
class MinMaxList: public virtual MinMax<Type>, DoublyLinkedList<Type> {

	using Node = typename DoublyLinkedList<Type>::Node;
	std::size_t _size;

	//--- ITERATEUR ---//

	private:
		class const_iterator: public MinMax<Type>::const_iterator {
			friend class MinMaxList;
			private:
				const MinMaxList* list;
				const Node* current;
				const_iterator() : list(nullptr), current(nullptr) {}
				const_iterator(const MinMaxList* _list, const Node* node) : list(_list), current(node) {}

			public:
				const Type& operator*() const override;
				const Type* operator-> () const;
				const_iterator operator++(int);
				const_iterator& operator++ ();

				inline friend bool operator!=(const const_iterator& iterator1, const const_iterator& iterator2)
						{return iterator1.current != iterator2.current;}
				inline friend bool operator== (const const_iterator& iterator1, const const_iterator& iterator2)
						{return iterator1.current == iterator2.current;}
		};
		void initWithIterator(MinMaxList*, const_iterator&&, const_iterator&&);

	public:
		using iterator = const_iterator;
		iterator begin() {return iterator(this, this->head->next);}
		iterator end() {return iterator(this, this->head);}
		const_iterator cbegin() const {return const_iterator(this, this->head->next);}
		const_iterator cend() const {return const_iterator(this, this->head);}

	//-----------------//

	public:
		Type front() const override;
		Type back() const override;
		void pop_front() override;
		void pop_back() override;
		void clear() override;
		const const_iterator& insert(Type element) override;

		inline Type max() const override {return front();}
		inline Type min() const override {return back();}
		inline void pop_max() override {pop_front();}
		inline void pop_min() override {pop_back();}

		std::size_t size() const override {return _size;}
		std::size_t max_size() const override {return _size;}
		bool empty() const override {return _size == 0;}

		//--- CONSTRUCTEURS ET OPERATEURS ---//

		MinMaxList(): _size(0) {}

		//constructeur initialisant
		MinMaxList (const Type& element): _size(0) {this->insert(element);}

		//constructeur de copie
		MinMaxList(const MinMaxList& list): _size(0) {
			initWithIterator(this, list.cbegin(), list.cend());
		}

		//construteur de transfert
		MinMaxList(MinMaxList&& list): _size(list._size) {
			this->head = list.head;
			list._size = 0;
			list.head = nullptr;
		}

		//opérateur d'assignation
		MinMaxList& operator= (const MinMaxList& list) {
			if (this != &list) {
				clear();
				initWithIterator(this, list.cbegin(), list.cend());
			}
		    return *this;
		}

		//opérateur de transfert
		MinMaxList& operator= (MinMaxList&& list){
			if (this != &list){
				_size = list._size;
				list._size = 0;
				this->head = list.head;
				list.head = nullptr;
			}
			//Retourne une référence à l'objet
			return *this;
		}

		template <typename T> friend inline void swap(MinMaxList<T>&, MinMaxList<T>&);
		bool operator== (const MinMaxList<Type>&);

		template <typename T>
		friend std::ostream& operator<<(std::ostream&, const MinMaxList<T>&);

		template <typename T>
		friend std::istream& operator>> (std::istream&,  MinMaxList<T>&);

		template <typename T>
		friend bool operator< (const MinMaxList<T>&, const MinMaxList<T>&);

		template <typename T>
		friend bool operator== (const MinMaxList<T>&, const MinMaxList<T>&);

};

// ------------------------------------------- //

template <typename Type>
void MinMaxList<Type>::initWithIterator(MinMaxList* list, const_iterator&& it1, const_iterator&& it2){
	for (; it1 != it2; ++it1){
		list->insert(*it1);
	}
}

// ------------------------------------------- //

template <typename Type>
Type MinMaxList<Type>::front() const {
	if (_size == 0)
		throw std::invalid_argument("MinMaxList<Type>::front(): la liste est vide");
	return this->head->next->value;
}

template <typename Type>
Type MinMaxList<Type>::back() const {
	if (_size == 0)
		throw std::invalid_argument("MinMaxList<Type>::back(): la liste est vide");
	return this->head->previous->value;
}

template <typename Type>
void MinMaxList<Type>::pop_front() {
	if (_size != 0){
		//S'assurer de ne pas supprimer l'élément bidon
		if (this->head != this->head->next){
			Node* to_delete = this->head->next;
			this->deleteNode(to_delete);
		}
	}
}

template <typename Type>
void MinMaxList<Type>::pop_back() {
	if (_size != 0){
		//S'assurer de ne pas supprimer l'élément bidon
		if (this->head != this->head->previous){
			Node* to_delete = this->head->previous;
			this->deleteNode(to_delete);
		}
	}
}

template <typename Type>
void MinMaxList<Type>::clear() {
	DoublyLinkedList<Type>::clear();
}

template <typename Type>
const typename MinMaxList<Type>::const_iterator& MinMaxList<Type>::insert(Type element) {
	this->_size++;
	this->insertNode(element);
	return MinMaxList<Type>::const_iterator(this, this->head->previous);
}

// ------------------------------------------- //
// ITERATEURS
// ------------------------------------------- //

template <typename T>
inline const T& MinMaxList<T>::const_iterator::operator*() const {
	return current->value;
}
template <typename T>
inline const T* MinMaxList<T>::const_iterator::operator->() const {
	return &(current->value);
}
template <typename T>
inline typename MinMaxList<T>::const_iterator MinMaxList<T>::const_iterator::operator++(int) {
	const_iterator temp(*this);
	current = current->next;
	return temp;
}
template <typename T>
inline typename MinMaxList<T>::const_iterator& MinMaxList<T>::const_iterator::operator++() {
	current = current->next;
	return *this;
}

// ------------------------------------------- //
// FONCTIONS EXTERNES
// ------------------------------------------- //

template <typename T>
inline void swap (MinMaxList<T>& list1, MinMaxList<T>& list2) {
	MinMaxList<T>::swap(list1._size, list2._size);
	MinMaxList<T>::swap(list1.head, list2.head);
}

template <typename Type>
std::ostream& operator<< (std::ostream& out, const MinMaxList<Type>& list) {
	out << "Il y a "<< list.size() << " éléments dans le conteneur \n";
	out << "[ ";
	for (typename MinMaxList<Type>::const_iterator p = list.cbegin(); p != list.cend(); ++p)
		out << *p << ", ";
	out << "] \n";
	return out;
}

template <typename T>
std::istream& operator>> (std::istream& in,  MinMaxList<T>& list){
	T value;
	in >>  value;
	list.insert(value);
	return in;
}

template <typename Type>
inline bool operator== (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2){
	bool result = list1.size() == list2.size();
	for (
			typename MinMaxList<Type>::const_iterator it1 = list1.cbegin(), it2 = list2.cbegin();
			result and it1!=list1.cend();
			++it1, ++it2){
		result = (*it1 == *it2);
	}
	return result;
}

template <typename Type>
inline bool operator!= (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2){
	return !(list1==list2);}

template <typename Type>
inline bool operator< (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2) {
  typename MinMaxList<Type>::const_iterator it1, it2;
  for (it1 = list1.cbegin(), it2 = list2.cbegin();
      it1 != list1.cend() && it2 != list2.cend();
      ++it1, ++it2){
    if (*it1 < *it2) return true;
    if (*it2 < *it1) return false;
  }
  return it1 == list1.cend() && it2 != list2.cend();
}

template <typename Type>
inline bool operator> (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2)
  {return list2 < list1;}

template <typename Type>
inline bool operator<= (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2)
  {return !(list2 < list1);}

template <typename Type>
inline bool operator>= (const MinMaxList<Type>& list1, const MinMaxList<Type>& list2)
  {return !(list1 < list2);}

#endif
