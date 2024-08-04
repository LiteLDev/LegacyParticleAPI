/**
 * @file   ParticlePTAPI_h
 * @author OEOTYAN (https://github.com/OEOTYAN)
 * @brief  Spawn Particles for Client User Interface
 *
 * @copyright Created by OEOTYAN on 2022/08/27.
 *
 */
#pragma once
#include "mc/math/Vec3.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/levelgen/structure/BoundingBox.h"
#include "mc/world/phys/AABB.h"


#ifdef PARTICLEAPI_EXPORTS
#define PARTICLE_API __declspec(dllexport)
#else
#define PARTICLE_API __declspec(dllimport)
#endif

class ParticleCUI {
public:
    unsigned int displayRadius;
    bool         highDetial;
    bool         doubleSide;
    ParticleCUI() : displayRadius(UINT_MAX), highDetial(true), doubleSide(true) {}
    explicit ParticleCUI(unsigned int dr, bool hd = true, bool ds = true)
    : displayRadius(dr),
      highDetial(hd),
      doubleSide(ds) {}
    enum Direction : char {
        NEG_Y = 0,
        POS_Y = 1,
        NEG_Z = 2,
        POS_Z = 3,
        NEG_X = 4,
        POS_X = 5,
    };

    enum PointSize : char {
        PX1  = 1,
        PX2  = 2,
        PX4  = 4,
        PX8  = 8,
        PX16 = 16,
    };

    enum NumType : char {
        NUM0  = 0,
        NUM1  = 1,
        NUM2  = 2,
        NUM3  = 3,
        NUM4  = 4,
        NUM5  = 5,
        NUM6  = 6,
        NUM7  = 7,
        NUM8  = 8,
        NUM9  = 9,
        NUMA  = 'A',
        NUMB  = 'B',
        NUMC  = 'C',
        NUMD  = 'D',
        NUME  = 'E',
        NUMF  = 'F',
        NUM10 = 10,
        NUM11 = 11,
        NUM12 = 12,
        NUM13 = 13,
        NUM14 = 14,
        NUM15 = 15,
        NUM16 = 16,
    };
    enum class ColorPalette {
        BLACK,
        INDIGO,
        LAVENDER,
        TEAL,
        COCOA,
        DARK,
        OATMEAL,
        WHITE,
        RED,
        APRICOT,
        YELLOW,
        GREEN,
        VATBLUE,
        SLATE,
        PINK,
        FAWN,
    };
    PARTICLE_API void spawnParticle(Vec3 const& pos, std::string const& particleName, int dimId);

