# Chap54~55 Framework

- Process
    - 실행 파일 단위
    - multi process
        - 프로세스간 데이터 공유에 문제 발생
            - IPC를 이용한 방식으로 진행.
                - 공유 메모리
                - 메세지
                - pipeline
            - IPC는 기본적으로 Interrupt 발생
                - I/O는 기본적으로 Interrupt.
                    - Cpu를 제외한 나머지 장치와의 값 이동은 Interrupt.
                    - sleep함수 또한 함수적으로 interrupt 발생.
                - hard disk에 입출력을 이용하여 메세지 저장.
                - multi-thread 환경으로 변화
- Thread
    - 기본적으로는 최소한의 실행 가능한 단위로 나누어 진행.
    - 일반적인 프로그래밍 환경에서는 함수 단위로 실행.
    - 동기 Thread
        - 하나의 thread 동작
        - 타 Thread가 기존 thread와 동기화 하여 진행.
    - 비동기 Thread
        - Thread간의 순서 관계 없이 진행.