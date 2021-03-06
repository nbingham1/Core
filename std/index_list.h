/*
 * index_list.h
 *
 *  Created on: Feb 4, 2014
 *      Author: nbingham
 */

#pragma once

#include <std/range.h>
#include <std/slice.h>

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

namespace core
{

struct index_list_end_item
{
	index_list_end_item()
	{
		next = this;
		prev = this;
		index = -1;
	}

	virtual ~index_list_end_item()
	{
	}

	index_list_end_item *next;
	index_list_end_item *prev;
	int index;
};

template <typename value_type>
struct index_list_item : index_list_end_item
{
	index_list_item()
	{
	}

	index_list_item(const value_type &value)
	{
		this->value = value;
	}

	~index_list_item()
	{
	}

	value_type value;
};

template <typename value_type>
struct index_list;

template <typename value_type>
struct index_list_const_iterator;

template <typename value_type>
struct index_list_iterator
{
	friend class index_list<value_type>;
	friend class index_list_const_iterator<value_type>;
	typedef value_type type;

	index_list<value_type> *root;
	index_list_end_item *loc;

	index_list_iterator(index_list<value_type> *root, index_list_end_item *loc)
	{
		this->root = root;
		this->loc = loc;
	}

	index_list_iterator()
	{
		root = NULL;
		loc = NULL;
	}

	index_list_iterator(const index_list_iterator &copy)
	{
		root = copy.root;
		loc = copy.loc;
	}

	~index_list_iterator() {}

	operator bool() const
	{
		return root != NULL && loc != &root->left && loc != &root->right;
	}

	value_type &operator*() const
	{
		return ((index_list_item<value_type>*)loc)->value;
	}
	value_type *operator->() const
	{
		return &((index_list_item<value_type>*)loc)->value;
	}

	value_type *ptr() const
	{
		return &((index_list_item<value_type>*)loc)->value;
	}

	value_type &get() const
	{
		return ((index_list_item<value_type>*)loc)->value;
	}

	index_list_iterator<value_type> &ref()
	{
		return *this;
	}

	const index_list_iterator<value_type> &ref() const
	{
		return *this;
	}

	int idx() const
	{
		return loc->index;
	}

	index_list_iterator<value_type> operator++(int)
	{
		index_list_iterator<value_type> result = *this;
		loc = loc->next;
		return result;
	}

	index_list_iterator<value_type> operator--(int)
	{
		index_list_iterator<value_type> result = *this;
		loc = loc->prev;
		return result;
	}

	index_list_iterator<value_type> &operator++()
	{
		loc = loc->next;
		return *this;
	}

	index_list_iterator<value_type> &operator--()
	{
		loc = loc->prev;
		return *this;
	}

	index_list_iterator<value_type> &operator+=(int n)
	{
		while (n > 0 && loc != &root->right)
		{
			loc = loc->next;
			n--;
		}

		while (n < 0 && loc != &root->left)
		{
			loc = loc->prev;
			n++;
		}

		return *this;
	}

	index_list_iterator<value_type> &operator-=(int n)
	{
		while (n < 0 && loc != &root->right)
		{
			loc = loc->next;
			n++;
		}

		while (n > 0 && loc != &root->left)
		{
			loc = loc->prev;
			n--;
		}

		return *this;
	}

	index_list_iterator<value_type> operator+(int n) const
	{
		index_list_iterator<value_type> result(*this);
		result += n;
		return result;
	}

	index_list_iterator<value_type> operator-(int n) const
	{
		index_list_iterator<value_type> result(*this);
		result -= n;
		return result;
	}

	bool operator==(index_list_iterator<value_type> i) const
	{
		return loc->index == i.loc->index;
	}

	bool operator!=(index_list_iterator<value_type> i) const
	{
		return loc->index != i.loc->index;
	}

	bool operator<(index_list_iterator<value_type> i) const
	{
		return loc->index < i.loc->index;
	}

