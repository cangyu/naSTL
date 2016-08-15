#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <cstdlib>
#include <algorithm>

namespace sjtu 
{
//a container like std::list
template<typename T>
class list
{
private:
	//list中的节点
	//不论是默认构造还是拷贝构造或是赋值，
	//均不拷贝对方的指针，只拷贝data，并保持指针指向自己
	class node
	{
		friend class list<T>;

	public:
		node *prev, *next;
		T data;

		node() :data()
		{
			prev = next = this;
		}

		node(const T &_data) :data(_data)
		{
			prev = next = this;
		}

		node(node *_prev, node *_next, const T &_data) :
			prev(_prev), 
			next(_next),
			data(_data)
		{}

		node(const node &rhs) :data(rhs.data)
		{
			prev = next = this;
		}

		node& operator=(node rhs)
		{
			exchange(rhs);
			return *this;
		}

		~node() = default;

	private:
		void exchange(node &rhs)
		{
			std::swap(data, rhs.data);
		}

		//given node a,b
		//insert a before b
		static void insert_single(node *a, node *b)
		{
			b->prev->next = a;
			a->next = b;
			a->prev = b->prev;
			b->prev = a;
		}

		//link node a and b
		//a<->b
		static void link(node *a, node *b)
		{
			a->next = b;
			b->prev = a;
		}
	};

	node *last;
	size_t elemCnt;

	void exchange(list &rhs)
	{
		std::swap(last, rhs.last);
		std::swap(elemCnt, rhs.elemCnt);
	}

public:
	//iterator that knows the ascription
	//由于list的插入删除不会引起其它的迭代器失效，
	//所以不需要在list中引入额外的指示list的last信息
	class const_iterator;
	class iterator 
	{			
		friend class const_iterator;
		friend class list<T>;

	private:
		list<T> *ascription;
		node *cur;

		void exchange(iterator &rhs)
		{
			std::swap(ascription, rhs.ascription);
			std::swap(cur, rhs.cur);
		}

	public:
		iterator(list<T> *_a = nullptr, node *_c = nullptr) : 
			ascription(_a), 
			cur(_c)
		{}

		iterator(const iterator &rhs) :
			ascription(rhs.ascription), 
			cur(rhs.cur)
		{}

		~iterator() = default;
		
		iterator& operator=(iterator rhs)
		{
			exchange(rhs);
			return *this;
		}
		
		//iter++
		iterator operator++(int) 
		{
			iterator tmp(*this);
			++*this;
			return tmp;
		}

		 //++iter
		iterator& operator++() 
		{
			cur = cur->next;
			return *this;
		}

		//iter--
		iterator operator--(int) 
		{
			iterator tmp(*this);
			--*this;
			return tmp;
		}

		//--iter
		iterator& operator--() 
		{
			cur = cur->prev;
			return *this;
		}

		//*iter
		T& operator*() const 
		{ 
			return cur->data;
		}
		
		//iter->
		T* operator->() const 
		{ 
			return &(operator*()); 
		}

		//check whether two iterators are same (pointing to the same memory).
		bool operator==(const iterator &rhs) const 
		{ 
			if (!ascription || !rhs.ascription || ascription != rhs.ascription)
				return false;
			else if (!cur || !rhs.cur || cur != rhs.cur)
				return false;
			else
				return true;
		}
		
		bool operator==(const const_iterator &rhs) const 
		{ 
			if (!ascription || !rhs.ascription || ascription != rhs.ascription)
				return false;
			else if (!cur || !rhs.cur || cur != rhs.cur)
				return false;
			else
				return true;
		}
		
		bool operator!=(const iterator &rhs) const 
		{ 
			return !operator==(rhs); 
		}
		
		bool operator!=(const const_iterator &rhs) const 
		{ 
			return !operator==(rhs);
		}

		//self check
		bool isValid(void *id) const
		{
			if (!id || (void *)ascription != id || !ascription->last)
				return false;

			node *p = ascription->last;
			do {
				if (cur == p)
					return true;
				else
					p = p->next;
			} while (p != ascription->last);

			return false;
		}
	};

	//Same as iterator,but can't modify data through it
	class const_iterator 
	{
		friend class iterator;
		friend class list<T>;

	private:
		const list<T> *ascription;
		const node *cur;

		void exchange(iterator &rhs)
		{
			std::swap(ascription, rhs.ascription);
			std::swap(cur, rhs.cur);
		}

	public:
		//default constructor
		const_iterator(const list<T> *_a = nullptr, const node *_c = nullptr) :
			ascription(_a),
			cur(_c)
		{}

		//construct from iterator
		const_iterator(const iterator &rhs) :
			ascription(rhs.ascription),
			cur(rhs.cur)
		{}

		//copy-constructor
		const_iterator(const const_iterator &rhs) :
			ascription(rhs.ascription),
			cur(rhs.cur)
		{}

		//destructor
		~const_iterator() = default;

		//assignment operator
		const_iterator& operator=(const_iterator rhs)
		{
			exchange(rhs);
			return *this;
		}

		//iter++
		const_iterator operator++(int)
		{
			iterator tmp(*this);
			++*this;
			return tmp;
		}

		//++iter
		const_iterator& operator++()
		{
			cur = cur->next;
			return *this;
		}

		//iter--
		const_iterator operator--(int)
		{
			const_iterator tmp(*this);
			--*this;
			return tmp;
		}

