# Chap47_AnimationTweening

- Animation Lerp : Animation Frame간 보간
    - float값을 이용하여 진행 시간에 따른 보간값 사용.
    - 프레임과 프레임 사이에 해당 동작의 자연스러운 변경을 위해 두개의 프레임을 해당 프레임간 소요시간에 따라서 lerp → 자연스러운 프레임간 이동
    - 해당 기능 미구현시 Animation이 프레임간 뚝뚝 끊기는 현상 발생,
    - TweenDesc.KeyFrameData.PlayTime를 매개변수로 활용.
    - Shader/45_Animation::SetAnimationWorld()
        
        ```
        
        for (int i = 0; i < 4; i++)
        {
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[0], clip[0], 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[0], clip[0], 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[0], clip[0], 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[0], clip[0], 0));
        	curr = Matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
            next = Matrix(n0, n1, n2, n3);
        }
        ```
        
    
- animation Tweening  : Animation Clip간 보간
    - Animation Lerp의 Animation Clip간 버전.
    - Animation간 변경시 한 동작의 애니메이션 변경시 뚝 끊기는 현상 발생.
    - 따라서 두개의 Animation Clip을 Shader에 전달, 해당 clip간 값 보간.
    - Shader에 두개의 Animation Clip 전달, 두번째 Clip의 Count가 -1이면 보간 X, -1보다 크면 해당 두개의 Animation 보간.
    - Shader/45_Animation::SetAnimationWorld()
        
        ```glsl
        [flatten]
        if(clip[1] > -1)
        {
        		c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[1], clip[1], 0));
        		c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[1], clip[1], 0));
        		c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[1], clip[1], 0));
        		c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[1], clip[1], 0));
        		
        		curr = Matrix(c0, c1, c2, c3);
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = Matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(curr, next, time[1]);
        
            currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
          }
        
        ```
        
    

[https://www.youtube.com/watch?v=cdkoax93658&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=15](https://www.youtube.com/watch?v=cdkoax93658&list=PLbo7gRk05HhjqJzGmj0rJsOUkkMhkIaJE&index=15)