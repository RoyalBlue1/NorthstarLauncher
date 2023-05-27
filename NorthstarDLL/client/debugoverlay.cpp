#include "dedicated/dedicated.h"
#include "core/convar/cvar.h"
#include "core/math/vector.h"
#include "NavMesh.h"
#include "mapColl.h"
#include <unordered_set>



AUTOHOOK_INIT()

struct __declspec(align(4)) CCollisionProperty
{
	void *vftable;
	BYTE gap8[8];
	uint32_t dword10;
	uint64_t qword14;
	uint32_t dword1C;
	uint64_t qword20;
	BYTE gap28[12];
	uint16_t word34;
	BYTE gap36[6];
	uint32_t dword3C;
	uint64_t qword40;
	uint32_t dword48;
	uint64_t qword4C;
};


struct CBaseEntity
{
	void *vtable;
	__int32 unknown_8;
	BYTE gap_C[4];
	uint64_t qword_10;
	uint64_t qword_18;
	__int32 unknown_20;
	BYTE gap_24[4];
	__int64 unknown_28;
	BYTE gap_30[40];
	__int16 entityIndex;
	BYTE gap_5A[14];
	__int32 unknown_68;
	BYTE gap_6C[4];
	char *className;
	float unknown_78;
	BYTE gap_7C[16];
	int int_8C;
	void *unknownPointer_90;
	BYTE gap_98[16];
	__int32 unknown_A8;
	BYTE gap_AC[4];
	float lastSpawnTime;
	int spawnerEntityIndex;
	BYTE gap_B8[4];
	bool damageNotifications;
	bool deathNotifications;
	BYTE gap_BE[2];
	__int32 unknownFlags_C0;
	int unknownFlags_C4;
	char *modelName;
	BYTE gap_D0[8];
	unsigned int unknownFlags_D8;
	BYTE gap_DC[2];
	__int16 unknown_DE;
	BYTE gap_E0[4];
	__int32 int_E4;
	__int32 hybernationBehaviour;
	__int32 unknown_EC;
	BYTE gap_F0[4];
	int int_F4;
	bool RenderingWithViewModels;
	BYTE gap_F9[3];
	int int_FC;
	float float_100;
	float float_104;
	float cloakEndTime;
	float float_10C;
	float float_110;
	float float_114;
	BYTE gap_118[8];
	BYTE randomStruct_120[12][8];
	BYTE gap_180[184];
	int highlightCurrentContext;
	int highlightShowToTeams;
	BYTE gap_240[4];
	float grenadeDebounce;
	BYTE gap_248[4];
	int int_24C;
	BYTE gap_250[64];
	__int64 unknownFlags_290;
	__int32 unknownFlags_298;
	BYTE gap_29C[4];
	__int64 name;
	int int_2A8;
	int int_2AC;
	char scriptName[64];
	char instanceName[64];
	__int64 unknown_330;
	__int64 unknown_338;
	BYTE gap_340[32];
	__int32 unknown_360;
	__int32 parentAttachmentIndex;
	BYTE gap_368[8];
	bool moveType;
	BYTE gap_371[7];
	__int32 parentIndex;
	BYTE gap_37C[12];
	CCollisionProperty collision_388;
	BYTE gap_3DC[28];
	__int32 ownerIndex;
	__int32 unknownFlags_3FC;
	__int64 unknown_400;
	__int64 unknown_408;
	BYTE gap_410[24];
	float velocity[3];
	float angualarVelocity[3];
	BYTE gap_440[80];
	float origin[3];
	__int32 unknown_49C;
	__int32 unknown_4A0;
	__int32 unknown_4A4;
	float localVelocity[3];
	BYTE gap_4B4[28];
	int maxHealth;
	int health;
	BYTE gap_4D8[24];
	char unknown_4F0;
	char unknown_4F1;
	char forceVisibleInPhaseShift;
	char takeDamageType;
	int invulnerableCounter;
	BYTE gap_4F8[24];
	int renderTypeMaybe;
	bool visible;
	BYTE gap_515[143];
	float localOrigin[3];
	float forwardVector[3];
	BYTE gap_5BC[36];
	__int32 unknown_5E0;
	int teamNumber;
	unsigned int passThroughFlags;
	int passThroughThickness;
	float passThroughDirection;
	BYTE gap_5F4[4];
	__int64 unknown_5F8;
	BYTE gap_600[4];
	int int_604;
	BYTE gap_608[4];
	float fadeDistance;
	BYTE gap_610[20];
	char LagCompensateMaybe;
	BYTE gap_625[27];
	__int64 unknown_640;
	BYTE gap_648[24];
	__int64 *ScriptInstance;
	char *scriptId;
	int int_670;
	int usableValue;
	int usablePriority;
	float usableRadius;
	float usableFOV;
	float usePromptSize;
	bool hasBeenSpawned;
	bool doDestroyCallback;
	BYTE gap_68A[250];
	int isSpottedByTeam;
	BYTE gap_788[40];
	int shieldHealth;
	int shieldHealthMax;
	BYTE gap_7B8[4];
	int linkedEntityAmount;
	BYTE gap_7C0[4];
	__declspec(align(1)) int *linkedEntityIDs;
	BYTE gap_7CC[504];
	int int_9C8;
	BYTE gap_9CC[4];
	uint64_t qword_9D0;
	bool debugBrokenInterpolation;
	bool physicsDummyEnableMotion;
	BYTE gap_9DA[1206];
	void *model;
	BYTE gap_E98[936];
	int hullType;
	__int32 unknown_1244;
	__int32 unknown_1248;
	__unaligned __declspec(align(1)) __int64 unknown_124C;
	__int32 unknown_1254;
	BYTE gap_1258[900];
	float unknown_15DC;
	BYTE gap_15E0[136];
	int int_1668;
	BYTE gap_166C[260];
	void *pointer_1770;
	BYTE gap1778[268];
	BYTE gap1884[144];
	float ejectTimeMaybe;
	float ejectUnknown_1918;
	float hotDropImpactDelay;
	BYTE gap_1920[724];
	char unknown_1BF4;
	BYTE gap_1BF5[291];
	float unknown_1D18;
	BYTE gap_1D1C[350];
	bool isPredicting;
	BYTE gap_1E7B[13];
	__int32 unknown_1E88;
	BYTE gap_1E8C[4];
	float unknown_1E90;
	BYTE gap_1E94[100];
	__int16 unknown_1EF8;
	BYTE gap_1EFA[6];
	__int32 unknown_1F00;
	BYTE gap_1F04[68];
	__int64 unknown_1F48;
	__int32 unknown_1F50;
	BYTE gap_1F54[20];
	unsigned int unknown_1F68;
	BYTE gap_1F6C[128];
	__unaligned __declspec(align(1)) __int64 unknown_1FEC;
	__int32 unknown_1FF4;
	BYTE gap_1FF8[8];
};


