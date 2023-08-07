#include <immintrin.h>

AUTOHOOK_INIT()

#define LODWORD(x) *(uint32_t*)&x

typedef uint8_t _BYTE;
typedef uint16_t _WORD;
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;

typedef uint64_t ulonglong;
typedef uint32_t uint;
typedef uint16_t ushort;
typedef _BYTE byte;
typedef _BYTE uchar;

struct ListController
{
	uint32_t headPointer_fromGPT;
	uint32_t tailPointer_fromGPT;
	int elementSize;
	int elementAmount;
	void *storagePointer;
};

struct assetLoader
{
	__int64 hash;
	const char *name;
	__int64 qword_10;
	__int64 qword_18;
	void *pointer_20;
	void *listPointer;
	int dword_30;
	unsigned int listElementSize;
	int dword_38;
	int listElementAmount;
	ListController list_40;
	__int64 qword_58;
};

struct uiImageAtlas
{
	_BYTE gap_0[12];
	uint16_t atlasCount;
	_WORD flags;

	_BYTE gap_10[8];
	_WORD word_18;
	_BYTE gap_1A[6];
	void *pointer_20;
	_BYTE gap_28[16];
	_QWORD textureMaybe;
	unsigned int bufferStructIndex;
	_DWORD dword_44;
};

struct unknown2
{
	float float_0;
	_BYTE byte_4;
	_BYTE byte_5;
	_BYTE byte_6;
	_BYTE byte_7;
};

struct testStruct
{
	__m128i m128_0;
	__m128i m128_10;
};

struct struct_v1
{
	uint8_t *pByte_0;
	uint32_t uint_8;
	_DWORD dword_C;
	_DWORD dword_10;
	_DWORD dword_14;
	_QWORD qword_18;
	unknown2 unk2[1];
	_BYTE gap_28[1424];
	unsigned int dword_5B8;
	_BYTE gap_5BC[8200];
	float float_25C4[192];
	_BYTE gap_28C4[4];
	uint32_t dword_28C8;
	uint32_t dword_28CC;
	_BYTE gap_28D0[1280];
	__m128 m128_2DD0[1];
	_BYTE gap_2DE0[3232];
	testStruct m128_3A80[3];
};

struct ruiArgCluster
{
	uint16_t argIndex;
	uint16_t argCount;
	uint8_t byte_4;
	uint8_t byte_5;
	_BYTE gap_6[4];
	uint16_t word_A;
	_BYTE gap_C[6];
};

struct ruiArg
{
	uint8_t argType;
	uint8_t unk1;
	uint16_t offset;
	uint16_t nameOffset;
	uint16_t shortHash;
};

struct unknown8dataStruct
{
	uint16_t type;
	uint16_t color_red;
	uint16_t color_green;
	uint16_t color_blue;
	uint16_t color_alpha;
	uint16_t word_A;
	uint16_t word_C;
	uint16_t word_E;
	uint16_t word_10;
	uint16_t word_12;
	uint16_t word_14;
	uint16_t word_16;
	uint16_t word_18;
	uint16_t word_1A;
	uint16_t word_1C;
	uint16_t fontIndex;
	uint16_t word_20;
	uint16_t word_22;
	uint16_t word_24;
	uint16_t word_26;
	uint16_t textSize;
	uint16_t stretchXOffset;
	uint16_t word_2C;
	uint16_t word_2E;
	uint16_t word_30;
	uint16_t uint16_32;
};

struct ruiUnknown10
{
	uint32_t dataCount;
	uint16_t unk1;
	uint16_t unk2;
	float *data;
};

struct unknown9dataStruct_2
{
	uint16_t type;
	uint16_t transformIndex;
	uint16_t uint16_4;
	uint16_t uint16_6;
	uint16_t uint16_8;
	uint16_t uint16_A;
	uint16_t uint16_C;
	uint16_t uint16_E;
	uint16_t uint16_10;
	uint16_t uint16_12;
	uint16_t uint16_14;
	_WORD word_16;
	uint8_t uint8_18;
};

struct ruiHeader
{
	const char *name;
	void *defaultValues;
	uint8_t *transformData;
	float elementWidth;
	float elementHeight;
	float elementWidthRatio;
	float elementHeightRatio;
	const char *argNames;
	ruiArgCluster *argClusters;
	ruiArg *args;
	__int16 argCount;
	__int16 unk10Count;
	uint16_t ruiDataStructSize;
	uint16_t defaultValuesSize;
	uint16_t unknown8Count;
	uint16_t unk_A4;
	uint16_t unknown9Count;
	uint16_t argClusterCount;
	unknown8dataStruct *unknown8data;
	uint8_t *unknown9data;
	ruiUnknown10 *unknown_10;
	void (__fastcall *dllFunc)(void *a1, void *, void *, char *);
	void (__fastcall *dllFuncHidden)(void *, void *, void *, void *);
};

struct unknown9dataStruct_0
{
	uint16_t type;
	uint16_t transformIndex;
	uint8_t styleDescriptorsIndices[4];
	uint16_t textOffset;
	uint16_t uint16_A;
	uint16_t uint16_C;
	uint16_t uint16_E;
	uint16_t uint16_10;
};

struct uiFontAtlas
{
	uint16_t fontCount;
	uint16_t uint16_2;
	uint16_t Width;
	uint16_t Hight;
	float float_8;
	float float_C;
	void *qword_10;
	char* qword_18;
	uiImageAtlas *atlas;
	_QWORD qword_28;
};


struct unknownRuiListElement
{
	_DWORD dword_0;
	_DWORD dword_4;
	uiFontAtlas *uiFontAtlas_8;
	uiImageAtlas *uiImageAtlas_10;
};


struct  ruiDataStruct
{
	ruiHeader *header;
	float canvasWidth;
	float canvasHeight;
	float canvasWidthRatio;
	float canvasHeightRatio;
	struct_v1 *v1;
	__int64 createTimeStamp;
	_BYTE byte_28;
	_BYTE error;
	_BYTE gap_2A[14];
	void* pvoid_38;
	char dataValues[1];
};

struct unknownFontStruct
{
	int dword_0;
	float float_4;
};

struct rpakFontGlyph
{
	float float_0;
	uint16_t word_4;
	uint8_t byte_6;
	_BYTE gap_7[25];
};

struct rpakFont
{
	char *fontName;
	_BYTE gap_8[6];
	unsigned __int16 unsigned___int16_E;
	_BYTE gap_10[4];
	_DWORD dword_14;
	_BYTE gap_18[4];
	float float_1C;
	_BYTE gap_20[4];
	float float_24;
	_BYTE gap_28[8];
	uint16_t *qword_30;
	uint16_t *qword_38;
	_QWORD *qword_40;
	_BYTE gap_48[8];
	rpakFontGlyph *fontGlyphs;
	unknownFontStruct *pointer_58;
};

struct struct_a1_2
{
	_DWORD dword_0;
	unsigned int pointer_10_ByteSize;
	_QWORD pointer_8;
	int *pointer_10;
	uint32_t (__fastcall *pfunc_18)(uint32_t);
	bool (__fastcall *pfunc_20)(_DWORD *, int);
	unsigned int dword_28;
	_DWORD dword_2C;
	_DWORD dword_30;
	_DWORD dword_34;
	_QWORD pointer8_elementByteSize;
	_RTL_SRWLOCK lock;
};

struct  struct_v3
{
	unknownRuiListElement* qword_0;
	unsigned int unsigned_int_8;
	_DWORD dword_C;
	_BYTE gap_10[16];
	_DWORD dword_20;
	_BYTE gap_24[4];
	_QWORD qword_28;
	_DWORD unk_StartIndex;
	_DWORD dword_34;
	_BYTE gap_38[8];
	_DWORD dword_40;
};



typedef unsigned int (*sub_FC0C0Type)(struct_v3* a1, uiImageAtlas *a2);

typedef uint64_t (*getFontGlyphIndexType)(rpakFont* a1,int c);
typedef uint64_t (*getUnicodeCharacter_GPTType)(char **a1);
typedef char *(*sub_F98F0Type)(ruiDataStruct *a1, __int64 a2, char **a3, __int64 a4,const char* a5);
typedef void (*sub_FFAE0Type)(__m128 *a1, const __m128i *a2, __int64 a3);
typedef void(*sub_FEF30Type)(void *a1, ruiDataStruct* a2, __m128 *a3, __m128i *a4, __m128 *a5);
typedef _DWORD *(*sub_F3BB0Type)(struct_a1_2 *a1, __int64 a2, _BYTE *a3);
typedef unsigned int *(*sub_F3E30Type)(struct_a1_2 *a1, __int64 a2);

typedef __int64 (*funcs5F4560Type)(__m128 *a1, __m128 *a2, _DWORD *a3, struct_v3 *a4);





//uiImageAtlas rpakUIMGAtlases[50];
uiImageAtlas* rpakUIMGAtlases;


AUTOHOOK(addAssetLoader, rtech_game.dll + 0x7BE0, __int64, __fastcall, (assetLoader* a1, unsigned int a2, unsigned int a3)) {
	if (a1->hash == 0xA676D6975) {
		a1->listElementAmount = 20;//sizeof(rpakUIMGAtlases)/sizeof(uiImageAtlas);
		a1->listPointer = rpakUIMGAtlases;
	}
	return addAssetLoader(a1,a2,a3);
}

short* word_12A2E50C;
BYTE* byte_12A2E50E;
BYTE* byte_12A2E50F;
uiFontAtlas* uiFontAtlases;
char* unk_12A2E508;

__m128* xmmword_12A4E830;
funcs5F4560Type* funcs_5F4560;
unsigned int (*sub_FC0C0)(struct_v3* a1, uiImageAtlas *a2);
rpakFont** rpakFontPointers;
struct_a1_2* assetIndexList;

uint64_t (*getFontGlyphIndex)(rpakFont* a1,int c);
uint64_t (*getUnicodeCharacter_GPT)(char **a1);
char *(*sub_F98F0)(ruiDataStruct *a1, __int64 a2,char **a3, __int64 a4,const char* a5);
//void (*sub_F9B80)(__int64 a1, __int64 a2, _QWORD *a3, __m128 *a4, const __m128i *a5, int a6, __int64 a7, __m128i *a8, __m128 *a9, __m128 *a10, __m128 *a11);
void (*sub_FFAE0)(__m128 *a1, const __m128i *a2, __int64 a3);
void(*sub_FEF30)(void *a1, ruiDataStruct* a2, __m128 *a3, __m128i *a4, __m128 *a5);
_DWORD *(*sub_F3BB0)(struct_a1_2 *a1, __int64 a2, _BYTE *a3);
unsigned int *(*sub_F3E30)(struct_a1_2 *a1, __int64 a2);
__m128* stru_5F4740;

__m128 xmmword_5F3DD0;
__m128 xmmword_5F3E20;
__m128 xmmword_5F3E50;
__m128 xmmword_5F3E70;
__m128 xmmword_5F3E80;
__m128 xmmword_5F3E90;
__m128 xmmword_5F3EB0;
__m128 xmmword_5F3EE0;
__m128 xmmword_5F3EF0;
__m128 xmmword_5F3F30;
__m128 xmmword_5F3F60;
__m128 xmmword_5F4600;
__m128 xmmword_5F4610;

__m128 xmmword_12A14650;
__m128 xmmword_12A146A0;
__m128 xmmword_12A146B0;
__m128 xmmword_12A146D0;

