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

 
## **비헤이비어 트리 시스템** 

- AIControlloer에 특정 로직을 부여할 수 있지만, 더 복잡한 행동 패턴 구현을 위해 비헤이비어 트리를 사용함
- 비헤이비어 트리
  - NPC가 해야 할 행동을 분석, 우선순위가 높은 행동부터 NPC가 실행할 수 있도록 트리 구조로 설계하는 기법

- 비헤이비어 트리 제작
  - 비헤이비어 트리 + 블랙보드 에셋을 생성
  - 블랙보드: 인공지능의 판단에 사용하는 데이터 집합 
  - 비헤이비어 트리: 블랙보드 데이터에 기반해 설계한 비헤이비어 트리의 정보를 저장한 애셋


- 비헤이비어 트리
  - 태스크는 독립적으로 실행될 수 없고, 반드시 컴포짓 노드를 거쳐 실행되어야함
  - 컴포짓 노드: 셀렉터, 시퀀스
    - 시퀀스: 연결된 태스크들이 False 결과가 나올 때까지 왼쪽에서 오른쪽으로 태스크를 계속 실행함
  - C++ 에서 사용하려면, AIModule 모듈을 BUild.cs 파일에 추가해야함

- UseBlackboard, RunBehaviorTree 함수를 사용하면, 비헤비어 트리를 C++ 상에서 동작시킬 수 있음


### 참고: lvalue to rvalue

