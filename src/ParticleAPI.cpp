//
// Created by OEOTYAN on 2022/08/27.
//
#include "ParticleAPI.h"
#include "ll/api/service/Bedrock.h"
#include "mc/deps/core/math/Color.h"
#include "mc/network/packet/SpawnParticleEffectPacket.h"
#include "mc/util/MolangVariableMap.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/phys/AABB.h"

namespace {
template <typename T>
std::string fto_string(const T a_value) {
    std::ostringstream out;
    out << a_value;
    return out.str();
}

int highestOneBit(unsigned int num) {
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    return num - (num >> 1);
}
std::vector<std::pair<double, int>> binSplit(double start, double end) {
    std::vector<std::pair<double, int>> lengthMap;
    double                              size   = end - start;
    int                                 length = static_cast<int>(size);
    if (length > 4096) {
        while (length >= 4096) {
            length     -= 2048;
            auto point  = 1024.0 + start;
            start      += 2048.0;
            lengthMap.emplace_back(point, 2048);
        }
        if (length > 0) {
            lengthMap.emplace_back(end - 1024.0, 2048);
        }
    } else {
        int potLength = highestOneBit(length);
        lengthMap.emplace_back(start + potLength * 0.5, potLength);
        if (length != potLength || length < size) {
            lengthMap.emplace_back(end - potLength * 0.5, potLength);
        }
    }
    return lengthMap;
}

} // namespace


static std::unordered_map<ParticleCUI::ColorPalette, std::pair<char, mce::Color>> const particleColors = {
    // clang-format off
        {ParticleCUI::ColorPalette::BLACK,    {'B', mce::Color("#000000")}},
        {ParticleCUI::ColorPalette::INDIGO,   {'I', mce::Color("#144A74")}},
        {ParticleCUI::ColorPalette::LAVENDER, {'L', mce::Color("#8E65F3")}},
        {ParticleCUI::ColorPalette::TEAL,     {'T', mce::Color("#07946E")}},
        {ParticleCUI::ColorPalette::COCOA,    {'C', mce::Color("#AB5236")}},
        {ParticleCUI::ColorPalette::DARK,     {'D', mce::Color("#56575F")}},
        {ParticleCUI::ColorPalette::OATMEAL,  {'O', mce::Color("#A2A3A7")}},
        {ParticleCUI::ColorPalette::WHITE,    {'W', mce::Color("#FFFFFF")}},
        {ParticleCUI::ColorPalette::RED,      {'R', mce::Color("#FF3040")}},
        {ParticleCUI::ColorPalette::APRICOT,  {'A', mce::Color("#FF7300")}},
        {ParticleCUI::ColorPalette::YELLOW,   {'Y', mce::Color("#FFEC27")}},
        {ParticleCUI::ColorPalette::GREEN,    {'G', mce::Color("#10E436")}},
        {ParticleCUI::ColorPalette::VATBLUE,  {'V', mce::Color("#29ADFF")}},
        {ParticleCUI::ColorPalette::SLATE,    {'S', mce::Color("#83769C")}},
        {ParticleCUI::ColorPalette::PINK,     {'P', mce::Color("#FF77A8")}},
        {ParticleCUI::ColorPalette::FAWN,     {'E', mce::Color("#FFCCAA")}},
    // clang-format on
};

static char getParticleColorType(ParticleCUI::ColorPalette const& p) { return particleColors.at(p).first; }