BYTE* fontIndices;
//void __fastcall ruiUnknown9Func_2 (__int64 a1, ruiDataStruct *a2, unknown9dataStruct_2 *a3, struct_v3 *a4)
AUTOHOOK(ruiUnknown9Func_2,engine.dll+0xF7A80,void,__fastcall,(__int64 a1, ruiDataStruct *a2, unknown9dataStruct_2 *a3, struct_v3 *a4))
{
	__int16 v6; // r9
	unknown8dataStruct *v7; // rdx
	__int64 v8; // r14
	testStruct *v9; // r11
	__m128 v10; // xmm2
	int v11; // ebx
	__m128 v12; // xmm6
	__m128i v13; // xmm1
	__m128 v14; // xmm7
	__int64 v15; // rcx
	__int64 v16; // rax
	__int64 v17; // rsi
	__int16 v18; // r15
	__int16 v19; // r12
	__int16 v20; // cx
	__int16 v21; // r15
	__m128 v22; // xmm10
	__m128 v23; // xmm11
	__m128 v24; // xmm12
	__m128 v25; // xmm13
	__m128 v26; // xmm15
	__m128 v27; // xmm14
	__m128 *v28; // rax
	float v29; // xmm9_4
	__m128 v34; // xmm4
	__m128 v35; // xmm3
	__int64 v36; // rcx
	__m128 v37; // xmm0
	__m128 v38; // xmm4
	__m128 v39; // xmm1
	__m128 v40; // xmm3
	__m128 v41; // xmm5
	__m128i v42; // xmm4
	__m128 v43; // xmm0
	__m128 v44; // xmm3
	__m128 v45; // xmm6
	__m128 v46; // xmm3
	__m128 v47; // xmm2
	__m128 v48; // xmm0
	__m128 v49; // xmm6
	__m128 v50; // xmm3
	__m128 v51; // xmm4
	__m128i v52; // xmm2
	__m128 v53; // xmm6
	__m128 v54; // xmm1
	__m128i v55; // xmm0
	__m128i v56; // xmm6
	__m128 *v57; // rcx
	float v58; // [rsp+20h] [rbp-198h]
	__m128 v59; // [rsp+30h] [rbp-188h]
	__m128 v60; // [rsp+40h] [rbp-178h]
	int v61[2]; // [rsp+50h] [rbp-168h] BYREF
	__m128i v62; // [rsp+58h] [rbp-160h]
	__m128i v63; // [rsp+68h] [rbp-150h]
	__m128 v64[6]; // [rsp+80h] [rbp-138h] BYREF
	__int16 v65; // [rsp+E0h] [rbp-D8h]
	__int16 v66; // [rsp+E2h] [rbp-D6h]
	__int16 v67; // [rsp+E4h] [rbp-D4h]
	__int16 v68; // [rsp+E6h] [rbp-D2h]
	float v69; // [rsp+1C8h] [rbp+10h]
	float v70; // [rsp+1D0h] [rbp+18h]

	v6 = a3->uint8_18;
	v7 = &a2->header->unknown8data[a3->uint8_18];
	if ( *(float *)&a2->dataValues[v7->color_alpha] > 0.0 )
	{
		v8 = a3->transformIndex;
		v9 = &a2->v1->m128_3A80[v8];
		v10 = _mm_sub_ps(
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_0, 255)), _mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_0, 0))),
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_0, 170)), _mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_0, 85))));
		if ( !_mm_movemask_ps(_mm_cmpeq_ps(v10, _mm_setzero_ps())) )
		{
			v11 = _mm_movemask_ps(v10) & 2;
			v12 = _mm_div_ps(_mm_xor_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_0, 39)), (__m128)xmmword_5F3E50), v10);
			v13 = _mm_castps_si128(_mm_mul_ps(_mm_xor_ps(v12, (__m128)xmmword_5F3DD0), _mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_10, 216))));
			v14 = _mm_add_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v13, 78)), _mm_castsi128_ps(v13));
			v15 = *(int *)&a2->dataValues[a3->uint16_4];
			if ( (_DWORD)v15 != -1 )
			{
				v66 = -1;
				v16 = a3->uint16_6;
				v17 = v15;
				v18 = (unsigned __int8)byte_12A2E50F[8 * v15];
				v19 = word_12A2E50C[4 * v15];
				v20 = a4->unk_StartIndex;
				v21 = a3->word_16 | v18;
				v65 = v19;
				v68 = v21;
				v67 = v6 + v20;
				v22 = _mm_load_ps((float *)&a2->dataValues[v16]);
				v23 = _mm_load_ps((float *)&a2->dataValues[a3->uint16_8]);
				v24 = _mm_load_ps((float *)&a2->dataValues[a3->uint16_A]);
				v25 = _mm_load_ps((float *)&a2->dataValues[a3->uint16_C]);
				v69 = *(float *)&a2->dataValues[a3->uint16_E];
				v70 = *(float *)&a2->dataValues[a3->uint16_10];
				v26 = _mm_load_ps((float *)&a2->dataValues[a3->uint16_12]);
				v27 = _mm_load_ps((float *)&a2->dataValues[a3->uint16_14]);
				v58 = *(float *)&a2->dataValues[v7->stretchXOffset];
				v28 = &a2->v1->m128_2DD0[v8];
				v29 = v28->m128_f32[0];
				if ( fminf(v28->m128_f32[0], v28->m128_f32[2]) > 0.0 )
				{
					if ( (unsigned int)sub_FC0C0(a4, &rpakUIMGAtlases[(unsigned __int8)byte_12A2E50E[8 * v17]]) )
					{
						v34 = _mm_unpacklo_ps(v26, v27);
						v35 = _mm_setzero_ps();
						v36 = *(_QWORD *)rpakUIMGAtlases[(unsigned __int8)byte_12A2E50E[8 * v17]].gap_10 + 32i64 * v19;// arg2 of function above
						v37 = _mm_unpacklo_ps(_mm_load_ps(&v69), _mm_load_ps(&v70));
						v38 = _mm_movelh_ps(v34, v34);
						v27.m128_f32[0] = (float)(v27.m128_f32[0] - v70) * v58;
						v59 = _mm_movelh_ps(v37, v37);
						v60 = _mm_max_ps(_mm_sub_ps(v38, v59), (__m128)xmmword_5F3F30);
						v39 = (__m128)xmmword_12A4E830[((__int64)v21 >> 4) & 3];
						v35.m128_f32[0] = (float)((float)(v28->m128_f32[2] * v58) * (float)(v26.m128_f32[0] - v69)) / v29;// v33 = v28.m128_f32[2]
						v40 = _mm_unpacklo_ps(v35, v27);
						v41 = _mm_div_ps(
							_mm_add_ps(
								_mm_sub_ps(
									*(__m128 *)v36,
									_mm_xor_ps(_mm_and_ps(_mm_min_ps(v59, v38), v39), (__m128)xmmword_5F3E20)),
								_mm_movelh_ps(v40, v40)),
							_mm_or_ps(
								_mm_and_ps(_mm_andnot_ps((__m128)xmmword_5F3DD0, v60), v39),
								_mm_andnot_ps(v39, (__m128)xmmword_5F3E90)));
						if ( !_mm_movemask_ps(_mm_cmplt_ps(v41, (__m128)xmmword_5F3F60)) )
						{
							v42 = _mm_castps_si128(_mm_xor_ps(
								_mm_min_ps(
									_mm_movelh_ps(
										_mm_xor_ps(_mm_unpacklo_ps(v22, v23), (__m128)xmmword_5F3DD0),
										_mm_unpacklo_ps(v24, v25)),
									v41),
								(__m128)xmmword_5F3E20));
							if ( !_mm_movemask_ps(_mm_cmple_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v42, 238)), _mm_castsi128_ps(_mm_shuffle_epi32(v42, 68)))) )
							{
								v43 = _mm_castpd_ps(_mm_loaddup_pd((const double *)(v36 + 24)));
								v44 = v12;
								v45 = _mm_mul_ps(v12, (__m128)xmmword_5F3E80);
								v46 = _mm_mul_ps(_mm_mul_ps(v44, v60), v43);
								v64[5] = _mm_setzero_ps();
								v64[3] = _mm_setzero_ps();
								v64[4] = _mm_setzero_ps();
								v61[0] = 4;
								v47 = _mm_add_ps(
									_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v14, v60), v59), v43),
									_mm_castsi128_ps(_mm_loadl_epi64((const __m128i *)(v36 + 16))));
								v61[1] = 4;
								v48 = _mm_movelh_ps(v46, v45);
								v49 = _mm_movehl_ps(v45, v46);
								v50 = _mm_castsi128_ps(_mm_shuffle_epi32(v42, 125));
								v51 = _mm_castsi128_ps(_mm_shuffle_epi32(v42, 160));
								v64[0] = v48;
								v64[1] = v49;
								v64[2] = _mm_movelh_ps(v47, _mm_sub_ps(_mm_mul_ps(v14, (__m128)xmmword_5F3E80), (__m128)xmmword_5F3E90));
								v52 = _mm_load_si128(&v9->m128_0);      // v32 = v9
								v53 = _mm_add_ps(
									_mm_add_ps(
										_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v52, 170)), v50),
										_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v52, 0)), v51)),
									_mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_10, 0)));
								v54 = _mm_add_ps(
									_mm_add_ps(
										_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v52, 255)), v50),
										_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v52, 85)), v51)),
									_mm_castsi128_ps(_mm_shuffle_epi32(v9->m128_10, 85)));
								v55 = _mm_castps_si128(_mm_unpacklo_ps(v53, v54));
								v56 = _mm_castps_si128(_mm_unpackhi_ps(v53, v54));
								if ( v11 == 2 )
								{
									v55 = _mm_shuffle_epi32(v55, 78);
									v56 = _mm_shuffle_epi32(v56, 78);
								}
								v57 = (__m128 *)a2->pvoid_38;
								v62 = v55;
								v63 = v56;
								funcs_5F4560[v57->m128_u32[0]](v57, v64, (_DWORD*)v61, a4);
							}
						}
					}
				}
			}
		}
	}
}



AUTOHOOK(rui_renderText,engine.dll+0xF6980,__m128,__fastcall,(ruiDataStruct *param_1,uint param_2))

{
	ushort *puVar1;
	char cVar2;
	ushort uVar3;
	struct_v1 *psVar4;
	uint currentUnicodeChar;
	uint uVar5;
	char *pcVar6;
	ushort uVar7;
	uchar *puVar8;
	ulonglong uVar9;
	float *pfVar10;
	ulonglong uVar11;
	rpakFontGlyph *glyph;
	uint uVar12;
	int iVar13;
	ulonglong uVar14;
	uchar *puVar15;
	ulonglong currentFontIndex;
	__m128 _Var16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float unk2;
	float fVar22;
	float fVar23;
	float fVar24;
	float fVar25;
	byte local_res18;
	float local_res20;
	uint local_1c8;
	uint32_t local_1c4;
	char *currenCharPointer;
	float local_1b8;
	uint local_1b4;
	uint32_t local_1b0;
	rpakFont *currentSelectedFont;
	unknown9dataStruct_0 *local_1a0;
	float textSizeX [4];
	float local_188 [4];
	float textSizeY [4];
	uiFontAtlas *local_168;
	char *printText;
	ulonglong local_158;
	char local_150 [8];
	rpakFont *rpakFonts [4];
	char *apcStack_128 [29];
	uint uEndVar;
	uint endVar2;
	byte imageAtlasIndex;
	byte styleDescriptorIndex_0;
	byte styleDescriptorIndex_1;
	byte styleDescriptorIndex_2;
	byte styleDescriptorIndex_3;
	unknown8dataStruct *styleDescriptors;
	float unk1;

	psVar4 = param_1->v1;
	local_158 = psVar4->qword_18;
	local_1a0 = (unknown9dataStruct_0 *)(param_1->header->unknown9data + (int)param_2);
	styleDescriptors = param_1->header->unknown8data;
	styleDescriptorIndex_1 = local_1a0->styleDescriptorsIndices[1];
	styleDescriptorIndex_0 = local_1a0->styleDescriptorsIndices[0];
	styleDescriptorIndex_2 = local_1a0->styleDescriptorsIndices[2];
	styleDescriptorIndex_3 = local_1a0->styleDescriptorsIndices[3];
	rpakFonts[1] = rpakFontPointers[styleDescriptors[styleDescriptorIndex_1].fontIndex];
	currentSelectedFont = rpakFontPointers[styleDescriptors[styleDescriptorIndex_0].fontIndex];
	rpakFonts[2] = rpakFontPointers[styleDescriptors[styleDescriptorIndex_2].fontIndex];
	rpakFonts[3] = rpakFontPointers[styleDescriptors[styleDescriptorIndex_3].fontIndex];
	textSizeY[0] = *(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_0].textSize);
	textSizeY[1] = *(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_1].textSize);
	textSizeY[2] = *(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_2].textSize);
	textSizeY[3] = *(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_3].textSize);
	textSizeX[0] = textSizeY[0] *
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_0].stretchXOffset);
	textSizeX[1] = textSizeY[1] *
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_1].stretchXOffset);
	textSizeX[2] = textSizeY[2] *
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_2].stretchXOffset);
	textSizeX[3] = textSizeY[3] *
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_3].stretchXOffset);
	local_188[0] = textSizeY[0] * currentSelectedFont->float_24 -
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_0].uint16_32
			);
	local_188[1] = textSizeY[1] * rpakFonts[1]->float_24 -
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_1].uint16_32
			);
	local_1c4 = psVar4->dword_28CC;
	unk2 = local_188[0];
	if (local_188[0] <= local_188[1]) {
		unk2 = local_188[1];
	}
	currentFontIndex = 0;
	local_188[2] = textSizeY[2] * rpakFonts[2]->float_24 -
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_2].uint16_32
			);
	unk1 = textSizeY[0] - local_188[0];
	if (textSizeY[0] - local_188[0] <= textSizeY[1] - local_188[1]) {
		unk1 = textSizeY[1] - local_188[1];
	}
	fVar24 = 0.0;
	fVar25 = 0.0;
	local_res20 = 0.0;
	local_1b8 = 0.0;
	fVar23 = 0.0;
	local_res18 = 0;
	uVar9 = 0;
	local_188[3] = textSizeY[3] * rpakFonts[3]->float_24 -
		*(float *)(param_1->dataValues + styleDescriptors[styleDescriptorIndex_3].uint16_32
			);
	local_1b4 = psVar4->dword_28C8;
	puVar15 = (uchar *)0x0;
	fVar19 = textSizeY[2] - local_188[2];
	if (textSizeY[2] - local_188[2] <= textSizeY[3] - local_188[3]) {
		fVar19 = textSizeY[3] - local_188[3];
	}
	fVar18 = local_188[2];
	if (local_188[2] <= local_188[3]) {
		fVar18 = local_188[3];
	}
	if (unk2 <= fVar18) {
		unk2 = fVar18;
	}
	if (unk1 <= fVar19) {
		unk1 = fVar19;
	}
	local_1c8 = (uint)currentSelectedFont->fontGlyphs->byte_6;
	fVar21 = 0.0;
	unk1 = unk1 + unk2;
	fVar22 = 0.0;
	printText = *(char **)(param_1->dataValues + local_1a0->textOffset);
	uVar14 = 0;
	unk2 = *(float *)(param_1->dataValues + local_1a0->uint16_C);
	fVar19 = *(float *)(param_1->dataValues + local_1a0->uint16_10);
	local_168 = uiFontAtlases +
		fontIndices[styleDescriptors[styleDescriptorIndex_0].fontIndex];
	fVar18 = 0.0;
	currenCharPointer = printText;
	local_1b0 = local_1c4;
	rpakFonts[0] = currentSelectedFont;
	uVar12 = 0;
