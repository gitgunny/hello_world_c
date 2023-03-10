#include <stdio.h>
#include <stdlib.h> // malloc, free

// 양방향 노드 구조체 정의
typedef struct _Node
{
    struct _Node *pPrevNode; // 이전 노드 주소
    int data;                // 노드 데이터
    struct _Node *pNextNode; // 다음 노드 주소
} Node;

// 이중 연결 리스트 구조체 정의(테일 포함)
typedef struct _DoublyLinkedList
{
    Node *pHeadNode; // 헤드 노드 주소
    Node *pTailNode; // 테일 노드 주소
} DoublyLinkedList;

void InitList(DoublyLinkedList *pList);                         // 리스트 초기화
void PrintList(DoublyLinkedList *pList);                        // 리스트 출력
void CreateFirstNode(DoublyLinkedList *pList, const int *data); // 첫번째 노드 생성
void PushFrontData(DoublyLinkedList *pList, const int *data);   // 데이터 전단 삽입
void PushBackData(DoublyLinkedList *pList, const int *data);    // 데이터 후단 삽입
void DeleteData(DoublyLinkedList *pList, const int *data);      // 데이터 삭제
void ReleaseList(DoublyLinkedList *pList);                      // 리스트 해제

int main()
{
    DoublyLinkedList d_linked_list; // 리스트 선언
    int select_menu, input_data;    // 삽입 및 삭제 선택 변수, 데이터

    InitList(&d_linked_list); // 리스트 초기화
    while (1)
    {
        if (d_linked_list.pHeadNode != NULL) // 리스트에 데이터가 있다면 출력
            PrintList(&d_linked_list);

        // 데이터 삽입 및 삭제 선택
        printf("1. Front Input 2. Back Input 3. Delete (Other Exit) : ");
        scanf("%d", &select_menu);

        // 삽입 및 삭제 데이터 값 입력
        if (select_menu == 1)
            printf("Front Input Data : ");
        else if (select_menu == 2)
            printf("Back Input Data : ");
        else if (select_menu == 3)
            printf("Delete Data : ");
        else
            break;
        scanf("%d", &input_data);

        // 데이터 삽입 및 삭제
        if (select_menu == 1)
            PushFrontData(&d_linked_list, &input_data);
        else if (select_menu == 2)
            PushBackData(&d_linked_list, &input_data);
        else if (select_menu == 3)
            DeleteData(&d_linked_list, &input_data);
    }

    ReleaseList(&d_linked_list); // 리스트 해제
    return 0;
}

void InitList(DoublyLinkedList *pList)
{
    pList->pHeadNode = NULL;
    pList->pTailNode = NULL;
}

void PrintList(DoublyLinkedList *pList)
{
    Node *pNode = pList->pHeadNode;
    while (pNode != NULL)
    {
        printf("%d", pNode->data);
        if (pNode == pList->pHeadNode || pNode == pList->pTailNode)
        {
            printf(" <- ");
            if (pNode == pList->pHeadNode)
                printf("Head ");
            if (pNode == pList->pTailNode)
                printf("Tail ");
        }
        printf("\n");
        pNode = pNode->pNextNode;
    }
}

void CreateFirstNode(DoublyLinkedList *pList, const int *data)
{
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->pPrevNode = NULL;
    pNode->data = *data;
    pNode->pNextNode = NULL;
    pList->pHeadNode = pNode;
    pList->pTailNode = pNode;
}

void PushFrontData(DoublyLinkedList *pList, const int *data)
{
    if (pList->pHeadNode == NULL)
    {
        CreateFirstNode(pList, data);
        return;
    }
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->pPrevNode = NULL;
    pNode->data = *data;
    pNode->pNextNode = pList->pHeadNode;
    pNode->pNextNode->pPrevNode = pNode;
    pList->pHeadNode = pNode;
}

void PushBackData(DoublyLinkedList *pList, const int *data)
{
    if (pList->pHeadNode == NULL)
    {
        CreateFirstNode(pList, data);
        return;
    }
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->pPrevNode = pList->pTailNode;
    pNode->data = *data;
    pNode->pNextNode = NULL;
    pNode->pPrevNode->pNextNode = pNode;
    pList->pTailNode = pNode;
}

void DeleteData(DoublyLinkedList *pList, const int *data)
{
    // 노드 시작점 설정
    Node *pNode = pList->pHeadNode;
    // 리스트 끝까지
    while (pNode != NULL)
    {
        // 삭제할 데이터를 찾았다면
        if (pNode->data == *data)
        {
            // 헤드에 삭제할 데이터가 있다면
            if (pNode == pList->pHeadNode)
            {
                // 헤드 다음 노드가 없다면(헤드 테일 같은 경우)
                if (pNode->pNextNode == NULL)
                {
                    pList->pHeadNode = NULL;
                    pList->pTailNode = NULL;
                }
                // 헤드 다음 노드가 있다면
                else
                {
                    pNode->pNextNode->pPrevNode = NULL;
                    pList->pHeadNode = pNode->pNextNode;
                }
            }
            // 테일에 삭제할 데이터가 있다면
            else if (pNode == pList->pTailNode)
            {
                pNode->pPrevNode->pNextNode = NULL;
                pList->pTailNode = pNode->pPrevNode;
            }
            // 헤드와 테일 아닌 중간에 삭제할 데이터가 있다면
            else
            {
                pNode->pNextNode->pPrevNode = pNode->pPrevNode;
                pNode->pPrevNode->pNextNode = pNode->pNextNode;
            }
            free(pNode); // 삭제할 데이터 동적 할당 해제
            break;       // 삭제할 데이터를 찾고 삭제 했으니 반복문 탈출
        }
        pNode = pNode->pNextNode; // 삭제할 데이터를 찾기 위해 노드 이동
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