extern "C" {
void PTAPI_spawnParticle(int displayRadius, Vec3 const& pos, std::string const& particleName, int dimId) {
    ll::service::getLevel()->forEachPlayer([&](Player& player) {
        if (player.getDimensionId() == dimId && displayRadius == UINT_MAX
            || player.getPosition().distanceTo(pos) < displayRadius) {
            SpawnParticleEffectPacket pkt(pos, particleName, dimId, std::move(player.mMolangVariables.get()));
            player.sendNetworkPacket(pkt);
        }
        return true;
    });
}
void PTAPI_drawPoint(
    int                                  displayRadius,
    Vec3 const&                          pos,
    int                                  dimId,
    char                                 lineWidth,
    enum class ParticleCUI::ColorPalette color
) {
    PTAPI_spawnParticle(
        displayRadius,
        pos,
        std::string("ll:point") + getParticleColorType(color) + fto_string(static_cast<int>(lineWidth)),
        dimId
    );
}
void PTAPI_drawNumber(
    int                                  displayRadius,
    Vec3 const&                          pos,
    int                                  dimId,
    char                                 num,
    enum class ParticleCUI::ColorPalette color
) {
    std::string particleName = "ll:num";
    if (static_cast<int>(num) <= 16) {
        particleName += fto_string(static_cast<int>(num));
    } else {
        particleName += static_cast<char>(num);
    }
    particleName += getParticleColorType(color);
    PTAPI_spawnParticle(displayRadius, pos, particleName, dimId);
}
void PTAPI_drawAxialLine(
    int                                  displayRadius,
    bool                                 highDetial,
    bool                                 doubleSide,
    const Vec3&                          originPoint,
    char                                 direction,
    double                               length,
    int                                  dimId,
    enum class ParticleCUI::ColorPalette color
) {
    if (length <= 0) return;
    if (length < 1) {
        Vec3 vstart = originPoint;
        Vec3 vend   = originPoint;
        switch (direction) {
        case ParticleCUI::Direction::NEG_Y:
            vend.y -= (float)length;
            break;
        case ParticleCUI::Direction::POS_Y:
            vend.y += (float)length;
            break;
        case ParticleCUI::Direction::NEG_Z:
            vend.z -= (float)length;
            break;
        case ParticleCUI::Direction::POS_Z:
            vend.z += (float)length;
            break;
        case ParticleCUI::Direction::NEG_X:
            vend.x -= (float)length;
            break;
        case ParticleCUI::Direction::POS_X:
            vend.x += (float)length;
            break;
        default:
            break;
        }
        if (length > 0.375) {
            PTAPI_drawOrientedLine(displayRadius, vstart, vend, dimId, ParticleCUI::PointSize::PX2, 0.125, 5, color);
        } else {
            PTAPI_drawOrientedLine(displayRadius, vstart, vend, dimId, ParticleCUI::PointSize::PX1, 0.0625, 5, color);
        }
        return;
    }

    double start = 0, end = 0;
    switch (direction) {
    case ParticleCUI::Direction::NEG_Y:
        start = originPoint.y - length;
        end   = originPoint.y;
        break;
    case ParticleCUI::Direction::POS_Y:
        start = originPoint.y;
        end   = originPoint.y + length;
        break;
    case ParticleCUI::Direction::NEG_Z:
        start = originPoint.z - length;
        end   = originPoint.z;
        break;
    case ParticleCUI::Direction::POS_Z:
        start = originPoint.z;
        end   = originPoint.z + length;
        break;
    case ParticleCUI::Direction::NEG_X:
        start = originPoint.x - length;
        end   = originPoint.x;
        break;
    case ParticleCUI::Direction::POS_X:
        start = originPoint.x;
        end   = originPoint.x + length;
        break;
    default:
        break;
    }

    auto                              list = binSplit(start, end);
    std::vector<std::pair<Vec3, int>> positionList;
    std::string                       axisString;
    switch (direction) {
    case ParticleCUI::Direction::NEG_Y:
    case ParticleCUI::Direction::POS_Y:
        axisString = "Y";
        for (auto i : list)
            positionList.push_back({
                {originPoint.x, (float)i.first, originPoint.z},
                i.second
            });
        break;
    case ParticleCUI::Direction::NEG_Z:
    case ParticleCUI::Direction::POS_Z:
        axisString = "Z";
        for (auto i : list)
            positionList.push_back({
                {originPoint.x, originPoint.y, (float)i.first},
                i.second
            });
        break;
    default:
        axisString = "X";
        for (auto i : list)
            positionList.push_back({
                {(float)i.first, originPoint.y, originPoint.z},
                i.second
            });
        break;
    }
    axisString += getParticleColorType(color);
    for (auto points : positionList) {
        std::string particleName = axisString + fto_string(points.second);
        PTAPI_spawnParticle(displayRadius, points.first, "ll:linep" + particleName, dimId);
        if (highDetial) {
            PTAPI_spawnParticle(displayRadius, points.first, "ll:linem" + particleName, dimId);
        }
        if (doubleSide) {
            PTAPI_spawnParticle(displayRadius, points.first, "ll:line_backp" + particleName, dimId);
            if (highDetial) {
                PTAPI_spawnParticle(displayRadius, points.first, "ll:line_backm" + particleName, dimId);
            }
        }
    }
}
void PTAPI_drawOrientedLine(
    int                                  displayRadius,
    const Vec3&                          start,
    const Vec3&                          end,
    int                                  dimId,
    char                                 lineWidth,
    double                               minSpacing,
    int                                  maxParticlesNum,
    enum class ParticleCUI::ColorPalette color
) {
    double length = (end - start).length() / minSpacing;
    length        = std::min(length, (double)maxParticlesNum);
    int len       = (int)ceil(length);
    for (int i = 0; i <= len; i++) {
        auto point = start + (end - start) * (float)(i / (double)len);
        PTAPI_drawPoint(displayRadius, point, dimId, lineWidth, color);
    }
}
void PTAPI_drawCuboid(
    int                                  displayRadius,
    bool                                 highDetial,
    bool                                 doubleSide,
    const AABB&                          aabb,
    int                                  dimId,
    enum class ParticleCUI::ColorPalette color
) {
    auto p1 = aabb.min, p2 = aabb.max;
    auto dx = p2.x - p1.x;
    auto dy = p2.y - p1.y;
    auto dz = p2.z - p1.z;
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p1, ParticleCUI::Direction::POS_X, dx, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p1, ParticleCUI::Direction::POS_Y, dy, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p1, ParticleCUI::Direction::POS_Z, dz, dimId, color);
    Vec3 p3{p2.x, p1.y, p2.z};
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p3, ParticleCUI::Direction::NEG_X, dx, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p3, ParticleCUI::Direction::POS_Y, dy, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p3, ParticleCUI::Direction::NEG_Z, dz, dimId, color);
    Vec3 p4{p2.x, p2.y, p1.z};
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p4, ParticleCUI::Direction::NEG_X, dx, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p4, ParticleCUI::Direction::NEG_Y, dy, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p4, ParticleCUI::Direction::POS_Z, dz, dimId, color);
    Vec3 p5{p1.x, p2.y, p2.z};
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p5, ParticleCUI::Direction::POS_X, dx, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p5, ParticleCUI::Direction::NEG_Y, dy, dimId, color);
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, p5, ParticleCUI::Direction::NEG_Z, dz, dimId, color);
}
void PTAPI_drawCircle(
    int                                  displayRadius,
    const Vec3&                          originPoint,
    char                                 facing,
    double                               radius,
    int                                  dimId,
    char                                 lineWidth,
    double                               minSpacing,
    int                                  maxParticlesNum,
    enum class ParticleCUI::ColorPalette color
) {
    static const double _2PI_ = 2.0 * 3.14159265358979323846;

    if (radius <= 0) return;
    auto length = radius * _2PI_ / minSpacing;
    length      = std::min(length, (double)maxParticlesNum);
    int len     = (int)ceil(length);
    switch (facing) {
    case ParticleCUI::Direction::NEG_Y:
    case ParticleCUI::Direction::POS_Y:
        for (int i = 0; i < len; i++) {
            auto point =
                originPoint + Vec3(radius * cos(i / (double)len * _2PI_), 0.0, radius * sin(i / (double)len * _2PI_));
            PTAPI_drawPoint(displayRadius, point, dimId, lineWidth, color);
        }
        break;
    case ParticleCUI::Direction::NEG_Z:
    case ParticleCUI::Direction::POS_Z:
        for (int i = 0; i < len; i++) {
            auto point =
                originPoint + Vec3(radius * sin(i / (double)len * _2PI_), radius * cos(i / (double)len * _2PI_), 0.0);
            PTAPI_drawPoint(displayRadius, point, dimId, lineWidth, color);
        }
        break;
    default:
        for (int i = 0; i < len; i++) {
            auto point =
                originPoint + Vec3(0.0, radius * cos(i / (double)len * _2PI_), radius * sin(i / (double)len * _2PI_));
            PTAPI_drawPoint(displayRadius, point, dimId, lineWidth, color);
        }
        break;
    }
}
}

