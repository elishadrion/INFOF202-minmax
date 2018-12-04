/*
 * DoublyLinkedList.hpp
 *
 *  Created on: 12 nov. 2017
 *      Prénom: Elisha
 *      Nom: Drion
 */

#ifndef MINMAXVECT_HPP_
#define MINMAXVECT_HPP_
#define DEF_SIZE 10

#include "minmax.hpp"
#include <stdexcept>

template <typename Type>
class MinMaxVect: public virtual MinMax<Type>{

	//nombre d'éléments
	std::size_t _count;
	//Taille du vecteur
	std::size_t _size;
	int _indice_begin;
	int _indice_end;
	Type* _array;

	int mod(int a, int b) {
		return (a%b+b)%b;
	}

	//--- ITERATEUR ---//

	private:
		class const_iterator: public MinMax<Type>::const_iterator  {
			friend class MinMaxVect;
			private:
				const MinMaxVect* _array;
				std::size_t _indice;
				const_iterator() : _array(nullptr), _indice(0) {}
				const_iterator(const MinMaxVect* _list, std::size_t indice) : _array(_list), _indice(indice) {
					if (_array->_count == 0)
						_indice = 0;
				}

			public:
				const Type& operator*() const override;
				const Type* operator-> () const;
				const_iterator operator++(int);
				const_iterator& operator++ ();

				inline friend bool operator!=(const const_iterator& iterator1, const const_iterator& iterator2)
						{return iterator1._indice != iterator2._indice;}
				inline friend bool operator== (const const_iterator& iterator1, const const_iterator& iterator2)
						{return iterator1._indice == iterator2._indice;}

		};

		void initWithIterator(MinMaxVect*, const_iterator&&, const_iterator&&);

	public:
		using iterator = const_iterator;
		iterator begin() {return iterator(this, _indice_begin);}
		iterator end() {return iterator(this, _indice_end);}
		const_iterator cbegin() const {return const_iterator(this, _indice_begin);}
		const_iterator cend() const {return const_iterator(this, _indice_end);}

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

		std::size_t size() const override {return _count;}
		std::size_t max_size() const override {return _size;}
		bool empty() const override {return _count == 0;}
		std::size_t get_indice_begin() const {return _indice_begin;}
		std::size_t get_indice_end() const {return _indice_end;}

		//--- CONSTRUCTEURS ET OPERATEURS ---//

		//constructeur avec un seul paramètre, valeur par défaut
		//_size+1 pour l'élément bidon qui est le end
		explicit MinMaxVect(std::size_t size = DEF_SIZE):  _count(0), _size(size+1), _indice_begin(0), _indice_end(0), _array(new Type[size+1]()) {}

		//constructeur initialisant
		MinMaxVect (std::size_t size, const Type& value): _count(0), _size(size+1),  _indice_begin(0), _indice_end(0), _array(new Type[size+1]()) {
			this->insert(value);
		}

		//constructeur de copie
		MinMaxVect (const MinMaxVect& vect): _count(0), _size(vect.max_size()),  _indice_begin(0), _indice_end(0), _array(new Type[vect.max_size()+1]()) {
			initWithIterator(this, vect.cbegin(), vect.cend());
		}

		//opérateur d'assignation
		MinMaxVect& operator= (const MinMaxVect& vect) {
			if (this != &vect) {
				clear();
				initWithIterator(this, vect.cbegin(), vect.cend());
			}
		    return *this;
		}

		//opérateur de transfert
		MinMaxVect& operator= (MinMaxVect&& vect){
			if (this != &vect and _size == vect._size){
				_size = vect._size;
				_count = vect._count;
				_array = vect._array;
				_indice_begin = vect._indice_begin;
				_indice_end = vect._indice_end;
				vect._count = 0;
				vect._indice_begin = 0;
				vect._indice_end = 1;
				vect._array = new Type[size+1]();
			}
			//Retourne une référence à l'objet
			return *this;
		}

		//destructeur
		~MinMaxVect () {delete[] _array;}

		//-------------//

		template <typename T> friend inline void swap(MinMaxVect<T>&, MinMaxVect<T>&);
		inline const Type& operator[] (std::ptrdiff_t) const;

		template <typename T>
		friend std::ostream& operator<<(std::ostream&, const MinMaxVect<T>&);

		template <typename T>
		friend std::istream& operator>> (std::istream&,  MinMaxVect<T>&);

		template <typename T>
		friend bool operator< (const MinMaxVect<T>&, const MinMaxVect<T>&);

		template <typename T>
		friend bool operator== (const MinMaxVect<T>&, const MinMaxVect<T>&);

};

// ------------------------------------------- //

template <typename Type>
void MinMaxVect<Type>::initWithIterator(MinMaxVect* vect, const_iterator &&it1, const_iterator &&it2){
	for (; it1 != it2; ++it1){
		vect->insert(*it1);
	}
}

// ------------------------------------------- //

template <typename Type>
Type MinMaxVect<Type>::front() const {
	if (_count == 0)
		throw std::invalid_argument("MinMaxVect<Type>::front(): le vecteur est vide");
	return _array[_indice_begin];
}

template <typename Type>
Type MinMaxVect<Type>::back() const {
	if (_count == 0)
		throw std::invalid_argument("MinMaxVect<Type>::back(): le vecteur est vide");
	return _array[_indice_end-1];
}

