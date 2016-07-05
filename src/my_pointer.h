#ifndef MY_POINTER_XXX
#define MY_POINTER_XXX
template <class T1>
class my_pointer_xxx
{
	public:
	my_pointer_xxx()
	{
		pp = new T1;
	}
	
	~my_pointer_xxx()
	{
		if(pp!=NULL)
		{
			delete pp;
		}
		pp = NULL;
	}
	
	T1 * get()
	{
		return pp;
	}
	
	private:
		T1 * pp;
}
#endif