		//--iter
		const_iterator& operator--()
		{
			cur = cur->prev;
			return *this;
		}

		//*iter
		const T& operator*() const
		{
			return cur->data;
		}

		//iter->
		const T* operator->() const
		{
			return &(operator*());
		}

		//check whether two iterators are same (pointing to the same memory).
		bool operator==(const iterator &rhs) const
		{
			if (!ascription || !rhs.ascription || ascription != rhs.ascription)
				return false;
			else if (!cur || !rhs.cur || cur != rhs.cur)
				return false;
			else
				return true;
		}

		bool operator==(const const_iterator &rhs) const
		{
			if (!ascription || !rhs.ascription || ascription != rhs.ascription)
				return false;
			else if (!cur || !rhs.cur || cur != rhs.cur)
				return false;
			else
				return true;
		}

		bool operator!=(const iterator &rhs) const
		{
			return !operator==(rhs);
		}

		bool operator!=(const const_iterator &rhs) const
		{
			return !operator==(rhs);
		}

		//self check
		bool isValid(void *id) const
		{
			if (!id || (void *)ascription != id || !ascription->last)
				return false;

			node *p = ascription->last;
			do {
				if (cur == p)
					return true;
				else
					p = p->next;
			} while (p != ascription->last);

			return false;
		}
	};
	
	//Constructor
	list() :
		elemCnt(0),
		last((node *)std::malloc(sizeof(node)))
	{
		last->prev = last->next = last;
	}

	list(const list &rhs):
		elemCnt(rhs.elemCnt),
		last((node *)std::malloc(sizeof(node)))
	{
		last->prev = last->next = last;

		node *p = nullptr;
		node *t = rhs.last->next;
		
		while (t != rhs.last)
		{
			p = new node(*t);			
			node::insert_single(p, last);
			t = t->next;
		}
	}

	//Destructor
	~list()
	{
		clear();
		free(last);
	}

	//assignment operator
	list &operator=(list rhs) 
	{
		exchange(rhs);
		return *this;
	}

	//access the first element
	//throw container_is_empty when it is empty
	const T& front() const 
	{
		if (elemCnt == 0)
			throw container_is_empty();
		
		return last->next->data;
	}

	//access the last element
	//throw container_is_empty when it is empty
	const T& back() const 
	{
		if (elemCnt == 0)
			throw container_is_empty();

		return last->prev->data;
	}

	//returns an iterator to the beginning
	iterator begin()
	{ 
		return iterator(this, last->next);
	}

	const_iterator cbegin() const
	{ 
		return const_iterator(this, last->next);
	}

	//returns an iterator to the end
	iterator end()
	{ 
		return iterator(this, last);
	}

	const_iterator cend() const 
	{ 
		return const_iterator(this, last); 
	}

	//checks whether the container is empty
	bool empty() const 
	{ 
		return elemCnt == 0; 
	}

	//returns the number of elements
	size_t size() const 
	{ 
		return elemCnt; 
	}
	
	//clears the contents
	void clear() 
	{ 
		node *p = last->next;
		node *t = nullptr;

		while (p != last)
		{
			t = p;
			p = p->next;
			delete t;
		}

		elemCnt = 0;
		last->prev = last->next = last;
	}

	//inserts value before pos
	//returns an iterator pointing to the inserted value
	iterator insert(iterator pos, const T &value) 
	{
		if (pos.ascription != this)
			throw invalid_iterator();

		node *p = new node(value);
		node::insert_single(p, pos.cur);
		++elemCnt;

		return iterator(this, p);
	}

	//removes the element at pos.
	//returns an iterator following the last removed element.
	//If there was anything wrong with the iterator, throw invalid_iterator.
	//If the iterator pos refers to the last element, the end() iterator is returned.
	iterator erase(iterator pos) 
	{
		if (pos.ascription != this || pos.cur == last)
			throw invalid_iterator();

		node::link(pos.cur->prev, pos.cur->next);
		node *p = pos.cur->next;
		delete pos.cur;
		--elemCnt;

		return iterator(this, p);
	}

	//removes the elements in range [first, last).
	//returns an iterator following the last removed element.
	//If there was anything wrong with these iterators, throw invalid_iterator.
	//In this case, return last.
	iterator erase(iterator _first, iterator _last) 
	{
		//check ascription
		if (_first.ascription != this || _last.ascription != this || !_first.cur || !_last.cur)
			throw invalid_iterator();

		//check range
		node *start = _first.cur, *finish = _last.cur, *t = nullptr;
		for (t = start; t != finish; t = t->next)
			if (!t || t == last)
				throw invalid_iterator();
		
		//splice
		node::link(start->prev, finish);
		
		//remove
		size_t eraseCnt = 0;
		while (start != finish)
		{
			t = start;
			start = start->next;
			delete t;
		}
		elemCnt -= eraseCnt;

		return iterator(this, finish);
	}

	//adds an element to the end
	void push_back(const T &value) 
	{ 
		insert(end(), value); 
	}
	
	//removes the last element
	void pop_back() 
	{ 
		erase(--end());
	}

	//adds an element to the beginning
	void push_front(const T &value)
	{ 
		insert(begin(), value);
	}
	
	//removes the first element
	void pop_front() 
	{ 
		erase(begin()); 
	}
};

}

#endif