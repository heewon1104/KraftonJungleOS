#include "rbtree.h"
#include <stdlib.h>

// NIL 노드 있게 하여 구현

// new_rbtree: rbtree를 생성하는 함수
rbtree *new_rbtree(void)
{
  // rbtree 동적 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // Nil 노드 동적 할당
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  // Nil 노드의 초기 색은 검정
  p->nil->color = RBTREE_BLACK;
  // 초기 루트를 Nil노드로 설정
  p->root = p->nil;
  // rbtree 반환
  return p;
}

// delete_rbtree: rbtree를 삭제하는 함수
void delete_rbtree(rbtree *t)
{
  // rbtree의 루트가 Nil노드(트리가 빌때까지) 루트 값을 삭제
  while (t->root != t->nil)
    rbtree_erase(t, t->root);

  // root값을 NULL로 초기화
  t->root = NULL;
  // Nil노드 동적 할당 해제
  free(t->nil);
  // rbtree 동적 할당 해제
  free(t);
}

// makeNode: 노드 생성 함수
node_t *makeNode(rbtree *t, const key_t key)
{
  // 노드 생성 및 값 초기화
  node_t *newNode = malloc(sizeof(node_t));
  newNode->key = key;
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;

  // 생성한 노드 반환
  return newNode;
}

// TransPlant: u 노드를 v 노드로 대체하는 함수
void TransPlant(rbtree *t, node_t *u, node_t *v)
{
  // u가 트리의 루트인 경우, v를 루트로 설정
  if (u->parent == t->nil)
    t->root = v;
  // u가 부모의 왼쪽 자식인 경우, 부모의 왼쪽 자식을 v로 설정
  else if (u == u->parent->left)
    u->parent->left = v;
  // u가 부모의 오른쪽 자식인 경우, 부모의 오른쪽 자식을 v로 설정
  else
    u->parent->right = v;

  // v의 부모를 u의 부모로 설정
  v->parent = u->parent;
}

// leftRotate: 왼쪽으로 회전하는 함수
// x->right의 값은 Nil 노드가 될 수 없음을 전제로 만듦
// Introduction to Algorithms 360p 참고
void leftRotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

// rightRotate: 오른쪽으로 회전하는 함수
// x->left 값은 Nil 노드가 될 수 없음을 전제로 만듦
void rightRotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

// RBInsertFixup: 입후 균형이 깨졌을 때 재조정하는 함수
void RBInsertFixup(rbtree *t, node_t *z)
{
  // y는 z의 삼촌 노드
  node_t *y = t->nil;

  // z의 부모가 검은색이 아닐때까지 반복
  while (z->parent->color == RBTREE_RED)
  {
    // 만약 z의 부모가 z 조부모의 왼쪽 노드라면,
    if (z->parent == z->parent->parent->left)
    {
      // 조부모의 오른쪽이 삼촌 노드
      y = z->parent->parent->right;
      // 삼촌이 빨간색인 경우
      if (y->color == RBTREE_RED)
      {
        // 부모와 삼촌의 색을 검정, 조부모의 색을 빨강으로 변경하여 재조정
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // z를 조부모로 설정하여 균형이 깨졌는지 다시 검사
        z = z->parent->parent;
      }

      // 삼촌이 검정색인 경우
      else
      {
        // 만약 z가 부모의 오른쪽 노드라면
        if (z == z->parent->right)
        {
          // z를 z의 부모 노드로 변경한 뒤 왼쪽으로 회전
          z = z->parent;
          leftRotate(t, z);
        }

        // z의 부모와 조부모의 색 변경
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // z의 조부모를 기준으로 오른쪽 회전
        rightRotate(t, z->parent->parent);
      }
    }

    // 위의 과정과 비슷하게 진행
    // 만약 z의 부모가 z 조부모의 오른쪽 노드라면,
    else
    {
      // 조부모의 왼쪽이 삼촌 노드
      y = z->parent->parent->left;
      // 삼촌이 빨간색인 경우
      if (y->color == RBTREE_RED)
      {
        // 부모와 삼촌의 색을 검정, 조부모의 색을 빨강으로 변경하여 재조정
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // z를 조부모로 설정하여 균형이 깨졌는지 다시 검사
        z = z->parent->parent;
      }

      // 삼촌이 검정색인 경우
      else
      {
        // 만약 z가 부모의 왼쪽 노드라면
        if (z == z->parent->left)
        {
          // z를 z의 부모 노드로 변경한 뒤 오른쪽으로 회전
          z = z->parent;
          rightRotate(t, z);
        }
        // z의 부모와 조부모의 색 변경
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // z의 조부모를 기준으로 왼쪽 회전
        leftRotate(t, z->parent->parent);
      }
    }
  }

  // 재조정이 끝난 뒤 루트는 항상 검정이므로 변경
  t->root->color = RBTREE_BLACK;
}

