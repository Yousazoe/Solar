#ifndef GFX_DEFINE
#define GFX_DEFINE

#define Mat4f row_major float4x4
#define OBJECTID SV_InstanceID

static const float INFINITY = 1e9;
static const float EPS = 0.01f;
static const float PI = 3.14159265358979323f;
static const float TWO_PI = 6.28318530717958648f;
static const float INV_PI = 1.f / PI;
static const float HALF_PI = 1.5707963267948966;


inline bool is_saturated(float a) { return a == saturate(a); }
inline bool is_saturated(float2 a) { return is_saturated(a.x) && is_saturated(a.y); }
inline bool is_saturated(float3 a) { return is_saturated(a.x) && is_saturated(a.y) && is_saturated(a.z); }
inline bool is_saturated(float4 a) { return is_saturated(a.x) && is_saturated(a.y) && is_saturated(a.z) && is_saturated(a.w); }

inline float rcp(in float a) { return 1.0f / a; }
inline float2 rcp(in float2 a) { return 1.0f / a; }
inline float3 rcp(in float3 a) { return 1.0f / a; }
inline float4 rcp(in float4 a) { return 1.0f / a; }

#endif
