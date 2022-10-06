# **CH9 충돌 설정과 대미지 전달**

- 액터들이 상호작용할 수 있도록 물리 엔진을 설정

## **콜리전 설정**

- 게임에서의 물리법칙
  - 움직임을 사실적으로 표현
  - 지정한 영역에 들어왔는지 감지 (기믹)

- 콜리전 영역
  - 스태틱메시 애셋
    - 메시 자체를 영역으로
    - 스태틱메시 에디터에서 설정 및 확인 가능
    - 스태틱메시 컴포넌트: 비주얼 + 충돌 기능 설정 가능
  - 기본 도형 컴포넌트
    - 스태틱메시와 별도로 충돌 영역을 제작하는 데 사용
    - 스켈레탈 메시를 움직일 때 주로 사용
    - 캐릭터이동: 캡슐컴포넌트 사용
  - 피직스 애셋
    - 관절이 흐느적 거리는 랙돌 효과를 위해 피직스 사용
    - 각 부위에 기본 도형으로 충돌 영역을 설정하고 이를 연결해 캐릭터의 물리 설정 가능
    - 피직스 애셋은 스켈레탈 메시에서만 사용 가능
  

- 충돌영역 설정
  -  스태틱 메시 애셋의 BlockAll 기본설정
    - 별도의 설정 없이 캐릭 이동 방해하는 레벨 콘텐츠
  - 물리설정: 플레이어가 레벨과 상호작용하는 영역은 아래 설정을 해야함
    1. 콜리전 채널과 기본 반응
    2. 콜리전 채널의 용도
    3. 다른 콜리전 채널과의 반응

### 물리 설정

- 충돌체에는 반드시 하나의 콜리전 채널을 설정해야함
  - 8개의 기본 콜리전: WorldStatic, WorldDynamic, Pawn, Visibility, Camera, PhysicsBody, Vehicle, Destructible
       - WorldStatic: 움직이지 x 정적 배경 액터에 사용, 주로 스태틱메시 액터에 있는 스태틱메시 컴포넌트에 사용
       - WorldDynamic: 움직이는 액터에 사용, 블프에 속한 스태틱 메시 컴포넌트에 사용
       - Pawn: 플레이어가 조종하는 물체에 주로 사용, 캐릭 충돌 담당 == 캡슐 컴포넌트
       - Visibility: 배경 물체가 시각적으로 보이는지 탐지하는데 사용(폰은 제외), 마우스 피킹 구현할 때 사용
       - Camera: 카메라와 목표물 간에 장애물이 있는지 탐지
       - PhysicsBody: 물리 시뮬레이션으로 움직이는 컴포넌트에 설정
       - Vehicle 
       - Destructible
  
- 캐릭터 루트 컴포넌트인 캡슐 컴포넌트에는, Pawn 이라는 콜리전 채널이 설정됨
  - 캡슐 컴포넌트 > 디테일 > Collision 섹션 > 콜리전 프리셋 > Object 타입(Pawn)
      > 콜리전 프리셋의 값 Pawn과 Object Type의 값 Pawn은 서로 다른 설정 값임으로 주의
  - Object Type > Collision Enabled  
      - Query: 두 물체의 충돌 영역이 서로 겹치는지 테스트하는 설정(Overlap), 충돌이 겹치면 BeginOverlap 이벤트 발생, Raycast, Sweep 기능
      - Physics: 물리 시뮬레이션을 사용할 때 사용
      - Query and Physics: 두 기능 모두 사용
      - 필요한 기능만 설정하는 것이 효과적
      - Query 기능: 관련 이벤트가 발생하도록 Generates Overlap Events 옵션이 체크돼 있음

- 다른 콜리전 채널과 반응 설정
  - Ignore: 콜리전이 있어도 아무 충돌이 일어나지 않음
  - Overlap: 무시와 동일하게 물체가 뚫고 지나갈 수 있지만, 이벤트를 발생시킴
  - Block: 물체가 뚫고 지나가지 못하도록 막음

- 충돌: 두 물체의 상호작용
  - 두 물체가 가진 반응 값에 따라 결과가 달라짐
  - UE에서 물리는 무시 반응을 최대화, 블록 반응을 최소화
    - 충돌 컴포넌트 중 하나를 무시반응 => 겹침과 블록 반응이 발생하지 않음
    - 충돌 컴포넌트 중 하나를 겹침반응 => 블록 반응이 발생하지 않음

### 이벤트

- 겹침: BeginOverlap
- 블록: Hit 
  - Generates Overlap Events 항목이 오버랩, 블록 컴포넌트에 설정되면
    - Hit, BeginOverlap 이벤트 둘 다 발생 가능
    
### 새로운 콜리전 

- 프로젝트 설정 > Collision 메뉴 > 새로운 물리 설정 
- 콜리전 채널:
  - 트레이스 채널: 어떤 행동에 설정하는 콜리전 채널
    - Visibility, Camera에 해당
  - 오브젝트 채널: 콜리전 영역에 지정하는 콜리전 채널
    - 트레이스 채널을 제외한 나머지에 해당

- 새로운 채널 생성시 주의점
  - 이미 만들어진 다른 콜리전 프리셋과 문제 없도록 설정해야함
  - '블록'을 설정할 경우, 블록 기능에 관련된 콜리전 프리셋과 문제 없지만, 블록 반응을 하면 안되는 다른 프리셋을 위해 조율해야함

### 새로운 콜리전 프리셋