// rbtree_insert: 값을 RBTree에 삽입하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 노드 생성
  node_t *z = makeNode(t, key);

  // x: 새로운 노드 z가 들어갈 위치
  node_t *x = t->root;
  // y: x의 부모 노드
  node_t *y = t->nil;

  // z가 들어갈 말단 노드로 x를 이동 시킴
  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  // 새로 삽입하는 노드 z의 부모를 y로 설정
  z->parent = y;
  // y가 Nil노드인 경우 -> y는 z의 부모이므로, z가 루트임을 알 수 있음
  if (y == t->nil)
    // 트리의 루트를 z로 설정
    t->root = z;
  // z의 키값이 부모인 y의 키값보다 작은경우
  else if (z->key < y->key)
    // y의 왼쪽에 z 연결
    y->left = z;
  else
    // 클 경우 y의 오른쪽에 z 연결
    y->right = z;

  // 삽입후 균형이 깨졌을 때 재조정하는 함수
  RBInsertFixup(t, z);
}

// RBDeleteFixUp: 삭제 후 트리의 규칙이 깨져 재조정을 해 주는 함수
void RBDeleteFixUp(rbtree *t, node_t *x)
{
  // x가 트리의 루트가 아니고, BLACK일 때까지 반복
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    // w는 x의 형제 노드
    node_t *w = t->nil;
    // x가 부모의 왼쪽 자식인 경우
    if (x == x->parent->left)
    {
      // w는 부모의 오른쪽 자식
      w = x->parent->right;

      // 형제 노드 w가 RED인 경우
      if (w->color == RBTREE_RED)
      {
        // 형제를 BLACK으로 변경
        w->color = RBTREE_BLACK;
        // 부모를 RED로 변경
        x->parent->color = RBTREE_RED;
        // 부모 노드 기준으로 왼쪽 회전
        leftRotate(t, x->parent);
        // 회전 후 다시 w 설정
        w = x->parent->right;
      }

      // 형제의 양쪽 자식이 모두 BLACK인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        // 형제의 색을 RED로 변경
        w->color = RBTREE_RED;
        // x를 부모로 올리고 반복 진행
        x = x->parent;
      }
      else
      {
        // 형제의 오른쪽 자식이 BLACK인 경우
        if (w->right->color == RBTREE_BLACK)
        {
          // 형제의 왼쪽 자식을 BLACK으로 변경
          w->left->color = RBTREE_BLACK;
          // 형제를 RED로 변경
          w->color = RBTREE_RED;
          // 형제 노드를 기준으로 오른쪽 회전
          rightRotate(t, w);
          // 회전 후 다시 w 설정
          w = x->parent->right;
        }
        // 형제의 색을 부모의 색으로 설정
        w->color = x->parent->color;
        // 부모를 BLACK으로 변경
        x->parent->color = RBTREE_BLACK;
        // 형제의 오른쪽 자식을 BLACK으로 변경
        w->right->color = RBTREE_BLACK;
        // 부모를 기준으로 왼쪽 회전
        leftRotate(t, x->parent);
        // 루트까지 도달하면 반복 종료
        x = t->root;
      }
    }

    // x가 부모의 오른쪽 자식인 경우
    else
    {
      // w는 부모의 왼쪽 자식
      w = x->parent->left;

      // 형제 노드 w가 RED인 경우
      if (w->color == RBTREE_RED)
      {
        // 형제를 BLACK으로 변경
        w->color = RBTREE_BLACK;
        // 부모를 RED로 변경
        x->parent->color = RBTREE_RED;
        // 부모 노드 기준으로 오른쪽 회전
        rightRotate(t, x->parent);
        // 회전 후 다시 w 설정
        w = x->parent->left;
      }

      // 형제의 양쪽 자식이 모두 BLACK인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        // 형제의 색을 RED로 변경
        w->color = RBTREE_RED;
        // x를 부모로 올리고 반복 진행
        x = x->parent;
      }
      else
      {
        // 형제의 왼쪽 자식이 BLACK인 경우
        if (w->left->color == RBTREE_BLACK)
        {
          // 형제의 오른쪽 자식을 BLACK으로 변경
          w->right->color = RBTREE_BLACK;
          // 형제를 RED로 변경
          w->color = RBTREE_RED;
          // 형제 노드를 기준으로 왼쪽 회전
          leftRotate(t, w);
          // 회전 후 다시 w 설정
          w = x->parent->left;
        }
        // 형제의 색을 부모의 색으로 설정
        w->color = x->parent->color;
        // 부모를 BLACK으로 변경
        x->parent->color = RBTREE_BLACK;
        // 형제의 왼쪽 자식을 BLACK으로 변경
        w->left->color = RBTREE_BLACK;
        // 부모를 기준으로 오른쪽 회전
        rightRotate(t, x->parent);
        // 루트까지 도달하면 반복 종료
        x = t->root;
      }
    }
  }

  // x를 BLACK으로 변경후 종료
  x->color = RBTREE_BLACK;
}

