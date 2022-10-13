# **CH14 게임 플레이 제작**

- 언리얼 엔진이 제공하는 플레이어 스테이트, 게임 스테이트
- 스테이트들로 데이터를 체계적으로 관리


## 캐릭터의 스테이트 설정

- 블루프린트에서 사용하기 위한 열거형 정의 
  - UENUM(BlueprintType)을 선언하고, uint8로 underly type을 지정

```
// ArenaBattle.h
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};
```


## 플레이어 데이터와 UI 연동

- PlayerState
  - 게임 점수와 같은 플레이어의 정보를 관리하기 위한 용도
  - FString 타입의 PlayerName 속성
  - float 타입의 Score 속성
  



- 게임 모드의 PlayerStateClass에 PlayerState 클래스 지정
  - 엔진은 플레이어 컨트롤러가 초기화될 때 함께 해당 클래스의 인스턴스를 생성
  - 그 포인터 값을 플레이어 컨트롤러의 PlayerState의 속성에 저장함
  - 플레이어 컨트롤러 설정 완료시점: 게임 모드의 PostLogin 함수


## 스코어 구현

- 스코어 
  - 입장시 0
  - NPC 처치 1 

- 길 찾기 영역 크게 설정 (내비메쉬 brush settings)

### 게임 데이터 관리

- 게임 스테이트 클래스(AGameStateBase) 사용
- 게임 데이터만 분리 가능

### LastHitBy

- 마지막으로 대미지를 입힌 컨트롤러의 기록
- 액터가 제거될 때 마지막에 피격을 가한 플레이어의 정보를 얻어올 수 있음

> 경험치 또한 LastHitBy를 사용해 처리하는 것이 좋음

### GetPlayerControllerIterator

- 현재 게임에 참여 중인 플레이어 컨트롤러의 목록 (월드에서 제공)
