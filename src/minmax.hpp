/*
 * DoublyLinkedList.hpp
 *
 *  Created on: 12 nov. 2017
 *      Pr�nom: Elisha
 *      Nom: Drion
 */

#ifndef MINMAX_HPP_
#define MINMAX_HPP_

#include <stdlib.h>
#include <cstddef>

template <typename Type>
// typename Type repr�sente le param�tre pass�
class MinMax {

	protected:
		template <typename T> static void swap (T& a, T& b)
			{const T tmp = a; a = b; b = tmp;}

	protected:
		class const_iterator {
			public:
				//N�cessaire d'avoir une classe abstraite
				virtual const Type& operator*() const = 0;
				virtual ~const_iterator() {}
		};

	public:
		virtual ~MinMax() {}
		virtual Type front() const = 0;
		virtual Type back() const = 0;
		virtual void pop_front() = 0;
		virtual void pop_back() = 0;
		virtual void clear() = 0;
		virtual const const_iterator& insert(Type element) = 0;

		//--- ALIAS ---//
		virtual Type max() const = 0;
		virtual Type min() const = 0;
		virtual void pop_max() = 0;
		virtual void pop_min() = 0;
		//-------------//

		virtual std::size_t size() const = 0;
		virtual std::size_t max_size() const = 0;
		virtual bool empty() const = 0;
};


#endif