void ParticleCUI::spawnParticle(Vec3 const& pos, std::string const& particleName, int dimId) {
    PTAPI_spawnParticle(displayRadius, pos, particleName, dimId);
}

void ParticleCUI::drawPoint(
    Vec3 const&                          pos,
    int                                  dimId,
    enum PointSize                       lineWidth,
    enum class ParticleCUI::ColorPalette color
) {
    PTAPI_drawPoint(displayRadius, pos, dimId, lineWidth, color);
}
void ParticleCUI::drawNumber(Vec3 const& pos, int dimId, enum NumType num, enum class ParticleCUI::ColorPalette color) {
    PTAPI_drawNumber(displayRadius, pos, dimId, num, color);
}
void ParticleCUI::drawAxialLine(
    const Vec3&                          originPoint,
    enum Direction                       direction,
    double                               length,
    int                                  dimId,
    enum class ParticleCUI::ColorPalette color
) {
    PTAPI_drawAxialLine(displayRadius, highDetial, doubleSide, originPoint, direction, length, dimId, color);
}
void ParticleCUI::drawOrientedLine(
    const Vec3&                          start,
    const Vec3&                          end,
    int                                  dimId,
    enum PointSize                       lineWidth,
    double                               minSpacing,
    int                                  maxParticlesNum,
    enum class ParticleCUI::ColorPalette color
) {
    PTAPI_drawOrientedLine(displayRadius, start, end, dimId, lineWidth, minSpacing, maxParticlesNum, color);
}
void ParticleCUI::drawCuboid(const AABB& aabb, int dimId, enum class ParticleCUI::ColorPalette color) {
    PTAPI_drawCuboid(displayRadius, highDetial, doubleSide, aabb, dimId, color);
}
void ParticleCUI::drawCircle(
    const Vec3&                          originPoint,
    enum Direction                       facing,
    double                               radius,
    int                                  dimId,
    enum PointSize                       lineWidth,
    double                               minSpacing,
    int                                  maxParticlesNum,
    enum class ParticleCUI::ColorPalette color
) {
    PTAPI_drawCircle(displayRadius, originPoint, facing, radius, dimId, lineWidth, minSpacing, maxParticlesNum, color);
}
