void CopyArray(const int *arr, int *copyarr)
{
    while (*arr)
	{
		*copyarr++ = *arr++;
	}
}

int main()
{
  return 0;
}
