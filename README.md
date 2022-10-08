# **CH12 AI 컨트롤러와 비헤비어 트리**

- 폰 == 조종 당하는 액터
- 비헤비어 트리 모델 => 컴퓨터 인공지능

## AIController와 내비게이션 시스템

- 스스로 행동하는 캐릭터 == NPC
  - 폰을 AIController에 빙의 시켜서 만들 수 있음
  
- AIController를 부모 클래스로 하는 ABAIController 를 생성


-  ABCharacter가 ABAIController를 사용하도록 설정하는 방법
  - AIController의 클래스 속성(``AIControllerClass``) 을 ABAIController의 클래스로 
  - AI 생성 옵션(``AutoPossessAI``)을 PlaceInWorldOrSpawned로 설정 (배치 or 새롭게 생성되는 ABCharacter는 ABAIController가 빙의됨)

### 네비게이션 시스템

- 스스로 움직이게하는 보조 장치
- 먼저, 레벨에 내비게이션 메시를 배치해야함
- Place Actors > Navigation > NavMesh Bounds Volume 을 원점에 배치 후, 볼륨의 크기를 넉넉히 1e4x1e4x5e2로 설정

> 뷰포트에서 P키를 누르면, 내비메시 영역이 녹색으로 보임


- ABAIController에 빙의한 폰에게 목적지를 알려줘 폰이 목적지까지 스스로 움직이도록 명령을 추가할 수 있음
  - ``GetRandomPointInNavigableRadius`` 함수: 이동 가능한 목적지 랜덤으로 가져옴
  - ``SimpleMoveToLocation`` 함수: 목적지로 이동시키는 함수
  - AIController 에는 PathFollowingComponent가 부착돼있음, 이것은 조종하는 폰의 길찾기를 지속적으로 관리함

>  UE4.20 부터 "NavigationSystem" 모듈을 Build.cs 파일에 추가해야함.  (NavigationSystemV1 로 변경됨, 구현부에 NavigationSystem.h 헤더 추가 선언, SimpleMoveToLocation 함수는 블루프린트 라이브러리로 이동)
> 
> 