LAB_1800f6ca0:
	do {
		while( true ) {
			currentUnicodeChar = getUnicodeCharacter_GPT(&currenCharPointer);
			local_res20 = (float)((int)local_res20 + 1);
			iVar13 = (int)uVar14;
			if ((0xefffe < currentUnicodeChar - 1) || (currentUnicodeChar == 0x60)) break;
			if (((currentUnicodeChar == 0x25) && (cVar2 = *currenCharPointer, ' ' < cVar2)) &&
				(('?' < cVar2 || ((1 << (cVar2 - 0x20U & 0x1f) & 0x80005002U) == 0)))) {
				if (cVar2 == '%') {
					currenCharPointer = currenCharPointer + 1;
					goto LAB_1800f6d4d;
				}
				pcVar6 = (char *)sub_F98F0(param_1,local_158,&currenCharPointer,(long long)local_150,printText);
				if (pcVar6 == (char *)0x0) goto LAB_1800f71fa;
				apcStack_128[uVar14] = currenCharPointer;
				uVar14 = (ulonglong)(iVar13 + 1);
				currenCharPointer = pcVar6;
			}
			else {
			LAB_1800f6d4d:
				uVar5 = getFontGlyphIndex(currentSelectedFont,(ulonglong)currentUnicodeChar);
				glyph = currentSelectedFont->fontGlyphs + uVar5;
				uVar7 = glyph->word_4;
				if (uVar7 < glyph[1].word_4) {
					do {
						if (currentSelectedFont->pointer_58[uVar7].dword_0 == uVar12) {
							fVar17 = currentSelectedFont->pointer_58[uVar7].float_4;
							goto LAB_1800f6d97;
						}
						uVar7 = uVar7 + 1;
					} while (uVar7 < glyph[1].word_4);
				}
				fVar17 = 0.0;
			LAB_1800f6d97:
				fVar17 = textSizeX[currentFontIndex] * fVar17 + fVar21;
				fVar21 = textSizeX[currentFontIndex] * glyph->float_0 + fVar17;
				uVar12 = currentUnicodeChar;
				if (puVar15 == (uchar *)0x0) {
					if (currentUnicodeChar == L' ') {
						local_res18 = 1;
					}
					else if (currentUnicodeChar == L'\n') {
						uVar12 = psVar4->dword_28C8;
						uVar11 = (ulonglong)uVar12;
						psVar4->dword_28C8 = uVar12 + 1;
						if (uVar12 < 0x40) {
							psVar4->float_25C4[uVar11 * 3 + 2] = fVar18;
							psVar4->float_25C4[uVar11 * 3 + 1] = local_res20;
							psVar4->float_25C4[uVar11 * 3 + 3] = 0.0;
						}
						local_1c4 = psVar4->dword_28CC;
						fVar21 = 0.0;
						local_1c8 = (uint)glyph->byte_6;
						local_res18 = 0;
						if (fVar23 <= fVar18) {
							fVar23 = fVar18;
						}
						fVar22 = fVar22 + fVar19 + unk1;
						fVar18 = fVar21;
						uVar12 = currentUnicodeChar;
					}
					else {
						uVar12 = (uint)local_res18 +
							(local_168->uint16_2 * local_1c8 + (uint)glyph->byte_6) * 2;
						if ((*(byte *)((ulonglong)(uVar12 >> 3) + local_168->qword_18) &
							1 << ((byte)uVar12 & 7)) != 0) {
							local_1c4 = psVar4->dword_28CC;
							fVar24 = fVar18;
							fVar25 = fVar17;
							local_1b8 = local_res20;
						}
						if (unk2 < fVar21) {
							uVar11 = (ulonglong)local_1c4;
							fVar18 = fVar19 + unk1;
							if (uVar11 < psVar4->dword_28CC) {
								puVar8 = psVar4->gap_28D0 + uVar11 * 0x14 + 8;
								do {
									uVar11 = uVar11 + 1;
									*(float *)(puVar8 + -4) =
										(float)((uint)fVar25 ^ 0x80000000) + *(float *)(puVar8 + -4);
									*(float *)puVar8 = fVar18 + *(float *)puVar8;
									*(float *)(puVar8 + 4) =
										(float)((uint)fVar25 ^ 0x80000000) + *(float *)(puVar8 + 4);
									*(float *)(puVar8 + 8) = fVar18 + *(float *)(puVar8 + 8);
									puVar8 = puVar8 + 0x14;
								} while (uVar11 < psVar4->dword_28CC);
							}
							uVar12 = psVar4->dword_28C8;
							uVar11 = (ulonglong)uVar12;
							psVar4->dword_28C8 = uVar12 + 1;
							if (uVar12 < 0x40) {
								psVar4->float_25C4[uVar11 * 3 + 2] = fVar24;
								psVar4->float_25C4[uVar11 * 3 + 1] = local_1b8;
								psVar4->float_25C4[uVar11 * 3 + 3] = 0.0;
							}
							local_1c4 = psVar4->dword_28CC;
							fVar21 = fVar21 + (float)((uint)fVar25 ^ 0x80000000);
							fVar22 = fVar22 + fVar18;
							if (fVar23 <= fVar24) {
								fVar23 = fVar24;
							}
						}
						local_res18 = 0;
						local_1c8 = (uint)glyph->byte_6;
						fVar18 = fVar21;
						uVar12 = currentUnicodeChar;
					}
				}
			}
		}
		if (currentUnicodeChar != 0) {
			if (currentUnicodeChar != L'`') {
				if (0x1fff < currentUnicodeChar - 0xf0000) {
					uVar12 = currentUnicodeChar;
					if ((currentUnicodeChar == 0xf2001) &&
						(fVar17 = fVar22, fVar20 = fVar22, puVar15 != (uchar *)0x0)) {
						while( true ) {
							uVar12 = 0;
							uVar5 = (uint)uVar9;
							if (uVar5 != 0) {
								for (; (uVar5 >> uVar12 & 1) == 0; uVar12 = uVar12 + 1) {
								}
							}
							if (uVar5 == 0) break;
							fVar18 = fVar22 - local_188[uVar12];
							uVar9 = (ulonglong)(uVar5 & uVar5 - 1);
							if (fVar18 <= fVar17) {
								fVar17 = fVar18;
							}
							if (fVar20 <= fVar18 + textSizeY[uVar12]) {
								fVar20 = fVar18 + textSizeY[uVar12];
							}
						}
						*(float *)(puVar15 + 0x10) = fVar20;
						*(float *)(puVar15 + 0xc) = fVar21;
						*(float *)(puVar15 + 8) = fVar17;
						puVar15 = (uchar *)0x0;
						local_1c8 = 0;
						local_res18 = 0;
						fVar18 = fVar21;
						uVar12 = currentUnicodeChar;
					}
					goto LAB_1800f6ca0;
				}
				uVar5 = psVar4->dword_28CC;
				uVar9 = (ulonglong)uVar5;
				if (0x3f < uVar5) {
					uVar9 = 0x3f;
				}
				psVar4->dword_28CC = uVar5 + 1;
				uVar11 = (ulonglong)currentUnicodeChar & 0xffff;
				*(short *)(psVar4->gap_28D0 + uVar9 * 0x14) = (short)currentUnicodeChar;
				*(short *)(psVar4->gap_28D0 + uVar9 * 0x14 + 2) = (short)currentFontIndex;
				puVar15 = psVar4->gap_28D0 + uVar9 * 0x14;
				uVar9 = 1i64 << ((byte)currentFontIndex & 0x3f);
				*(float *)(puVar15 + 4) = fVar21;
				imageAtlasIndex = *(byte *)(assetIndexList->pointer_8 + 6 + uVar11 * 8);
				uVar7 = *(ushort *)(assetIndexList->pointer_8 + 4 + uVar11 * 8);
				puVar1 = (ushort *)
					(*(ulonglong *)&rpakUIMGAtlases[imageAtlasIndex].word_18 + (ulonglong)(ushort)uVar7 * 4
						);
				if (uVar12 == 0xf2000) {
					if (uVar7 < rpakUIMGAtlases[imageAtlasIndex].flags) {
						local_1c8 = 0;
						pfVar10 = (float *)((ulonglong)(ushort)uVar7 * 0x20 +
							(ulonglong)rpakUIMGAtlases[imageAtlasIndex].pointer_20);
						local_res18 = 0;
						fVar21 = fVar21 + (pfVar10[2] + *pfVar10) * (float)(uint)*puVar1;
						fVar18 = fVar21;
						uVar12 = currentUnicodeChar;
						goto LAB_1800f6ca0;
					}
				}
				else {
					uVar7 = *puVar1;
					uVar3 = puVar1[1];
					fVar18 = local_188[currentFontIndex];
					*(float *)(puVar15 + 8) = fVar22 - fVar18;
					fVar17 = ((float)(uint)uVar7 / (float)(uint)uVar3) * textSizeX[currentFontIndex];
					*(float *)(puVar15 + 0x10) = (fVar22 - fVar18) + textSizeY[currentFontIndex];
					fVar21 = fVar21 + fVar17;
					*(float *)(puVar15 + 0xc) = fVar17 + *(float *)(puVar15 + 4);
					puVar15 = (uchar *)0x0;
				}
				local_1c8 = 0;
				local_res18 = 0;
				fVar18 = fVar21;
				uVar12 = currentUnicodeChar;
				goto LAB_1800f6ca0;
			}
			styleDescriptorIndex_1 = *currenCharPointer - 0x30;
			currentFontIndex = (ulonglong)styleDescriptorIndex_1;
			if (3 < styleDescriptorIndex_1) {
			LAB_1800f71fa:
				uEndVar = psVar4->dword_28C8;
				if ((uEndVar != local_1b4) && (uEndVar < 65)) {
					psVar4->float_25C4[(ulonglong)(uEndVar - 1) * 3 + 3] = fVar21;
				}
				if (fVar23 <= fVar21) {
					fVar23 = fVar21;
				}
				endVar2 = param_2 >> 4;
				unk2 = *(float *)(param_1->dataValues + local_1a0->uint16_A);
				psVar4->unk2[endVar2].byte_6 = (uchar)local_1b0;
				fVar19 = unk2;
				if (unk2 <= fVar23) {
					fVar19 = fVar23;
				}
				psVar4->unk2[endVar2].float_0 = unk2 / fVar19;
				cVar2 = *(char *)&psVar4->dword_28CC;
				psVar4->unk2[endVar2].byte_4 = (uchar)local_1b4;
				fVar23 = (unk2 / fVar19) * fVar23;
				psVar4->unk2[endVar2].byte_7 = cVar2 - (uchar)local_1b0;
				_Var16.m128_f32[1] = fVar23;
				_Var16.m128_f32[0] = fVar23;
				_Var16.m128_f32[2] = fVar22 + unk1;
				_Var16.m128_f32[3] = fVar22 + unk1;
				psVar4->unk2[endVar2].byte_5 = *(char *)&psVar4->dword_28C8 - (uchar)local_1b4;
				return _Var16;
			}
			currenCharPointer = currenCharPointer + 1;
			currentSelectedFont = rpakFonts[currentFontIndex];
			uVar9 = (ulonglong)((uint)uVar9 | (uint)(1L << (styleDescriptorIndex_1 & 0x3f)));
			uVar12 = 0;
			goto LAB_1800f6ca0;
		}
		if (iVar13 == 0) goto LAB_1800f71fa;
		uVar14 = (ulonglong)(iVar13 - 1);
		currenCharPointer = apcStack_128[uVar14];
	} while( true );
}