- 프로젝트 설정 > Collision 메뉴 > Preset 섹션 (하단)

- 블록이 기본반응이면, Trigger 프리셋과 충돌이 일어날 것.
  - Trigger 프리셋 설정 > 새로운 프리셋 반응 설정

- '블록'이 기본 반응이면 신경 써야할 프리셋
  - OverlapAll: 겹침으로 설정
  - OverlapAllDynamic: 겹침
  - IgnoreOnlyPawn: 폰만 충돌을 무시하도록 설정
  - OverlapOnlyPawn: 폰만 겹침으로 설정
  - Spectator: 외부 관중과의 충돌을 설정 ('블록'인 ABCharacter는 무시로 설정)
  - CharacterMesh: 캐릭터 메시에 사용하는 물리설정, 무시로 설정
  - RagDoll: 스켈레탈 메시의 피직스 애셋 물리를 가동하기 위한 물리 설정, 무시로 설정
  - Trigger: 지정한 영역에 물체가 들어오면, 이벤트가 발동하는 용도로 사용 (겹침으로 설정)
  - UI: UI 요소에 사용하는 설정, 겹침으로 설정


- 아래 코드로 프리셋 기본값 설정
```cpp
# ABCharacter.cpp
GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));
```

> 핫리로드 방식 컴파일 : 기본 설정을 건드리면, 에디터 다시 시작하는게 좋음


## 트레이스 채널의 활용

- 물리엔진을 사용해 캐릭터의 공격 기능을 구현
- 공격 행동: 공격 애니메이션이 일어나는 특정 타이밍에 공격 범위 안에 위치한 액터가 있는지 감지, 감지된 액터에게 대미지를 전달하는 행위

### 트레이스 채널 생성

- 트레이스 설정(Attack 채널을 사용하는 액터의 물리적 행동은 ABCharacter 콜리전 프리셋에만 반응)
  - 프로젝트 설정 > Collision > Trace 섹션 > Attack 채널 추가 > 기본반응 무시
  - ABCharacter 프리셋 > Attack 트레이스 채널과의 설정을 블록으로 설정


### 공격 로직

- 물리는 월드의 기능 == ``GetWorld()`` 함수로 월드에게 명령을 내림
- ``SweepSingleByChannel``: 트레이스 채널을 사용하여 물리적 충돌 여부를 가리는 함수
  - 기본 도형을 인자로 받음
  - 시작지점 ~ 끝지점을 쓸면서(Sweep) 물리판정이 일어났는지 조사
  - HitResult: 물리적 충돌이 탐지될 경우, 충돌 결과값을 담을 구조체
  - Start: 탐색 시작 위치
  - End: 탐색 끝 위치
  - Rot: 탐색에 사용할 도형의 회전
  - TraceChannel: 물리 충돌 감지에 사용할 트레이스 채널 정보
  - CollisionShape: 탐색에 사용할 기본 도형 정보 (구, 캡슐, 박스)
  - Params: 탐색 방법에 대한 설정값
  - ResponseParams: 탐색반응 설정값

- TraceChannel을 C++ 로 가져오는 방법
  - ECollisionChannel 열거형 사용 
  - 사용자 오브젝트, 트레이스 채널은 ECC_GameTraceChannel 1번 부터 18번 까지 하나 배정받음.
  - 배정값은 Config/DefaultEngine.ini 파일에서 확인 가능 (ECC_GameTraceChannel2)

> UE4 에서는 총 32개의 콜리전 채널을 제공함, 32개 중, 여덟 개는 언리얼 엔진이 기본으로 사용하고, 여섯 개는 엔진에서 다른 용도로 사용하도록 예약, 사용자는 나머지 18개만 사용 가능

1. 콜리전 채널 지정 
2. MakeSphere 탐지에 사용할 도형 생성 
3. 도형 탐색 영역 지정 
4. 탐색 방법 지정(공격하는 자신은 탐색대상에서 제외) 
5. 액터 충돌 탐지 결과얻을 구조체 FHitResult 생성


> 언리얼 실행환경... GC... 사용여부확인 == 오브젝트 참조로 판단 ... FHitResult의 Actor는 약포인터로 구현됨.. 따라서 유효한지 점검 필요


## **디버그 드로잉**

- 공격 범위가 시각적으로 보이게함
- DrawDebugHelpers.h 헤더 추가
- DrawDebugCapsule 함수 사용하여 원이 움직인 궤적 표현


## **대미지 프레임워크**

- 감지된 액터에게 대미지를 전달
- UE 에서 제공하는 대미지 프레임워크 사용하면 간편하게 처리 가능


- AACtor::TakeDamage
  - DamageAmount: 전달할 대미지의 세기
  - DamageEvent: 대미지 종류
  - EventInstigator: 공격 명령을 내린 가해자
  - DamageCauser: 대미지 전달을 위해 사용한 도구


- 대미지를 전달하는 행위: 항상 가해자와 피해자가 존재
  - 가해자 == 피해를 입히는 주체 (폰에게 명령을 내린 플레이어 컨트롤)
  - EventInstigator == 컨트롤러 정보
  
- 공격 범위 내 대미지를 전달하는 로직
  ```cpp
  		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
  ```
  
- 피해를 입은 액터에 관련 로직
  - TakeDamage 함수 오버라이드
  - 디테일 > Actor > can be damaged 속성으로 무적 상태 설정 가능

### 죽는 애니메이션

- 죽은 후, SetActorEnableCollision 함수로 충돌 설정을 꺼서 충돌 이벤트 관리
