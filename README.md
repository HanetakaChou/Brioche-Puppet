# Brioche Puppet  

I am currently working on the free and open-source **VTuber** software **Brioche Puppet**.  

As a rendering engineer, the **rendering** is implemented by myself. But the third-party libraries [BulletPhysics](https://github.com/HanetakaChou/BulletPhysics) and [MeidaPipe](https://github.com/HanetakaChou/mediapipe) are used for **ragdoll physics** and **motion capture**.  

## 1\. Asset Import  
  
[Brioche Asset Import](./index.html?file=https://raw.githubusercontent.com/HanetakaChou/Brioche-Asset-Import/refs/heads/master/README.md)  

- [ ] Scene  
    - [ ] Mesh  
        - [ ] [MMD (PMX)](https://github.com/MMD-Blender/blender_mmd_tools/tree/main)  
        - [ ] [glTF (glTF, glB, VRM)](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)  
    - [ ] Animation  
        - [ ] [MMD (VMD)](https://github.com/MMD-Blender/blender_mmd_tools/tree/main)  
        - [ ] [glTF (glTF, glB, VRMA)](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)     
- [ ] Image  
    - [ ] [Albedo Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#FromRGBtoSpectra)  
        - [x] [WebP](https://chromium.googlesource.com/webm/libwebp)  
        - [x] [PNG](https://github.com/pnggroup/libpng)  
        - [x] [JPEG](https://github.com/libjpeg-turbo/libjpeg-turbo)  
    - [ ] [Illuminant Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#x6-RGBIlluminants)  
        - [ ] [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)  
- [ ] Environment Map (Environment Lighting Image)  
    - [ ] [Equirectangular (Latitude-Longitude) Map](https://www.pbr-book.org/3ed-2018/Light_Sources/Infinite_Area_Lights)  
    - [ ] [Octahedral Map](https://www.pbr-book.org/4ed/Light_Sources/Infinite_Area_Lights#ImageInfiniteLights)  
    - [ ] [~~Cube Map~~](https://dev.epicgames.com/documentation/en-us/unreal-engine/creating-cubemaps?application_version=4.27)  

## 2\. Motion    
    
[Brioche Motion](./index.html?file=https://raw.githubusercontent.com/HanetakaChou/Brioche-Motion/refs/heads/master/README.md)  

- [x] Motion Capture  
    - [x] Video Capture (Backend [OpenCV](https://github.com/HanetakaChou/OpenCV))  
    - [x] Motion Detector (Backend [MeidaPipe](https://github.com/HanetakaChou/mediapipe))  
- [ ] Animation  
    - [ ] IK (Inverse Kinematics)  
        - [ ] Reaching IK (Target Position)  
            - [x] Two Joints IK  
            - [ ] ~~Three Joints IK~~  
            - [x] CCD (Cyclic Coordinate Descent) IK  
            - [ ] ~~FABRIK (Forward And Backward Reaching Inverse Kinematics)~~  
            - [ ] ~~Biped Foot IK~~  
            - [ ] ~~Quadruped Foot IK~~  
        - [ ] Aim IK (Target Rotation)  
            - [ ] ~~Look At IK~~  
    - [ ] Skeleton Mapper  
        - [x] Ragdoll Mapping   
        - [ ] ~~Animation Retargeting Mapping~~  
    - [ ] Ragdoll  
        - [ ] Ragdoll Kinematics Controller  
        - [ ] ~~Ragdoll Motors Controller~~  

## 3\. Physics  

[Brioche Physics](./index.html?file=https://raw.githubusercontent.com/HanetakaChou/Brioche-Physics/refs/heads/master/README.md)  

- [x] Backend  
    - [x] [Bullet Physics](https://github.com/HanetakaChou/BulletPhysics)  
    - [ ] [Jolt Physics](https://github.com/HanetakaChou/JoltPhysics)  
- [x] Rigid Body  
    - [x] Shape
        - [x] Sphere  
        - [x] Box  
        - [x] Capsule  
    - [x] Motion  
        - [x] Fixed  
        - [x] Keyframe  
        - [x] Dynamic  
- [ ] Constraint 
  - [x] Fixed    
  - [x] Ball and Socket  
  - [x] Hinge
  - [x] Prismatic  
  - [x] Ragdoll  
  - [ ] ~~6 DOF~~  

## 4\. Rendering  

[Brioche ANARI](./index.html?file=https://raw.githubusercontent.com/HanetakaChou/Brioche-Analytic-Rendering-Interface/refs/heads/master/README.md)  
  
- [ ] Rasterization  
    - [x] [PBR (Microfacet Model: Trowbridge Reitz)](https://pharr.org/matt/blog/2022/05/06/trowbridge-reitz)  
    - [ ] [~~Toon Shading~~](https://github.com/unity3d-jp/UnityChanToonShaderVer2_Project)  
    - [ ] [~~SSS (Skin: Subsurface Scattering)~~](https://zero-radiance.github.io/post/sampling-diffusion/)  
    - [ ] [~~LTC (Area Lighting: Linearly Transformed Cosine)~~](https://github.com/selfshadow/ltc_code)  
    - [x] [IBL (Environment Lighting: Spherical Harmonics + Split Sum Approximation)](https://github.com/HanetakaChou/Environment-Lighting)  
    - [ ] [~~VXGI (Global Illumination: Clipmap Cone Tracing)~~](https://dl.acm.org/doi/abs/10.1145/2775280.2792546)  
- [ ] Ray Tracing  
    - [ ] [ReSITR (Reservoir-Based Spatiotemporal Importance Resampling)](https://intro-to-restir.cwyman.org/)  
    - [ ] [~~SVGF (Denoiser: Spatiotemporal Variance Guided Filtering)~~](https://github.com/NVIDIA-RTX/NRD)  
    - [ ] [~~SSS (Skin: Subsurface Scattering)~~](https://github.com/NVIDIA-RTX/RTXCR/blob/main/docs/RtxcrSssGuide.md)  
    - [ ] [~~SHaRC (Spatially Hashed Radiance Cache)~~](https://github.com/NVIDIA-RTX/SHARC)  
- [ ] Nerual Rendering  
    - [ ] [~~NS (Nerual Shading)~~](https://github.com/NVIDIA-RTX/RTXNS/blob/main/docs/ShaderTraining.md)  
    - [ ] [~~NeRF (Neural Radiance Fields)~~ ](https://www.matthewtancik.com/nerf)  
    - [ ] [~~NRC (Neural Radiance Cache)~~](https://github.com/NVIDIA-RTX/NRC)  