AUTOHOOK(sub_F9B80,0,__int64,__fastcall,(void* a1, ruiDataStruct* a2, struct_v3 *a3, __m128 *a4, const __m128i *a5, int a6, __int64 a7, __m128i *a8, __m128 *a9, __m128 *a10, __m128 *a11))
{
	__int64 v15; // rax
	unknownRuiListElement *v16; // r8
	__int64 v17; // rdx
	uiImageAtlas *v18; // r13
	uiImageAtlas *v19; // rax
	int v20; // eax
	int v21; // eax
	int v23; // er14
	__int16 v24; // r14
	unsigned __int16 v25; // cx
	__m128i v26; // xmm1
	__m128 v27; // xmm2
	__m128 v28; // xmm3
	__m128 v29; // xmm5
	__m128 v30; // xmm6
	__m128 v31; // xmm0
	__m128i v32; // xmm5
	__m128i v33; // xmm0
	__m128 *v34; // rcx
	__int64 v35; // rcx
	__m128 v36; // xmm2
	__m128 v37; // xmm0
	__m128 v38; // xmm1
	__m128 v39; // xmm5
	__m128 v40; // xmm13
	__m128 v41; // xmm4
	__m128 v42; // xmm6
	__m128 v43; // xmm10
	__m128 v44; // xmm1
	__m128i v45; // xmm1
	__m128 v46; // xmm9
	__m128 v47; // xmm1
	__m128 v48; // xmm3
	__m128 v49; // xmm2
	__m128 v50; // xmm0
	__m128 v51; // xmm15
	__m128 v52; // xmm12
	__m128i v53; // xmm7
	__m128 v54; // xmm1
	__m128 v55; // xmm11
	__m128i v56; // xmm8
	__m128 v57; // xmm10
	__m128 v58; // xmm14
	__m128 v59; // xmm13
	__m128 v60; // xmm0
	__m128 v61; // xmm9
	__m128i v62; // xmm7
	__m128i v63; // xmm9
	__m128 v64; // xmm13
	char v65; // al
	__m128i v66; // xmm6
	__m128 v67; // xmm1
	__m128 v68; // xmm2
	__m128 *v69; // r9
	__m128 v70; // xmm4
	__m128 v71; // xmm5
	__m128 v72; // xmm0
	__m128 v73; // xmm0
	__m128i v74; // xmm4
	__m128i v75; // xmm0
	unsigned __int64 v76; // rax
	__m128 *v77; // rcx
	__m128i v78; // xmm1
	__m128 v79; // xmm2
	__m128 v80; // xmm3
	__m128 *v81; // r9
	__m128 v82; // xmm5
	__m128 v83; // xmm6
	__m128 v84; // xmm0
	__m128 v85; // xmm0
	__m128i v86; // xmm5
	__m128i v87; // xmm0
	unsigned __int64 v88; // rax
	__m128 *v89; // rcx
	char v90; // al
	__m128i v91; // xmm1
	__m128 v92; // xmm2
	__m128 v93; // xmm3
	__m128 *v94; // r9
	__m128 v95; // xmm5
	__m128 v96; // xmm6
	__m128 v97; // xmm0
	__m128 v98; // xmm0
	__m128i v99; // xmm5
	__m128i v100; // xmm0
	unsigned __int64 v101; // rax
	unsigned int *v102; // rcx

	__m128i v104; // xmm1
	__m128 v105; // xmm2
	__m128 v106; // xmm3
	__m128 *v107; // r9
	__m128 v108; // xmm5
	__m128 v109; // xmm6
	__m128 v110; // xmm0
	__m128 v111; // xmm0
	__m128i v112; // xmm5
	__m128i v113; // xmm0
	unsigned __int64 v114; // rax
	unsigned int *v115; // rcx

	__m128i v117; // xmm2
	__m128 v118; // xmm3
	__m128 v119; // xmm4
	__m128 v120; // xmm6
	__m128 v121; // xmm1
	__m128i v122; // xmm0
	__m128i v123; // xmm6
	unsigned __int64 v124; // rax
	unsigned int *v125; // rcx

	__m128i v127; // xmm1
	__m128 v128; // xmm2
	__m128 v129; // xmm3
	__m128 *v130; // r9
	__m128 v131; // xmm5
	__m128 v132; // xmm6
	__m128 v133; // xmm0
	__m128 v134; // xmm0
	__m128i v135; // xmm5
	__m128i v136; // xmm0
	unsigned __int64 v137; // rax
	unsigned int *v138; // rcx

	char v140; // al
	__m128i v141; // xmm1
	__m128 v142; // xmm2
	__m128 v143; // xmm3
	__m128 *v144; // r9
	__m128 v145; // xmm5
	__m128 v146; // xmm6
	__m128 v147; // xmm0
	__m128 v148; // xmm0
	__m128i v149; // xmm5
	__m128i v150; // xmm0
	unsigned __int64 v151; // rax
	__m128 *v152; // rcx
	__m128 v153; // xmm2
	__m128 v154; // xmm15
	__m128i v155; // xmm1
	__m128 v156; // xmm2
	__m128 v157; // xmm3
	__m128 *v158; // r9
	__m128 v159; // xmm5
	__m128 v160; // xmm6
	__m128 v161; // xmm0
	__m128 v162; // xmm0
	__m128i v163; // xmm5
	__m128i v164; // xmm0
	unsigned __int64 v165; // rax
	unsigned int *v166; // rcx

	__m128i v168; // xmm1
	__m128 v169; // xmm2
	__m128 v170; // xmm3
	__m128 *v171; // r9
	__m128 v172; // xmm5
	__m128 v173; // xmm6
	__m128 v174; // xmm0
	__m128 v175; // xmm0
	__m128i v176; // xmm5
	__m128i v177; // xmm0
	unsigned __int64 v178; // rax
	__m128 *v179; // rcx
	__m128 v180; // [rsp+30h] [rbp-D0h] BYREF
	__m128 v181; // [rsp+40h] [rbp-C0h]
	int v182[2]; // [rsp+50h] [rbp-B0h] BYREF
	__m128i v183; // [rsp+58h] [rbp-A8h]
	__m128i v184; // [rsp+68h] [rbp-98h]
	__m128 v185; // [rsp+80h] [rbp-80h] BYREF
	__m128 v186; // [rsp+90h] [rbp-70h]
	__m128 v187; // [rsp+A0h] [rbp-60h]
	unsigned __int64 v191; // [rsp+E0h] [rbp-20h]
	int v192; // [rsp+F0h] [rbp-10h]
	int v193; // [rsp+F4h] [rbp-Ch]
	__m128 v194; // [rsp+100h] [rbp+0h] BYREF
	__m128 v195; // [rsp+110h] [rbp+10h]
	__m128 v196[5]; // [rsp+120h] [rbp+20h] BYREF
	int v197; // [rsp+250h] [rbp+150h]
	char a7a; // [rsp+270h] [rbp+170h]

	v15 = a3->unsigned_int_8;
	v16 = a3->qword_0;
	v17 = v15;
	v18 = &rpakUIMGAtlases[*(unsigned __int8 *)(a7 + 6)];
	v19 = v16[v15].uiImageAtlas_10;
	if ( v19 != v18 )
	{
		if ( !v19 || (v20 = a3->dword_40, v16[v17].dword_4 == v20) )
		{
			v16[v17].uiImageAtlas_10 = v18;
		}
		else
		{
			v16[v17].dword_4 = v20;
			if ( ++a3->unsigned_int_8 == a3->dword_C )
				return 0i64;
			v16[v17 + 1].dword_4 = a3->dword_40;
			v21 = v16[v17].dword_0;
			v16[v17 + 1].uiFontAtlas_8 = 0i64;
			v16[v17 + 1].dword_0 = v21;
			v16[v17 + 1].uiImageAtlas_10 = v18;
		}
	}
	v23 = a4[6].m128_i16[3];
	v182[0] = 4;
	v182[1] = 4;
	v24 = (~v23 >> 8) & 0xF;
	if ( v24 )
		sub_FFAE0((__m128 *)a5, (const __m128i *)&a2->header->elementWidth, (__int64)v196);
	v25 = *(_WORD *)(a7 + 4);
	if ( v25 < v18->flags )
	{
		v35 = (__int64)v18->pointer_20 + 32 * (__int16)v25;
		v36 = _mm_rcp_ps(*a11);
		v37 = _mm_sub_ps((__m128)xmmword_5F3E90, _mm_mul_ps(v36, *a11));
		v38 = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i *)&a2->canvasWidth));
		v39 = _mm_xor_ps(*(__m128 *)v35, (__m128)xmmword_5F3E70);
		v40 = _mm_and_ps(*(__m128 *)v35, (__m128)xmmword_12A14650);
		v41 = _mm_add_ps(_mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i *)v35, 238)), v39);
		v42 = _mm_sub_ps((__m128)xmmword_5F3E90, v41);
		v43 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v37, v37), v37), v36), v36);
		v44 = _mm_mul_ps(_mm_mul_ps(_mm_unpacklo_ps(v38, v38), _mm_castsi128_ps(_mm_shuffle_epi32(*a5, 216))), v43);
		v45 = _mm_castps_si128(_mm_mul_ps(v44, v44));
		v46 = _mm_max_ps(
			_mm_mul_ps(
				_mm_sqrt_ps(_mm_add_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v45, 78)), _mm_castsi128_ps(v45))),
				_mm_castpd_ps(_mm_loaddup_pd((const double *)(v35 + 16)))),
				_mm_castpd_ps(_mm_loaddup_pd((const double *)(v35 + 24))));
		v47 = _mm_sub_ps(v46, v41);
		v48 = _mm_rcp_ps(v47);
		v49 = _mm_sub_ps((__m128)xmmword_5F3E90, _mm_mul_ps(v48, v47));
		v50 = _mm_movelh_ps(v46, (__m128)xmmword_5F3E90);
		v51 = _mm_mul_ps(v50, a4[2]);
		v52 = _mm_mul_ps(v50, a4[1]);
		v194 = _mm_add_ps(_mm_sub_ps((__m128)xmmword_5F3E90, v50), v51);
		v53 = _mm_castps_si128(_mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v49, v49), v49), v48), v48));
		v54 = _mm_movelh_ps(_mm_mul_ps(_mm_mul_ps(v42, v46), _mm_castsi128_ps(v53)), (__m128)xmmword_5F3E90);
		v55 = _mm_mul_ps(v54, *a4);
		v56 = _mm_castps_si128(_mm_mul_ps(
			_mm_sub_ps(
				_mm_add_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v53, 238)), v39), (__m128)xmmword_5F4600),
				*a10),
			v43));
		v57 = _mm_mul_ps(v50, *a4);
		v58 = _mm_mul_ps(v54, a4[1]);
		v59 = _mm_sub_ps(v40, _mm_mul_ps(_mm_mul_ps(v40, v42), _mm_castsi128_ps(v53)));
		v60 = _mm_castsi128_ps(_mm_shuffle_epi32(v56, 216));
		v61 = _mm_castsi128_ps(_mm_shuffle_epi32(*a8, 216));
		v62 = _mm_castps_si128(_mm_unpackhi_ps(v61, v60));
		v63 = _mm_castps_si128(_mm_unpacklo_ps(v61, v60));
		v64 = _mm_add_ps(v59, _mm_mul_ps(v54, a4[2]));
		v65 = _mm_movemask_ps(_mm_cmple_ps(*a9, _mm_xor_ps(_mm_castsi128_ps(v56), (__m128)xmmword_5F3E20)));
		a7a = v65;
		v197 = _mm_movemask_ps(_mm_cmple_ps(*a9, _mm_xor_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v56, 78)), (__m128)xmmword_5F3E20)));
		if ( (v65 & 3) == 0 )
		{
			v66 = _mm_load_si128(a5);
			v67 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 20));
			v68 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 80));
			v69 = &stru_5F4740[v24 & 5];
			v70 = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v66, 170)), v67),
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v66, 0)), v68)),
				_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
			v180 = v70;
			v71 = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v66, 255)), v67),
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v66, 85)), v68)),
				_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
			v72 = _mm_cmpneq_ps(*v69, _mm_setzero_ps());
			v181 = v71;
			if ( _mm_movemask_ps(v72) )
			{
				sub_FEF30(a1, a2, v196, (__m128i *)v69, &v180);
				v71 = v181;
				v70 = v180;
			}
			v73 = v70;
			v74 = _mm_castps_si128(_mm_unpackhi_ps(v70, v71));
			v75 = _mm_castps_si128(_mm_unpacklo_ps(v73, v71));
			if ( a6 == 2 )
			{
				v75 = _mm_shuffle_epi32(v75, 78);
				v74 = _mm_shuffle_epi32(v74, 78);
			}
			v76 = a4[6].m128_u64[0];
			v77 = (__m128 *)a2->pvoid_38;
			v187 = v51;
			v185 = v57;
			v186 = v52;

			v191 = v76;
			v183 = v75;
			v184 = v74;

			if ( !(unsigned int)funcs_5F4560[v77->m128_u32[0]](v77, &v185, (_DWORD*)v182, a3) )
				return 0i64;
			v65 = a7a;
		}
		v193 = v197 & 5;
		if ( v193 | v65 & 2 )
			goto LABEL_76;
		v78 = _mm_load_si128(a5);
		v79 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 20));
		v80 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 245));
		v81 = &stru_5F4740[v24 & 4];
		v82 = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v78, 170)), v79),
				_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v78, 0)), v80)),
					_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
		v180 = v82;
		v83 = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v78, 255)), v79),
				_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v78, 85)), v80)),
					_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
		v84 = _mm_cmpneq_ps(_mm_setzero_ps(), *v81);
		v181 = v83;
		if ( _mm_movemask_ps(v84) )
		{
			sub_FEF30(a1, a2, v196, (__m128i *)v81, &v180);
			v83 = v181;
			v82 = v180;
		}
		v85 = v82;
		v86 = _mm_castps_si128(_mm_unpackhi_ps(v82, v83));
		v87 = _mm_castps_si128(_mm_unpacklo_ps(v85, v83));
		if ( a6 == 2 )
		{
			v87 = _mm_shuffle_epi32(v87, 78);
			v86 = _mm_shuffle_epi32(v86, 78);
		}
		v88 = a4[6].m128_u64[0];
		v89 = (__m128 *)a2->pvoid_38;
		v183 = v87;
		v184 = v86;
		v191 = v88;

		v187 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v51), _mm_and_ps(v64, (__m128)xmmword_12A146D0));
		v185 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v57), _mm_and_ps(v55, (__m128)xmmword_12A146D0));
		v186 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v52), _mm_and_ps(v58, (__m128)xmmword_12A146D0));
		if ( (unsigned int)funcs_5F4560[v89->m128_u32[0]](v89, &v185, (_DWORD*)v182, a3) )
		{
		LABEL_76:
			v90 = a7a;
			if ( (a7a & 6) == 0 )
			{
				v91 = _mm_load_si128(a5);
				v92 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 20));
				v93 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 175));
				v94 = &stru_5F4740[v24 & 6];
				v95 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v91, 170)), v92),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v91, 0)), v93)),
							_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
				v180 = v95;
				v96 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v91, 255)), v92),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v91, 85)), v93)),
							_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
				v97 = _mm_cmpneq_ps(*v94, _mm_setzero_ps());
				v181 = v96;
				if ( _mm_movemask_ps(v97) )
				{
					sub_FEF30(a1, a2, v196, (__m128i *)v94, &v180);
					v96 = v181;
					v95 = v180;
				}
				v98 = v95;
				v99 = _mm_castps_si128(_mm_unpackhi_ps(v95, v96));
				v100 = _mm_castps_si128(_mm_unpacklo_ps(v98, v96));
				if ( a6 == 2 )
				{
					v100 = _mm_shuffle_epi32(v100, 78);
					v99 = _mm_shuffle_epi32(v99, 78);
				}
				v101 = a4[6].m128_u64[0];
				v102 = (unsigned int *)a2->pvoid_38;
				v183 = v100;
				v184 = v99;
				v191 = v101;
				v185 = v57;
				v186 = v52;

				v187 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v51), _mm_and_ps(v194, (__m128)xmmword_12A146D0));
				if ( !funcs_5F4560[*v102]((__m128*)v102, &v185, (_DWORD*)v182,a3) )
					return 0i64;
				v90 = a7a;
			}
			v192 = v197 & 0xA;
			if ( v192 | v90 & 1 )
				goto LABEL_77;
			v104 = _mm_load_si128(a5);
			v105 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 125));
			v106 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 80));
			v107 = &stru_5F4740[v24 & 1];
			v108 = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v104, 170)), v105),
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v104, 0)), v106)),
				_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
			v180 = v108;
			v109 = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v104, 255)), v105),
					_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v104, 85)), v106)),
				_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
			v110 = _mm_cmpneq_ps(*v107, _mm_setzero_ps());
			v181 = v109;
			if ( _mm_movemask_ps(v110) )
			{
				sub_FEF30(a1, a2, v196, (__m128i *)v107, &v180);
				v109 = v181;
				v108 = v180;
			}
			v111 = v108;
			v112 = _mm_castps_si128(_mm_unpackhi_ps(v108, v109));
			v113 = _mm_castps_si128(_mm_unpacklo_ps(v111, v109));
			if ( a6 == 2 )
			{
				v113 = _mm_shuffle_epi32(v113, 78);
				v112 = _mm_shuffle_epi32(v112, 78);
			}
			v114 = a4[6].m128_u64[0];
			v115 = (unsigned int *)a2->pvoid_38;
			v183 = v113;
			v184 = v112;
			v191 = v114;
			v187 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v51), _mm_and_ps(v64, (__m128)xmmword_12A146A0));
			v185 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v57), _mm_and_ps(v55, (__m128)xmmword_12A146A0));
			v186 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v52), _mm_and_ps(v58, (__m128)xmmword_12A146A0));
			if ( funcs_5F4560[*v115]((__m128*)v115, &v185, (_DWORD *)v182,a3) )
			{
			LABEL_77:
				if ( v197 )
					goto LABEL_78;
				v117 = _mm_load_si128(a5);
				v118 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 125));
				v119 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 245));
				v120 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v117, 170)), v118),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v117, 0)), v119)),
					_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
				v121 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v117, 255)), v118),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v117, 85)), v119)),
					_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
				v122 = _mm_castps_si128(_mm_unpacklo_ps(v120, v121));
				v123 = _mm_castps_si128(_mm_unpackhi_ps(v120, v121));
				if ( a6 == 2 )
				{
					v122 = _mm_shuffle_epi32(v122, 78);
					v123 = _mm_shuffle_epi32(v123, 78);
				}
				v124 = a4[6].m128_u64[0];
				v125 = (unsigned int *)a2->pvoid_38;
				v183 = v122;
				v187 = v64;
				v185 = v55;
				v191 = v124;
				v184 = v123;
				v186 = v58;
				if ( funcs_5F4560[*v125]((__m128*)v125, &v185, (_DWORD*)v182,a3) )
				{
				LABEL_78:
					if ( v192 | a7a & 4 )
						goto LABEL_52;
					v127 = _mm_load_si128(a5);
					v128 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 125));
					v129 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 175));
					v130 = &stru_5F4740[v24 & 2];
					v131 = _mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v127, 170)), v128),
							_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v127, 0)), v129)),
								_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
					v180 = v131;
					v132 = _mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v127, 255)), v128),
							_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v127, 85)), v129)),
						_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
					v133 = _mm_cmpneq_ps(_mm_setzero_ps(), *v130);
					v181 = v132;
					if ( _mm_movemask_ps(v133) )
					{
						sub_FEF30(a1, a2, v196, (__m128i *)v130, &v180);
						v132 = v181;
						v131 = v180;
					}
					v134 = v131;
					v135 = _mm_castps_si128(_mm_unpackhi_ps(v131, v132));
					v136 = _mm_castps_si128(_mm_unpacklo_ps(v134, v132));
					if ( a6 == 2 )
					{
						v136 = _mm_shuffle_epi32(v136, 78);
						v135 = _mm_shuffle_epi32(v135, 78);
					}
					v137 = a4[6].m128_u64[0];
					v138 = (unsigned int *)a2->pvoid_38;
					v183 = v136;
					v184 = v135;
					v191 = v137;
					v187 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v194), _mm_and_ps(v64, (__m128)xmmword_12A146A0));
					v185 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v57), _mm_and_ps(v55, (__m128)xmmword_12A146A0));
					v186 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0, v52), _mm_and_ps(v58, (__m128)xmmword_12A146A0));
					if ( funcs_5F4560[*v138]((__m128*)v138, &v185, (_DWORD *)v182,a3) )
					{
					LABEL_52:
						v140 = a7a;
						if ( (a7a & 9) != 0 )
						{
							v154 = v194;
						}
						else
						{
							v141 = _mm_load_si128(a5);
							v142 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 235));
							v143 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 80));
							v144 = &stru_5F4740[v24 & 9];
							v145 = _mm_add_ps(
								_mm_add_ps(
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v141, 170)), v142),
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v141, 0)), v143)),
										_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
							v180 = v145;
							v146 = _mm_add_ps(
								_mm_add_ps(
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v141, 255)), v142),
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v141, 85)), v143)),
										_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
							v147 = _mm_cmpneq_ps(*v144, _mm_setzero_ps());
							v181 = v146;
							if ( _mm_movemask_ps(v147) )
							{
								sub_FEF30(a1, a2, v196, (__m128i *)v144, &v180);
								v146 = v181;
								v145 = v180;
							}
							v148 = v145;
							v149 = _mm_castps_si128(_mm_unpackhi_ps(v145, v146));
							v150 = _mm_castps_si128(_mm_unpacklo_ps(v148, v146));
							if ( a6 == 2 )
							{
								v150 = _mm_shuffle_epi32(v150, 78);
								v149 = _mm_shuffle_epi32(v149, 78);
							}
							v151 = a4[6].m128_u64[0];
							v152 = (__m128 *)a2->pvoid_38;
							v183 = v150;
							v184 = v149;
							v185 = v57;
							v191 = v151;
							v153 = _mm_andnot_ps((__m128)xmmword_12A146A0, v51);
							v154 = v194;
							v186 = v52;
							v187 = _mm_or_ps(v153, _mm_and_ps(v194, (__m128)xmmword_12A146A0));
							if ( !(unsigned int)funcs_5F4560[v152->m128_u32[0]](v152, &v185, (_DWORD*)v182, a3) )
								return 0i64;
							v140 = a7a;
						}
						if ( v193 | v140 & 8 )
							goto LABEL_79;
						v155 = _mm_load_si128(a5);
						v156 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 235));
						v157 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 245));
						v158 = &stru_5F4740[v24 & 8];
						v159 = _mm_add_ps(
							_mm_add_ps(
								_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v155, 170)), v156),
								_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v155, 0)), v157)),
									_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
						v180 = v159;
						v160 = _mm_add_ps(
							_mm_add_ps(
								_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v155, 255)), v156),
								_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v155, 85)), v157)),
									_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
						v161 = _mm_cmpneq_ps(*v158, _mm_setzero_ps());
						v181 = v160;
						if ( _mm_movemask_ps(v161) )
						{
							sub_FEF30(a1, a2, v196, (__m128i *)v158, &v180);
							v160 = v181;
							v159 = v180;
						}
						v162 = v159;
						v163 = _mm_castps_si128(_mm_unpackhi_ps(v159, v160));
						v164 = _mm_castps_si128(_mm_unpacklo_ps(v162, v160));
						if ( a6 == 2 )
						{
							v164 = _mm_shuffle_epi32(v164, 78);
							v163 = _mm_shuffle_epi32(v163, 78);
						}
						v165 = a4[6].m128_u64[0];
						v166 = (unsigned int *)a2->pvoid_38;
						v183 = v164;
						v184 = v163;
						v191 = v165;
						v187 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v154), _mm_and_ps(v64, (__m128)xmmword_12A146D0));
						v185 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v57), _mm_and_ps(v55, (__m128)xmmword_12A146D0));
						v186 = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0, v52), _mm_and_ps(v58, (__m128)xmmword_12A146D0));
						if ( funcs_5F4560[*v166](
							(__m128*)v166,
							&v185,
							(_DWORD*)v182,a3) )
						{
						LABEL_79:
							if ( (a7a & 0xC) != 0 )
								return 1i64;
							v168 = _mm_load_si128(a5);
							v169 = _mm_castsi128_ps(_mm_shuffle_epi32(v62, 235));
							v170 = _mm_castsi128_ps(_mm_shuffle_epi32(v63, 175));
							v171 = &stru_5F4740[v24 & 0xA];
							v172 = _mm_add_ps(
								_mm_add_ps(
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v168, 170)), v169),
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v168, 0)), v170)),
								_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
							v180 = v172;
							v173 = _mm_add_ps(
								_mm_add_ps(
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v168, 255)), v169),
									_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v168, 85)), v170)),
								_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
							v174 = _mm_cmpneq_ps(_mm_setzero_ps(), *v171);
							v181 = v173;
							if ( _mm_movemask_ps(v174) )
							{
								sub_FEF30(a1, a2, v196, (__m128i *)v171, &v180);
								v173 = v181;
								v172 = v180;
							}
							v175 = v172;
							v176 = _mm_castps_si128(_mm_unpackhi_ps(v172, v173));
							v177 = _mm_castps_si128(_mm_unpacklo_ps(v175, v173));
							if ( a6 == 2 )
							{
								v177 = _mm_shuffle_epi32(v177, 78);
								v176 = _mm_shuffle_epi32(v176, 78);
							}
							v178 = a4[6].m128_u64[0];
							v179 = (__m128 *)a2->pvoid_38;
							v187 = v154;
							v185 = v57;
							v186 = v52;
							v191 = v178;
							v183 = v177;
							v184 = v176;
							if ( (unsigned int)funcs_5F4560[v179->m128_u32[0]](v179, &v185, (_DWORD*)v182, a3) )
								return 1i64;
						}
					}
				}
			}
		}
		return 0i64;
	}
	v26 = _mm_load_si128(a5);
	v27 = _mm_castsi128_ps(_mm_shuffle_epi32(*a8, 125));
	v28 = _mm_castsi128_ps(_mm_shuffle_epi32(*a8, 160));
	v29 = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v26, 170)), v27),
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v26, 0)), v28)),
			_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 0)));
	v194 = v29;
	v30 = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v26, 255)), v27),
			_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v26, 85)), v28)),
				_mm_castsi128_ps(_mm_shuffle_epi32(a5[1], 85)));
	v195 = v30;
	if ( v24 )
	{
		sub_FEF30(a1, a2, v196, (__m128i *)&stru_5F4740[v24], &v194);
		v30 = v195;
		v29 = v194;
	}
	v31 = v29;
	v32 = _mm_castps_si128(_mm_unpackhi_ps(v29, v30));
	v33 = _mm_castps_si128(_mm_unpacklo_ps(v31, v30));
	if ( a6 == 2 )
	{
		v33 = _mm_shuffle_epi32(v33, 78);
		v32 = _mm_shuffle_epi32(v32, 78);
	}
	v34 = (__m128 *)a2->pvoid_38;
	v183 = v33;
	v184 = v32;
	return funcs_5F4560[v34->m128_u32[0]](v34, a4, (_DWORD*)v182, a3);
}

