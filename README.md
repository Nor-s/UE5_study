# **CH11 게임 데이터와 UI 위젯**

- 테이블 데이터로부터 캐릭터의 능력치를 체계적으로 관리
- 게임 데이터를 효과적으로 관리하기에 적합한 게임 인스턴스 클래스와 방대한 로직 분산
- 현재 스탯을 시각적으로 표시하는 UI 구현

## **엑셀 데이터의 활용**

- 스탯 테이블은 앱이 초기화될 때 불러들임
- 게임 인스턴스 UE 오브젝트: 게임 앱 관리용 객체, 스탯관리를 이 객체가하면, 앱 초기화시 스탯 데이터를 불러들이고, 종료까지 데이터를 보존함

- 새로운 C++ 클래스 > 모든 클래스 표시 > GameInstance 부모 클래스 선택
  - 프로젝트 세팅 > Map & Mode > GameInstance 교체

- UE는 앱 초기화시 GameInstance의 Init 함수 호출함
- GameInstance 클래스의 Init 함수에서는 스탯 테이블을 불러들임

### 게임 시작 프로세스

1. 게임 앱 초기화 (``UGameInstance::Init``)
2. 레벨에 속한 액터의 초기화 (``AActor::PostInitizlizeComponents``)
3. 플레이어의 로그인 (``AGameMode::PostLogin``)
4. 게임의 시작 (``AGameMode::StartPlay``, ``AActor::BeginPlay``)

### CSV 파일 임포트

- 먼저, 테이블 각 열의 이름과 타입이 동일한 구조체를 생성해야함
- FTableRowBase 구조체를 상속받는 구조체를 GameInstance 헤더파일에 선언

- 구조체 생성 언리얼 규칙(에디터 인터페이스와 연동을 위해)
  -  USTRUCT() 매크로로 구조체 선언 윗부분에 넣음
  -  GENERATED_BODY() 매크로 구조체 내부에 넣음

- 테이블의 첫 열 (Name)은 언리얼에서 자동으로 키값으로 사용하므로 선언에서 제외

- 임포트 > CSV 파일 선택 > 데이터 테이블 옵션 > 테이블 행 유형 선택(새로 선언한 구조체)

### 게임 인스턴스에서 CSV 로딩

- 게임인스턴스 클래스에,  테이블 데이터를 관리하는 DataTable 언리얼 오브젝트 멤버변수 선언


## 액터 컴포넌트 제작

- 새로운 액터 컴포넌트 클래스 생성 및 캐릭터에 부착 (캐릭터 스탯 관리하는 컴포넌트)
- 새로운 C++ 클래스 > ActorComponent

- 자동으로 제공되는 템플릿 코드 == BeginPlay, TickComponent 함수
- 스탯에 변경이 일어날 때만 관련 데이터 처리 == TickComponent 함수 필요 없음
- InitializeComponent 는 액터의 PostInitializeComponents 에 대응
    - PostInitialzieComponents 함수가 호출되기 바로 전에 호출됨
    - bWantInitizeComponent 값이 true여야함


### 직렬화 기능

- 언리얼 오브젝트의 직렬화 기능
  - UPROPERTY 속성을 저장하고 로딩 가능
  - Transient 키워드를 추가해 해당 속성을 직렬화에서 제외시킬 수 있음 (CurrentHP값은 게임 시작할 때마다 변경됨, 이 예제는 이 값을 저장할 필요가 없음)

  