- 책 예제와 다르게 언리얼의 최신 버전에서 AAIController::Blackboard의 타입은 TObjectPtr<UBlackboardComponent>
- 이 타입은 Raw 포인터 대신에 사용하는 것
- 이는 Raw 포인터로 `자동 변환` 됨
- 그러므로 예제를 그대로 넣으면 오류가 뜸
  - ``Non-const lvalue reference to type UBlackboardComponent* cannot bind to lvalue of type TObjectPtr<UBlackboardComponent>```
  - Get 사용시: ``Non-const lvalue reference to type UBlackboardComponent* cannot bind to rvalue of type UBlackboardComponent*``
- 그 이유는 암시적 형변환의 lvalue to rvalue 때문인 것 같음.

- https://stackoverflow.com/questions/54032945/error-cannot-bind-non-const-lvalue-reference-of-type-int-to-an-rvalue-of-typ

- https://blog.seulgi.kim/2017/06/cpp11-value-category.html

- https://dydtjr1128.github.io/cpp/2019/06/10/Cpp-values.html


## NPC 순찰 기능

- 블랙보드의 데이터를 비헤이비어 트리에서 활용
- 순찰기능구현에 필요한 블랙보드 키값
  1. NPC 생성됐을 때의 위치값 (블랙보드에서 이를 저장, Vector 키 생성, HomePos 이름)
  2. NPC가 순찰할 위치 정보(블랙보드에서, Vector 키, PatrolPos 이름)



- 순찰 이동 위치 PatrolPos 데이터 생성해야함
  - 순찰할 때마다 바뀌므로, 태스크를 제작해 비헤이비어 트리에서 블랙보드에 값을 쓰도록 설계하는 것이 좋음

- Task
  - 새로운 C++ 클래스 > BTTaskNode 
  - Build.cs 에 "GameplayTasks" 추가
 
- ExecuteTask
  - 비헤이비어 트리가 태스크를 실행할 때 호출하는 태스크 멤버 함수
  - 리턴값은 다음 중 하나
    - Aborted: 태스크 실행 중에 중단됨, 실패
    - Failed: 태스크를 수행했지만, 실패
    - Succeeded: 태스크를 성공적으로 수행
    - InProgress: 태스크를 계속 수행하고 있음, 테스크의 실행 결과는 나중에 알림
  - 이 리턴값에 따라 컴포짓 내에 있는 다음 태스크를 수행할 것인지 결정 (시퀀스 컴포짓은 태스크를 실패할 때까지 계속 실행)

- MoveTo 태스크
  - UE가 제공하는 태스크
  - 위치를 지정 (PatrolPos) 하면, 빙의하고 있는 폰을 그 위치로 이동시킴



## NPC의 추격 기능 구현

- 정찰 중에 플레이어 발견시 추격하도록 기능을 추가
- 블랙보드에 플레이어 정보를 저장할 데이터 Object 타입으로 Target 변수 추가
- 기반 클래스 ABCharacter 설정

- 셀렉터 컴포짓
  - 행동패턴은 발견 여부에 따라 변경되므로 이를 사용하는게 적절

- 추격과 정찰 중에서 추격에 더 우선권(왼쪽에 노드를 둠)을 주고, 추격은 Target 위치로

- 감지 및 추격 기능
  - 서비스 노드 사용

- 서비스 노드
  - 새로운 C++ 클래스 > BTService
  - 독립적으로 동작하지 않고, 컴포짓 노드에 부착되는 노드
  - 해당 컴포짓에 속한 태스크들이 실행되는 동안, 반복적인 작업을 실행하는 데 적합
    - 속한 컴포짓 노드가 활성화될 경우 주기적으로 ``TickNode`` 함수를 호출
    - ``TickNode`` 함수 호출 주기는 ``Interval`` 속성으로 설정
   
- 플레이어를 감지하는 루틴 반복 == 플레이어를 감지하는 서비스 노드를 생성, 셀렉터 컴포짓에 추가 

- OverlapMultiByChannel 함수
  - 반경 내에 모든 캐릭터를 감지
  - 반경 내에 감지된 모든 캐릭터 정보목록을 관리하는데 적합한 TArray 로 전달됨

- 컴포짓 우클릭, 서비스 메뉴 Detect 선택 및 부착

- SetValueAsObject()로 플레이어 캐릭터 타겟으로 설정

### 부드러운 회전

- NPC를 위한 ControlMode 추가
- NPC 이동 방향에 따라 회전하도록 무브먼트 변경
  - 이는 GTA 방식과 유사 (움직이는 방향으로 캐릭터 회전)


### 셀렉터 데코레이터 로직 구성

- 서비스가 실행된 결과에 따라 왼쪽의 추격, 오른쪽의 정찰 로직이 나눠지도록 트리 로직 구성
- 서비스 결과 == Target 키값의 여부
- 데코레이터 노드
  - 블랙보드 값을 기반으로 특정 컴포짓의 실행 여부를 결정하는 노드 (블랙보드 데코레이터 노드)
  - 컴포짓에 삽입가능

- 추격 > 블랙보드 데코레이터 선택 > 디테일 > Target 키값 설정
  - Blackboard Key = Target
  - Key Query = Is Set
  - Observer aborts = Self (이를 설정하지 않으면, 시야를 벗어나도 계속 쫒아옴)
  - Notify Observer = OnValueChange (키값 변경이 감지되면, 현재 컴포짓 노드의 실행을 바로 취소)

- 정찰 > 블랙보드 데코레이터 선택 > 디테일 > Target 키값 설정
  - KeyQuery = Is Not Set
  - 관찰자 설정은 추격과 동일
  - NPC는 정찰 중에 플레이어 감지할 경우 정찰 바로 중단, 추격 시작


## NPC의 공격

- 왼쪽 추격로직에서 따라 잡으면 공격하는 기능을 추가
- 셀렉터 컴포짓 추가하여 추격과 공격을 분기
- 블랙보드의 값을 참조하지 않고, 타겟이 공격범위에 있는지 판단하는 데코레이터
  - CalculateRawConditionValue 함수를 오버라이드 (const로 선언되었음을 주의)

### BTTaskNode

- 공격 태스크
- 공격 애니메이션 끝날 때까지 대기
- ExecuteTask의 결과 값을 InProgress로 반환, 공격이 끝났을 때 태스크 끝을 알림
- FinishLatentTask를 호출하여 태스크 종료
  - Tick 기능을 활성화해야함

### 회전 문제

- NPC 뒤에 있어도 계속해서 NPC는 같은 방향을 공격함
- BTTaskNode 태스크 생성, 플레이어 폰을 향해 일정한 속도로 회전하는 FMath::RInterpTo 함수 사용
- 공격로직 컴포짓을 심플 패러럴 컴포짓으로 대체 후, 공격을 메인, 회전을 보조로 설정
- 심플 패러럴 컴포짓 => 공격과 회전을 동시 실행



### 오류
- 참고 https://mingyu0403.tistory.com/266
```
Fatal error: [File:D:\build\++UE5\Sync\Engine\Source\Runtime\Core\Private\Windows\WindowsPlatformMisc.cpp] [Line: 441]
Pure virtual function being called




