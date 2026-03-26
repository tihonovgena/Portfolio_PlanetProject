# Portfolio_PlanetProject
A pet project inspired by games like Astroneer and Outer Wilds.  Date of dev start: February 20, 2026.

Date March 16, 2026
[YoutubeLink](https://youtu.be/pEuiPZgKgmw?si=xW_Hs3763l_jcww2)
![Playground_1](./Screenshots/Image_Playground_1.png)
Description: Added Voxel Plugin 2.0 to create planets and composed all mechanics and solutions into one test level.
Problems and fueature work:
- Requires terraforming tool added for testing.

Date March 15, 2026
[YoutubeLink](https://youtu.be/xbRxen8aRhs?si=GcUwqQ1tPIV_m3VL)
![GravitySysmte_2](./Screenshots/Image_GravitySystem_2.png)
Description: Added character gravity movement.
Problems and fueature work:
- Too fast changing rotation to new gravity attractor, probably better rotate to new source if velocity high enough
- Requires more testing in different sutiations.

Date March 13, 2026
[YoutubeLink](https://youtu.be/8Zs5AkZgMYs?si=TYFQ58QVBwfV2dXV)
![GravitySysmte_1](./Screenshots/Image_GravitySystem_1.png)
Description: Added base planet sphere gravity atttractors affected on physics actors using Chaos Callbacks. Idea based on https://dev.epicgames.com/community/learning/tutorials/lydy/unreal-engine-using-chaos-callbacks-for-a-custom-gravity-system-working-with-round-worlds but refactored into more usable solution.
Problems and fueature work:
- Requrires test what is happaned near the attractor origin, probably enourmouse velocity applied.
- Reduce gravity speed near the attractor origin.

Date February 21, 2026
[YoutubeLink](https://youtu.be/g0srvQBo0Rc?si=LOVJi3EMkopikrvG)
![HQSignalSystem_1](./Screenshots/Image_HQSignalSystem_1.png)
Description: Created HQ Signal System which have sources and recivers, reciver can get signal from source based on distance and attitution signal strength is changed.
Problems and feature work: 
- Lost signal is too fast to reach, better have about 5 seconds delay before signal tottaly lost.
- Requires stress test, how it is going to work if 500 sources applies.
