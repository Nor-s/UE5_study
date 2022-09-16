# CH3

- 분수대 애셋 제작
  - 스태틱 메시와 같은 컴포넌트 C++ 클래스 작성 기초에 대해 배움

- 프로젝트 구조에 대해 배움
  - 프로젝트 재구성시 .vs, Binaries, Intermediate, sln 파일 제거 

## 컴포넌트 종류

- 트랜스폼 정보가 필수적인 컴포넌트: 씬 컴포넌트
- 기능만 제공하는 컴포넌트: 액터 컴포넌트

## 무브먼트 컴포넌트 활용

- Floating... : 중력의 영향을 받지 않는 액터의 움직임(입력에 의해 동작
- RotatingMovement: 지정한 속도로 액터 회전
- InterpMovement: 지정한 위치로 이동
- ProjectileMovement: 중력의 영향 포물선 이동

## 주요 이벤트 함수

1. 액터에 속한 컴포넌트들이 세팅이 완료되면, UE는 PostInitializeComponents 함수 호출
2. 액터가 게임에 참여할 때 BeginPlay함수 호출
3. 매 프레임마다 Tick 함수 호출
4. 퇴장할 때 EndPlay 함수 호출

## 문자열

- TEXT 매크로 사용 : 2바이트 문자열 지원
- 애셋의 경로는 게임 실행 중 변경되지 않으므로, static으로 선언 
    - ConstructorHelpers::FObjectFinder<UStaticMesh> 등으로 불러올 때

## UPROPERTY

- VisibleAnywhere
  - 수정가능(대입으로 값자체가 아닌, 속성 등), 디테일 창에 보여짐
- EditAnywhere
  - 값 수정 가능(대입)
- Category
  - 디테일 창의 카테고리 이름
- Meta = (AllowPrivateAccess = true)
  - 은닉..캡슐화
  
## 언리얼 오브젝트 클래스
  
- 클래스 선언 매크로
  - UCLASS
  - GENERATE_BODY
- 클래스 이름 접두사
  - A: 액터 클래스
  - U: 액터 아닌 클래스
  - ex) AFountain, UStaticMeshComponent
- generated.h 헤더파일
- 외부 모듈에의 공개 여부
    - 모듈명_API 키워드

## IWYU
  
- 불필요한 헤더파일 참조로 인한 컴파일 시간, 인텔리센스 부하 최소화


## 언리얼 콘텐츠의 구성요소
- 월드
      - 뷰포트에 보이는 작업공간
- 월드 구성요소
  - 공간: 3차원 영역 (트렌스폼 구조체, cm 단위)
  - 시간: 초단위, 스케일 조정가능
  - 물리: 중력등, 콜리전 정보 필요
  - 렌더링: 빝, 머티리얼, 물리기반 렌더링
  - setting>worldsetting
  
  
  
# CH4
- 게임을 만드는 작업 = 레벨을 구성하는 작업 + 게임플레이 설계하는 작업 
- 게임 플레임워크: 게임의 규칙을 관리하는 게임 모드 + 플레이어가 조종하는 액터인 폰

## 게임 규칙
- 무형적인 요소
- 심판에 해당하는 역할

## 언리얼의 게임모드
- 게임의 틀을 잡아주는 역할
- 플레이어가 입장할 때마다 플레이어를 점검, 게임에 입장한 플레이어가 조종할 액터를 생성해 전달
- 플레이어가 조종할 수 있는 액터: 폰

- 게임모드 적용(레벨)
    - settings > world settings > Game Mode > Overrider 항목 선택

- 주의: 폰을 생성하고 지정하는 것이 아니라, 클래스 정보를 지정함
    - 멀티를 고려한다면, 미리 폰을 만들어두는 것이 아니라 클래스 정보만 저장, 플레이어 입장 시 생성하는 것이 합리적
    - Auto Possess Player 속성을 사용하면, 이미 배치된 폰에 컨트롤러가 빙의 가능 (detail 패널)
        - Player0 == 로컬 플레이어
        - 이를 설정하면 게임 모드는 폰 액터를 생성하지 않고, 플레이어 컨트롤러에게 레벨에 배치한 액터에 빙의하라는 명령을 함
        - 또는 코드로 블프 폰 지정 가능 (특별한 _C 경로 사용, 블루 프린트 애셋의 클래스 정보 가져옴)
        ```cpp
        static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/.... Chracter_C"));
        if(BP_PAWN_C.Succeeded())
        {
        DefaultPawnClass = BP_PAWN_C.Class;
        }
        ```
   > ConstructorHelpers 는 UObject의 Constructor에서만 사용 가능
    
## 플레이어 입장

- 입장
  - 게임모드는 플레이어에게 폰을 배정
  - 플레이어 컨트롤러라는 특별한 액터도 함께 배정
  - 플레이 버튼 누르면 입장됨
     1. 플레이어 컨트롤러의 생성
     2. 플레이어 폰의 생성
     3. 플레이어 컨트롤러가 플레이어 폰을 빙의
     4. 게임 시작
 
- 플레이어 컨트롤러

  - 실제 플레이어를 대변하는 무형의 액터
  - 게임 세계에서 플레이어와 1:1 소통, 폰 조정하는 역할
  - 플레이어가 입장할 대 배정, 배정된 플레이어 컨트롤러는 변경 불가

- 폰
  - 플레이어 컨트롤러에게 조종당하는 액터
  - 실제로 보여짐, 레벨과 물리적인 충돌, 다른 액터와 상호작용
  - 플레이어는 여러 폰을 조종할 수 있음 (Possess, 빙의)
  
- 로그인
  - 플레이어가 게임에 입장하는 것(로그인 과정에서 플레이어 컨트롤러 생성)
  - 로그인 완료 => 게임 모드의 PostLogin 이벤트 함수 호출
      - 폰 생성, 플레이어 컨트롤러가 해당 폰에 빙의 
      - 생성 시점은 각 액터의 PostInitializeComponents 함수로 파악
      - 빙의 시점은 컨트롤러의 Possess, 폰의 PossessedBy 함수로 파악



## UE5 주의

- 컨트롤러 Possess 함수 => OnPossess() 함수 (4.22 부터 Possess는 final 함수)