    PARTICLE_API void drawPoint(
        Vec3 const&                          pos,
        int                                  dimId,
        enum PointSize                       lineWidth = PointSize::PX4,
        enum class ParticleCUI::ColorPalette color     = ParticleCUI::ColorPalette::WHITE
    );
    PARTICLE_API void drawNumber(
        Vec3 const&                          pos,
        int                                  dimId,
        enum NumType                         num   = NumType::NUM0,
        enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
    );
    PARTICLE_API void drawAxialLine(
        const Vec3&                          originPoint,
        enum Direction                       direction,
        double                               length,
        int                                  dimId,
        enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
    );
    PARTICLE_API void drawOrientedLine(
        const Vec3&                          start,
        const Vec3&                          end,
        int                                  dimId,
        enum PointSize                       lineWidth       = PointSize::PX4,
        double                               minSpacing      = 1,
        int                                  maxParticlesNum = 64,
        enum class ParticleCUI::ColorPalette color           = ParticleCUI::ColorPalette::WHITE
    );
    PARTICLE_API void drawCuboid(
        const AABB&                          aabb,
        int                                  dimId,
        enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
    );
    PARTICLE_API void drawCircle(
        const Vec3&                          originPoint,
        enum Direction                       facing,
        double                               radius,
        int                                  dimId,
        enum PointSize                       lineWidth       = PointSize::PX4,
        double                               minSpacing      = 1,
        int                                  maxParticlesNum = 64,
        enum class ParticleCUI::ColorPalette color           = ParticleCUI::ColorPalette::WHITE
    );
    void inline spawnParticle(BlockPos const& pos, std::string const& particleName, int dimId) {
        spawnParticle(pos + 0.5f, particleName, dimId);
    }
    void inline drawPoint(
        BlockPos const&                      pos,
        int                                  dimId,
        PointSize                            lineWidth,
        enum class ParticleCUI::ColorPalette color
    ) {
        drawPoint(pos + 0.5f, dimId, lineWidth, color);
    }
    void inline drawNumber(BlockPos const& pos, int dimId, NumType num, enum class ParticleCUI::ColorPalette color) {
        drawNumber(pos + 0.5f, dimId, num, color);
    }
    void inline drawOrientedLine(
        const BlockPos&                      start,
        const BlockPos&                      end,
        int                                  dimId,
        PointSize                            lineWidth,
        double                               minSpacing,
        int                                  maxParticlesNum,
        enum class ParticleCUI::ColorPalette color
    ) {
        drawOrientedLine(start + 0.5f, end + 0.5f, dimId, lineWidth, minSpacing, maxParticlesNum, color);
    }
    void inline drawCuboid(const BoundingBox& box, int dimId, enum class ParticleCUI::ColorPalette color) {
        drawCuboid(box, dimId, color);
    }
    void inline drawCuboid(const BlockPos& pos, int dimId, enum class ParticleCUI::ColorPalette color) {
        drawCuboid(pos, dimId, color);
    }
    void inline drawCircle(
        const BlockPos&                      originPoint,
        Direction                            facing,
        double                               radius,
        int                                  dimId,
        PointSize                            lineWidth,
        double                               minSpacing,
        int                                  maxParticlesNum,
        enum class ParticleCUI::ColorPalette color
    ) {
        drawCircle(originPoint + 0.5f, facing, radius, dimId, lineWidth, minSpacing, maxParticlesNum, color);
    }
};

extern "C" {
PARTICLE_API void
PTAPI_spawnParticle(int displayRadius, class Vec3 const& pos, std::string const& particleName, int dimId);
PARTICLE_API void PTAPI_drawPoint(
    int                                  displayRadius,
    class Vec3 const&                    pos,
    int                                  dimId,
    char                                 lineWidth = ParticleCUI::PointSize::PX4,
    enum class ParticleCUI::ColorPalette color     = ParticleCUI::ColorPalette::WHITE
);
PARTICLE_API void PTAPI_drawNumber(
    int                                  displayRadius,
    class Vec3 const&                    pos,
    int                                  dimId,
    char                                 num   = ParticleCUI::NumType::NUM0,
    enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
);
PARTICLE_API void PTAPI_drawAxialLine(
    int                                  displayRadius,
    bool                                 highDetial,
    bool                                 doubleSide,
    const class Vec3&                    originPoint,
    char                                 direction,
    double                               length,
    int                                  dimId,
    enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
);
PARTICLE_API void PTAPI_drawOrientedLine(
    int                                  displayRadius,
    const class Vec3&                    start,
    const class Vec3&                    end,
    int                                  dimId,
    char                                 lineWidth       = ParticleCUI::PointSize::PX4,
    double                               minSpacing      = 1,
    int                                  maxParticlesNum = 64,
    enum class ParticleCUI::ColorPalette color           = ParticleCUI::ColorPalette::WHITE
);
PARTICLE_API void PTAPI_drawCuboid(
    int                                  displayRadius,
    bool                                 highDetial,
    bool                                 doubleSide,
    const class AABB&                    aabb,
    int                                  dimId,
    enum class ParticleCUI::ColorPalette color = ParticleCUI::ColorPalette::WHITE
);
PARTICLE_API void PTAPI_drawCircle(
    int                                  displayRadius,
    const class Vec3&                    originPoint,
    char                                 facing,
    double                               radius,
    int                                  dimId,
    char                                 lineWidth       = ParticleCUI::PointSize::PX4,
    double                               minSpacing      = 1,
    int                                  maxParticlesNum = 64,
    enum class ParticleCUI::ColorPalette color           = ParticleCUI::ColorPalette::WHITE
);
}
