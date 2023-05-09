# Chap57 Mutex

- race condition
    - thread간의 자원 선점을 위한 경합 상태.
    - 특정 자원을 점유하기 위해 발생
- ciritical section
    - 값의 변경에 대해 접근권한을 설정하여 값 무결성 보장.
- DeadLock
    - 자원 소유가 서로 맞물려 진행이 불가능한 상태
    - 원인
        - 비선점
        - 상호배제
        - 점유대기
        - 순환대기
        - 네가지 조건 모두 만족시 발생.
    - 해결 방법.
        - 예방
            - 위의 네가지 조건중 하나 해결.
            - 비선점 → 선점
            - 상호 배제 → 여러 Thread가 자원 공유 가능
            - 순환 대기 → 순환 대기 부정 : 각 자원에 번호 할당, 해당 번호보다 작은 자원만 획득 가능/큰 번호만 획득 가능
            - 점유대기 → 프로세스 실행시 모든 자원 획득 / 자원 점유하지 않은 상태에서만 자원 획득 가능하도록 수행.
            - 해당 조건들은 성능 감소를 유발.
        - 회피
            - 전제 조건
                - **프로세스 수 고정**
                - **자원의 종류와 수 고정**
                - **프로세스가 요구하는 최대 자원의 수를 알아야 함**
                - **프로세스는 자원 사용 후 반드시 반납**
                
            - 은행원 알고리즘. → 사실상 불가능
            - 자원할당 그래프 알고리즘. → 그래프 생성하여, 해당 자원 획득시 사이클 생성하지 않도록 하는 경우에만 자원 할당.

- functional 객체는 매개변수를 ref로 넘기는 경우 ref(val) 형태로 넘김.
- auto는 사용하지 말자. error 확률 증가.
    
    ```cpp
    function<void(int&)> f = bind(&ThreadDemo::RaceCondition, this, placeholders::_1);
    // 정상 케이스
    
    decltype(bind(&ThreadDemo::RaceCondition, this, placeholders::_1)) f = bind(&ThreadDemo::RaceCondition, this, placeholders::_1);
    // 정상 수행
    
    auto f = bind(bind(&ThreadDemo::RaceCondition, this, placeholders::_1));
    // auto는 쓰지 말자. error.
    
    threads.push_back(thread(f, ref(counter)));
    // ref/ 비ref 구분해주어야 함.
    ```