struct ruiRenderList
{
	void *globals;
	_QWORD qword_8;
	uint16_t word_10;
	uint16_t word_12;
	_WORD word_14;
	uint16_t ruiCount;
	ruiDataStruct *ruiInstances[1];
};


//__int64 __fastcall ruiUnknown9Func_0(ruiRenderList *a1, ruiDataStruct *a2, unknown9dataStruct_0 *a3, struct_v3 *a4)
AUTOHOOK(ruiUnknown9Func_0,engine.dll +0xF5840,__int64,__fastcall,(ruiRenderList *a1, ruiDataStruct *a2, unknown9dataStruct_0 *a3, struct_v3 *a4))
{
	struct_v1 *v4; // r12
	__int64 transformIndex; // r10
	__m128i v8; // xmm10
	testStruct *v10; // rcx MAPDST
	__m128 v11; // xmm2
	__m128 v13; // xmm0
	__m128 v14; // xmm12
	__m128i v15; // xmm1
	__m128 v16; // xmm13
	ruiHeader *v17; // r15
	__m128 v18; // xmm6
	unknown8dataStruct *styleDescriptorBasePointer; // rax
	__m128 v26; // xmm3
	__m128i v31; // xmm15
	float v38; // xmm0_4
	float v39; // xmm2_4
	float v40; // xmm0_4
	float v42; // xmm0_4
	unsigned __int64 v43; // rsi
	unsigned __int8 v44; // al
	__int64 v45; // rbx
	__m128i v46; // xmm0
	__m128 v47; // xmm6
	int v48; // edi
	__m128 v49; // xmm7
	__int64 v50; // rax
	__int64 v51; // rdx
	char *v52; // r10
	__m128 v53; // xmm0
	__m128 v54; // xmm2
	__m128 v55; // xmm4
	__m128 v56; // xmm1
	__int64 v57; // r8
	__int64 v58; // rax
	__m128 v59; // xmm8
	__m128 v60; // xmm0
	__int16 v61; // cx
	__m128 v62; // xmm0
	__m128 v63; // xmm1
	__m128i v64; // xmm2
	__m128 v65; // xmm0
	int v66; // eax
	unknownRuiListElement *v69; // rdx
	uiFontAtlas *v70; // r8
	__int64 v71; // rax
	__int64 v72; // rcx
	uiFontAtlas *v73; // r9
	int v74; // er9
	unsigned int v75; // eax
	int v76; // eax
	int v77; // ecx
	unsigned __int8 selectedFontIndex; // bl
	char *printText; // rdx
	bool v80; // zf
	char *currentCharPointer1; // rax
	__int64 v82; // rdx
	unsigned int v83; // eax
	unsigned int v84; // er13
	float v85; // xmm14_4
	float v86; // xmm14_4
	unsigned int v87; // eax
	float v88; // xmm0_4
	float v89; // xmm12_4
	rpakFontGlyph *v91; // rbx
	unsigned __int16 *v93; // rcx
	float v94; // xmm11_4
	__m128 v95; // xmm12
	__m128 v96; // xmm13
	float v97; // xmm10_4
	__m128 v98; // xmm8
	__m128 v99; // xmm8
	__m128 v100; // xmm5
	__m128 v101; // xmm4
	int v102; // xmm6_4
	__m128 v103; // xmm9
	__m128 v104; // xmm3
	__m128i v105; // xmm12
	__m128 v106; // xmm0
	__m128 v107; // xmm3
	__m128 v108; // xmm0
	float v109; // xmm9_4
	float v110; // xmm8_4
	int v111; // er14
	unsigned int v112; // esi
	float v113; // xmm10_4
	__m128 v114; // xmm13
	__m128 v115; // xmm15
	unsigned int v116; // er15
	int currentUnicodeChar; // eax
	int v118; // edi
	char v119; // dl
	char *v120; // rax
	__int64 v121; // rdx
	bool v122; // r12
	float v123; // xmm11_4
	__int32 v124; // eax
	rpakFont *v125; // r10
	int v126; // edx
	int v127; // er8
	unknownFontStruct *v128; // rcx
	float v129; // xmm0_4
	float *v130; // rax
	float v131; // xmm5_4
	bool v132; // r15
	float v133; // xmm0_4
	__m128i v134; // xmm4
	__m128i v135; // xmm5
	__m128 v136; // xmm0
	__m128 v137; // xmm1
	unsigned __int64 v138; // rdx
	float *v139; // r8
	float v140; // xmm7_4
	__m128 v141; // xmm8
	__m128 v142; // xmm9
	__m128 v143; // xmm4
	__m128 v144; // xmm2
	__m128 v145; // xmm3
	float v146; // xmm7_4
	__m128 v147; // xmm1
	__m128 v148; // xmm5
	__m128 v149; // xmm5
	__m128i v150; // xmm6
	__m128 v151; // xmm6
	__m128i v152; // xmm2
	void *v153; // rcx
	__m128i v154; // xmm1
	__m128i v155; // xmm2
	unsigned int *v156; // rcx
	__int64 v158; // rax
	float v159; // xmm0_4
	float v160; // xmm0_4
	__m128i v161; // xmm1
	float v162; // xmm10_4
	__m128i v163; // xmm0
	__int64 v164; // r9
	float v165; // xmm1_4
	float *v166; // rdx
	float v167; // xmm1_4
	bool v168; // [rsp+60h] [rbp-A0h]
	char *currentCharPointer; // [rsp+68h] [rbp-98h] BYREF
	unsigned int v170; // [rsp+70h] [rbp-90h]
	unsigned int v171; // [rsp+74h] [rbp-8Ch]
	float v172; // [rsp+78h] [rbp-88h]
	float v173; // [rsp+7Ch] [rbp-84h]
	unsigned int v174; // [rsp+80h] [rbp-80h]
	__m128 v175; // [rsp+90h] [rbp-70h] BYREF
	int v176; // [rsp+A0h] [rbp-60h]
	float v177; // [rsp+A4h] [rbp-5Ch]
	__m128 v178; // [rsp+B0h] [rbp-50h] BYREF
	__m128 v179; // [rsp+C0h] [rbp-40h]
	__m128 v180; // [rsp+D0h] [rbp-30h] BYREF
	__m128 v181; // [rsp+E0h] [rbp-20h]
	unsigned int v182; // [rsp+F0h] [rbp-10h]
	int v183; // [rsp+F4h] [rbp-Ch]
	float v184; // [rsp+F8h] [rbp-8h]
	rpakFont *a1a; // [rsp+100h] [rbp+0h]
	float v186; // [rsp+108h] [rbp+8h]
	struct_v1 *v188; // [rsp+118h] [rbp+18h]
	__m128 v189; // [rsp+120h] [rbp+20h] BYREF
	char *v190; // [rsp+130h] [rbp+30h]
	char v191[8]; // [rsp+138h] [rbp+38h] BYREF
	__m128i v192; // [rsp+140h] [rbp+40h]
	unknown8dataStruct* styleDescriptors[4]; // [rsp+150h] [rbp+50h]
	__m128 v194; // [rsp+170h] [rbp+70h] BYREF
	__m128 v195; // [rsp+180h] [rbp+80h]
	rpakFont* fontArray[4]; // [rsp+190h] [rbp+90h]
	__m128 v197; // [rsp+1B0h] [rbp+B0h]
	__m128 v198; // [rsp+1C0h] [rbp+C0h]
	__m128 v199; // [rsp+1D0h] [rbp+D0h]
	__m128i v200; // [rsp+1E0h] [rbp+E0h]
	int v201[2]; // [rsp+1F0h] [rbp+F0h] BYREF
	__m128i v202; // [rsp+1F8h] [rbp+F8h]
	__m128i v203; // [rsp+208h] [rbp+108h]
	__m128 v204; // [rsp+220h] [rbp+120h]
	__m128i v205; // [rsp+230h] [rbp+130h]
	__m128 v206; // [rsp+240h] [rbp+140h]
	__m128 v207[7]; // [rsp+250h] [rbp+150h] BYREF
	__int16 v208; // [rsp+2B0h] [rbp+1B0h]
	__int16 v209; // [rsp+2B2h] [rbp+1B2h]
	__int16 v210; // [rsp+2B4h] [rbp+1B4h]
	__int16 v211; // [rsp+2B6h] [rbp+1B6h]
	__m128 v212[6]; // [rsp+2C0h] [rbp+1C0h] BYREF
	__int16 v213; // [rsp+320h] [rbp+220h]
	__int16 v214; // [rsp+322h] [rbp+222h]
	__int16 v215; // [rsp+324h] [rbp+224h]
	__int16 v216; // [rsp+326h] [rbp+226h]
	__m128 v217[5]; // [rsp+330h] [rbp+230h] BYREF

	v4 = a2->v1;
	transformIndex = a3->transformIndex;
	v188 = v4;
	v8 = _mm_castps_si128(v4->m128_2DD0[transformIndex]);
	v205 = v8;
	if ( _mm_movemask_ps(_mm_cmpeq_ps(_mm_setzero_ps(), _mm_castsi128_ps(v8))))
		return 1i64;
	v10 = &v4->m128_3A80[transformIndex];
	//test if ui is in screen
	v11 = _mm_sub_ps(
		_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_0, 255)), _mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_0, 0))),
		_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_0, 170)), _mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_0, 85))));
	if ( _mm_movemask_ps(_mm_cmpeq_ps(v11, _mm_setzero_ps())) )
		return 1i64;



	v13 = _mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_10, 216));
	v14 = _mm_div_ps(_mm_xor_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_0, 39)), (__m128)xmmword_5F3E50), v11);
	v176 = _mm_movemask_ps(v11) & 2;
	v197 = v14;
	v15 = _mm_castps_si128(_mm_mul_ps(_mm_xor_ps(v14, (__m128)xmmword_5F3DD0), v13));
	v16 = _mm_add_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v15, 78)), _mm_castsi128_ps(v15));
	v198 = v16;
	v17 = a2->header;
	v18 = _mm_rcp_ps(_mm_castsi128_ps(v8));
	v26 = _mm_sub_ps((__m128)xmmword_5F3E90, _mm_mul_ps(v18, _mm_castsi128_ps(v8)));
	v31 = _mm_castps_si128(_mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v26, v26), v26), v18), v18));
	v192 = v31;

	float unk1 = std::numeric_limits<float>::min();
	styleDescriptorBasePointer = a2->header->unknown8data;
	for (int i = 0; i < 4; i++) {
		styleDescriptors[i] = &styleDescriptorBasePointer[a3->styleDescriptorsIndices[i]];
		fontArray[i] = rpakFontPointers[styleDescriptors[i]->fontIndex];
		unk1 = fmax(unk1,(float)(*(float *)&a2->dataValues[styleDescriptors[i]->textSize] * fontArray[i]->float_24) - *(float*)&a2->dataValues[styleDescriptors[i]->uint16_32]);
	}




	v38 = *(float *)_mm_shuffle_epi32(v31, 255).m128i_i32;
	v39 = v38 * unk1;
	v172 = v39;
	v40 = v38 * *(float *)&a2->dataValues[a3->uint16_10];
	v184 = v40;
	v42 = *(float *)v31.m128i_i32 * *(float *)&a2->dataValues[a3->uint16_E];
	v43 = (unsigned __int64)(unsigned int)((_DWORD)a3 - LODWORD(v17->unknown9data)) >> 4;
	v177 = v42;
	v44 = v4->unk2[v43].byte_7;
	if ( v44 )
	{
		v45 = (unsigned __int8)v4->unk2[v43].byte_6;
		v46 = _mm_setzero_si128();
		*(float *)v46.m128i_i32 = v39;
		v47 = _mm_castsi128_ps(_mm_shuffle_epi32(v31, 216));
		v48 = v45 + v44;
		v49 = _mm_castsi128_ps(_mm_shuffle_epi32(v46, 17));
		v180 = (__m128)xmmword_5F4600;
		while ( 1 )
		{
			v50 = *(unsigned __int16 *)&v4->gap_28D0[20 * v45];
			v51 = *((__int16 *)&unk_12A2E508 + 4 * v50 + 2);
			v52 = (char *)&unk_12A2E508 + 8 * v50;
			v53 = _mm_mul_ps(_mm_castpd_ps(_mm_loaddup_pd((const double *)&v4->gap_28D0[20 * v45 + 4])), v47);
			v54 = _mm_sub_ps(_mm_mul_ps(_mm_castpd_ps(_mm_loaddup_pd((const double *)&v4->gap_28D0[20 * v45 + 12])), v47), v53);
			v55 = _mm_add_ps(v49, v53);
			v56 = _mm_rcp_ps(v54);
			v57 = *(_QWORD *)rpakUIMGAtlases[(unsigned __int8)v52[6]].gap_10 + 32 * v51;
			v58 = *(unsigned __int16 *)&v4->gap_28D0[20 * v45 + 2];
			v59 = _mm_sub_ps(xmmword_5F3E90, _mm_mul_ps(v56, v54));
			v60 = *(__m128 *)v57;
			v207[6].m128_i16[0] = v51;
			v207[6].m128_i16[1] = -1;
			v61 = LOWORD(a4->unk_StartIndex) + *(&a3->styleDescriptorsIndices[0] + v58);
			v175 = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(v60, (__m128)xmmword_5F3E20), v54), v55);
			v189 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v59, v59), v59), v56), v56);
			v207[6].m128_i16[2] = v61;
			v207[6].m128_i16[3] = 7936;
			v62 = _mm_castpd_ps(_mm_loaddup_pd((const double *)(v57 + 24)));
			v63 = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(v16, v55), v189), v62);
			v64 = _mm_castps_si128(_mm_mul_ps(_mm_mul_ps(v14, v189), v62));
			v178 = _mm_xor_ps(_mm_mul_ps(v189, v55), xmmword_5F3DD0);
			v65 = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i *)(v57 + 16)));
			v207[5] = _mm_setzero_ps();
			v207[3] = _mm_setzero_ps();
			v207[2] = _mm_add_ps(v63, v65);
			v207[0] = _mm_castsi128_ps(_mm_shuffle_epi32(v64, 68));
			v207[4] = _mm_setzero_ps();
			v207[1] = _mm_castsi128_ps(_mm_shuffle_epi32(v64, 238));
			v66 = __autohookfuncsub_F9B80(a1->globals, a2, a4, v207, &v10->m128_0, v176, (__int64)v52, (__m128i *)&v175, &v180, &v178, &v189);
			if ( !v66 )
				return 0i64;
			v45 = (unsigned int)(v45 + 1);
			if ( (_DWORD)v45 == v48 )
				break;

		}
		v42 = v177;
	}
	v69 = a4->qword_0;
	v70 = &uiFontAtlases[fontIndices[styleDescriptors[0]->fontIndex]];
	v71 = a4->unsigned_int_8;
	v72 = v71;
	v73 = a4->qword_0[v71].uiFontAtlas_8;
	if ( v73 != v70 )
	{
		if ( !v73 || (v74 = a4->dword_40, v69[v71].dword_4 == v74) )
		{
			v69[v71].uiFontAtlas_8 = v70;
		}
		else
		{
			v75 = v71 + 1;
			a4->unsigned_int_8 = v75;
			if ( v75 != a4->dword_C )
			{
				v69[v72].dword_4 = v74;
				v69[v72 + 1].dword_4 = a4->dword_40;
				v76 = v69[v72].dword_0;
				v69[v72 + 1].uiFontAtlas_8 = v70;
				v69[v72 + 1].dword_0 = v76;
				v69[v72 + 1].uiImageAtlas_10 = 0i64;
			}
		}
	}
	v77 = 0;
	selectedFontIndex = 0;
	printText = *(char **)&a2->dataValues[a3->textOffset];
	v170 = 0;
	currentCharPointer = printText;

	//select font
	if ( *printText == '`' )
	{
		do
		{
			selectedFontIndex = currentCharPointer[1] - 48;
			if ( selectedFontIndex >= 4u )
				break;
			currentCharPointer += 2;
			++v77;
			;
		}
		while ( *currentCharPointer == '`' );
		v170 = v77;
	}
	v82 = (unsigned __int8)v4->unk2[v43].byte_4;
	v83 = v82 + (unsigned __int8)v4->unk2[v43].byte_5;
	v84 = 0;
	v171 = -1;
	v85 = 0.0;
	v174 = v82;
	v182 = v83;
	if ( (unsigned int)v82 < v83 )
	{
		v86 = *(float *)v8.m128i_i32 - v4->float_25C4[3 * v82 + 2];
		v87 = LODWORD(v4->float_25C4[3 * v82 + 1]);
		v174 = v82 + 1;
		v171 = v87;
		v85 = v86 * v42;
	}
	v88 = v4->unk2[v43].float_0;
	v173 = 0.0;
	v186 = v88;
	sub_FFAE0((__m128 *)v10, (const __m128i *)&v17->elementWidth, (__int64)v217);
	v89 = 0.0;
	v189 = _mm_setzero_ps();
	v204 = _mm_castsi128_ps(_mm_shuffle_epi32(v8, 216));
	while ( 2 )
	{
		v181 = _mm_setzero_ps();
		v179 = _mm_setzero_ps();
		v175 = _mm_setzero_ps();

		v91 = 0i64;
		a1a = fontArray[selectedFontIndex];
		v93 = (unsigned __int16 *)styleDescriptors[selectedFontIndex];

		v216 = 0;
		v215 = LOWORD(a4->unk_StartIndex) + *(&a3->styleDescriptorsIndices[0] + selectedFontIndex);
		v94 = fmaxf(*(float *)&a2->dataValues[v93[23]], v89);
		v95 = _mm_load_ps(&v186);
		v96 = _mm_load_ps((float *)&a2->dataValues[v93[20]]);
		v95.m128_f32[0] = (float)(v186 * *(float *)&a2->dataValues[v93[21]]) * v96.m128_f32[0];
		v97 = *(float *)&a2->dataValues[v93[22]];
		v98 = _mm_unpacklo_ps(v95, v96);
		v99 = _mm_movelh_ps(v98, v98);
		v100 = _mm_rcp_ps(v99);
		v101 = _mm_sub_ps((__m128)xmmword_5F3E90, _mm_mul_ps(v100, v99));
		v168 = fmaxf(
			*(float *)&a2->dataValues[v93[4]],
			fminf(fmaxf(*(float *)&a2->dataValues[v93[8]], *(float *)&a2->dataValues[v93[12]]), v97)) > 0.0;
		*(float *)&v102 = *(float *)v31.m128i_i32 * v95.m128_f32[0];
		v103 = _mm_unpacklo_ps(
			_mm_load_ps((float *)&a2->dataValues[v93[17]]),
			_mm_load_ps((float *)&a2->dataValues[v93[18]]));
		v104 = _mm_load_ps((float *)&a2->dataValues[v93[19]]);
		v105 = _mm_shuffle_epi32(v31, 255);
		v183 = v102;
		*(float *)v105.m128i_i32 = *(float *)v105.m128i_i32 * v96.m128_f32[0];
		v106 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_load_ps((float *)&a2->dataValues[v93[24]]),
					_mm_load_ps((float *)&a2->dataValues[v93[24]]),
				0),
			(__m128)xmmword_5F3EB0);
		v107 = _mm_max_ps(
			_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(v104, v104, 0), (__m128)xmmword_5F3EB0),
				_mm_xor_ps(_mm_movelh_ps(v103, v103), (__m128)xmmword_5F3E20)),
			v106);
		v106.m128_f32[0] = v97 + v94;
		v178 = _mm_mul_ps(
			_mm_xor_ps(_mm_add_ps(v107, _mm_shuffle_ps(v106, v106, 0)), (__m128)xmmword_5F3E20),
			_mm_castsi128_ps(_mm_shuffle_epi32(v31, 216)));
		v108 = _mm_castpd_ps(_mm_loaddup_pd((const double *)(&a1a->float_1C)));
		v109 = v178.m128_f32[3];
		v110 = v178.m128_f32[1];
		v201[0] = 4;
		v201[1] = 4;
		v111 = 0;
		v112 = 0;
		v113 = 0.0;
		v114 = _mm_setzero_ps();
		v115 = _mm_setzero_ps();
		v199 = _mm_mul_ps(
			_mm_mul_ps(_mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v101, v101), v101), v100), v100), v204),
			v108);
		v200 = _mm_castps_si128(_mm_mul_ps(v197, v199));
		v206 = _mm_mul_ps(v198, v199);
		v180 = (__m128)xmmword_5F4610;
		while ( 1 )
		{
			v116 = v170;
			while ( 1 )
			{
				while ( 1 )
				{
					currentUnicodeChar = getUnicodeCharacter_GPT(&currentCharPointer);
					++v116;
					v118 = currentUnicodeChar;
					v170 = v116;
					if ( currentUnicodeChar == '%')
						break;
					if ( currentUnicodeChar || !v84 )
						goto LABEL_35;
					currentCharPointer = (char *)v207[0].m128_u64[--v84];
				}
				v119 = *currentCharPointer;
				if ( *currentCharPointer <= 32 || v119 <= 63 && ((1 << (v119 - 32)) & 0x80005002) != 0 )
					goto LABEL_35;
				if ( v119 == 37 )
					break;
				v120 = (char *)sub_F98F0(a2, (__int64)a1, &currentCharPointer, (__int64)v191,*(char**) & a2->dataValues[a3->textOffset]);
				if ( !v120 )
					return 1i64;
				v121 = v84++;
				v207[0].m128_u64[v121] = (unsigned __int64)currentCharPointer;
				currentCharPointer = v120;
			}
			++currentCharPointer;
		LABEL_35:
			if ( (unsigned int)(currentUnicodeChar - 1) < 0xEFFFF )
				v122 = currentUnicodeChar == '`';
			else
				v122 = 1;
			v123 = 0.0;
			if ( v122 )
			{
				v131 = v175.m128_f32[0];
				v130 = (float *)v175.m128_u64[1];
				v125 = a1a;
			}
			else
			{
				v124 = getFontGlyphIndex(a1a, currentUnicodeChar);
				v125 = a1a;
				v91 = &a1a->fontGlyphs[v124];
				v126 = v91->word_4;
				v127 = v91[1].word_4;
				if ( v126 >= v127 )
				{
				LABEL_46:
					v129 = 0.0;
				}
				else
				{
					v128 = a1a->pointer_58;
					while ( v128[(unsigned __int16)v126].dword_0 != v111 )
					{
						v126 = v126 + 1;
						if ( (unsigned __int16)v126 >= v127 )
							goto LABEL_46;
					}
					v129 = v128[(unsigned __int16)v126].float_4;
				}
				v175.m128_i32[1] = v124;
				v123 = *(float *)&v102 * v91->float_0;
				v130 = &v91->float_0;
				v175.m128_u64[1] = (unsigned __int64)v91;
				v85 = v85 + (float)(v129 * *(float *)&v102);
				v131 = v85;
				v175.m128_f32[0] = v85;
			}
			v132 = v116 >= v171;
			if ( v168 )
			{
				if ( v132 || v122 )
				{
					if ( v112 > 1 )
					{
						v143 = v180;
					}
					else
					{
						if ( !v112 )
							goto LABEL_72;
						v143 = _mm_add_ps(v180, (__m128)xmmword_5F3EE0);
						v181 = v179;
						v180 = v143;
					}
					v139 = (float *)v179.m128_u64[1];
					v138 = v181.m128_u64[1];
					v112 = 0;
					v144 = _mm_load_ps((float *)(v181.m128_u64[1] + 20));
					v145 = _mm_load_ps((float *)(v181.m128_u64[1] + 28));
					v144.m128_f32[0] = fminf(v144.m128_f32[0], *(float *)(v179.m128_u64[1] + 20));
					v145.m128_f32[0] = fmaxf(v145.m128_f32[0], *(float *)(v179.m128_u64[1] + 28));
					v146 = (float)(*(float *)&v102 * *(float *)(v179.m128_u64[1] + 24)) + v179.m128_f32[0];
					v180 = _mm_add_ps(v143, (__m128)xmmword_5F3EF0);
					v140 = v146 + v178.m128_f32[2];
					v144.m128_f32[0] = (float)(v144.m128_f32[0] * *(float *)v105.m128i_i32) + v110;
					v145.m128_f32[0] = (float)(v145.m128_f32[0] * *(float *)v105.m128i_i32) + v109;
					v141 = v144;
					v142 = v145;
				}
				else
				{
					v133 = *(float *)&v91->gap_7[9];
					if ( v133 == *(float *)&v91->gap_7[17] )
					{
						v85 = v85 + v123;
						v111 = v118;
						v91 = 0i64;
						continue;
					}
					if ( v112 <= 1 )
					{
						v134 = v105;
						v135 = v105;
						*(float *)v134.m128i_i32 = (float)(*(float *)v105.m128i_i32 * *(float *)&v91->gap_7[13]) + v110;
						*(float *)v135.m128i_i32 = (float)(*(float *)v105.m128i_i32 * *(float *)&v91->gap_7[21]) + v109;
						if ( v112 )
						{
							v136 = _mm_setzero_ps();
							v137 = _mm_setzero_ps();
							v136.m128_f32[0] = v114.m128_f32[0];
							v137.m128_f32[0] = v115.m128_f32[0];
							v114 = v136;
							v115 = v137;
							v114.m128_f32[0] = fminf(v136.m128_f32[0], *(float *)v134.m128i_i32);
							v115.m128_f32[0] = fmaxf(v137.m128_f32[0], *(float *)v135.m128i_i32);
						}
						else
						{
							v114 = _mm_castsi128_ps(v134);
							v115 = _mm_castsi128_ps(v135);
							v113 = (float)((float)(*(float *)&v102 * v133) + v178.m128_f32[0]) + v85;
							v180 = _mm_sub_ps(v180, (__m128)xmmword_5F3EE0);
						}
						++v112;
						v85 = v85 + v123;
					LABEL_60:
						v181 = v179;
						v179 = v175;
					LABEL_61:
						v111 = v118;
						v91 = 0i64;
						continue;
					}
					v138 = v181.m128_u64[1];
					v139 = (float *)v179.m128_u64[1];
					v141 = _mm_load_ps((float *)(v181.m128_u64[1] + 20));
					v142 = _mm_load_ps((float *)(v181.m128_u64[1] + 28));
					v140 = (float)((float)((float)(*(float *)(v181.m128_u64[1] + 24) + v130[4]) * *(float *)&v102)
						+ (float)(v181.m128_f32[0] + v131))
						* 0.5;
					v141.m128_f32[0] = (float)(fminf(fminf(v141.m128_f32[0], *(float *)(v179.m128_u64[1] + 20)), v130[5])
						* *(float *)v105.m128i_i32)
						+ v178.m128_f32[1];
					v142.m128_f32[0] = (float)(fmaxf(fmaxf(v142.m128_f32[0], *(float *)(v179.m128_u64[1] + 28)), v130[7])
						* *(float *)v105.m128i_i32)
						+ v178.m128_f32[3];
				}
				v147 = _mm_setzero_ps();
				v148 = _mm_setzero_ps();
				v147.m128_f32[0] = v140;
				v148.m128_f32[0] = v113;
				v149 = _mm_shuffle_ps(v148, v147, 0);
				v150 = _mm_castps_si128(_mm_shuffle_ps(
					_mm_load_ps((float *)(*(_QWORD *)v125->gap_48 + 8i64 * *(unsigned __int8 *)(v138 + 7))),
						_mm_load_ps((float *)(*(_QWORD *)v125->gap_48 + 8i64 * *((unsigned __int8 *)v139 + 7))),
					0));
				v212[2] = _mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							v206,
							_mm_mul_ps(
								_mm_unpacklo_ps(
									_mm_unpacklo_ps(v181, v179),
									_mm_unpacklo_ps(_mm_load_ps(&v172), _mm_load_ps(&v172))),
								v199)),
						_mm_castsi128_ps(v150)),
					_mm_load_ps(v139 + 1));
				v212[0] = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v200, 68)), _mm_castsi128_ps(v150));
				v212[5] = _mm_castsi128_ps(_mm_shuffle_epi32(v150, 216));
				v212[1] = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v200, 238)), _mm_castsi128_ps(v150));
				v212[3] = _mm_setzero_ps();
				v212[4] = _mm_setzero_ps();
				v213 = *(_WORD *)&v125->gap_28[4] + v181.m128_i16[2];
				v214 = v179.m128_i16[2] + *(_WORD *)&v125->gap_28[4];
				v151 = _mm_add_ps(
					_mm_unpacklo_ps(_mm_unpacklo_ps(v114, v142), _mm_unpacklo_ps(v115, v141)),
					_mm_shuffle_ps(_mm_load_ps((float *)&v172), _mm_load_ps((float *)&v172), 0));
				v152 = _mm_load_si128(&v10->m128_0);
				v194 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v152, 0)), v149),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v152, 170)), v151)),
					_mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_10, 0)));
				v153 = a1->globals;
				v195 = _mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v152, 85)), v149),
						_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(v152, 255)), v151)),
					_mm_castsi128_ps(_mm_shuffle_epi32(v10->m128_10, 85)));
				sub_FEF30(v153, a2, v217, (__m128i *)&v180, &v194);
				v154 = _mm_castps_si128(_mm_unpackhi_ps(v194, v195));
				v155 = _mm_castps_si128(_mm_unpacklo_ps(v194, v195));
				if ( v176 == 2 )
				{
					v155 = _mm_shuffle_epi32(v155, 78);
					v154 = _mm_shuffle_epi32(v154, 78);
				}
				v202 = v155;
				v156 = (unsigned int *)a2->pvoid_38;
				v203 = v154;
				if ( !funcs_5F4560[*v156]((__m128*)v156, v212, (_DWORD *)v201, a4) )
					return 0i64;
				v102 = v183;
				v113 = v140;
				v114 = v141;
				v115 = v142;
				v109 = v178.m128_f32[3];
				v110 = v178.m128_f32[1];
				v180 = _mm_and_ps(v180, (__m128)xmmword_12A146B0);
			}
		LABEL_72:
			if ( v132 )
			{
				v172 = v172 + (float)(*(float *)v105.m128i_i32 + v184);
				if ( v174 >= v182 )
				{
					v171 = -1;
					v159 = v188->float_25C4[3 * v182];
				}
				else
				{
					v158 = 3i64 * v174;
					v171 = LODWORD(v188->float_25C4[3 * v174++ + 1]);
					v159 = v188->float_25C4[v158 + 2];
				}
				v85 = (float)(*(float *)v205.m128i_i32 - v159) * v177;
				if ( !v91 || (v160 = *(float *)&v91->gap_7[9], v160 == *(float *)&v91->gap_7[17]) )
				{
					v112 = 0;
				}
				else
				{
					v161 = v105;
					v175.m128_f32[0] = v85;
					v112 = 1;
					v162 = *(float *)&v102 * v160;
					v163 = v105;
					*(float *)v161.m128i_i32 = (float)(*(float *)v105.m128i_i32 * *(float *)&v91->gap_7[21]) + v109;
					v113 = (float)(v162 + v178.m128_f32[0]) + v85;
					v115 = _mm_castsi128_ps(v161);
					*(float *)v163.m128i_i32 = (float)(*(float *)v105.m128i_i32 * *(float *)&v91->gap_7[13]) + v110;
					v114 = _mm_castsi128_ps(v163);
				}
			}
			v85 = v85 + v123;
			if ( !v122 )
				goto LABEL_60;
			if ( !v118 )
				return 1i64;
			if ( v118 == '`' )
				break;
			if ( (unsigned int)(v118 - 0xF0000) >= 0x2000 )
			{
				if ( v118 != 991233 )
					goto LABEL_61;
				v85 = v85 + v173;
				v111 = 991233;
				v91 = 0i64;
				v173 = 0.0;
			}
			else
			{
				v164 = *(__int16 *)(assetIndexList->pointer_8 + 8i64 * (unsigned __int16)v118 + 4);
				v165 = (float)*(unsigned __int16 *)(*(_QWORD *)&rpakUIMGAtlases[*(unsigned __int8 *)(assetIndexList->pointer_8
					+ 8i64
					* (unsigned __int16)v118
					+ 6)].word_18
					+ 4 * v164);
				if ( v111 == 991232 )
				{
					if ( (unsigned __int16)v164 >= rpakUIMGAtlases[*(unsigned __int8 *)(assetIndexList->pointer_8
						+ 8i64 * (unsigned __int16)v118
						+ 6)].flags )
					{
						v173 = 0.0;
						v111 = v118;
						v85 = v85 + 0.0;
						v91 = 0i64;
					}
					else
					{
						v111 = v118;
						v166 = (float *)((char *)rpakUIMGAtlases[*(unsigned __int8 *)(assetIndexList->pointer_8
							+ 8i64 * (unsigned __int16)v118
							+ 6)].pointer_20
							+ 32 * v164);
						v91 = 0i64;
						v167 = *(float *)v192.m128i_i32 * v165;
						v85 = v85 + (float)(v167 * *v166);
						v173 = v167 * v166[2];
					}
				}
				else
				{
					v111 = v118;
					v91 = 0i64;
					v85 = v85
						+ (float)((float)(v165
							/ (float)*(unsigned __int16 *)(*(_QWORD *)&rpakUIMGAtlases[*(unsigned __int8 *)(assetIndexList->pointer_8 + 8i64 * (unsigned __int16)v118 + 6)].word_18
								+ 4 * v164
								+ 2))
							* *(float *)&v102);
				}
			}
		}
		selectedFontIndex = *currentCharPointer - 48;
		if ( selectedFontIndex < 4u )
		{
			++currentCharPointer;
			v31 = v192;
			v89 = v189.m128_f32[0];
			continue;
		}
		return 1i64;
	}
}