	bool operator>(index_list_iterator<value_type> i) const
	{
		return loc->index > i.loc->index;
	}

	bool operator<=(index_list_iterator<value_type> i) const
	{
		return loc->index <= i.loc->index;
	}

	bool operator>=(index_list_iterator<value_type> i) const
	{
		return loc->index >= i.loc->index;
	}

	bool operator==(index_list_const_iterator<value_type> i) const
	{
		return loc->index == i.loc->index;
	}

	bool operator!=(index_list_const_iterator<value_type> i) const
	{
		return loc->index != i.loc->index;
	}

	bool operator<(index_list_const_iterator<value_type> i) const
	{
		return loc->index < i.loc->index;
	}

	bool operator>(index_list_const_iterator<value_type> i) const
	{
		return loc->index > i.loc->index;
	}

	bool operator<=(index_list_const_iterator<value_type> i) const
	{
		return loc->index <= i.loc->index;
	}

	bool operator>=(index_list_const_iterator<value_type> i) const
	{
		return loc->index >= i.loc->index;
	}

	int operator-(index_list_iterator<value_type> i) const
	{
		return loc->index - i.loc->index;
	}

	int operator-(index_list_const_iterator<value_type> i) const
	{
		return loc->index - i.loc->index;
	}

	core::slice<range<index_list_iterator<value_type> > > sub(int length) const
	{
		if (length < 0)
			return range<index_list_iterator<value_type> >(*this+length, *this);
		else
			return range<index_list_iterator<value_type> >(*this, *this+length);
	}

	index_list<value_type> subcpy(int length) const
	{
		if (length < 0)
			return index_list<value_type>(*this+length, *this);
		else
			return index_list<value_type>(*this, *this+length);
	}

	core::slice<range<index_list_iterator<value_type> > > sub() const
	{
		return range<index_list_iterator<value_type> >(*this, root->end());
	}

	index_list<value_type> subcpy() const
	{
		return index_list<value_type>(*this, root->end());
	}

	void drop(int n = 1)
	{
		index_list_end_item* start = loc->prev;

		if (n > 0)
		{
			for (int i = 0; i < n && loc != &root->right; i++)
			{
				index_list_end_item *temp = loc->next;
				delete loc;
				loc = temp;
			}
				
			start->next = loc;
			loc->prev = start;
		}
		else if (n < 0)
		{
			for (int i = 0; i > n && start != &root->left; i--)
			{
				index_list_end_item *temp = start->prev;
				delete start;
				start = temp;
			}
			
			start->next = loc;
			loc->prev = start;
		}

		root->update_index(start);
	}

	index_list<value_type> pop(int n = 1)
	{
		index_list<value_type> result;
		index_list_end_item *start = loc;
		for (int i = 0; i < n && loc != &root->right; i++)
			loc = loc->next;
		for (int i = 0; i > n && start != &root->left && start != root->left.next; i--)
			start = start->prev;

		if (start != loc)
		{
			result.left.next = start;
			result.right.prev = loc->prev;
			start->prev->next = loc;
			loc->prev = start->prev;
			result.left.next->prev = &result.left;
			result.right.prev->next = &result.right;
		}

		root->update_index(loc->prev);
		result.update_index(&result.left);

		return result;
	}

	void push(value_type v) const
	{
		index_list_end_item *start = loc->prev;
		start->next = new index_list_item<value_type>(v);
		start->next->prev = start;
		start = start->next;
		start->next = loc;
		loc->prev = start;
		root->update_index(start->prev);
	}

	template <class container>
	void append(const container &c) const
	{
		index_list_end_item *start = loc->prev;
		for (typename container::const_iterator i = c.begin(); i != c.end(); i++)
		{
			start->next = new index_list_item<value_type>(*i);
			start->next->index = start->index+1;
			start->next->prev = start;
			start = start->next;
		}

		start->next = loc;
		loc->prev = start;
		root->update_index(start);
	}

