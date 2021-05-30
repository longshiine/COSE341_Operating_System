# COSE341_Operating_System
## Summary
COSE341: Operating System Course that learns process management, virtual memory, synchronization 

**과목개요**  
컴퓨팅 시스템의 기본이 되는 운영체제에 대하여 공부한다. 운영체제의 필요성과 구성 원리를 이해하고, 운영체제가 갖추어야 할 기본 기능에 대해 배운다. 기본 기능들은 프로세스 관리, 메모리 및 가상 메모리 관리, 동기화 등이다.

**학습목표**  
운영체제의 근가이 되는 abstraction에 대한 이해와 이 abstraction 들이 프로세스 관리, 메모리 관리를 구현하는데 어떻게 활용되는지 공부하여, 인공지능과 같은 응용을 시스템 관점에서 이해할 수 있도록 한다. 또한, 리눅스를 활용한 실습 및 숙제를 통하여 abstraction 들을 체험할 수 있도록 한다.

## Contents
### 1. Kernel Compile
1. 목적  
이번 과제의 목적은 일반 배포판 리눅스에 새로운 Kernel을 컴파일하고 수정해서 새로운 System Call을 추가
하는 것이다. System Call은 Kernel이 시스템이나 하드웨어에 특권이 있어야만 하는 동작들을 지정해둔 함수로써 User-Space에서 Kernel-Space로 전환하기 위해서 특별히 지정된 Trap Instruction을 거쳐야 한다. 본 과제에서는 Kernel을 직접 컴파일하고, 이후 기존의 Kernel에 새로운 System Call을 추가한 후에 이를 호출할 수 있는 User-Application까지를 구현하도록 한다.
새로 추가한 system call은 내부적으로 Queue 자료구조를 갖도록 한다. 그리고 이 Queue에 integer값을 Enqueue하거나 Dequeue하는 역할을 수행하도록 작성한다. 단, 중복된 integer값을 저장할 수 없다. 그 밖에 정 해진 Queue의 구현 방법은 없으며 자유롭게 구현할 수 있다. 기본적인 Enqueue와 Dequeue가 가능하면 된다. Queue에 저장할 수 있는 entry의 개수 또한 고정적으로 하여도 무관하다.


2. 과제 목표
A. Kernel
  -  VirtualBox를 이용한 가상 머신 환경에 구현하도록 한다.
  - 리눅스는 Ubuntu 18.04.2 LTS 배포판으로 한다.
  - 새로 컴파일하는 Kernel Version은 4.20.11 로 한다.
  - 커널 상에 Queue를 구현한다.
  - System Call시 전달받은 정수형 인자를 printk로 출력하도록 한다.
B. User Process
  - syscall를 이용해서 새로 작성한 System Call을 호출하도록 한다.
  - Enqueue와 Dequeue가 제대로 동작하는지 확인한다.
  - System Call의 반환 값을 출력하도록 한다.

3. 세부사항
**A. System call**  
앞에서도 간략하게 설명했지만 System Call이란 리눅스 커널에 의해 제공되는 Kernel-level 서비스이다. 예를
들어 프로그래머가 파일을 읽는 서비스를 이용하고 싶다면 그에 해당하는 System Call을 이용해야 한다. C를 이 용해서 프로그래밍할 경우 대부분의 System Call은 libc를 통한 Wrapper Function 형태로 제공받을 수 있다. 예를 들어 User Application에서 파일을 읽는 read 함수가 호출되었다고 하면 Figure 1과 같은 호출이 일어나게 된다.
Figure 1과 같이 User Process에서 read를 호출하면 Wrapper Function을 통해서 0x80의 Trap Instruction이 발생 한다. 0x80 번째 Trap은 System Call로 예약이 되어 있고 이 Trap Handler는 Sys_call_table의 번호를 확인해서 3 번째 Table에 저장되어 있는 sys_read를 호출하게 된다. 이렇게 커널에서의 서비스가 끝나면 다시 User Process 로 결과값과 함께 return하게 된다. 이번 과제에서는 sys_call_table에 2개의 Entry를 더 추가해서 sys_my_enqueue, sys_my_dequeue을 User Process에서 호출하고 dmesg를 통해서 printk로 출력한 메시지를 확인하도록 한다.

