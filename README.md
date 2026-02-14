# Brioche Puppet  

![](Brioche-Puppet.png)  

[![build windows](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-windows.yml/badge.svg)](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-windows.yml)

[![build linux](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-linux.yml/badge.svg)](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-linux.yml)  

[![build macos](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-macos.yml/badge.svg)](https://github.com/HanetakaChou/Brioche-Puppet/actions/workflows/build-macos.yml)  

[![](Steam.png)](https://store.steampowered.com/app/4286130/Brioche_Puppet/)  

I am currently working on the free and open-source **VTuber** software [Brioche Puppet](https://github.com/HanetakaChou/Brioche-Puppet).  

As a rendering engineer, the **rendering** is implemented by me. But the third-party libraries [BulletPhysics](https://github.com/HanetakaChou/BulletPhysics) and [MeidaPipe](https://github.com/HanetakaChou/mediapipe) are used for **ragdoll physics** and **motion capture**.  

## 1\. Rendering (High Level)  

[Brioche ANARI](https://github.com/HanetakaChou/Brioche-Analytic-Rendering-Interface)  
  
- [ ] Rasterization  
    - [x] [PBR (Lambert + Trowbridge Reitz)](https://pharr.org/matt/blog/2022/05/06/trowbridge-reitz)  
    - [x] [Toon Shading (Celluloid Face Shadow Map (Midsagittal Plane SDF) + Three Base Color + Rim Light)](https://github.com/Unity-Technologies/com.unity.toonshader)  
    - [ ] [~~SSS (Skin Material | Subsurface Scattering)~~](https://github.com/HanetakaChou/Subsurface-Scattering)  
    - [x] [LTC (Area Lighting | Linearly Transformed Cosine)](https://github.com/HanetakaChou/Linearly-Transformed-Cosine)  
    - [x] [SH (Environment Lighting | Spherical Harmonics)](https://github.com/HanetakaChou/Spherical-Harmonic)  
    - [x] [VCT (Global Illumination | Voxel Cone Tracing)](https://github.com/HanetakaChou/Voxel-Cone-Tracing)  
- [ ] Ray Tracing  
    - [ ] [~~ReSITR (Reservoir-Based Spatiotemporal Importance Resampling)~~](https://intro-to-restir.cwyman.org/)  
    - [ ] [~~SVGF (Denoiser | Spatiotemporal Variance Guided Filtering)~~](https://github.com/NVIDIA-RTX/NRD)  
    - [ ] [~~SSS (Skin Material | Subsurface Scattering)~~](https://github.com/NVIDIA-RTX/RTXCR/blob/main/docs/RtxcrSssGuide.md)  
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
        - [x] [glTF (glTF, glB, VRM)](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)  
    - [ ] Animation  
        - [x] [MMD (VMD)](https://github.com/MMD-Blender/blender_mmd_tools/tree/main)  
        - [ ] [~~glTF (glTF, glB, VRMA)~~](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)     
- [ ] Image  
    - [ ] [Albedo Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#FromRGBtoSpectra)  
        - [x] [WebP](https://chromium.googlesource.com/webm/libwebp)  
        - [x] [PNG](https://github.com/pnggroup/libpng)  
        - [x] [JPEG](https://github.com/libjpeg-turbo/libjpeg-turbo)  
        - [ ] TIFF  
        - [x] [TGA](https://tgalib.sourceforge.net/)  
        - [ ] BMP  
        - [ ] PNM  
    - [ ] [Illuminant Image](https://www.pbr-book.org/4ed/Radiometry,_Spectra,_and_Color/Color#x6-RGBIlluminants)  
        - [x] [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)  
        - [ ] [Radiance HDR](https://radsite.lbl.gov/radiance/refer/filefmts.pdf)  

## 4\. Animation    
    
[Brioche Motion](https://github.com/HanetakaChou/Brioche-Motion)  

- [x] Motion Capture  
    - [x] Video Capture (Backend [OpenCV](https://github.com/BriochePuppet/OpenCV))  
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