	void replace(int n, value_type v)
	{
		if (n < 0)
		{
			*this += n;
			n = -n;
		}

		if (loc == &root->right)
		{
			push(v);
			loc = loc->next;
		}
		else
		{
			((index_list_item<value_type>*)loc)->value = v;
			loc = loc->next;
			drop(n-1);
		}
	}
	
	template <class container>
	void replace(int n, const container &c)
	{
		if (n < 0)
		{
			*this += n;
			n = -n;
		}

		typename container::const_iterator j = c.begin();
		while (n > 0 && j && loc != &root->right)
		{
			((index_list_item<value_type>*)loc)->value = *j;
			loc = loc->next;
			n--;
			j++;
		}

		if (j)
			append(j.sub());
		else if (n > 0)
			drop(n);
	}

	template <class iterator_type>
	void swap(iterator_type i) const
	{
		value_type temp;
		memcpy(&temp, &((index_list_item<value_type>*)loc)->value, sizeof(value_type));
		memcpy(&((index_list_item<value_type>*)loc)->value, &((index_list_item<value_type>*)i.loc)->value, sizeof(value_type));
		memcpy(&((index_list_item<value_type>*)i.loc)->value, &temp, sizeof(value_type));
	}

	void swap(index_list_iterator<value_type> &i)
	{
		index_list_end_item *next1 = loc->next;
		index_list_end_item *prev1 = loc->prev;
		index_list_end_item *next2 = i.loc->next;
		index_list_end_item *prev2 = i.loc->prev;
		int temp_index = loc->index;
		loc->index = i.loc->index;
		i.loc->index = temp_index;
		next1->prev = i.loc;
		prev1->next = i.loc;
		next2->prev = loc;
		prev2->next = loc;
		loc->next = next2;
		loc->prev = prev2;
		i.loc->next = next1;
		i.loc->prev = prev1;

		next1 = loc;
		loc = i.loc;
		i.loc = next1;
	}

	index_list_iterator<value_type> &operator=(index_list_iterator<value_type> i)
	{
		root = i.root;
		loc = i.loc;
		return *this;
	}
};

template <typename value_type>
struct index_list_const_iterator
{
	friend class index_list<value_type>;
	friend class index_list_iterator<value_type>;
	typedef value_type type;

	const index_list<value_type> *root;
	const index_list_end_item *loc;

	index_list_const_iterator(const index_list<value_type> *l, const index_list_end_item *n)
	{
		root = l;
		loc = n;
	}

	index_list_const_iterator()
	{
		root = NULL;
		loc = NULL;
	}

	index_list_const_iterator(const index_list<value_type> *l)
	{
		root = l;
		loc = &l->left;
	}

	index_list_const_iterator(const index_list_iterator<value_type> &i)
	{
		root = i.root;
		loc = i.loc;
	}

	index_list_const_iterator(const index_list_const_iterator<value_type> &i)
	{
		root = i.root;
		loc = i.loc;
	}

	~index_list_const_iterator() {}

	operator bool() const
	{
		return root != NULL && loc != &root->left && loc != &root->right;
	}

	const value_type &operator*() const
	{
		return ((index_list_item<value_type>*)loc)->value;
	}

	const value_type *operator->() const
	{
		return &((index_list_item<value_type>*)loc)->value;
	}

	const value_type &get() const
	{
		return ((index_list_item<value_type>*)loc)->value;
	}

	const value_type *ptr() const
	{
		return &((index_list_item<value_type>*)loc)->value;
	}

	index_list_const_iterator<value_type> &ref()
	{
		return *this;
	}

	const index_list_const_iterator<value_type> &ref() const
	{
		return *this;
	}

	int idx() const
	{
		return loc->index;
	}

	index_list_const_iterator<value_type> operator++(int)
	{
		index_list_const_iterator<value_type> result = *this;
		loc = loc->next;
		return result;
	}