// rbtree_erase: Red-Black Tree에서 p 노드를 삭제하는 함수
int rbtree_erase(rbtree *t, node_t *p)
{
  // 삭제할 노드를 y로 설정 (대체될 노드가 될 수도 있음)
  node_t *y = p;
  // y의 원래 색을 저장 (추후 색상 재조정에 사용)
  color_t yOriginalColor = y->color;
  // x는 삭제 이후의 노드 (y가 삭제된 이후 재조정에 사용될 노드)
  node_t *x = t->nil;

  // p의 왼쪽 자식이 NIL인 경우, 오른쪽 자식을 p의 위치에 이식
  if (p->left == t->nil)
  {
    // p의 오른쪽 자식을 x로 설정 (재조정에 사용될 노드)
    x = p->right;
    // p를 p의 오른쪽 자식으로 대체
    TransPlant(t, p, p->right);
  }

  // p의 오른쪽 자식이 NIL인 경우, 왼쪽 자식을 p의 위치에 이식 (간단한 삭제)
  else if (p->right == t->nil)
  {
    // p의 왼쪽 자식을 x로 설정 (재조정에 사용될 노드)
    x = p->left;
    // p를 p의 왼쪽 자식으로 대체
    TransPlant(t, p, p->left);
  }

  // p의 양쪽 자식이 모두 있는 경우
  else
  {
    // y를 삭제할 값의 직전값(전임자)으로 설정
    y = p->left;
    while (y->right != t->nil)
    {
      y = y->right;
    }

    // y의 색을 저장
    yOriginalColor = y->color;
    // y의 왼쪽 자식을 x로 설정, x는 Nil노드가 될 수 있음
    x = y->left;
    // x가 Nil노드가 될 수 있는데, Nil의 경우 부모에 대한 정보가 없으므로, x의 부모를 y로 설정
    x->parent = y;

    // y가 p의 자식이 아닌 경우(떨어져 있는 경우)
    if (y != p->left)
    {
      // y를 y의 왼쪽 자식으로 대체
      TransPlant(t, y, y->left);
      // y의 왼쪽 자식을 p의 왼쪽 자식으로 설정
      y->left = p->left;
      // 새롭게 설정된 y의 왼쪽 자식의 부모를 y로 설정
      y->left->parent = y;
    }

    // p를 y로 대체
    TransPlant(t, p, y);
    // y의 오른쪽 자식을 p의 오른쪽 자식으로 설정
    y->right = p->right;
    // 새롭게 설정된 y의 오른쪽 자식의 부모를 y로 설정
    y->right->parent = y;
    // y의 색을 p의 색으로 설정
    y->color = p->color;
  }

  // y의 원래 색이 BLACK인 경우 (Double Black이므로 재조정 필요)
  if (yOriginalColor == RBTREE_BLACK)
    // 재조정 함수 RBDeleteFixUp 호출
    RBDeleteFixUp(t, x);

  // 삭제 하기 전 반환 값 저장
  key_t tmpKey = p->key;
  // 메모리 해제
  free(p);
  // 삭제한 노드의 키 값 반환
  return tmpKey;
}

