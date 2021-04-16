# os_project2_pthread

이 프로젝트는 운영체제론 수업에서 진행한 두 번째 프로젝트로 pthread함수들을 이용해서 pthread 생성, 흐름에 대해 알아보는 목적의 프로젝트이다.

## 주요 함수

1. pthread_create(pthread_t *th_id, const pthread_attr_t *attr, void* 함수명, void *arg)  
   pthread를 생성하는 함수이다.
2. pthread_join(pthread_t th_id, void\*\* thread_return)  
   특정 pthread가 종료되는 것을 기다리는 함수이다.

## 느낀점

pthread함수를 직접 사용해보면서 pthread_create(), pthread_join() 사용 방법을 익힐 수 있었다. 특정 쓰레드가 먼저 실행되었다 하더라도 그 쓰레드가 먼저 끝난다는 보장은 없다는 것을 알게되었다. 이것은 스케줄링 알고리즘에 따라 다를 수 있다.

check_rows,check_columns, check_subgird함수 구현할 때 처음에는 각 숫자(1, 2, 3, 4, 5, 6, 7,8 9)의 합 45를 기준으로 성공과 실패를 구분하는 알고리즘을 짰는데 이것은 치명적인 오류라는 것을 알게 되었다. 만약 이 알고리즘이 맞다면 (2, 2, 2, 4, 5, 6, 7, 8, 9) 이와 같은 상황이 성공으로 구분이 되는 오류가 발생한다. 그래서 정렬 알고리즘을 사용하여 숫자 하나하나씩 비교하는 알고리즘으로 변경했다.