	index_list_const_iterator<value_type> operator--(int)
	{
		index_list_const_iterator<value_type> result = *this;
		loc = loc->prev;
		return result;
	}

	index_list_const_iterator<value_type> &operator++()
	{
		loc = loc->next;
		return *this;
	}

	index_list_const_iterator<value_type> &operator--()
	{
		loc = loc->prev;
		return *this;
	}

	index_list_const_iterator<value_type> &operator+=(int n)
	{
		while (n > 0 && loc != &root->right)
		{
			loc = loc->next;
			n--;
		}

		while (n < 0 && loc != &root->left)
		{
			loc = loc->prev;
			n++;
		}

		return *this;
	}

	index_list_const_iterator<value_type> &operator-=(int n)
	{
		while (n < 0 && loc != &root->right)
		{
			loc = loc->next;
			n++;
		}

		while (n > 0 && loc != &root->left)
		{
			loc = loc->prev;
			n--;
		}

		return *this;
	}

	index_list_const_iterator<value_type> operator+(int n) const
	{
		index_list_const_iterator<value_type> result(*this);
		result += n;
		return result;
	}

	index_list_const_iterator<value_type> operator-(int n) const
	{
		index_list_const_iterator<value_type> result(*this);
		result -= n;
		return result;
	}

	bool operator==(index_list_const_iterator<value_type> i) const
	{
		return loc->index == i.loc->index;
	}

	bool operator!=(index_list_const_iterator<value_type> i) const
	{
		return loc->index != i.loc->index;
	}

	bool operator<(index_list_const_iterator<value_type> i) const
	{
		return loc->index < i.loc->index;
	}

	bool operator>(index_list_const_iterator<value_type> i) const
	{
		return loc->index > i.loc->index;
	}

	bool operator<=(index_list_const_iterator<value_type> i) const
	{
		return loc->index <= i.loc->index;
	}

	bool operator>=(index_list_const_iterator<value_type> i) const
	{
		return loc->index >= i.loc->index;
	}
	
	bool operator==(index_list_iterator<value_type> i) const
	{
		return loc->index == i.loc->index;
	}

	bool operator!=(index_list_iterator<value_type> i) const
	{
		return loc->index != i.loc->index;
	}

	bool operator<(index_list_iterator<value_type> i) const
	{
		return loc->index < i.loc->index;
	}

	bool operator>(index_list_iterator<value_type> i) const
	{
		return loc->index > i.loc->index;
	}

	bool operator<=(index_list_iterator<value_type> i) const
	{
		return loc->index <= i.loc->index;
	}

	bool operator>=(index_list_iterator<value_type> i) const
	{
		return loc->index >= i.loc->index;
	}

	int operator-(index_list_const_iterator<value_type> i) const
	{
		return loc->index - i.loc->index;
	}

	int operator-(index_list_iterator<value_type> i) const
	{
		return loc->index - i.loc->index;
	}

	core::slice<range<index_list_const_iterator<value_type> > > sub(int length) const
	{
		if (length < 0)
			return range<index_list_const_iterator<value_type> >(*this+length, *this);
		else
			return range<index_list_const_iterator<value_type> >(*this, *this+length);
	}

	index_list<value_type> subcpy(int length) const
	{
		if (length < 0)
			return index_list<value_type>(*this+length, *this);
		else
			return index_list<value_type>(*this, *this+length);
	}

	core::slice<range<index_list_const_iterator<value_type> > > sub() const
	{
		return range<index_list_const_iterator<value_type> >(*this, root->end());
	}

	index_list<value_type> subcpy() const
	{
		return index_list<value_type>(*this, root->end());
	}

	index_list_const_iterator<value_type> &operator=(index_list_const_iterator<value_type> i)
	{
		root = i.root;
		loc = i.loc;
		return *this;
	}