// rbtree_find: key값을 찾는 함수
// 값이 존재하는 경우 해당 노드 반환, 값이 없는경우 NULL 반환
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // 현재 값을 root로 초기화
  node_t *current = t->root;

  // 현재 값이 Nil노드가 아닐 때까지 반복
  while (current != t->nil)
  {
    // 현재 값이 키값보다 크면 왼쪽으로 이동
    if (current->key > key)
      current = current->left;
    // 현재 값이 키값보다 작으면 오른쪽으로 이동
    else if (current->key < key)
      current = current->right;
    // 일치하면 반복문 종료
    else
      break;
  }
  // 현재 값이 Nil 노드라면 일치하는 값을 찾지 못하고 트리의 끝까지 내려간 것이므로 NULL 반환
  if (current == t->nil)
    return NULL;
  // Nil 노드가 아니면, 일치하는 값을 찾은 것이므로 해당 노드 반환
  else
    return current;
}

// rbtree_min: 트리의 최솟값을 찾는 함수
node_t *rbtree_min(const rbtree *t)
{
  // 현재 값을 root로 초기화
  node_t *current = t->root;

  // 트리의 가장 왼쪽값이 최솟값이므로, 가장 왼쪽 노드까지 내려감
  while (current->left != t->nil)
    current = current->left;

  // 트리의 가장 왼쪽값 반환
  return current;
}

// rbtree_max: 트리의 최댓값을 찾는 함수
node_t *rbtree_max(const rbtree *t)
{
  // 현재 값을 root로 초기화
  node_t *current = t->root;

  // 트리의 가장 오른쪽 값이 최댓값이므로, 가장 오른쪽 노드까지 내려감
  while (current->right != t->nil)
    current = current->right;

  // 트리의 가장 오른쪽 값 반환
  return current;
}

// inorder_traversal: 트리를 중위순회 하는 함수
void inorder_traversal(const rbtree *t, node_t *current, int *idx, int size, key_t *arr)
{
  // 현재 노드가 말단노드거나, 배열의 인덱스가 배열 크기보다 크면 return
  if (current == t->nil || *idx >= size)
    return;

  // 현 노드의 왼쪽 노드 재귀로 호출
  inorder_traversal(t, current->left, idx, size, arr);
  // 배열의 인덱스가 배열 크기보다 작으면, 배열에 노드 값 저장, 배열의 인덱스 1증가
  if (*idx < size)
  {
    arr[*idx] = current->key;
    *idx += 1;
  }
  // 현 노드의 오른쪽 노드 재귀로 호출
  inorder_traversal(t, current->right, idx, size, arr);
}

// rbtree_to_array: RBtree의 내용을 오름차순으로 array로 변환
// array의 size는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환, 이후는 버림
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // idx는 배열의 인덱스
  int idx = 0;
  // 중위 순회로 트리를 순회할 경우 오름차순으로 순회 가능
  inorder_traversal(t, t->root, &idx, n, arr);
  // 순회가 끝나면 1 출력
  return 1;
}