0x00007fff5c9964f6 UnrealEditor-Core.dll!PureCallHandler() [D:\build\++UE5\Sync\Engine\Source\Runtime\Core\Private\Windows\WindowsPlatformMisc.cpp:443]
0x00007fffb5356e44 VCRUNTIME140.dll!UnknownFunction []
0x00007fff5ba4c546 UnrealEditor-CoreUObject.dll!UGCObjectReferencer::VerifyGCObjectNames() [D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\Misc\GCObjectReferencer.cpp:199]
0x00007fff5ba1a151 UnrealEditor-CoreUObject.dll!TBaseUObjectMethodDelegateInstance<0,UGCObjectReferencer,void __cdecl(void),FDefaultDelegateUserPolicy>::ExecuteIfSafe() [D:\build\++UE5\Sync\Engine\Source\Runtime\Core\Public\Delegates\DelegateInstancesImpl.h:611]
0x00007fff5ba8217d UnrealEditor-CoreUObject.dll!TMulticastDelegate<void __cdecl(void),FDefaultDelegateUserPolicy>::Broadcast() [D:\build\++UE5\Sync\Engine\Source\Runtime\Core\Public\Delegates\DelegateSignatureImpl.inl:967]
0x00007fff5bc196c8 UnrealEditor-CoreUObject.dll!CollectGarbageInternal() [D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\GarbageCollection.cpp:2232]
0x00007fff5bc18eec UnrealEditor-CoreUObject.dll!CollectGarbage() [D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\GarbageCollection.cpp:2543]
0x00007fff359b1540 UnrealEditor-UnrealEd.dll!UEditorEngine::EndPlayMap() [D:\build\++UE5\Sync\Engine\Source\Editor\UnrealEd\Private\PlayLevel.cpp:415]
0x00007fff3539fa40 UnrealEditor-UnrealEd.dll!UEditorEngine::Tick() [D:\build\++UE5\Sync\Engine\Source\Editor\UnrealEd\Private\EditorEngine.cpp:2097]
0x00007fff35d2aac6 UnrealEditor-UnrealEd.dll!UUnrealEdEngine::Tick() [D:\build\++UE5\Sync\Engine\Source\Editor\UnrealEd\Private\UnrealEdEngine.cpp:474]
0x00007ff7691282f6 UnrealEditor.exe!FEngineLoop::Tick() [D:\build\++UE5\Sync\Engine\Source\Runtime\Launch\Private\LaunchEngineLoop.cpp:5215]
0x00007ff769140d9c UnrealEditor.exe!GuardedMain() [D:\build\++UE5\Sync\Engine\Source\Runtime\Launch\Private\Launch.cpp:183]
0x00007ff769140e8a UnrealEditor.exe!GuardedMainWrapper() [D:\build\++UE5\Sync\Engine\Source\Runtime\Launch\Private\Windows\LaunchWindows.cpp:147]
0x00007ff769143c4d UnrealEditor.exe!LaunchWindowsStartup() [D:\build\++UE5\Sync\Engine\Source\Runtime\Launch\Private\Windows\LaunchWindows.cpp:283]
0x00007ff769155564 UnrealEditor.exe!WinMain() [D:\build\++UE5\Sync\Engine\Source\Runtime\Launch\Private\Windows\LaunchWindows.cpp:330]
0x00007ff769158736 UnrealEditor.exe!__scrt_common_main_seh() [d:\a01\_work\6\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288]
0x00007fffd7f0244d KERNEL32.DLL!UnknownFunction []
0x00007fffd980df78 ntdll.dll!UnknownFunction []
```

## 궁금점

- 델리게이트.. AddLambda 등으로 등록하고, 등록한 객체가 사라지면 추가한 람다 등 도 사라지는지? 등록한 채인지?
    - 참고: https://kyoun.tistory.com/143
    - [참고](https://docs.unrealengine.com/5.0/en-US/multicast-delegates-in-unreal-engine/):It is always safe to call Broadcast() on a multi-cast delegate, even if nothing is bound. The only time you need to be careful is if you are using a delegate to initialize output variables, which is generally very bad to do.
    - [참고](https://forums.unrealengine.com/t/bind-delegate-with-one-parameter/295500/13)
    - [참고](https://forums.unrealengine.com/t/do-weaklambdas-need-extra-steps-to-unbind/483656): AddLambda 등에서 리턴하는 FDelegateHandle 로 바인딩해제 해야함
- 예제에서 AddLambda가 여러번 호출되는 함수안에 포함되어 있기에 아래 코드로 실험해봄

  ```cpp
    ABCharacter->OnAttackEnd.AddLambda([this]() -> void {
        ABLOG(Warning, TEXT("Attack End"));
        IsAttacking = false;
    }); 
  ```

  - 로그 결과는 다음과 같음.
    - 공격마다 람다호출이 증가함을 확인 가능
  
  ``` bash
  // 결과
  ArenaBattle: Warning: AABCharacter::PostInitializeComponents::<lambda_e240e047e357246560126bb6d46bfdd0>::operator ()(175) OnNextAttackCheck
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ArenaBattle: Warning: AABCharacter::AttackCheck(396) Hit Actor Name : ABCharacter_0
  ArenaBattle: Warning: AABCharacter::TakeDamage(199) Actor : ABCharacter_0 took Damage : 10.000000
  ArenaBattle: Warning: AABCharacter::PostInitializeComponents::<lambda_e240e047e357246560126bb6d46bfdd0>::operator ()(175) OnNextAttackCheck
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ArenaBattle: Warning: AABCharacter::AttackCheck(396) Hit Actor Name : ABCharacter_0
  ArenaBattle: Warning: AABCharacter::TakeDamage(199) Actor : ABCharacter_0 took Damage : 10.000000
  ArenaBattle: Warning: AABCharacter::PostInitializeComponents::<lambda_e240e047e357246560126bb6d46bfdd0>::operator ()(175) OnNextAttackCheck
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ArenaBattle: Warning: UBTTask_Attack::ExecuteTask::<lambda_03082f79029a61ba81a4d9efea3f9cd4>::operator ()(24) Attack End
  ```
