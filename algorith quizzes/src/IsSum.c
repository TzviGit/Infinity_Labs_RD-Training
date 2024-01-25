
int *IsSum(int *arr ,int size, int *ret)
{
	int i = 0, j = size -1, sum = 0;
	
	while (j > i)
	{
		if (sum == arr[i] + arr[j])
		{
			*ret = 1; 
			*(ret + 1) = i;
			*(ret +2) = j;
			return ret;
		}			
		else if (sum > arr[i] + arr[j])
		{
			++i;
		}
		else 
		{
			--j;
		}
	}
	
	*ret = 0; 
	*(ret + 1) = 0;
	*(ret +2) = 0;
	
	return ret;
}
	
		
		
