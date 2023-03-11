#include <stdio.h>
#include <stdlib.h> // malloc, free

// 원형 단방향 노드 구조체 정의
typedef struct _Node
{
    int data;                // 노드 데이터
    struct _Node *pNextNode; // 다음 노드 주소
} Node;

// 원형 연결 리스트 구조체 정의
typedef struct _CircularLinkedList
{
    Node *pHeadNode; // 헤드 노드 주소
} CircularLinkedList;

void InitList(CircularLinkedList *pList);                         // 리스트 초기화
void PrintList(CircularLinkedList *pList);                        // 리스트 출력
void CreateFirstNode(CircularLinkedList *pList, const int *data); // 첫번째 노드 생성
void PushData(CircularLinkedList *pList, const int *data);        // 데이터 삽입
void DeleteData(CircularLinkedList *pList, const int *data);      // 데이터 삭제
void ReleaseList(CircularLinkedList *pList);                      // 리스트 해제

int main()
{
    CircularLinkedList c_linked_list; // 리스트 선언
    int select_menu, input_data;      // 삽입 및 삭제 선택 변수 / 데이터

    InitList(&c_linked_list); // 리스트 초기화

    while (1)
    {
        // 리스트에 데이터가 있다면 출력
        if (c_linked_list.pHeadNode != NULL)
            PrintList(&c_linked_list);

        // 데이터 삽입 및 삭제 선택
        printf("1. Input 2. Delete (Other Exit) : ");
        scanf("%d", &select_menu);

        // 삽입 및 삭제 데이터 값 입력
        if (select_menu == 1)
            printf("Input Data : ");
        else if (select_menu == 2)
            printf("Delete Data : ");
        else // Other
            break;
        scanf("%d", &input_data);

        // 데이터 삽입 및 삭제
        if (select_menu == 1)
            PushData(&c_linked_list, &input_data); // 데이터 삽입을 선택 했다면 데이터 삽입
        else if (select_menu == 2)
            DeleteData(&c_linked_list, &input_data); // 데이터 삭제를 선택 했다면 데이터 삭제
    }

    ReleaseList(&c_linked_list); // 리스트 해제
    return 0;
}

// 리스트 초기화
void InitList(CircularLinkedList *pList)
{
    pList->pHeadNode = NULL;
}

// 리스트 출력
void PrintList(CircularLinkedList *pList)
{
    Node *pNode = pList->pHeadNode;
    do
    {
        printf("%d", pNode->data); // 노드 데이터 출력
        if (pNode == pList->pHeadNode)
            printf(" <- Head"); // 출력한 노드가 헤드라면 출력
        if (pNode->pNextNode == pList->pHeadNode)
            printf(" <- Next Node is Head"); // 출력한 노드 다음 노드가 헤드라면 출력
        printf("\n");
        pNode = pNode->pNextNode;
    } while (pNode != pList->pHeadNode);
}

// 첫번째 노드 생성
void CreateFirstNode(CircularLinkedList *pList, const int *data)
{
    // 헤드 노드 생성
    Node *pNode = (Node *)malloc(sizeof(Node));
    pList->pHeadNode = pNode;
    pNode->data = *data;
    pNode->pNextNode = pList->pHeadNode;
}

// 데이터 삽입
void PushData(CircularLinkedList *pList, const int *data)
{
    if (pList->pHeadNode == NULL) // 헤드 노드가 없다면
    {
        CreateFirstNode(pList, data); // 헤드 노드 생성
        return;
    }

    // 마지막 노드 찾기
    Node *pNode = pList->pHeadNode;
    while (pNode->pNextNode != pList->pHeadNode)
        pNode = pNode->pNextNode;

    // 마지막 노드 다음에 노드 생성
    Node *pNewNode = (Node *)malloc(sizeof(Node));
    pNode->pNextNode = pNewNode;
    pNewNode->data = *data;
    pNewNode->pNextNode = pList->pHeadNode;
}

// 데이터 삭제
void DeleteData(CircularLinkedList *pList, const int *data)
{
    // 삭제할 노드 찾기
    Node *pNode = pList->pHeadNode;
    Node *pTmpNode = NULL;
    while (pNode->data != *data)
    {
        pTmpNode = pNode;
        pNode = pNode->pNextNode;
    }

    // 삭제할 데이터가 헤드 노드에 있다면
    if (pNode == pList->pHeadNode)
    {
        // 헤드 다음 노드가 없다면
        if (pNode->pNextNode == pList->pHeadNode)
        {
            pList->pHeadNode = NULL; // 헤드 노드 없음 처리
        }
        // 헤드 다음 노드가 있다면
        else
        {
            // 마지막 노드 찾기
            pTmpNode = pNode;
            while (pTmpNode->pNextNode != pList->pHeadNode)
                pTmpNode = pTmpNode->pNextNode;
            pList->pHeadNode = pNode->pNextNode;    // 헤드 다음 노드를 헤드 노드로 만들어 주기
            pTmpNode->pNextNode = pList->pHeadNode; // 마지막 노드를 헤드 노드에 이어주기
        }
    }
    // 삭제할 데이터가 다른 노드에 있다면
    else
    {
        pTmpNode->pNextNode = pNode->pNextNode;
    }
    free(pNode);
}

// 리스트 해제
void ReleaseList(CircularLinkedList *pList)
{
    // 동적 할당 해제 할 노드가 없다면
    if (pList->pHeadNode == NULL)
        return; // 바로 종료

    // 동적 할당 해제 할 노드가 헤드 밖에 없다면
    else if (pList->pHeadNode->pNextNode == pList->pHeadNode)
    {
        free(pList->pHeadNode); // 헤드 노드 하나만 동적 할당 해제
        return;
    }

    // 동적 할당 해제할 노드가 여러 개라면
    else
    {
        Node *pNode = pList->pHeadNode->pNextNode;
        Node *pTmpNode = NULL;
        do
        {
            pTmpNode = pNode->pNextNode;
            free(pNode); // 차례대로 동적 할당 해제(헤드 다음 노드부터 헤드 이전 노드까지)
            pNode = pTmpNode;
        } while (pNode != pList->pHeadNode);
        free(pList->pHeadNode); // 마지막 헤드 노드 동적 할당 해제
    }
}