	index_list_const_iterator<value_type> &operator=(index_list_iterator<value_type> i)
	{
		root = i.root;
		loc = i.loc;
		return *this;
	}
};

template <class value_type>
struct index_list : container<value_type, index_list_iterator<value_type>, index_list_const_iterator<value_type> >
{
	typedef container<value_type, index_list_iterator<value_type>, index_list_const_iterator<value_type> > super;
	using typename super::type;
	using typename super::iterator;
	using typename super::const_iterator;

	friend class index_list_const_iterator<value_type>;
	friend class index_list_iterator<value_type>;

	typedef index_list_end_item end_item;
	typedef index_list_item<value_type> item;

	end_item left;
	end_item right;
	
	index_list()
	{
		left.next = &right;
		right.prev = &left;
		right.index = 0;
	}

	index_list(const value_type &c)
	{
		left.next = &right;
		right.prev = &left;
		right.index = 0;
		end().push(c);
	}

	template <class container>
	index_list(const container &c)
	{
		left.next = &right;
		right.prev = &left;
		right.index = 0;
		for (typename container::const_iterator i = c.begin(); i; i++)
			end().push(*i);
	}

	index_list(const index_list<value_type> &c)
	{
		left.next = &right;
		right.prev = &left;
		this->right.index = 0;
		for (const_iterator i = c.begin(); i; i++)
			end().push(*i);
	}

	// Initialize this index_list as a copy of some other container
	template <class container>
	index_list(typename container::const_iterator left, typename container::const_iterator right)
	{
		this->left.next = &this->right;
		this->right.prev = &this->left;
		this->right.index = 0;
		for (typename container::const_iterator i = left; i != right; i++)
			end().push(*i);
	}

	index_list(const_iterator left, const_iterator right)
	{
		this->left.next = &this->right;
		this->right.prev = &this->left;
		this->right.index = 0;
		for (const_iterator i = left; i != right; i++)
			end().push(*i);
	}

	// Initialize this index_list as a copy of some other container
	template <class container>
	index_list(typename container::iterator left, typename container::iterator right)
	{
		this->left.next = &this->right;
		this->right.prev = &this->left;
		this->right.index = 0;
		for (typename container::iterator i = left; i != right; i++)
			end().push(*i);
	}

	index_list(iterator left, iterator right)
	{
		this->left.next = &this->right;
		this->right.prev = &this->left;
		this->right.index = 0;
		for (iterator i = left; i != right; i++)
			end().push(*i);
	}
	
	virtual ~index_list()
	{
		clear();
	}

	int size() const
	{
		return right.index;
	}

	iterator begin()
	{
		return iterator(this, left.next);
	}

	iterator end()
	{
		return iterator(this, &right);
	}

	iterator rbegin()
	{
		return iterator(this, right.prev);
	}

	iterator rend()
	{
		return iterator(this, &left);
	}

	const_iterator begin() const
	{
		return const_iterator(this, left.next);
	}

	const_iterator end() const
	{
		return const_iterator(this, &right);
	}

	const_iterator rbegin() const
	{
		return const_iterator(this, right.prev);
	}

	const_iterator rend() const
	{
		return const_iterator(this, &left);
	}

	iterator at(int i)
	{
		return i < 0 ? end()+i : begin()+i;
	}

	const_iterator at(int i) const
	{
		return i < 0 ? end()+i : begin()+i;
	}

	value_type &front()
	{
		return *begin();
	}

	const value_type &front() const
	{
		return *begin();
	}

	value_type &back()
	{
		return *rbegin();
	}

	const value_type &back() const
	{
		return *rbegin();
	}

	value_type &get(int i)
	{
		return i < 0 ? (end()+i).get() : (begin()+i).get();
	}

	const value_type &get(int i) const
	{
		return i < 0 ? (end()+i).get() : (begin()+i).get();
	}

	value_type *ptr(int i)
	{
		return i < 0 ? (end()+i).ptr() : (begin()+i).ptr();
	}

