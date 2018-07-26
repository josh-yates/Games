//class header for 2d array TYPE
#ifndef ARRAY2D_INCLUDED
#define ARRAY2D_INCLUDED

template <class TYPE> class array2D {
private:
	int m_rows, n_cols;
	TYPE *array_contents{nullptr};
public:
	//CONSTRUCTORS/DESTRUCTORS
	//default
	array2D():m_rows(0),n_cols(0){}
	//parametised
	array2D(const int m_in, const int n_in, const TYPE starting_val) {
		if (m_in < 1 || n_in < 1) {
			throw std::invalid_argument("array2D constructor: Invalid size");
		}
		//if passed range check, set all elements to 0
		m_rows = m_in;
		n_cols = n_in;
		array_contents = new TYPE[m_rows*n_cols];
		for (int el{ 0 }; el < m_rows*n_cols; el++) {
			array_contents[el] = starting_val;
		}
	}
	//copy/move
	array2D(const array2D<TYPE>& arr_in) {
		m_rows = arr_in.m_rows;
		n_cols = arr_in.n_cols;
		//check if array was undefined
		if (m_rows == 0 || n_cols == 0) {
			array_contents = nullptr;
		}
		else {
			//copy array contents
			array_contents = new TYPE[m_rows*n_cols];
			for (int el{ 0 }; el < m_rows*n_cols; el++) {
				array_contents[el] = arr_in.array_contents[el];
			}
		}
	}
	array2D(array2D<TYPE>&& arr_in) {
		m_rows = arr_in.m_rows;
		n_cols = arr_in.n_cols;
		//shallow copy and repoint arr_in's pointer
		array_contents = arr_in.array_contents;
		arr_in.m_rows = 0;
		arr_in.n_cols = 0;
		arr_in.array_contents = nullptr;
	}
	//destructor
	~array2D() {
		//clear and free memory
		delete[] array_contents;
		array_contents = nullptr;
	}
	//GETTERS
	//get array dimensions
	int get_m_rows()const;
	int get_n_cols()const;
	//get array element using (i,j)
	TYPE operator()(const int, const int)const;
	//SETTERS
	//set array element to TYPE using i,j
	void set_element(const int, const int, TYPE);
	//ASSIGNMENT OPERATORS
	array2D<TYPE>& operator=(const array2D<TYPE>&);	//copy
	array2D<TYPE>& operator=(array2D<TYPE>&&);		//move
};

//FUNCTION DEFINITIONS

//GETTERS
//get array dimensions
template<class TYPE> int array2D<TYPE>::get_m_rows()const {
	return m_rows;
}
template<class TYPE> int array2D<TYPE>::get_n_cols()const {
	return n_cols;
}
//get array element using (i,j)
template<class TYPE> TYPE array2D<TYPE>::operator()(const int i, const int j)const {
	if (i < 0 || i >= m_rows || j < 0 || j >= n_cols) {
		throw std::invalid_argument("array2D operator (): element out of array range");
	}
	//ith row down, jth column across
	return array_contents[(i*n_cols) + j];
}
//SETTERS
//set array element to TYPE using i,j
template<class TYPE> void array2D<TYPE>::set_element(const int i, const int j, TYPE val) {
	if (i < 0 || i >= m_rows || j < 0 || j >= n_cols) {
		throw std::invalid_argument("array2D set_element: element out of array range");
	}
	array_contents[(i*n_cols) + j] = val;
}
//ASSIGNMENT OPERATORS
template<class TYPE> array2D<TYPE>& array2D<TYPE>::operator=(const array2D<TYPE>& arr_in) {
	//handle self-assignment
	if (&arr_in == this) {
		return *this;
	}
	else {
		//clear current contents
		m_rows = 0;
		n_cols = 0;
		delete[] array_contents;
		array_contents = nullptr;
		//check if array was undefined
		m_rows = arr_in.m_rows;
		n_cols = arr_in.n_cols;
		if (m_rows == 0 || n_cols == 0) {
			array_contents = nullptr;
		}
		else {
			//deep copy arr_in's contents
			array_contents = new TYPE[m_rows*n_cols];
			for (int el{ 0 }; el < m_rows*n_cols; el++) {
				array_contents[el] = arr_in.array_contents[el];
			}
		}
		return *this;
	}
}
template<class TYPE> array2D<TYPE>& array2D<TYPE>::operator=(array2D<TYPE>&& arr_in) {
	//directly swap all RH(in) values with LH(out) values
	std::swap(m_rows, arr_in.m_rows);
	std::swap(n_cols, arr_in.n_cols);
	std::swap(array_contents, arr_in.array_contents);
	return *this;
}
#endif