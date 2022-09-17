# **CH6 캐릭터의 제작과 컨트롤**

- UE가 제공하는 캐릭터 모델
- 카메라 컨트롤

## **캐릭터 모델**

- 폰 모델보다 더 효과적으로 인간형 폰을 제작할 수 있는 모델
- `ACharacter`
  - `APawn` 클래스를 상속받음
  - Capsule, SkeletalMesh 컴포넌트를 사용
  - CharacterMovement 컴포넌트로 움직임을 관리
    - FloatingPawnMovement 컴포넌트에 비해 가지는 장점은 다음과 같음
    1. 점프와 같은 중력을 반영한 움직임 제공
    2. 다양한 움직임 설정 가능 (걷기, 기어가기, 날아가기, 수영 등 이동모드 설정)
    3. 멀티 플레이 네트워크 환경에서 캐릭터들의 움직임을 자동으로 동기화
  
- 입장 시 부여받는 두 종류의 액터
  - 플레이어 컨트롤러
    - 플레이어와 1:1 대응
    - 게임 월드의 물리적인 요소를 고려하지 않은 플레이어의 의지에 관련된 데이터를 관리
    - 회전 속성 제공
  - 폰
    - 물리적인 상황을 관리
    - 속도 속성 제공
  
## **컨트롤 회전의 활용**

- 마우스 움직임에 따라 폰이 회전해야 할 최종 목표 회전 값을 설정하고, 이를 향해 폰이 일정한 속도로 회전하는 기능
- 마우스의 상하좌우
  - 삼인칭 템플릿의 Turn(Z축회전)과 Lookup(Y축회전) Axis 입력 설정
  
- 캐릭터 회전
  - AddControllerInputYaw, Roll, Pitch

- 게임을 실행하고, ~ 키를 눌러 콘솔 명령어 입력 창에 다음 명령어 입력하면,
  - 컨트롤러의 컨트롤 회전 값이 화면에 나타남
  - displayall PlayerController ControlRotation
  - 이 회전 값은 캐릭터의 카메라 설정에서 다양하게 사용됨
    - 기본적으로 컨트롤 회전의 Yaw회전(Z축 회전) 값과 폰의 Yaw 회전이 서로 연결되어 있음 (이 때문에 캐릭터가 회전하는 것)
    - Detail > Pawn 의 UseControllerRotationYaw, Pitch 로 이를 지정할 수 있음(Yaw는 기본값으로 True, Pitch는 False)

- 스케일 지정 (컨트롤 회전 값의 변화량이 변화)
  - 플레이어 컨트롤러에 내장된 속성 = input Yaw/Roll/Pitch scale 값 변경

## **삼인칭 컨트롤 구현(GTA 방식)**

- 삼인칭 템플릿 플프 기본 설정
  - 캐릭터 이동
    - 상하, 좌우 방향으로 마네킹이 이동, 카메라는 고정
  - 캐릭터 회전
    - 캐릭터가 이동하는 방향으로 마네킹이 회전
  - 카메라 지지대 길이
    - 450cm
  - 카메라 회전
    - 마우스 상하좌우 이동에 따라 카메라 지지대가 상하좌우로 회전
  - 카메라 줌
    - 카메라 시선과 캐릭터 사이에 장애물이 감지되면, 캐릭터가 보이도록 카메라를 장애물 앞으로 줌인
  
- SpringArm
  - 플레이어 컨트롤러의 컨트롤 회전 값을 활용하도록 여러 속성들이 설계돼 있음.

- 회전과 방향
  - 액터의 회전값(0, 0, 0)은 그 액터가 바라보는 방향이 월드의 X축 방향(1, 0, 0)임을 의미함
    - 월드의 X축 방향은 기본 회전 값에 대응하는 방향 값
    - 액터가 회전하면, 액터의 시선 방향도 자연스럽게 다른 값으로 변함
  
  - 스프링암의 회전 값은 컨트롤 회전 값과 동일하게 설정할 수 있음(bUsePawnControlRotation)
    - 즉, 카메라가 바라보는 방향 == 컨트롤 회전 값 

  - 컨트롤 회전 값으로부터 회전 행렬을 생성한 후, 원하는 방향 축을 대입해 캐릭터가 움직일 방향 값을 가져올 수 있음.
    - UE에서 시선 방향은 X축, 우측방향은 Y축을 의미
    ```cpp
        //AABCharacter::UpDown()
    	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
    ```
  - 캐릭터가 움직이는 방향으로 캐릭터를 자동으로 회전시켜주는 캐릭터 무브먼트 컴포넌트의 OrientRotationToMovement 기능 
    ```cpp
        //AABCharacter::SetControlMode()
        CharacterMovement()->bOrientRotationToMovement = true;
        CharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    ```
  
### **주 의**

- UpDown 함수는 카메라가 수직 즉, 회전의 x축 벡터가 (0, 0, 1)일 경우 잘 동작하지 않음
  -  각도에 따라 이동 속도가 달라짐

- 이는 xy 평면에서의 회전행렬 x축 벡터의 정사영을 구하고, 정규화하면 해결할 수 있음
  ```cpp
- 	auto xAxis =  FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	xAxis.Z = 0;
	xAxis = xAxis.GetSafeNormal();
- ```
- 결과값은 캐릭터가 카메라의 시선방향으로 이동할 때 `GetActorForwardVector()` 값과 같음

- LeftRight함수 또한 y축 벡터의 z값을 0으로 설정하고 정규화하면 해결됨.
