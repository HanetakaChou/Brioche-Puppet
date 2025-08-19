# Brioche Puppet  

![](Brioche-Puppet.png)  

[![build windows](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-windows.yml/badge.svg)](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-windows.yml)

I am currently working on the free and open-source **VTuber** software [Brioche Puppet](https://github.com/HanetakaChou/Brioche-Puppet).  

As a rendering engineer, the **rendering** is implemented by me. But the third-party libraries [BulletPhysics](https://github.com/HanetakaChou/BulletPhysics) and [MeidaPipe](https://github.com/HanetakaChou/mediapipe) are used for **ragdoll physics** and **motion capture**.  

## 1\. Rendering (High Level)  

[Brioche ANARI](https://github.com/HanetakaChou/Brioche-Analytic-Rendering-Interface)  
  
- [ ] Rasterization  
    - [x] [PBR (Microfacet Model: Trowbridge Reitz)](https://pharr.org/matt/blog/2022/05/06/trowbridge-reitz)  
    - [ ] [~~Toon Shading~~](https://github.com/unity3d-jp/UnityChanToonShaderVer2_Project)  
    - [ ] [~~SSS (Skin: Subsurface Scattering)~~](https://zero-radiance.github.io/post/sampling-diffusion/)  
    - [ ] [~~LTC (Area Lighting: Linearly Transformed Cosine)~~](https://github.com/selfshadow/ltc_code)  
    - [ ] [IBL (Environment Lighting)](https://github.com/HanetakaChou/Environment-Lighting)  
        - [x] [Diffuse (Spherical Harmonics)](https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Vendor/EXT_lights_image_based/README.md#irradiance-coefficients)  
        - [ ] [~~Specular (Split Sum Approximation)~~](https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Vendor/EXT_lights_image_based/README.md#prefiltered-specular-radiance-cubemaps)      
    - [x] [VXGI (Voxel Cone Tracing)](https://hanetakachou.github.io/index.html?file=Rendering/Rasterization/Lighting/Voxel-Cone-Tracing.md)  
- [ ] Ray Tracing  
    - [ ] [~~ReSITR (Reservoir-Based Spatiotemporal Importance Resampling)~~](https://intro-to-restir.cwyman.org/)  
    - [ ] [~~SVGF (Denoiser: Spatiotemporal Variance Guided Filtering)~~](https://github.com/NVIDIA-RTX/NRD)  
    - [ ] [~~SSS (Skin: Subsurface Scattering)~~](https://github.com/NVIDIA-RTX/RTXCR/blob/main/docs/RtxcrSssGuide.md)  
    - [ ] [~~SHaRC (Spatially Hashed Radiance Cache)~~](https://github.com/NVIDIA-RTX/SHARC)  
- [ ] Nerual Rendering  
    - [ ] [~~NS (Nerual Shading)~~](https://github.com/NVIDIA-RTX/RTXNS/blob/main/docs/ShaderTraining.md)  
    - [ ] [~~NeRF (Neural Radiance Fields)~~ ](https://www.matthewtancik.com/nerf)  
    - [ ] [~~NRC (Neural Radiance Cache)~~](https://github.com/NVIDIA-RTX/NRC)  

## 2\. Rendering (Low Level)  

[Brioche PAL](https://github.com/HanetakaChou/Brioche-Platform-Abstraction-Layer)  

- [ ] Backend  
    - [x] Direct3D12  
    - [x] Vulkan  
    - [ ] Metal  

## 3\. File Format  
  
[Brioche Asset Import](https://github.com/HanetakaChou/Brioche-Asset-Import)  

- [ ] Scene  
    - [ ] Mesh  
        - [x] [MMD (PMX)](https://github.com/MMD-Blender/blender_mmd_tools/tree/main)  
        - [ ] [glTF (glTF, glB, VRM)](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)  
    - [ ] Animation  
        - [x] [MMD (VMD)](https://github.com/MMD-Blender/blender_mmd_tools/tree/main)  
        - [ ] [glTF (glTF, glB, VRMA)](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)     
- [ ] Image  
    - [ ] [Albedo Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#FromRGBtoSpectra)  
        - [x] [WebP](https://chromium.googlesource.com/webm/libwebp)  
        - [x] [PNG](https://github.com/pnggroup/libpng)  
        - [x] [JPEG](https://github.com/libjpeg-turbo/libjpeg-turbo)  
        - [ ] TIFF  
        - [x] [TGA](https://tgalib.sourceforge.net/)  
        - [ ] BMP  
        - [ ] PNM  
    - [x] [Illuminant Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#x6-RGBIlluminants)  
        - [x] [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)  
        - [ ] [Radiance HDR](https://radsite.lbl.gov/radiance/refer/filefmts.pdf)  

## 4\. Animation    
    
[Brioche Motion](https://github.com/HanetakaChou/Brioche-Motion)  

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

## 5\. Physics  

[Brioche Physics](https://github.com/HanetakaChou/Brioche-Physics)  

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