enum OverlayType_t
{
	OVERLAY_BOX = 0,
	OVERLAY_SPHERE,
	OVERLAY_LINE,
	OVERLAY_TRIANGLE,
	OVERLAY_SWEPT_BOX,
	OVERLAY_BOX2,
	OVERLAY_CAPSULE
};

struct OverlayBase_t
{
	OverlayBase_t()
	{
		m_Type = OVERLAY_BOX;
		m_nServerCount = -1;
		m_nCreationTick = -1;
		m_flEndTime = 0.0f;
		m_pNextOverlay = NULL;
	}

	OverlayType_t m_Type; // What type of overlay is it?
	int m_nCreationTick; // Duration -1 means go away after this frame #
	int m_nServerCount; // Latch server count, too
	float m_flEndTime; // When does this box go away
	OverlayBase_t* m_pNextOverlay;
	void* m_pUnk;
};

struct OverlayLine_t : public OverlayBase_t
{
	OverlayLine_t()
	{
		m_Type = OVERLAY_LINE;
	}

	Vector3 origin;
	Vector3 dest;
	int r;
	int g;
	int b;
	int a;
	bool noDepthTest;
};

struct OverlayBox_t : public OverlayBase_t
{
	OverlayBox_t()
	{
		m_Type = OVERLAY_BOX;
	}

	Vector3 origin;
	Vector3 mins;
	Vector3 maxs;
	QAngle angles;
	int r;
	int g;
	int b;
	int a;
};

class CClientTools {
	struct Vtable {
		void* unk_0[19];
		bool (__fastcall* GetLocalPlayerEyePosition)(__int64 a1, Vector3 *pos, QAngle *angle, float *fov);
		void* unk_20[18];
	};
	struct Vtable2 {
		void* unk_0[2];
	};
public:
	Vtable* vtable;
	Vtable2* vtable2;
	void* unk_func;
};

SourceInterface<CClientTools>* g_pClientTools;


static HMODULE sEngineModule;