	const value_type *ptr(int i) const
	{
		return i < 0 ? (end()+i).ptr() : (begin()+i).ptr();
	}

	value_type &operator[](int i)
	{
		return i < 0 ? (end()+i).get() : (begin()+i).get();
	}

	const value_type &operator[](int i) const
	{
		return i < 0 ? (end()+i).get() : (begin()+i).get();
	}

	core::slice<index_list<value_type> > deref()
	{
		return *this;
	}

	index_list<int> idx()
	{
		index_list<int> result;
		for (iterator i = begin(); i != end(); i++)
			result.push_back(i->idx());
		return result;
	}

	core::slice<range<iterator> > sub(int start, int end)
	{
		return range<iterator>(at(start), at(end));
	}

	core::slice<range<iterator> > sub(int start)
	{
		return range<iterator>(at(start), this->end());
	}

	core::slice<range<iterator> > sub()
	{
		return range<iterator>(begin(), end());
	}

	core::slice<range<const_iterator> > sub(int start, int end) const
	{
		return range<const_iterator>(at(start), at(end));
	}

	core::slice<range<const_iterator> > sub(int start) const
	{
		return range<const_iterator>(at(start), this->end());
	}

	core::slice<range<const_iterator> > sub() const
	{
		return range<const_iterator>(begin(), end());
	}

	index_list<value_type> subcpy(int start, int end) const
	{
		return index_list<value_type>(at(start), at(end));
	}

	index_list<value_type> subcpy(int start) const
	{
		return index_list<value_type>(at(start), this->end());
	}

	index_list<value_type> subcpy() const
	{
		return *this;
	}

	template <class container>
	index_list<typename container::iterator> sample(container &c)
	{
		index_list<typename container::iterator> result;
		for (iterator i = begin(); i != end(); i++)
			result.push_back(c.at(*i));
		return result;
	}

	template <class container>
	index_list<typename container::const_iterator> sample(const container &c)
	{
		index_list<typename container::const_iterator> result;
		for (iterator i = begin(); i != end(); i++)
			result.push_back(c.at(*i));
		return result;
	}

	static void drop(iterator start, iterator end)
	{
		index_list<value_type> result;
		result.left.next = start.loc;
		result.right.prev = end.loc->prev;
		start.loc->prev->next = end.loc;
		end.loc->prev = start.loc->prev;
		result.left.next->prev = &result.left;
		result.right.prev->next = &result.right;
		result.release();
		end.root->update_index(end.loc->prev);
	}

	index_list<value_type> drop(int start, int end)
	{
		iterator l = start < 0 ? this->end()+start : this->begin()+start;
		iterator r = end < 0 ? this->end()+end : this->begin()+end;
		return pop(l, r);
	}

	void drop_front(unsigned int n = 1)
	{
		drop(begin(), begin()+(int)n);
	}

	void drop_back(unsigned int n = 1)
	{
		drop(end()-(int)n, end());
	}

	static index_list<value_type> pop(iterator start, iterator end)
	{
		index_list<value_type> result;
		result.left.next = start.loc;
		result.right.prev = end.loc->prev;
		start.loc->prev->next = end.loc;
		end.loc->prev = start.loc->prev;
		result.left.next->prev = &result.left;
		result.right.prev->next = &result.right;
		result.update_index(&result.left);
		end.root->update_index(end.loc->prev);
		return result;
	}

	index_list<value_type> pop(int start, int end)
	{
		iterator l = start < 0 ? this->end()+start : this->begin()+start;
		iterator r = end < 0 ? this->end()+end : this->begin()+end;
		return pop(l, r);
	}

	index_list<value_type> pop_back(unsigned int n = 1)
	{
		return pop(end()-(int)n, end());
	}

	index_list<value_type> pop_front(unsigned int n = 1)
	{
		return pop(begin(), begin()+(int)n);
	}

	void push_back(const value_type &value)
	{
		end().push(value);
	}
	