img

**B. Kernel에서 문자열 출력 (printk 함수)**  
Kernel 영역에서는 기존에 사용하던 printf를 사용하지 못한다. 커널 영역에서 메시지를 출력하려면 printk를 사용 해야 하며, 함수의 사용법은 기본적으로 printf와 같다. printk는 특별히 중요도를 지정해서 출력할 수도 있지만 이 번 과제에서는 사용하지 않도록 한다. System Call에서 printk를 통해 메시지를 출력했다면 일반 Terminal 화면으 로는 볼 수 없고, 최근에 발생한 커널 메시지를 확인하는 dmesg 명령어를 통해서 확인할 수 있다.

### 2. CFS Scheduler
1. 목적
본 과제는 리눅스의 프로세스 및 프로세스 스케줄러를 이해하기 위해, 프로세스의 CPU 점유 시간 (burst) 및
vruntime을 살펴본다. 리눅스 프로세스 스케줄러는 각 프로세스에 CPU를 점유하는 프로세스 점유 시간을 할당하
는데, 점유 시간의 할당 방식은 프로세스의 종류, 컴퓨터의 종류, 스케줄러의 종류에 따라 그 특성이 다르게 나
타난다. 또한 vruntime 값에 가중치를 부여하여 프로세스의 우선순위에 따라 CPU를 점유할 수 있도록 한다. 이
에 본 과제에서는 실제 리눅스 운영체제에서 스케줄러가 프로세스에 어떻게 CPU 점유 시간을 할당하는지, 그리
고 현대의 프로세스와 시스템에서 프로세스의 CPU 점유 시간이 어떠한 분포를 나타내는 지 실제로 조사해본다.

img

2. 과제 목표
**A. CPU burst, vruntime 측정을 위한 리눅스 스케줄러 수정**  
  - 리눅스 기본 스케줄러에서 할당되는 CPU burst, vruntime 값을 측정할 수 있도록 스케줄러 코드를
수정한다.
  - CPU burst, vruntime 값의 출력 방식(e.g., printk, proc 파일 시스템 등)은 어떠한 방식을 사용해도 무
방하다.
  - CPU burst, vruntime 값은 burst값이 1,000회 바뀔 때 마다 1회 샘플링 하여 기록한다.
**B. CPU burst, vruntime 측정 실험 수행**  
  - 30분 동안의 전체 프로세스의 CPU burst, vruntime 측정
    i. 단, 기본 커널 데몬 및 쓰레드 외에 CPU 사이클을 소모하는 프로세스를 실행하고
    분석 시 구동한 프로세스의 대한 설명을 쓴다.

  - 우선순위를 변경한 뒤 전체 프로세스의 CPU burst, vruntime 측정
    i. 앞서 측정한 프로세스들 중 하나를 선정해 우선순위를 10, -10으로 변화시켜 측정한다.
(Default 우선순위 = 0)
      ◼ 우선순위를 변경하는 프로세스는 동일해야 하며, 자유롭게 선택가능 함.
    ii. 총 세번의 측정 결과를 비교하여 vruntime 누적 값과 CPU burst 누적 값이 우선순위에 따라 어
    떻게 변하는지를 분석한다.
      ◼ 우선순위를 변경한 프로세스에 대해서 분석을 진행하면 됨.
**C. 프로세스 및 스케줄러에 대한 이해**  
  - 2.B에서 실험한 항목에 대해 Figure 1과 같은 형태의 그래프를 작성한다.
  - 그래프를 통해 프로세스 및 스케줄러의 동작에 대해 이해하고 분석하여, 보고서를 작성한다.