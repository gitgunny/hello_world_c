#include <stdio.h>
#include <stdlib.h> // malloc, free

// 양방향 노드 구조체 정의
typedef struct _Node
{
    struct _Node *pPrevNode; // 이전 노드 주소
    int data;                // 노드 데이터
    struct _Node *pNextNode; // 다음 노드 주소
} Node;

// 이중 연결 리스트 구조체 정의(테일 불포함)
typedef struct _DoublyLinkedList
{
    Node *pHeadNode; // 헤드 노드 주소
} DoublyLinkedList;

void InitList(DoublyLinkedList *pList);                        // 리스트 초기화
void CreateHeadNode(DoublyLinkedList *pList, const int *data); // 리스트 헤드 노드
void PushFrontData(DoublyLinkedList *pList, const int *data);  // 데이터 전단 삽입
void PushBackData(DoublyLinkedList *pList, const int *data);   // 데이터 후단 삽입
void DeleteData(DoublyLinkedList *pList, const int *data);     // 데이터 삭제
void ReleaseList(DoublyLinkedList *pList);                     // 리스트 해제

int main()
{
    DoublyLinkedList d_linked_list;
    int select_menu, input_data;

    InitList(&d_linked_list); // 리스트 초기화

    while (1)
    {
        if (d_linked_list.pHeadNode != NULL) // 리스트에 데이터가 있다면 데이터 출력
        {
            Node *pNode = d_linked_list.pHeadNode;
            while (pNode != NULL)
            {
                printf("%d\n", pNode->data);
                pNode = pNode->pNextNode;
            }
        }

        // 데이터 삽입 및 삭제 선택
        printf("1. Front Input 2. Back Input 3. Delete (Other Exit) : ");
        scanf("%d", &select_menu);

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
            PushFrontData(&d_linked_list, &input_data); // 데이터 전단 삽입
        else if (select_menu == 2)
            PushBackData(&d_linked_list, &input_data); // 데이터 후단 삽입
        else if (select_menu == 3)
            DeleteData(&d_linked_list, &input_data); // 데이터 삭제
    }

    ReleaseList(&d_linked_list); // 리스트 해제
    return 0;
}

void InitList(DoublyLinkedList *pList)
{
    pList->pHeadNode = NULL;
}

void CreateHeadNode(DoublyLinkedList *pList, const int *data)
{
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->pPrevNode = NULL;
    pNode->data = *data;
    pNode->pNextNode = NULL;
    pList->pHeadNode = pNode;
}

void PushFrontData(DoublyLinkedList *pList, const int *data) // 리스트에 헤드 노드가 있을 때 편리함
{
    if (pList->pHeadNode == NULL)
    {
        CreateHeadNode(pList, data);
        return;
    }
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->pPrevNode = NULL;
    pNode->data = *data;
    pNode->pNextNode = pList->pHeadNode;
    pList->pHeadNode = pNode;
}

void PushBackData(DoublyLinkedList *pList, const int *data) // 리스트에 테일 노드가 있을 때 편리함
{
    if (pList->pHeadNode == NULL)
    {
        CreateHeadNode(pList, data);
        return;
    }
    Node *pNode = pList->pHeadNode;
    Node *pTmpNode = NULL;
    while (pNode != NULL)
    {
        pTmpNode = pNode;
        pNode = pNode->pNextNode;
    }
    pNode = (Node *)malloc(sizeof(Node));
    pTmpNode->pNextNode = pNode;
    pNode->pPrevNode = pTmpNode;
    pNode->data = *data;
    pNode->pNextNode = NULL;
}

void DeleteData(DoublyLinkedList *pList, const int *data)
{
    Node *pNode = pList->pHeadNode;
    while (pNode != NULL)
    {
        if (pNode->data == *data)
        {
            if (pNode == pList->pHeadNode)
            {
                if (pNode->pNextNode != NULL)
                    pNode->pNextNode->pPrevNode = NULL;
                pList->pHeadNode = pNode->pNextNode;
                free(pNode);
                break;
            }
            pNode->pPrevNode->pNextNode = pNode->pNextNode;
            free(pNode);
            break;
        }
        pNode = pNode->pNextNode;
    }
}

void ReleaseList(DoublyLinkedList *pList)
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