AUTOHOOK(sub_FB960,engine.dll + 0xFB960,void,__fastcall,(uiImageAtlas *a1, __int64 a2, __int64 a3))
{
	__int64 v3; // rdi
	unsigned int i; // ebx
	_DWORD *v8; // rax
	int v9; // edx
	char a3a; // [rsp+48h] [rbp+10h] BYREF

	v3 = 0i64;
	if ( a3 )
	{
		for ( i = 0;
			i < *(unsigned __int16 *)(a3 + 12);
			sub_F3E30(assetIndexList, *(unsigned int *)(*(_QWORD *)(a3 + 40) + 8i64 * i++)) )
		{
			;
		}
	}
	if ( a2 )
	{
		*a1 = *(uiImageAtlas *)a2;
		AcquireSRWLockExclusive(&assetIndexList->lock);
		if ( *(_WORD *)(a2 + 12) )
		{
			do
			{
				v8 = sub_F3BB0(assetIndexList, *(unsigned int *)(8 * v3 + *(_QWORD *)(a2 + 40)), (_BYTE*) &a3a);
				v9 = *(_DWORD *)(8 * v3 + *(_QWORD *)(a2 + 40));
				*((_WORD *)v8 + 2) = v3;
				*((_BYTE *)v8 + 6) = a1 - rpakUIMGAtlases;
				*v8 = v9;
				*((_BYTE *)v8 + 7) = *(_BYTE *)(*(_QWORD *)(a2 + 40) + 8 * v3 + 4);
				assetIndexList->pointer_10[assetIndexList->dword_34] = assetIndexList->dword_30;
				++assetIndexList->dword_0;
				v3 = (unsigned int)(v3 + 1);
			}
			while ( (unsigned int)v3 < *(unsigned __int16 *)(a2 + 12) );
		}
		ReleaseSRWLockExclusive(&assetIndexList->lock);
	}
}


