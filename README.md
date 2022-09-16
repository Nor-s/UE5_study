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