typedef void (*RenderLineType)(Vector3 v1, Vector3 v2, Color c, bool bZBuffer);
static RenderLineType RenderLine;
typedef void (*RenderBoxType)(Vector3 vOrigin, QAngle angles, Vector3 vMins, Vector3 vMaxs, Color c, bool bZBuffer, bool bInsideOut);
typedef void (*RenderTriangleType)(float* a,float* b,float* c,Color color,char unk);
typedef void (*RenderOriginMarkerType)(Vector3 a1, float a2, char a6);

typedef CBaseEntity* (*getEntByIndexType)(void* a1,CBaseEntity* last);

static RenderBoxType RenderBox;
static RenderBoxType RenderWireframeBox;
static RenderTriangleType RenderTriangle;
static RenderOriginMarkerType RenderOriginMarker;
static RenderOriginMarkerType RenderUnknown;
dtNavMesh** navMeshes;

CCollisionBSPData* bspData;

getEntByIndexType getEntByIndex;
void* entArray;

static OverlayBase_t** drawnOverlays;

ConVar* cvar_renderNavmeshIndex;
ConVar* cvar_renderNavmeshThroughWall;
ConVar* cvar_renderNavmeshMask;

ConVar* cvar_tricoll_unique_flag_index;
ConVar* cvar_tricoll_end;
ConVar* cvar_tricollmaxscale;
ConVar* cvar_tricoll_maxdistance;
// clang-format off
AUTOHOOK(DrawOverlay, engine.dll + 0xABCB0, 
void, __fastcall, (OverlayBase_t * pOverlay))
// clang-format on
{
	EnterCriticalSection((LPCRITICAL_SECTION)((char*)sEngineModule + 0x10DB0A38)); // s_OverlayMutex

	void* pMaterialSystem = *(void**)((char*)sEngineModule + 0x14C675B0);

	switch (pOverlay->m_Type)
	{
	case OVERLAY_LINE:
	{
		OverlayLine_t* pLine = static_cast<OverlayLine_t*>(pOverlay);
		RenderLine(pLine->origin, pLine->dest, Color(pLine->r, pLine->g, pLine->b, pLine->a), pLine->noDepthTest);
	}
	break;
	case OVERLAY_BOX:
	{
		OverlayBox_t* pCurrBox = static_cast<OverlayBox_t*>(pOverlay);
		if (pCurrBox->a > 0)
		{
			RenderBox(
				pCurrBox->origin,
				pCurrBox->angles,
				pCurrBox->mins,
				pCurrBox->maxs,
				Color(pCurrBox->r, pCurrBox->g, pCurrBox->b, pCurrBox->a),
				false,
				false);
		}
		if (pCurrBox->a < 255)
		{
			RenderWireframeBox(
				pCurrBox->origin,
				pCurrBox->angles,
				pCurrBox->mins,
				pCurrBox->maxs,
				Color(pCurrBox->r, pCurrBox->g, pCurrBox->b, 255),
				false,
				false);
		}
	}
	break;
	}
	LeaveCriticalSection((LPCRITICAL_SECTION)((char*)sEngineModule + 0x10DB0A38));
}
Color tricollColor_flag{200,0,255,255};
static void drawMeshTilePortal( const dtMeshTile* tile)
{
	// Draw portals
	const float padx = 0.04f;
	const float pady = tile->header->walkableClimb;


	for (int side = 0; side < 8; ++side)
	{
		unsigned short m = 0x8000 | (unsigned short)side;

		for (int i = 0; i < tile->header->polyCount; ++i)
		{
			dtPoly* poly = &tile->polys[i];
			bool depthTest = !cvar_renderNavmeshThroughWall->GetBool();
			// Create new links.
			const int nv = poly->vertCount;
			for (int j = 0; j < nv; ++j)
			{
				// Skip edges which do not point to the right side.
				if (poly->neis[j] != m)
					continue;

				// Create new links
				const Vector3 va = tile->verts[poly->verts[j]];
				const Vector3 vb = tile->verts[poly->verts[(j+1) % nv]];

				if (side == 2 || side == 6)
				{
					Color col = side == 0 ? Color(255,0,0,255) : Color(255,0,255,255);

					const float x = va.x + ((side == 0) ? -padx : padx);

					RenderLine(Vector3(x,va.y-pady,va.z), Vector3(x,va.y+pady,va.z), col, depthTest);

					RenderLine(Vector3(x,va.y+pady,va.z), Vector3(x,vb.y+pady,vb.z), col, depthTest);

					RenderLine(Vector3(x,vb.y+pady,vb.z), Vector3(x,vb.y-pady,vb.z), col, depthTest);

					RenderLine(Vector3(x,vb.y-pady,vb.z), Vector3(x,va.y-pady,va.z), col, depthTest);
				}
				else if (side == 0 || side == 4)
				{
					Color col = side == 2 ? Color(0,255,0,255) : Color(0,255,255,255);

					const float z = va.z + ((side == 2) ? -padx : padx);


					RenderLine(Vector3(va.x,va.y-pady,z), Vector3(va.x,va.y+pady,z), col, depthTest);

					RenderLine(Vector3(va.x,va.y+pady,z), Vector3(vb.x,vb.y+pady,z), col, depthTest);

					RenderLine(Vector3(vb.x,vb.y+pady,z), Vector3(vb.x,vb.y-pady,z), col, depthTest);

					RenderLine(Vector3(vb.x,vb.y-pady,z), Vector3(va.x,va.y-pady,z), col, depthTest);
				}

			}
		}
	}
}



