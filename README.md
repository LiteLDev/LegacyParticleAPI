# LegacyParticleAPI

## Usage

ParticleAPI provides a rich and easy-to-use particle generation interface for plugin development, which intuitive design and efficient and stable performance support developers to write better plugins.

Including headfile `#include "LegacyParticleAPI/ParticleAPI.h"` is the first step to use ParticleAPI。
It provides the generation class `ParticleAPI` which is closer to the bottom layer and the further encapsulated CUI class `ParticleCUI`. The difference between the two is very small, and only the members of the CUI class are introduced here.

### CUI
| Type | Name                | Illustration              |
| ---- | ------------------- | ------------------------- |
| Var  | displayRadius: uint | maximum visible distance  |
| Var  | highDetial: bool    | draw type HD option       |
| Var  | doubleSide: bool    | draw type two-side option |
| Func | spawnParticle       | generate the particle     |
| Func | spawnPoint          | generate the point        |
| Func | spawnNumber         | generate the number       |
| Func | drawOrientedLine    | draw oriented line        |
| Func | drawCuboid          | draw 3D rectangle         |
| Func | drawCircle          | draw 2D circle            |