ON_DLL_LOAD("rtech_game.dll", RpakTest, (CModule module)) {
	AUTOHOOK_DISPATCH_MODULE(rtech_game.dll)
}

ON_DLL_LOAD("engine.dll", EngineTest, (CModule module)) {
	AUTOHOOK_DISPATCH_MODULE(engine.dll)

		xmmword_5F3DD0 = *module.Offset(0x5F3DD0).As<__m128*>();
	xmmword_5F3E20 = *module.Offset(0x5F3E20).As<__m128*>();
	xmmword_5F3E50 = *module.Offset(0x5F3E50).As<__m128*>();
	xmmword_5F3E70 = *module.Offset(0x5F3E70).As<__m128*>();
	xmmword_5F3E80 = *module.Offset(0x5F3E80).As<__m128*>();
	xmmword_5F3E90 = *module.Offset(0x5F3E90).As<__m128*>();
	xmmword_5F3EB0 = *module.Offset(0x5F3EB0).As<__m128*>();
	xmmword_5F3EE0 = *module.Offset(0x5F3EE0).As<__m128*>();
	xmmword_5F3EF0 = *module.Offset(0x5F3EF0).As<__m128*>();
	xmmword_5F3F30 = *module.Offset(0x5F3F30).As<__m128*>();
	xmmword_5F3F60 = *module.Offset(0x5F3F60).As<__m128*>();
	xmmword_5F4600 = *module.Offset(0x5F4600).As<__m128*>();
	xmmword_5F4610 = *module.Offset(0x5F4610).As<__m128*>();

	xmmword_12A14650 = *module.Offset(0x12A14650).As<__m128*>();
	xmmword_12A146A0 = *module.Offset(0x12A146A0).As<__m128*>();
	xmmword_12A146B0 = *module.Offset(0x12A146B0).As<__m128*>();
	xmmword_12A146D0 = *module.Offset(0x12A146D0).As<__m128*>();
	rpakUIMGAtlases = module.Offset(0x12A26140).As<uiImageAtlas*>();
	assetIndexList = module.Offset(0x12A4E508).As<struct_a1_2*>();

	word_12A2E50C = module.Offset(0x12A2E50C).As<short*>();
	byte_12A2E50E = module.Offset(0x12A2E50E).As<BYTE*>();
	byte_12A2E50F = module.Offset(0x12A2E50F).As<BYTE*>();
	uiFontAtlases = module.Offset(0x12A26080).As<uiFontAtlas*>();//font atlas lol
	unk_12A2E508 = module.Offset(0x12A2E508).As<char*>();

	xmmword_12A4E830 = module.Offset(0x12A4E830).As<__m128* >();
	funcs_5F4560 = module.Offset(0x5F4560).As<funcs5F4560Type* >();
	sub_FC0C0= module.Offset(0xFC0C0).As<sub_FC0C0Type>();
	rpakFontPointers = module.Offset(0x12A4E550).As<rpakFont**>();


	getFontGlyphIndex = module.Offset(0xFAE80).As<getFontGlyphIndexType>();
	getUnicodeCharacter_GPT = module.Offset(0x00F2C40).As<getUnicodeCharacter_GPTType>();
	sub_F98F0 = module.Offset(0xF98F0).As<sub_F98F0Type>();
	//void (*sub_F9B80)(__int64 a1, __int64 a2, _QWORD *a3, __m128 *a4, const __m128i *a5, int a6, __int64 a7, __m128i *a8, __m128 *a9, __m128 *a10, __m128 *a11);
	sub_FFAE0 = module.Offset(0xFFAE0).As<sub_FFAE0Type>();
	sub_FEF30 = module.Offset(0xFEF30).As<sub_FEF30Type>();
	sub_F3BB0 = module.Offset(0xF3BB0).As<sub_F3BB0Type>();
	sub_F3E30 = module.Offset(0xF3E30).As<sub_F3E30Type>();
	stru_5F4740 = module.Offset(0x5F4740).As<__m128*>();
	fontIndices = module.Offset(0x12A4E650).As<BYTE*>();
}