void RenderNavmesh() {
	int navMeshIndex = cvar_renderNavmeshIndex->GetInt();
	if((navMeshIndex<0)||(navMeshIndex>=4))
		return;
	dtNavMesh* mesh = navMeshes[navMeshIndex];
	if(!mesh)
		return;
	for (int i = 0; i < mesh->m_maxTiles; i++) {
		int mask = cvar_renderNavmeshMask->GetInt();
		dtMeshTile* tile = &mesh->m_tiles[i];
		
		if (tile->header && tile->header->magic == 0x444E4156) {
			//drawPolygons

			if (mask & 1) {
				for (int j = 0; j < tile->header->polyCount; j++) {
					dtPoly* poly= &tile->polys[j];
					int v = 0;
					float a[3];
					float b[3];
					for (v = 0; v < (poly->vertCount - 1); v++) {
						RenderLine(tile->verts[poly->verts[v]],tile->verts[poly->verts[v+1]],Color(0,0,255,255),!cvar_renderNavmeshThroughWall->GetBool());

					}
					RenderLine(tile->verts[poly->verts[v]],tile->verts[poly->verts[0]],Color(0,0,255,255),!cvar_renderNavmeshThroughWall->GetBool());
					//RenderLine(a,b,Color(0,0,255,255),false);
				}
			}
			
			//draw offMeshConnections
			if (mask & 2) {
				for (int j = 0; j < tile->header->offMeshConCount; j++) {
					dtOffMeshConnection* con = &tile->offMeshConnections[j];
					RenderLine(con->origin,con->dest,Color(255,0,0,255),!cvar_renderNavmeshThroughWall->GetBool());
				}
			}
			if(mask & 4)
				drawMeshTilePortal(tile);
			if (mask & 8) {
				for (int j = 0; j < tile->header->maxLinkCount; j++) {

				}
			}
				
		
		}
	}
	
}

inline __m128 calculatePlaneNormalFromAngle(float angLR,float angUD) {
	float alpha = angUD*M_PI/180;
	float beta = angLR*M_PI/180;
	float xf = cos(alpha)*cos(beta);
	float yf = cos(alpha)*sin(beta);
	float zf = sin(alpha);
	__m128 x = _mm_load_ss(&xf);
	__m128 y = _mm_load_ss(&yf);
	__m128 z = _mm_load_ss(&zf); 
	__m128 xy = _mm_movelh_ps(x, y);
	return _mm_shuffle_ps(xy, z, _MM_SHUFFLE(2, 0, 2, 0));
}

