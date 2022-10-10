# **CH13 프로젝트의 설정과 무한 맵의 제작**

- 효과적으로 소스를 관리하도록 프로젝트 구조를 변경
- 게임 설정에 관련된 데이터를 별도의 모듈로 분리
- 언리얼 엔진의 설정 시스템을 이용해 게임의 기본 데이터를 INI  파일로 관리
- 레벨의 요소를 섹션이라는 단위로 개편, 무한으로 증가하는 레벨을 설계

## 프로젝트의 정리

- 언리얼 모듈 소스 코드 구조로 변경
    - Classes 폴더: 언리얼 오브젝트에 관련한 헤더들
    - Public 폴더: 외부에 공개하는 선언 파일
    - Private 폴더: 외부에 공개하지 않는 파일


- 윈도우 탐색기 => 프로젝트의 Source/ArenaBattle 폴더안의 구조를 변경 > uproject에서 sln 생성 > 빌드

## 모듈 추가

- 언리얼 에디터는 C++ 프로젝트를 생성할 때 주 게임 모듈을 자동으로 생성, but 추가 모듈은 x

  - 새로운 모듈 추가 == 언리얼 빌드 규칙 이해 및 준수, 폴더와 파일
    1. 모듈 폴더와 빌드 설정파일: 모듈 폴더, 모듈명.Build.cs
    2. 모듈 정의 파일: 모듈명 .cpp (+ 모듈명 .h)
      ```cpp 
      #include "ArenaBattleSetting.h"
      #include "Modules/ModuleManager.h"
  
      IMPLEMENT_MODULE( FDefaultModuleImpl, ArenaBattleSetting );
      ```

    3. 모듈 빌드: ArenaBattle.Target.cs, ArenaBattleEditor.Target.cs 파일 정보 수정
       - 이들은 게임 빌드와 에디터 빌드 설정을 지정해줌
    4. 모듈 로딩: 빌드 후 Binaries 폴더에 DLL 파일 확인, UE 에디터가 이를 로딩하도록해야함
       - uproject 파일에 새로운 모듈에 대한 정보를 기입해야함
       - 다른 모듈보다 먼저 로딩 설정 : "LoadingPhase": "PreDefault"
       - ArenaBattle모듈에 의존성 추가
       ```
  		{
			"Name": "ArenaBattleSetting",
			"Type": "Runtime",
			"LoadingPhase": "PreDefault",
		},
        ...
            "AdditionalDependencies": [
                  ... ,
                "ArenaBattleSetting"
            ],
       ```
  
### Build.cs 파일 수정

- PrivateDependencyModuleNames 에 "ArenaBattleSetting" 추가

## INI 설정과 애셋의 지연 로딩

### 새로운 모듈 ABCharacterSetting 의 역할
 
- 앞으로 사용할 캐릭터 애셋의 목록을 보관, 이를 생성자 코드에서 지정하면, 애셋이 변경될 때마다 다시 컴파일해야함

- 언리얼 오브젝트의 기본 값을 유연하게 관리하는 기능 == 외부 INI 파일에서 기본 속성 값을 지정하는 기능
  - INI 파일에서 ABCharacterSetting 기본 속성 값을 지정

- 애셋
  - 경로 정보 => 프로그램에서 참조해 로딩 가능
  - FSoftObjectPath == 애셋 경로 정보 저장
  
### INI 로드

- UCLASS 매크로에 config 키워드 추가, INI 파일 이름 지정, PROPERTY 속성에 config 키워드 선언
  ```cpp
  UCLASS(config=ArenaBattle)
  class ARENABATTLESETTING_API UABCharacterSetting : public UObject
  {
      GENERATED_BODY()
  
  public:
     UABCharacterSetting();
  
  	 UPROPERTY(config)
  	 TArray<FSoftObjectPath> CharacterAssets;
  };

  ```
  - 로드할 INI 파일을 Config 폴더에 위치시킴 (f"Default{config}.ini")

### CDO (Class Default Object)

- 언리얼 엔진이 초기화되면, 엔진 구동에 필요한 모듈이 순차적으로 로딩됨
- 모듈이 로딩되면서, 모듈은 자신에게 속한 모든 UE 오브젝트의 기본값을 지정해 생생해내는 클래스
- 따라서, 엔진이 초기화되면, 모든 CDO가 메모리에 올라온 상태임
- ``GetDefault`` 함수를 사용해 가져올 수 있음 (언제든지 호출가능)
    ```cpp
  	auto DefaultSetting = GetDefault<UABCharacterSetting>();
	if(DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset: DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("CharacterAsset : %s"), *CharacterAsset.ToString());
		}
    }
    ```
  
- ``FStreamableManager``: 비동기방식 애셋 로딩 클래스
    - 이 매니저 클래스는 프로젝트에서 하나만 있는게 좋음
      - 유일한 인스턴스인 ABGameInstance 클래스에서 멤버 변수로 선언
    - AsyncLoad 함수: 비동기적으로 애셋 로딩
        - FStreamableDelegate 델리게이트를 인자로 넘겨줄 수 있음.
    - CreateUObject: 즉석에서 델리게이트 생성, 함수와 연동 후 넘겨줌
    - UAssetManager 오브젝트에 이미 선언돼있음
      - Engle/AssetManager.h 헤더 파일 선언 후, UAssetManager::GetStreamableManager() 함수를 대신 사용 가능
  


> 참고: 싱글톤 클래스는 프로젝트 설정 > 일반 세팅 > Default classes 에서 지정 가능


## **무한 맵 생성**

- 레벨을 섹션이라는 단위로 나누고, 하나의 섹션을 클리어하면 새로운 섹션이 등장하는 무한 맵 스테이지 제작
- 섹션 액터가 하는 일
  - 섹션의 배경과 네 방향 문 제공
  - 섹션 진입 모든 문 닫음
  - 문을 닫고 일정 시간 후에 중앙에서 NPC 생성
  - 문을 닫고 일정 시간 후에 아이템 상자를 랜덤한 위치에 생성
  - NPC가 모두 죽으면 모든 문 개방
  - 통과한 문으로 이어지는 새로운 섹션을 생성

- 예제의 배경은 출입 각 방향에 소켓이 있음
  - 철문 애셋을 소켓에 부착


### Trigger

- ABCharacter만 감지하는 ABTrigger 콜리전 프리셋 추가
- 플레이어 입장 감지 + 섹션 클리어 후 출구 선택할 때 사용


### 스테이트 머신 설정

- 준비
  - 액터의 시작 스테이트
  - 문을 열어놓고 대기하다, 중앙의 박스 트리거로 플레이어의 진입을 감지하면 전투 스테이트로 이동
- 전투
  - 문을 닫고 일정 시간이 지나면 NPC를 소환
  - 일정 시간이 지나면, 랜덤한 위치에 아이템 상자 생성
  - 소환한 NPC가 죽으면 완료 스테이트로 이동
- 완료
  - 닫힌 문을 엶
  - 각 문의 트리거가 플레이어 감지하면, 이동한 문의 방향으로 새로운 섹션 소환


