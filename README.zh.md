# LegacyParticleAPI

[English](README.md) | 简体中文

**如果您想看到粒子，请先在服务器上安装 [LiteLoaderBDS-CUI](https://github.com/OEOTYAN/LiteLoaderBDS-CUI/releases)**  
您只需将资源包解压缩到 `./plugins/LegacyParticleAPI/ResourcePacks` 中即可

## 使用方法

ParticleAPI 为插件开发提供了丰富易用的粒子生成接口，直观的设计和高效稳定的性能支持开发者编写更好的插件。

包含头文件 `#include "LegacyParticleAPI/ParticleAPI.h"` 是使用 ParticleAPI 的第一步。
它提供了更接近底层的生成类 `ParticleAPI` 和进一步封装的 CUI 类 `ParticleCUI`。两者之间的区别很小，这里只介绍 CUI 类的成员。

### CUI

| 类型 | 名称                | 说明             |
| ---- | ------------------- | ---------------- |
| Var  | displayRadius: uint | 最大可见距离     |
| Var  | highDetial：bool    | 绘制类型高清选项 |
| Var  | doubleSide: bool    | 绘制类型双面选项 |
| Func | spawnParticle       | 生成粒子         |
| Func | spawnPoint          | 生成点           |
| Func | spawnNumber         | 生成数字         |
| Func | drawOrientedLine    | 绘制定向线       |
| Func | drawCuboid          | 绘制 3D 矩形     |
| Func | drawCircle          | 绘制二维圆       |
