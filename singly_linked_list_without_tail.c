#include <stdio.h>
#include <stdlib.h> // malloc, free

// 단방향 노드 구조체 정의
typedef struct _Node
{
    int data;                // 노드 데이터
    struct _Node *pNextNode; // 다음 노드 주소
} Node;

// 단순 연결 리스트 구조체 정의
typedef struct _SinglyLinkedList
{
    Node *pHeadNode; // 헤드 노드 주소
} SinglyLinkedList;

void InitList(SinglyLinkedList *pList);                      // 리스트 초기화
void PushFrontData(SinglyLinkedList *pList, const int data); // 데이터 전단 삽입
void PushBackData(SinglyLinkedList *pList, const int data);  // 데이터 후단 삽입
void DeleteData(SinglyLinkedList *pList, const int data);    // 데이터 삭제
void ReleaseList(SinglyLinkedList *pList);                   // 리스트 해제

int main()
{
    SinglyLinkedList s_linked_list;
    int select_menu, input_data;

    InitList(&s_linked_list); // 리스트 초기화

    while (1)
    {
        if (s_linked_list.pHeadNode != NULL) // 리스트에 데이터가 있다면 데이터 출력
        {
            Node *pNode = s_linked_list.pHeadNode;
            while (pNode != NULL)
            {
                printf("%d\n", pNode->data);
                pNode = pNode->pNextNode;
            }
        }

        // 데이터 삽입 및 삭제 선택
        printf("1. Front Input 2. Back Input 3. Delete (Other Exit) : ");
        scanf("%d", &select_menu);

        // 삽입 및 삭제 데이터 입력
        if (select_menu == 1)
            printf("Front Input Data : ");
        else if (select_menu == 2)
            printf("Back Input Data : ");
        else if (select_menu == 3)
            printf("Delete Data : ");
        else
            break;
        scanf("%d", &input_data);

        if (select_menu == 1)
            PushFrontData(&s_linked_list, input_data); // 데이터 전단 삽입
        else if (select_menu == 2)
            PushBackData(&s_linked_list, input_data); // 데이터 후단 삽입
        else if (select_menu == 3)
            DeleteData(&s_linked_list, input_data); // 데이터 삭제
    }

    ReleaseList(&s_linked_list); // 리스트 해제
    return 0;
}

void InitList(SinglyLinkedList *pList)
{
    pList->pHeadNode = NULL;
}

void PushFrontData(SinglyLinkedList *pList, const int data)
{
    if (pList->pHeadNode == NULL)
    {
        pList->pHeadNode = (Node *)malloc(sizeof(Node));
        pList->pHeadNode->data = data;
        pList->pHeadNode->pNextNode = NULL;
    }
    else
    {
        Node *pNode = pList->pHeadNode;
        Node *pTmpNode = (Node *)malloc(sizeof(Node));
        pTmpNode->data = data;
        pTmpNode->pNextNode = pNode;
        pList->pHeadNode = pTmpNode;
    }
}

void PushBackData(SinglyLinkedList *pList, const int data)
{
    if (pList->pHeadNode == NULL)
    {
        pList->pHeadNode = (Node *)malloc(sizeof(Node));
        pList->pHeadNode->data = data;
        pList->pHeadNode->pNextNode = NULL;
    }
    else
    {
        Node *pNode = pList->pHeadNode;
        Node *pTmpNode = NULL;
        while (pNode != NULL)
        {
            pTmpNode = pNode;
            pNode = pNode->pNextNode;
        }
        pNode = (Node *)malloc(sizeof(Node));
        pTmpNode->pNextNode = pNode;
        pNode->data = data;
        pNode->pNextNode = NULL;
    }
}

void DeleteData(SinglyLinkedList *pList, const int data)
{
    Node *pNode = pList->pHeadNode;
    Node *pTmpNode = NULL;
    while (pNode != NULL)
    {
        if (pNode->data == data)
        {
            if (pNode == pList->pHeadNode)
            {
                pTmpNode = pNode->pNextNode;
                free(pNode);
                pList->pHeadNode = pTmpNode;
                break;
            }
            pTmpNode->pNextNode = pNode->pNextNode;
            free(pNode);
            break;
        }
        pTmpNode = pNode;
        pNode = pNode->pNextNode;
    }
}

void ReleaseList(SinglyLinkedList *pList)
{
    if (pList->pHeadNode != NULL)
    {
        Node *pNode = pList->pHeadNode;
        Node *pTmpNode = NULL;
        while (pNode != NULL)
        {
            pTmpNode = pNode->pNextNode;
            free(pNode);
            pNode = pTmpNode;
        }
    }
}