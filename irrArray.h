#ifndef __IRRARRAY_H__
#define __IRRARRAY_H__

template <class T>
class array
{
private:
	T* data;
	unsigned int allocated;
	unsigned int used;
	bool free_when_destroyed;
	bool is_sorted;

public:
	array(): data(0), used(0), allocated(0), free_when_destroyed(true), 
         is_sorted(false)
	{
	}

	array(unsigned int start_count): data(0), used(0), allocated(0),
		 free_when_destroyed(true),	is_sorted(false)
	{
		reallocate(start_count);
	}

	//! Copy constructor
	array(const array<T>& other): data(0)
	{
		*this = other;
	}
	
	~array()
	{
		if (free_when_destroyed)
			delete [] data;
	}
	
	void reallocate(unsigned int new_size)
	{
		T* old_data = data;

		data = new T[new_size];
		allocated = new_size;
		
		int end = used < new_size ? used : new_size;
		for (int i = 0; i < end; ++i)
			data[i] = old_data[i];

		if (allocated < used)
			used = allocated;
		
		delete [] old_data;
	}

	//! If the array is to small to add this new element, the array is made bigger.
	void push_back(const T& element)
	{
		if (used + 1 > allocated)
		{
			// reallocate(used * 2 +1);
			// this doesn't work if the element is in the same array. So
			// we'll copy the element first to be sure we'll get no data
			// corruption

			T e;
			e = element;           // copy element
			reallocate(used*2 + 1); // increase data block
			data[used++] = e;        // push_back
			is_sorted = false; 
			return;
		}

		data[used++] = element;
		is_sorted = false;
	}

	//! f the array is to small to add this new element, the array is made bigger. 
    //! Note that this is slow, because the whole array needs to be copied for this.
	void push_front(const T& element)
	{
		if (used + 1 > allocated)
			reallocate(used * 2 +1);

		for (int i = (int)used; i > 0; --i)
			data[i] = data[i - 1];

		data[0] = element;
		is_sorted = false;
		++used;
	}

	//! Use this only if you know what you are doing (possible performance loss). 
	//! The preferred method of adding elements should be push_back().
	void insert(const T& element, unsigned int index = 0) 
	{
		if (used + 1 > allocated)
			reallocate(used * 2 +1);

		for (unsigned int i = used++; i > index; i--) 
			data[i] = data[i - 1];

		data[index] = element;
		is_sorted = false;
	}

	void clear()
	{
		delete [] data;
		data = 0;
		used = 0;
		allocated = 0;
		is_sorted = true;
	}

	//! Sets pointer to new array, using this as new workspace.
	void set_pointer(T* newPointer, unsigned int size)
	{
		delete [] data;
		data = newPointer;
		allocated = size;
		used = size;
		is_sorted = false;
	}

	void set_free_when_destroyed(bool f)
	{
		free_when_destroyed = f;
	}

	void set_used(unsigned int usedNow)
	{
		if (allocated < usedNow)
			reallocate(usedNow);

		used = usedNow;
	}

	//! Assignement operator
	void operator=(const array<T>& other)
	{
		if (data)
			delete [] data;

		//if (allocated < other.allocated)
		if (other.allocated == 0)
			data = 0;
		else
			data = new T[other.allocated];

		used = other.used;
		free_when_destroyed = other.free_when_destroyed;
		is_sorted = other.is_sorted;
		allocated = other.allocated;

		for (unsigned int i = 0; i < other.used; ++i)
			data[i] = other.data[i];
	}

	//! Direct access operator
	T& operator [](unsigned int index)
	{
		return data[index];
	}

	//! Direct access operator
	const T& operator [](unsigned int index) const
	{
		return data[index];
	}

    //! Gets last frame
	const T& getLast() const
	{
		return data[used - 1];
	}

    //! Gets last frame
	T& getLast()
	{
		return data[used - 1];
	}
    
	//! Returns a pointer to the array.
	T* pointer()
	{
		return data;
	}

	//! Returns a const pointer to the array.
	const T* const_pointer() const
	{
		return data;
	}

	unsigned int size() const
	{
		return used;
	}

	//! Returns amount memory allocated.
	unsigned int allocated_size() const
	{
		return allocated;
	}
	
	//! Returns true if array is empty
	bool empty() const
	{
		return used == 0;
	}

	//void sort()
	//{
		//if (is_sorted || used < 2)
			//return;

		//heapsort(data, used);
		//is_sorted = true;
	//}

	//int binary_search(const T& element)
	//{
		//return binary_search(element, 0, used - 1);
	//}

	//! The array will be sorted before the binary search if it is not already sorted (-1 if not found).
	/*s32 binary_search(const T& element, s32 left, s32 right)
	{
		if (!used)
			return -1;

		sort();

		s32 m;

		do
		{
			m = (left+right)>>1;

			if (element < data[m])
				right = m - 1;
			else
				left = m + 1;

		} while((element < data[m] || data[m] < element) && left<=right);

		// this last line equals to:
		// " while((element != array[m]) && left<=right);"
		// but we only want to use the '<' operator.
		// the same in next line, it is "(element == array[m])"

		if (!(element < data[m]) && !(data[m] < element))
			return m;

		return -1;
	}*/

    //Slow
	int linear_search(T& element)
	{
		for (unsigned int i = 0; i < used; ++i)
			if (!(element < data[i]) && !(data[i] < element))
				return (int)i;

		return -1;
	}

	int linear_reverse_search(T& element)
	{
		for (int i = used - 1; i >= 0; --i)
			if (data[i] == element)
				return (int)i;

		return -1;
	}

	//! May be slow
	void erase(unsigned int index)
	{
		for (unsigned int i = index + 1; i < used; ++i)
			data[i - 1] = data[i];

		--used;
	}

	//! May be slow
	void erase(unsigned int index, int count)
	{
		for (unsigned int i = index + count; i < used; ++i)
			data[i - count] = data[i];

		used -= count;
	}

	//! Sets if the array is sorted
	void set_sorted(bool srt)
	{
		is_sorted = srt;
	}

};

#endif
