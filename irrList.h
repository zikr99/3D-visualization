#ifndef __IRRLIST_H__
#define __IRRLIST_H__

template <class T>
class list
{
private:
	struct listnode
	{
		listnode(): next(0), prev(0) {};
		listnode* next;
		listnode* prev;
		T element;
	};
	
	listnode* root;
    listnode* last;
	int size;

public:
	class Iterator
	{
    private:
        Iterator(listnode* begin) : current(begin) {};
		friend class list<T>;
		listnode* current;   
         
	public:
		Iterator(): current(0) {};
	
		Iterator& operator ++() { current = current->next; return *this; };
		Iterator& operator --() { current = current->prev; return *this; };
		Iterator operator ++(int) { Iterator tmp = *this; current = current->next; return tmp; };
  		Iterator operator --(int) { Iterator tmp = *this; current = current->prev; return tmp; };

		Iterator operator + (int num) const 
		{ 
			Iterator tmp = *this; 

			if (num >= 0) 
				while (num-- && tmp.current != 0) ++tmp; 
			else 
				while (num++ && tmp.current != 0) --tmp; 

			return tmp; 
		} 

		Iterator& operator += (int num) 
		{ 
			if (num >= 0) 
				while (num-- && this->current != 0) ++(*this); 
			else 
				while (num++ && this->current != 0) --(*this); 

			return *this; 
		} 

		Iterator operator - (int num) const  { return (*this) + (-num); } 
		Iterator operator -= (int num) const { (*this) += (-num); return *this; }

		bool operator == (const Iterator& other) const { return current == other.current; };
		bool operator != (const Iterator& other) const { return current != other.current; };

		T& operator *() { return current->element; };
	};

	list(): root(0), last(0), size(0) {}
	~list() { clear(); }
	int getSize() const { return size; }

	void clear()
	{
		listnode* node = root;
		while(node)
		{
			listnode* next = node->next;
			delete node;
			node = next;
		}

		root = 0;
		last = 0;
		size = 0;
	}

	bool empty() const { return root == 0; }

	void push_back(const T& element)
	{
		listnode* node = new listnode;
		node->element = element;

		++size;

		if (root == 0)
			root = node;

		node->prev = last;

		if (last != 0)
			last->next = node;

		last = node;
	}

	void push_front(const T& element)
	{
		listnode* node = new listnode;
		node->element = element;

		++size;

		if (root == 0)
		{
			last = node;
			root = node;
		}
		else
		{
			node->next = root;
			root->prev = node;
			root = node;
		}
	}

	Iterator begin() const { return Iterator(root); }
	Iterator end() const { return Iterator(0); }
	Iterator getLast() const { return Iterator(last); }

	void insert_after(Iterator& it, const T& element)
	{
		listnode* node = new listnode;
		node->element = element;

		node->next = it.current->next;

		if (it.current->next)
			it.current->next->prev = node;

		node->prev = it.current;
		it.current->next = node;
		++size;

		if (it.current == last) 
			last = node;
	}

	void insert_before(Iterator& it, const T& element)
	{
		listnode* node = new listnode;
		node->element = element;

		node->prev = it.current->prev;

		if (it.current->prev)
			it.current->prev->next = node;

		node->next = it.current;
		it.current->prev = node;
		++size;

		if (it.current == root) 
             root = node;
	}

	Iterator erase(Iterator& it)
	{
		Iterator returnIterator(it);
		++returnIterator;

		if (it.current == root)
			root = it.current->next;

		if (it.current == last)
			last = it.current->prev;

		if (it.current->next)
			it.current->next->prev = it.current->prev;

		if (it.current->prev)
			it.current->prev->next = it.current->next;

		delete it.current;
		it.current = 0;
		--size;

		return returnIterator;
	}
};

#endif

