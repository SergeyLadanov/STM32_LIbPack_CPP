/* Перегрузка операторов работы с памятью */

void* operator new(std::size_t size)
{
    return malloc(size);
}

void operator delete(void* ptr)
{
    free(ptr);
}

void* operator new[](std::size_t size)
{
    return malloc(size);
}

void operator delete[](void* ptr)
{
    free(ptr);
}
