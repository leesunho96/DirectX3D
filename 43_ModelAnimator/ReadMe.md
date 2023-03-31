# Chap42 AnimationReadClip

- Animation : Clip의 모음
- Clip : 한 프레임에서의 mesh의 bone의 위치 모음.
- Animation은 Clip을 연속적으로 재생.

- Mixamo의 경우 모든 clip을 별도로 저장.
- 그러나 하나의 clip에 여러개의 clip을 연속적으로 저장하는 경우도 존재.
- aiScene::mNumAnimations : 해당 애니메이션의 clip의 수.
- aiAnimation : 해당 clip 제어를 위한 객체.

- 일반적으로 animation은 T-Pose 사용, UE의 경우 A-Pose 사용