	void push_front(const value_type &value)
	{
		begin().push(value);
	}

	template <class container>
	void append_back(const container &c)
	{
		end().append(c);
	}

	template <class container>
	void append_front(const container &c)
	{
		begin().append(c);
	}

	static void replace(iterator start, iterator end, value_type v)
	{
		if (start != end)
		{
			start.get() = v;
			start++;
			drop(start, end);
		}
		else
			start.push(v);
	}

	void replace(int start, int end, value_type v)
	{
		replace(at(start), at(end), v);
	}

	template <class container>
	static void replace(iterator start, iterator end, const container &c)
	{
		typename container::const_iterator i = c.begin();
		while (start != end && i)
		{
			start.get() = *i;
			start++;
			i++;
		}

		if (start != end)
			drop(start, end);
		else if (i)
			start.append(i.sub());
	}

	template <class container>
	void replace(int start, int end, const container &c)
	{
		replace(at(start), at(end), c);
	}

	void replace_back(int n, const value_type &v)
	{
		replace(at(-n), end(), v);
	}

	template <class container>
	void replace_back(int n, const container &c)
	{
		replace(at(-n), end(), c);
	}

	void replace_front(int n, const value_type &v)
	{
		replace(begin(), at(n), v);
	}

	template <class container>
	void replace_front(int n, const container &c)
	{
		replace(begin(), at(n), c);
	}

	void swap(index_list<value_type> &lst)
	{
		end_item* tmp_left = left.next;
		end_item* tmp_right = right.prev;

		left.next = lst.left.next;
		left.prev = &left;
		left.next->prev = &left;
		right.prev = lst.right.prev;
		right.next = &right;
		right.prev->next = &right;

		lst.left.next = tmp_left;
		lst.left.prev = &lst.left;
		lst.left.next->prev = &lst.left;
		lst.right.prev = tmp_right;
		lst.right.next = &lst.right;
		lst.right.prev->next = &lst.right;
	}

	void resize(int n, const value_type &v = value_type())
	{
		iterator i = begin();
		while (i != end() && n > 0)
		{
			i++;
			n--;
		}

		if (i != end())
			drop(i, end());

		while (n > 0)
		{
			push_back(v);
			n--;
		}
	}

	void clear()
	{
		end_item *curr = left.next, *prev;
		while (curr != &right)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
		}
		left.next = &right;
		right.prev = &left;
		right.index = 0;
	}

	void release()
	{
		end_item *curr = left.next, *prev;
		while (curr != &right)
		{
			prev = curr;
			curr = curr->next;
			delete prev;
		}
		left.next = &right;
		right.prev = &left;
		right.index = 0;
	}

	template <class container>
	index_list<value_type> &operator=(const container &c)
	{
		clear();
		append_back(c);
		return *this;
	}

	index_list<value_type> &operator=(const index_list<value_type> &c)
	{
		clear();
		append_back(c);
		return *this;
	}

	void update_index(end_item *loc)
	{
		for (; loc != &right; loc = loc->next)
			loc->next->index = loc->index + 1;
	}
};

template <class value_type>
index_list<value_type> &operator<<(index_list<value_type> &os, const value_type &v)
{
	os.push_back(v);
	return os;
}

template <class value_type, class container>
index_list<value_type> &operator<<(index_list<value_type> &os, const container &c)
{
	os.append_back(c);
	return os;
}

template <class value_type>
index_list<value_type> operator+(index_list<value_type> os, const value_type &v)
{
	os.push_back(v);
	return os;
}

template <class value_type, class container>
index_list<value_type> operator+(index_list<value_type> os, const container &c)
{
	os.append_back(c);
	return os;
}

template <typename value_type>
index_list<value_type> index_list_t(int n, ...)
{
	index_list<value_type> result;
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
		result.push_back(va_arg(args, value_type));
	va_end(args);

	return result;
}

}

