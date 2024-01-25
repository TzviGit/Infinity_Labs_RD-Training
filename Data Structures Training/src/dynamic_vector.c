#include <stdlib.h> /*malloc*/
#include <assert.h>/*assert*/
#include "dynamic_vector.h" /*vector funcs*/

#define G_FACTOR (2)

struct vector
{
	void **base_ptr;
	size_t size;
	size_t capacity;
};

/*approved by vadim*/
vector_t *DVectorCreate(size_t capacity)
{
    vector_t *vector_p = (vector_t *)malloc(sizeof(vector_t));
    void ** temp_p = malloc(capacity * sizeof(void *));

	if (!temp_p || !vector_p)
    {
        free(temp_p); temp_p = NULL;
		free(vector_p); vector_p = NULL;
		return NULL;
    }

	vector_p->base_ptr = temp_p;
    vector_p->capacity = capacity;
    vector_p->size = 0;

    return vector_p;
}

void DVectorDestroy(vector_t *vector)
{
    if (vector)
	{
		free(vector->base_ptr);
    	vector->base_ptr = NULL;
    	free(vector);  /*leave making NULL to the user*/
	}
}

int DVectorPushBack(vector_t *vector, void *element)
{
	assert(vector);

    if (vector->size == vector->capacity)
    {
        if (DVectorReserve(vector, (G_FACTOR * vector->capacity)))
        {
            return 1;
        }
    }

    vector->base_ptr[(vector->size)++] = element;
    return 0;
}

void *DVectorPopBack(vector_t *vector)
{
    assert(vector);
    return vector->base_ptr[--(vector->size)];
}

void *DVectorGet(const vector_t *vector, size_t index)
{
    assert(vector && (index < vector->size));
	return vector->base_ptr[index];
}

void DVectorSet(vector_t *vector, size_t index, void *element)
{
    assert(vector && (index < vector->size));
	vector->base_ptr[index] = element;
}

size_t DVectorGetSize(const vector_t *vector)
{
    assert(vector);
	return vector->size;
}

size_t DVectorGetCapacity(const vector_t *vector)
{
    assert(vector);
	return vector->capacity;
}

int DVectorReserve(vector_t *vector, size_t new_capacity)
{
    void **temp_p = NULL;
    assert(vector && new_capacity > 0);

    temp_p = (void **)realloc(vector->base_ptr, (sizeof(void *) * new_capacity));
    if (NULL == temp_p)
    {
        return 1; /*failure*/
    }
    vector->size = (vector->size <=new_capacity)? vector->size : new_capacity;
    vector->capacity = new_capacity;
    vector->base_ptr = temp_p;

    return 0;
}

int DVectorShrinkToFit(vector_t *vector)
{
    assert(vector);
    return DVectorReserve(vector, vector->size);
}
