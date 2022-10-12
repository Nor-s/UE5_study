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


