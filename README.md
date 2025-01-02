# Brioche-Puppet  

**Brioche Puppet** is the VTuber application based on **Brioche** (which is the **Platform Abstraction Layer** over the underlying **Vulkan** and **Direct3D12** API).  

## Rendering  

**Brioche Puppet** supports the following rendering approaches:  
- [ ] Toon (MikuMikuDance)  
- [ ] [PBR (Physically Based Rendering)](https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Archived/KHR_materials_pbrSpecularGlossiness/examples)  
- [ ] [Disney SSS (Subsurface Scattering)](https://zero-radiance.github.io/post/sampling-diffusion/)  
- [ ] [Volume Rendering (Participating Media) SSS (Subsurface Scattering)](https://pbr-book.org/3ed-2018/Light_Transport_II_Volume_Rendering/Subsurface_Scattering_Using_the_Diffusion_Equation)  

## Assets  

**Brioche Puppet** supports the following **scene** formats:  
- [ ] [PMX (MikuMikuDance)](https://github.com/UuuNyaa/blender_mmd_tools)    
- [ ] [glTF](https://github.com/KhronosGroup/glTF-Sample-Viewer)  

**Brioche Puppet** supports the following **image** formats:  
- [ ] [DDS](https://github.com/microsoft/DirectXTex) (BC7_UNORM, BC7_SRGB)  
- [ ] [PVR](https://github.com/powervr-graphics/Native_SDK/blob/master/framework/PVRCore/textureio/TextureReaderPVR.cpp) (BC7_UNORM, BC7_SRGB, ASTC_4x4_UNORM, ASTC_4x4_SRGB)  
- [ ] [PNG](https://github.com/pnggroup/libpng) (R8G8B8A8_UNORM, R8G8B8A8_UNORM)  
- [ ] [JPEG](https://github.com/libjpeg-turbo/libjpeg-turbo) (R8G8B8A8_UNORM, R8G8B8A8_UNORM)  

## Animation Retargeting  

**Brioche Puppet** supports the **animation retargeting** based on the predefined names:  

**Brioche Puppet** supports the following predefined **joint** names:  

[MMD Internal Dictionary](https://github.com/UuuNyaa/blender_mmd_tools/blob/main/mmd_tools/translations.py#L30)  

[VRM Defined Bones](https://github.com/vrm-c/vrm-specification/blob/master/specification/0.0/README.md#defined-bones)  

[VRM-Addon-for-Blender](https://github.com/saturday06/VRM-Addon-for-Blender/blob/main/src/io_scene_vrm/common/human_bone_mapper/mmd_mapping.py)  

JP | EN  
:-: | :-:  
首 | neck  
頭 | head  
左目 | leftEye  
右目 | rightEye  
顎 | jaw  
センター | hips  
上半身 | spine  
上半身2 | chest  
N/A | upperChest  
左肩 | leftShoulder  
右肩 | rightShoulder  
左腕 | leftUpperArm  
右腕 | rightUpperArm  
左ひじ | leftLowerArm  
右ひじ | rightLowerArm  
左手首 | leftHand  
右手首 | rightHand  
左足 | leftUpperLeg  
右足 | rightUpperLeg  
左ひざ | leftLowerLeg  
右ひざ | rightLowerLeg  
左足首 | leftFoot  
右足首 | rightFoot  
左つま先 | leftToes  
右つま先 | rightToes  
左親指０ | leftThumbProximal  
左親指１ | leftThumbIntermediate  
左親指２ | leftThumbDistal  
右親指０ | rightThumbProximal  
右親指１ | rightThumbIntermediate  
右親指２ | rightThumbDistal  
左人指１ | leftIndexProximal  
左人指２ | leftIndexIntermediate  
左人指３ | leftIndexDistal  
右人指１ | rightIndexProximal  
右人指２ | rightIndexIntermediate  
右人指３ | rightIndexDistal  
左中指１ | leftMiddleProximal  
左中指２ | leftMiddleIntermediate  
左中指３ | leftMiddleDistal  
右中指１ | rightMiddleProximal  
右中指２ | rightMiddleIntermediate  
右中指３ | rightMiddleDistal  
左薬指１ | leftRingProximal  
左薬指２ | leftRingIntermediate  
左薬指３ | leftRingDistal  
右薬指１ | rightRingProximal  
右薬指２ | rightRingIntermediate  
右薬指３ | rightRingDistal  
左小指１ | leftLittleProximal  
左小指２ | leftLittleIntermediate  
左小指３ | leftLittleDistal  
右小指１ | rightLittleProximal  
右小指２ | rightLittleIntermediate  
右小指３ | rightLittleDistal  

**Brioche Puppet** supports the following predefined **morph target** names:  

[VRM BlendShape Groups](https://github.com/vrm-c/vrm-specification/blob/master/specification/0.0/README.md#blendshape-groups-jsonextensionsvrmblendshapemasterblendshapegroups)  

JP | EN  
:-: | :-:  
N/A | Neutral  
あ | A  
い | I  
う | U  
え | E  
お | O  
まばたき | Blink  
笑い | Joy  
怒り | Angry  
困る | Sorrow  
喜び | Fun  
上 | LookUp  
下 | LookDown  
上左 | LookLeft  
上右 | LookRight  
ウィンク | Blink_L  
ウィンク右 | Blink_R  

## Procedural Animation  

### Face Landmark Detection  

**Brioche Puppet** supports the **face landmark detection** by the following third party libraries:  
- [ ] [OpenSeeFace](https://github.com/emilianavt/OpenSeeFace)  
- [ ] MediaPipe  

### Ragdoll Animation  

TODO (Bullet by Mikumikudance)  
