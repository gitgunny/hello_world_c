#include <stdio.h>
#include <stdlib.h> // malloc, free

// 가변 배열 구조체 정의
typedef struct DynamicArray
{
    int *data_array;    // 가변 배열(동적 할당과 해제로 유동적 크기 조절 가능)
    int data_count;     // 가변 배열내 데이터 개수
    int max_count;      // 가변 배열 크기(동적 할당과 해제로 유동적)
} DynaArr;
                                                // 가변 배열
void InitArray(DynaArr *ptr_arr);               // 동적 할당
void ExtendArray(DynaArr *ptr_arr);             // 동적 할당 확장(데이터 삽입시)
void PushData(DynaArr *ptr_arr, int data);      // 데이터 삽입
void ReduceArray(DynaArr *ptr_arr);             // 동적 할당 축소(데이터 삭제시)
void DeleteData(DynaArr *ptr_arr, int data);    // 데이터 삭제
void ReleaseArray(DynaArr *ptr_arr);            // 동적 할당 해제

int main()
{
    DynaArr dyna_arr;   // 가변 배열 선언
    int select_menu, input_data;

    InitArray(&dyna_arr);   // 가변 배열 동적 할당

    while (1)
    {
        if (dyna_arr.data_count > 0)    // 가변 배열내 데이터가 있다면 데이터 출력
        {
            for (int i = 0; i < dyna_arr.data_count; i++)
                printf("%d\n", dyna_arr.data_array[i]);
        }

        printf("1. Input 2. Delete (Other Exit) : ");   // 데이터 삽입 및 삭제 선택
        scanf("%d", &select_menu);

        if (select_menu == 1)
            printf("Input Data : ");
        else if (select_menu == 2)
            printf("Delete Data : ");
        else
            break;
        scanf("%d", &input_data);

        if (select_menu == 1)
            PushData(&dyna_arr, input_data);    // 데이터 삽입
        else if (select_menu == 2)
            DeleteData(&dyna_arr, input_data);  // 데이터 삭제
    }

    ReleaseArray(&dyna_arr);    // 동적 할당 해제
    return 0;
}

void InitArray(DynaArr *ptr_arr)
{
    ptr_arr->data_array = (int *)malloc(sizeof(int) * 1);
    ptr_arr->data_count = 0;
    ptr_arr->max_count = 1;
}

void ExtendArray(DynaArr *ptr_arr)
{
    int *tmp_arr;

    tmp_arr = (int *)malloc(sizeof(int) * (ptr_arr->data_count + 1));

    for (int i = 0; i < ptr_arr->data_count; i++)
        tmp_arr[i] = ptr_arr->data_array[i];

    ReleaseArray(ptr_arr);
    ptr_arr->data_array = tmp_arr;
    ptr_arr->max_count++;
}

void PushData(DynaArr *ptr_arr, int data)
{
    if (ptr_arr->data_count == ptr_arr->max_count)
        ExtendArray(ptr_arr);

    ptr_arr->data_array[ptr_arr->data_count++] = data;
}

void ReduceArray(DynaArr *ptr_arr)
{
    int *tmp_arr, j = 0;

    tmp_arr = (int *)malloc(sizeof(int) * (ptr_arr->data_count - 1));

    for (int i = 0; i < ptr_arr->data_count; i++)
    {
        if (ptr_arr->data_array[i] != 0)
            tmp_arr[j++] = ptr_arr->data_array[i];
    }

    ReleaseArray(ptr_arr);
    ptr_arr->data_array = tmp_arr;
    ptr_arr->max_count--;
}

void DeleteData(DynaArr *ptr_arr, int data)
{
    for (int i = 0; i < ptr_arr->data_count; i++)
    {
        if (ptr_arr->data_array[i] == data)
        {
            ptr_arr->data_array[i] = 0;
            ReduceArray(ptr_arr);
            ptr_arr->data_count--;
            break;
        }
    }
}

void ReleaseArray(DynaArr *ptr_arr)
{
    free(ptr_arr->data_array);
}