inline __m128 loadVector3(Vector3& vec) {
	__m128 x = _mm_load_ss(&vec.x);
	__m128 y = _mm_load_ss(&vec.y);
	__m128 z = _mm_load_ss(&vec.z);
	__m128 xy = _mm_movelh_ps(x, y);
	return _mm_shuffle_ps(xy, z, _MM_SHUFFLE(2, 0, 2, 0));
}
/*
bool shouldTriBeCulled(Vector3& camPos, __m128& rightNormal, __m128& leftNormal, Vector3* tri0, Vector3* tri1, Vector3* tri2) {
	__m128 origin = loadVector3(camPos);
	__m128 pos = loadVector3(*tri0);
	__m128 sub = _mm_sub_ps(pos,origin);
	__m128 mul= _mm_mul_ps(sub,rightNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	mul= _mm_mul_ps(sub,leftNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	pos = loadVector3(*tri1);
	sub = _mm_sub_ps(pos,origin);
	mul= _mm_mul_ps(sub,rightNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	mul= _mm_mul_ps(sub,leftNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	pos = loadVector3(*tri2);
	sub = _mm_sub_ps(pos,origin);
	mul= _mm_mul_ps(sub,rightNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	mul= _mm_mul_ps(sub,leftNormal);
	if((mul.m128_f32[0]+mul.m128_f32[1]+mul.m128_f32[2])>=0)
		return false;
	return true;
}
*/
bool shouldTriBeCulled(Vector3& camPos, __m128& rightNormal, __m128& leftNormal, Vector3* tri0, Vector3* tri1, Vector3* tri2) {
	bool isInRange = false;
	bool isInRightView = false;
	bool isInLeftView = false;
	float maxDist = cvar_tricoll_maxdistance->GetFloat() * cvar_tricoll_maxdistance->GetFloat();
	__m128 origin = loadVector3(camPos);
	__m128 pos = loadVector3(*tri0);
	__m128 sub = _mm_sub_ps(pos, origin);
	__m128 mul = _mm_mul_ps(sub, rightNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInRightView = true;
	mul = _mm_mul_ps(sub, leftNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInLeftView = true;
	mul = _mm_mul_ps(sub, sub);
	if (mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2] < maxDist) {
		isInRange = true;
	}
	if (isInRange && isInRightView && isInLeftView)
		return false;
	isInRange = false;
	isInRightView = false;
	isInLeftView = false;
	pos = loadVector3(*tri1);
	sub = _mm_sub_ps(pos, origin);
	mul = _mm_mul_ps(sub, rightNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInRightView = true;
	mul = _mm_mul_ps(sub, leftNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInLeftView = true;
	mul = _mm_mul_ps(sub, sub);
	if (mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2] < maxDist) {
		isInRange = true;
	}
	if (isInRange && isInRightView && isInLeftView)
		return false;
	isInRange = false;
	isInRightView = false;
	isInLeftView = false;
	pos = loadVector3(*tri2);
	sub = _mm_sub_ps(pos, origin);
	mul = _mm_mul_ps(sub, rightNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInRightView = true;
	mul = _mm_mul_ps(sub, leftNormal);
	if ((mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2]) >= 0)
		isInLeftView = true;
	mul = _mm_mul_ps(sub, sub);
	if (mul.m128_f32[0] + mul.m128_f32[1] + mul.m128_f32[2] < maxDist) {
		isInRange = true;
	}
	if (isInRange && isInRightView && isInLeftView)
		return false;
	return true;
}

std::set<int> renderedtricolls;
void renderTricoll(Tricoll* tricoll,Vector3 &camPos,__m128& rightPlaneNormal,__m128& leftPlaneNormal) {
	
	Vector3* vertBase = &bspData->mapVerts[tricoll->vertBaseIndex];
	Tricoll_Tri* tris = getTricollTris(tricoll);
	Tricoll_Bevel_Start* bevelStarts = getTricollBevelStarts(tricoll);

	for (int j = 0; j < tricoll->trisCount; j++) {

		uint32_t direction = (tris[j].val>>24) & 0xFF;

		uint32_t tri_0 = tris[j].val & 0x3FF;
		uint32_t tri_1 = tri_0 + (tris[j].val>>10) & 0x7F;
		uint32_t tri_2 = tri_0 + (tris[j].val>>17) & 0x7F;
		if(shouldTriBeCulled(camPos,rightPlaneNormal,leftPlaneNormal,&vertBase[tri_0],&vertBase[tri_1],&vertBase[tri_2]))continue;

		RenderLine(vertBase[tri_0], vertBase[tri_1], tricollColor_flag, !cvar_renderNavmeshThroughWall->GetBool());
		RenderLine(vertBase[tri_1], vertBase[tri_2], tricollColor_flag, !cvar_renderNavmeshThroughWall->GetBool());
		RenderLine(vertBase[tri_2], vertBase[tri_0], tricollColor_flag, !cvar_renderNavmeshThroughWall->GetBool());

	}
}



void renderCollision() {
	if(!g_pClientTools)return;
	Vector3 camPos;
	QAngle camAngle;
	float camFov;
	(*g_pClientTools)->vtable->GetLocalPlayerEyePosition(0,&camPos,&camAngle,&camFov);
	__m128 rightPlaneNormal = calculatePlaneNormalFromAngle(camAngle.y +90-camFov/2,camAngle.z);
	__m128 leftPlaneNormal= calculatePlaneNormalFromAngle(camAngle.y -90+camFov/2,camAngle.z);

	for (int i = 0; i < bspData->collisionGeoCount; i++) {
		GeoSet* geoSet = &bspData->collisionGeoSets[i];
		int start = (geoSet->primStart>>8) & 0x1FFFFF;
		for (int j = 0; j < geoSet->primCount; j++) {
			CollPrimitive* prim = &bspData->collisionPrims[start+j];
			if(prim->type!=64)continue;//continue when not tricoll
			if(renderedtricolls.contains(*(short*)prim->index))continue;
			if(!(bspData->collision_unique_contents[prim->unique_contents]&(1<<cvar_tricoll_unique_flag_index->GetInt())))continue;
			renderedtricolls.insert(*(short*)prim->index);
			
			renderTricoll(bspData->tricollPointer[*(short*)prim->index],camPos,rightPlaneNormal,leftPlaneNormal);
		}
	}
	renderedtricolls.clear();
}




AUTOHOOK(drawDebugOverlays, engine.dll + 0xAB780, void, __fastcall, (char a1)) {
	drawDebugOverlays(a1);
	Vector3 a{ 0,0,0 };
	Vector3 b{ 100,0,0 };
	Vector3 c{ 0,100,0 };
	Vector3 d{ 100,100,0 };
	Vector3 e{ 100,-100,0 };
	RenderOriginMarker(a,40.0,0);
	//RenderNavmesh();
	renderCollision();
	
;
}

__m128 xmmword_5C6330;
__m128 xmmword_5C6340;
__m128 xmmword_5C6320;
__m128 absoluteMask;
__m128 Vector3Mask;
__m128 all_0_5;
__m128 all_0_046875;
/*
//fuction that detects if hitbox is within tricoll node. only usage of tricoll nodes found yet
AUTOHOOK(sub_1D1B10, engine.dll + 0x1D1B10, __int64, __fastcall, (Tricoll* a1, const __m128i* testPointOrigin, const __m128i* a3, const __m128i* a4, _DWORD* a5, _QWORD* a6)) {


	__m128 tricollOrigin; // xmm0
	__int64 trisCount; // rbx
	Tricoll_Node *tricollNodes; // rsi
	__int64 v10; // r8
	__int64 v11; // r11
	__m128 v12; // xmm9
	__int64 v13; // r9
	__m128 v14; // xmm1
	__m128 v15; // xmm7
	__m128 v16; // xmm9
	__m128 v17; // xmm8
	__int64 v18; // rcx
	unsigned __int64 v19; // rbx
	__int64 v20; // r8
	unsigned __int64 v21; // rax
	unsigned __int64 v22; // rcx
	unsigned __int64 v23; // rdx
	__m128 v24; // xmm6
	unsigned __int64 v25; // rax
	__m128i v26; // xmm1
	__int64 v27; // rcx
	__m128 v28; // xmm2
	__m128 v29; // xmm0
	__m128 v30; // xmm5
	__m128 v31; // xmm3
	__m128 v32; // xmm1
	__m128 v33; // xmm4
	__m128 v34; // xmm0
	__m128 v35; // xmm2
	__m128 v36; // xmm3
	__int64 v37; // rdi
	__m128 v38; // xmm12
	__m128i v39; // xmm1
	__m128 v40; // xmm2
	__m128 v41; // xmm0
	__m128 v42; // xmm6
	__m128 v43; // xmm4
	__m128 v44; // xmm1
	__m128 v45; // xmm5
	__m128 v46; // xmm4
	__m128 v47; // xmm2
	__m128 v48; // xmm3
	unsigned __int64 v49; // rcx
	__int16 v51; // cx

	tricollOrigin = *(__m128 *)&a1->origin.x;
	trisCount = a1->trisCount;

	tricollNodes = (Tricoll_Node*)&a1->data;
	*a5 = 0x10000;
	a5[1] = 0x30002;
	v10 = 0i64;
	v11 = 0i64;
	v13 = (unsigned int)trisCount;
	v14 = _mm_shuffle_ps(tricollOrigin, tricollOrigin, 255);
	v15 = _mm_mul_ps(
		_mm_mul_ps(_mm_and_ps(_mm_castsi128_ps(_mm_lddqu_si128(a3)), Vector3Mask), all_0_5),
		v14);
	v16 = _mm_mul_ps(
		_mm_add_ps(
			_mm_and_ps(_mm_castsi128_ps(_mm_lddqu_si128(a4)), Vector3Mask),
			(__m128)all_0_046875),
		v14);
	v17 = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(
				_mm_and_ps(_mm_castsi128_ps(_mm_lddqu_si128(testPointOrigin)),Vector3Mask),
				v14),
			tricollOrigin),
		v15);
	if ( (unsigned int)trisCount <= 3 )
	{
	LABEL_15:
		*a6 = (_QWORD)((char *)a5 + 2 * v11);
		a6[1] = (_QWORD) & tricollNodes[v10];
		return v13 - v11;
	}
	v13 = 4i64;
	v18 = 5 - ((int)trisCount + 3) % 6;
	v19 = (unsigned __int64)(trisCount - v18) >> 1;
	v20 = v19 + v18;

	v22 = 0;
	_umul128((v19 + v18 - 4),0xAAAAAAAAAAAAAAABui64,&v23 );
	v23 = v23 >> 1;
	v10 = v23 + v20;
	if ( !v23 )
	{
	LABEL_8:
		v37 = v13;
		v38 = _mm_shuffle_ps(v15, v15, 177);
		do
		{
			v39 = _mm_load_si128(&tricollNodes[*((unsigned __int16 *)a5 + v11)].m128i);
			v40 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v39, v39), 0x10u));// convert first 4 shorts to float
			v41 = _mm_sub_ps(v17, v40);
			v42 = _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v39, v39), 0x10u)), v16);// converts second 4 shorts to float and add v16
			v43 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v40, v40, 255), xmmword_5C6330), xmmword_5C6340);
			v44 = _mm_mul_ps(_mm_shuffle_ps(v42, v42, 255), xmmword_5C6320);
			v45 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v41, v41, 177), v44), _mm_mul_ps(v41, v43));
			v46 = _mm_add_ps(_mm_mul_ps(v43, v15), _mm_mul_ps(v44, v38));
			v47 = _mm_shuffle_ps(v46, v46, 210);//move 3rd field
			v48 = _mm_and_ps(v46, absoluteMask);
			__m128 test2 = _mm_and_ps(
				_mm_min_ps(
					_mm_sub_ps(_mm_add_ps(v42, v48), _mm_and_ps(v45, absoluteMask)),
					_mm_sub_ps(
						_mm_add_ps(
							_mm_mul_ps(_mm_shuffle_ps(v42, v42, 210), v48),
							_mm_mul_ps(_mm_and_ps(v47, absoluteMask), v42)),
						_mm_and_ps(
							_mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(v45, v45, 210), v46), _mm_mul_ps(v47, v45)),
							absoluteMask))),
				Vector3Mask);
			if ( !_mm_movemask_ps(test2) )
			{
				v49 = *((unsigned __int16 *)a5 + v11) - v23;
				if ( v49 < v19 )
				{
					v51 = 2 * v49;
					*((_WORD *)a5 + v13) = v51;
					v13 += 2i64;
					*((_WORD *)a5 + v13 - 1) = v51 + 1;
				}
				else
				{
					*((_WORD *)a5 + v13++) = v49 + v19;
				}
			}
			++v11;
		}
		while ( v11 != v37 );
		goto LABEL_15;
	}
	v24 = _mm_shuffle_ps(v15, v15, 177);
	while ( 1 )
	{
		v25 = v22;
		v26 = _mm_load_si128(&tricollNodes[v22].m128i);
		v27 = 0i64;
		*(_QWORD *)((char *)a5 + 2 * v13) = 0x4000400040004i64 * v25 + 0x7000600050004i64;
		v28 = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v26, v26), 0x10u));
		v29 = _mm_sub_ps(v17, v28);
		v30 = _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v26, v26), 0x10u)), v16);
		v31 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v28, v28, 255), xmmword_5C6330), xmmword_5C6340);
		v32 = _mm_mul_ps(_mm_shuffle_ps(v30, v30, 255), xmmword_5C6320);
		v33 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v29, v29, 177), v32), _mm_mul_ps(v29, v31));
		v34 = _mm_add_ps(_mm_mul_ps(v24, v32), _mm_mul_ps(v31, v15));
		v35 = _mm_shuffle_ps(v34, v34, 210);
		v36 = _mm_and_ps(v34, absoluteMask);//absolute
		__m128 test = _mm_and_ps(
			_mm_min_ps(
				_mm_sub_ps(_mm_add_ps(v30, v36), _mm_and_ps(v33, absoluteMask)),
				_mm_sub_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_shuffle_ps(v30, v30, 210), v36),
						_mm_mul_ps(_mm_and_ps(v35, absoluteMask), v30)),
					_mm_and_ps(
						_mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(v33, v33, 210), v34), _mm_mul_ps(v35, v33)),
						absoluteMask))),
			Vector3Mask);
		if ( !_mm_movemask_ps(test))
			v27 = 4i64;
		++v11;
		v13 += v27;
		if ( v11 == v13 )
			return 0i64;
		v22 = *((unsigned __int16 *)a5 + v11);
		if ( v22 >= v23 )
			goto LABEL_8;
	}
}
*/

ON_DLL_LOAD_CLIENT_RELIESON("engine.dll", DebugOverlay, ConVar, (CModule module))
{
	AUTOHOOK_DISPATCH()

	RenderLine = module.Offset(0x192A70).As<RenderLineType>();
	RenderBox = module.Offset(0x192520).As<RenderBoxType>();
	RenderWireframeBox = module.Offset(0x193DA0).As<RenderBoxType>();
	RenderTriangle = module.Offset(0x193940).As<RenderTriangleType>();
	RenderOriginMarker = module.Offset(0x1924D0).As<RenderOriginMarkerType>();
	RenderUnknown = module.Offset(0x1924E0).As<RenderOriginMarkerType>();
	sEngineModule = reinterpret_cast<HMODULE>(module.m_nAddress);
	drawnOverlays = module.Offset(0x10DB0968).As<OverlayBase_t**>();
	bspData = module.Offset(0x130DED80).As<CCollisionBSPData*>();


	cvar_renderNavmeshIndex = new ConVar(
		"renderNavmeshIndex",
		"-1",
		FCVAR_CHEAT,
		"");
	cvar_renderNavmeshThroughWall = new ConVar(
		"renderNavmeshThroughWall",
		"0",
		FCVAR_CHEAT,
		"");
	cvar_renderNavmeshMask = new ConVar(
		"renderNavmeshMask",
		"255",
		FCVAR_CHEAT,
		"");
	cvar_tricoll_unique_flag_index = new ConVar("tricoll_unique_flag","0",FCVAR_CHEAT,"");//0-31
	cvar_tricoll_maxdistance = new ConVar("tricoll_maxdist","500",FCVAR_CHEAT,"");
	// not in g_pCVar->FindVar by this point for whatever reason, so have to get from memory
	ConVar* Cvar_enable_debug_overlays = module.Offset(0x10DB0990).As<ConVar*>();
	Cvar_enable_debug_overlays->SetValue(false);
	Cvar_enable_debug_overlays->m_pszDefaultValue = (char*)"0";
	Cvar_enable_debug_overlays->AddFlags(FCVAR_CHEAT);


	xmmword_5C6330.m128_u32[0] = 0xB8000000;
	xmmword_5C6330.m128_u32[1] = 0xB8000000;
	xmmword_5C6330.m128_u32[2] = 0;
	xmmword_5C6330.m128_u32[3] = 0;

	xmmword_5C6340.m128_u32[0] = 0;
	xmmword_5C6340.m128_u32[1] = 0;
	xmmword_5C6340.m128_u32[2] = 0x3F800000;
	xmmword_5C6340.m128_u32[3] = 0;

	xmmword_5C6320.m128_u32[0] = 0xB8000000;
	xmmword_5C6320.m128_u32[1] = 0x38000000;
	xmmword_5C6320.m128_u32[2] = 0;
	xmmword_5C6320.m128_u32[3] = 0;

	absoluteMask.m128_u32[0] = 0x7FFFFFFF;
	absoluteMask.m128_u32[1] = 0x7FFFFFFF;
	absoluteMask.m128_u32[2] = 0x7FFFFFFF;
	absoluteMask.m128_u32[3] = 0x7FFFFFFF;

	Vector3Mask.m128_u32[0] = 0xFFFFFFFF;
	Vector3Mask.m128_u32[1] = 0xFFFFFFFF;
	Vector3Mask.m128_u32[2] = 0xFFFFFFFF;
	Vector3Mask.m128_u32[3] = 0;

	all_0_5.m128_f32[0] = 0.5;
	all_0_5.m128_f32[1] = 0.5;
	all_0_5.m128_f32[2] = 0.5;
	all_0_5.m128_f32[3] = 0.5;


	all_0_046875.m128_f32[0] = 0.046875;
	all_0_046875.m128_f32[1] = 0.046875;
	all_0_046875.m128_f32[2] = 0.046875;
	all_0_046875.m128_f32[3] = 0.046875;


}

ON_DLL_LOAD("server.dll", serverNav, (CModule module)) {
	navMeshes = module.Offset(0x105F5D0).As<dtNavMesh**>();
	getEntByIndex = module.Offset(0x452F00).As<getEntByIndexType>();
	entArray = module.Offset(0x112D770).As<void*>();

}

ON_DLL_LOAD("client.dll", clientOverlay, (CModule module)) {
	g_pClientTools = new SourceInterface<CClientTools>("client.dll","VCLIENTTOOLS001");
}
