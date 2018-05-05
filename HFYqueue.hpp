#ifndef __HFY_QUEUE_HPP__
#define __HFY_QUEUE_HPP__

typedef unsigned int uint;

template <typename T>
class HFYqueue
{
public:
	HFYqueue();
	HFYqueue(uint capacity);
	explicit HFYqueue(const HFYqueue & h);			//��������
	~HFYqueue();
public:
	//��β����һ��Ԫ��
	void push(T data);
	//ɾ�������ض�ͷԪ��
	T pop();
	//�ж϶����Ƿ�Ϊ��,Ϊ���򷵻�0
	bool empty();
	//���ض���Ԫ�ظ���
	uint size();
	//��ն���
	void clear();
private:
	T * data = NULL;
	uint m_size = 0;
	uint capacity = 35;//Ĭ������35
};
template<typename T>
HFYqueue<T>::HFYqueue()
{
	this->data = new T[35];
	this->m_size = 0;
	this->capacity = 35;//Ĭ������35
}
template<typename T>
HFYqueue<T>::HFYqueue(uint capacity)
{
	this->capacity = capacity;
	this->data = new T[this->capacity];
	this->m_size = 0;
}
template<typename T>
HFYqueue<T>::HFYqueue(const HFYqueue & h)
{
	this->m_size = h.m_size;
	this->capacity = h.capacity;
	T *temp = new T[this->capacity];
	memset(temp, 0, this->capacity);
	for (int i = 0; i < this->m_size; i++)
	{
		temp[i] = h.data[i];
	}
	if (this->data != NULL)
	{
		delete[] this->data;
	}
	this->data = temp;
}
template<typename T>
HFYqueue<T>::~HFYqueue()
{
	this->capacity = 0;
	this->m_size = 0;
	if (this->data != NULL)
	{
		delete[] this->data;
		this->data = NULL;
	}
	//cout << "����" << endl;
}
template<typename T>
void HFYqueue<T>::push(T data)
{
// 	if (this->m_size == this->capacity)
// 	{
// 		//cout << "err!! m_size will biger than capacity!" << endl;
// 		return;
// 	}
	this->data[this->m_size] = data;
	++m_size;
}
template<typename T>
T HFYqueue<T>::pop()
{

	 T temp = this->data[0];
	for (int i = 0; i < this->m_size; ++i)
	{
		this->data[i] = this->data[i + 1];
	}
	--m_size;
	return temp;
}

template<typename T>
bool HFYqueue<T>::empty()
{
	if (this->m_size == 0)
	{
		return true;
	}
	return false;
}

template<typename T>
uint HFYqueue<T>::size()
{
	return this->m_size;
}
template<typename T>
void HFYqueue<T>::clear()
{
	for (int i = 0; i < this->m_size; i++)
	{
		this->data[i] = 0;
	}
	this->m_size = 0;
}
#endif // !__HFY_QUEUE_HPP__