template <typename Type>
void MinMaxVect<Type>::pop_front() {
	if (_count != 0){
		_indice_begin = (_indice_begin+1)%_size;
		_count -= 1;
	}
}

template <typename Type>
void MinMaxVect<Type>::pop_back() {
	if (_count != 0) {
		_indice_end = mod(_indice_end-1, _size);
		_count -= 1;
	}
}

template <typename Type>
void MinMaxVect<Type>::clear() {
	_indice_begin = 0;
	_indice_end = 0;
	_count = 0;
}

template <typename Type>
const typename MinMaxVect<Type>::const_iterator& MinMaxVect<Type>::insert(Type element) {
	if (_count == _size-1)
		throw std::invalid_argument("MinMaxVect<Type>::insert: le vecteur est rempli");

	typename MinMaxVect<Type>::const_iterator res_iterator;
	if (_count == 0){
		_array[0] = element;
		_count +=1;
		_indice_end = 1;
		res_iterator = this->cbegin();
	}
	else {
		//On insère l'élément à sa bonne place tout en préservant l'ordre
		//On décale tout ce qui est à droite de la place où l'élément doit être
		//inséré de 1
		int index = _indice_begin;
		//recherche de la position d'insertion
		while (_array[index] > element)
			index = (1+index)%_size;
		//Cas où le nouvel élément est le nouveau maximum
		if (element >= _array[_indice_begin]) {
			_indice_begin = index;
		}
		_indice_end = (1+_indice_end)%_size;
		//décalage vers la droite
		for (int i = _indice_end-1; i != index; --i) {
			i = mod(i, _size);
			_array[mod(i, _size)] = _array[mod(i-1, _size)];
		}
		//insertion de l'élément
		_array[index] = element;
		_count += 1;
		res_iterator = MinMaxVect<Type>::const_iterator(this, index);
	}
	return res_iterator;
}

// ------------------------------------------- //
// ITERATEURS
// ------------------------------------------- //

template <typename T>
inline const T& MinMaxVect<T>::const_iterator::operator*() const {
	return _array->_array[_indice];
}
template <typename T>
inline const T* MinMaxVect<T>::const_iterator::operator->() const {
	return &_array->_array[_indice];
}
template <typename T>
inline typename MinMaxVect<T>::const_iterator MinMaxVect<T>::const_iterator::operator++(int) {
	const_iterator temp(*this);
	_indice = ((_indice+1) % (_array->_size));
	return temp;
}
template <typename T>
inline typename MinMaxVect<T>::const_iterator& MinMaxVect<T>::const_iterator::operator++() {
	_indice = ((_indice+1) % (_array->_size));
	return *this;
}

// ------------------------------------------- //
// FONCTIONS EXTERNES
// ------------------------------------------- //

// Du fait de l'élément bidon, on doit adopter une autre logique concernant les
// indices du tableau. Toute les fonctions utilisées prennent en compte le fait que
// l'élément d'indice 0 est l'élément bidon.
template <typename Type>
const Type& MinMaxVect<Type>::operator[] (std::ptrdiff_t i) const {
  if (std::size_t(i) >= _count)
    throw std::out_of_range("*** Index out of range ***");
  return _array[i+1];
}

template <typename T>
inline void swap (MinMaxVect<T>& vect1, MinMaxVect<T>& vect2) {
	MinMaxVect<T>::swap(vect1._count, vect2._count);
	MinMaxVect<T>::swap(vect1._size, vect2._size);
	MinMaxVect<T>::swap(vect1._array, vect2._array);
}

template <typename Type>
std::ostream& operator<< (std::ostream& out, const MinMaxVect<Type>& vect) {
	out << "Il y a "<< vect.size() << " éléments dans le conteneur \n";
	out << "[ ";
	for (typename MinMaxVect<Type>::const_iterator p = vect.cbegin(); p != vect.cend(); ++p)
		out << *p << ", ";
	out << "]\n";
	return out;
}

template <typename T>
std::istream& operator>> (std::istream& in,  MinMaxVect<T>& vect){
	T value;
	in >>  value;
	vect.insert(value);
	return in;
}

template <typename Type>
inline bool operator== (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2){
	bool result = vect1.size() == vect2.size();
	for (
			typename MinMaxVect<Type>::const_iterator it1 = vect1.cbegin(), it2 = vect2.cbegin();
			result and it1!=vect1.cend();
			++it1, ++it2){
		result = (*it1 == *it2);
	}
	return result;
}

template <typename Type>
inline bool operator!= (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2){
	return !(vect1==vect2);}

template <typename Type>
inline bool operator< (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2) {
  typename MinMaxVect<Type>::const_iterator it1, it2;
  for (it1 = vect1.cbegin(), it2 = vect2.cbegin();
      it1 != vect1.cend() && it2 != vect2.cend();
      ++it1, ++it2){
    if (*it1 < *it2) return true;
    if (*it2 < *it1) return false;
  }
  return it1 == vect1.cend() && it2 != vect2.cend();
}

template <typename Type>
inline bool operator> (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2)
  {return vect2 < vect1;}

template <typename Type>
inline bool operator<= (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2)
  {return !(vect2 < vect1);}

template <typename Type>
inline bool operator>= (const MinMaxVect<Type>& vect1, const MinMaxVect<Type>& vect2)
  {return !(vect1 < vect2);}

#endif
