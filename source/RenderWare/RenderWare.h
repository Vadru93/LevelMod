#pragma once
#include "Extension\Defines.h"
#include "d3d8to9\d3d8to9.hpp"
#define RWFORCEENUMSIZEINT ((DWORD)((~((DWORD)0))>>1))

struct RwObject
{
    BYTE type;                /**< Internal Use */
    BYTE subType;             /**< Internal Use */
    BYTE flags;               /**< Internal Use */
    BYTE privateFlags;        /**< Internal Use */
    void* parent;              /**< Internal Use */
                                 /* Often a Frame  */
};

struct RwGeometry;

struct RpTriangle
{
    WORD matIndex;
    WORD a;
    WORD b;
    WORD c;
};

struct RwTexCoords
{
    float u, v;
};

struct RpVertexNormal
{
    char          x;
    char          y;
    char          z;
    BYTE          pad; /* pad character to force alignment */
};

enum RxClusterForcePresent
{
    rxCLALLOWABSENT = FALSE, /**<The cluster is allowed to be absent if no prior
                              *  nodes create it and no subsequent nodes require it */
                              rxCLFORCEPRESENT = TRUE, /**<The cluster must be present even if no prior
                                                        *  nodes create it and no subsequent nodes require it */

                                                        rxCLUSTERFORCEPRESENTFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RxClusterForcePresent RxClusterForcePresent;

/**
 * \ingroup rwcoregeneric
 * \struct RxClusterRef
 * Structure describing a cluster reference */
struct RxClusterRef
{
    void* clusterDef;           /**< Cluster definition */
    RxClusterForcePresent forcePresent;         /**< Specifies whether the cluster should be forced present */
    DWORD                 reserved;             /**< Omit or initialize to zero */
};

#define rxCLRESERVED       ((RwUInt32)0)

/**
 * \ingroup rwcoregeneric
 * \struct RxIoSpec
 * Structure describing an input/output specification of a node */
struct RxIoSpec
{
    DWORD         numClustersOfInterest;/**< Number of Clusters of interest */
    RxClusterRef* clustersOfInterest;   /**< Clusters of interest array */
    void* inputRequirements;    /**< Requirements of the clusters of interest on input to the node */
    DWORD numOutputs;           /**< Number of outputs from the node */
    void* outputs;              /**< Output specification array */
};

typedef struct RxPipelineNodeParam;

/**
 * \ingroup rwcoregeneric
 * \ref RxNodeBodyFn is the callback to be
 * called during pipeline execution -- and, typically, process
 * \ref RxPacket's -- for the owning pipeline node.
 *
 * \param  self   A pointer to the pipeline node being executed
 * \param  params   A pointer to a parameter structure
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RxNodeMethods
 */

typedef struct RxPipelineNode;

typedef BOOL(*RxNodeBodyFn) (RxPipelineNode* self,
    const RxPipelineNodeParam* params);

typedef struct rxHeapFreeBlock rxHeapFreeBlock;
typedef struct rxHeapSuperBlockDescriptor rxHeapSuperBlockDescriptor;
typedef struct RxHeap RxHeap;
typedef struct rxHeapBlockHeader rxHeapBlockHeader;


struct rxHeapFreeBlock
{
    DWORD            size;
    rxHeapBlockHeader* ptr;
};

struct rxHeapSuperBlockDescriptor
{
    void* start;
    DWORD size;
    rxHeapSuperBlockDescriptor* next;
};

struct RxHeap
{
    DWORD            superBlockSize; /**< Granularity of heap growth */
    rxHeapSuperBlockDescriptor* head;   /**< Internally used superblock pointer */
    rxHeapBlockHeader* headBlock;      /**< Internally used block pointer */
    rxHeapFreeBlock* freeBlocks;     /**< Internally used free blocks pointer */
    DWORD            entriesAlloced; /**< Number of entries allocated */
    DWORD            entriesUsed;    /**< Number of entries used */
    BOOL              dirty;          /**< Internally used boolean, flags whether
                                         *   the heap needs resetting or not. */
};

struct RxPipelineNodeParam
{
    void* dataParam;   /**< The data pointer passed in to \ref RxPipelineExecute */
    RxHeap* heap;        /**< The heap associated with the current pipeline exeuction */
};

enum rxEmbeddedPacketState
{
    rxPKST_PACKETLESS = 0, /* Packet not created */
    rxPKST_UNUSED = 1, /* Packet created and then destroyed */
    rxPKST_INUSE = 2, /* Packet created but not yet destroyed and fetched but not yet dispatched */
    rxPKST_PENDING = 3, /* Packet created but not destroyed and dispatched but not yet fetched */
    rxEMBEDDEDPACKETSTATEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum rxEmbeddedPacketState rxEmbeddedPacketState;

struct RwBBox
{
    /* Must be in this order */
    Vertex sup;   /**< Supremum vertex. */
    Vertex inf;   /**< Infimum vertex. */
};

struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

struct RwLinkList
{
    RwLLLink link;
};

struct RwRaster
{
    RwRaster* parent; /* Top level raster if a sub raster */
    BYTE* cpPixels; /* Pixel pointer when locked */
    BYTE* palette; /* Raster palette */
    DWORD            width, height, depth; /* Dimensions of raster */
    DWORD             stride; /* Lines bytes of raster */
    WORD             nOffsetX, nOffsetY; /* Sub raster offset */
    BYTE             cType;  /* Type of raster */
    BYTE             cFlags; /* Raster flags */
    BYTE             privateFlags; /* Raster private flags */
    BYTE             cFormat; /* Raster format */

    BYTE* originalPixels;
    DWORD             originalWidth;
    DWORD             originalHeight;
    DWORD             originalStride;

    //Th3 Extension
    BYTE unk[0x10];
    Direct3DBaseTexture8* base;
    DWORD unk1;
    DWORD alpha;
};

struct RwTexDictionary
{
    RwObject            object; /* Homogeneous type */
    RwLinkList          texturesInDict; /* List of textures in dictionary */
    RwLLLink            lInInstance; /* Link list of all dicts in system */
};

struct RwTexture
{
    struct Animation
    {
        float UVel;//0
        float VVel;//4
        float UFrequency;//8//  fsin(UFreq*t+UPhase)*UAmp+t*UVel
        float VFrequency;//12
        float UAmplitude;//16  fsin(amp*360*Uphase*0.02)*UFreq
        float VAmplitude;//20
        float UPhase;//24
        float VPhase;//28

    };

    struct ShaderObject2
    {
        DWORD flags;
        DWORD blend_op;
        DWORD src_blend;
        DWORD dest_blend;
        union
        {
            Animation* anim;
            float env_tiling[2];
        };
        DWORD alphaRef;
    };

    RwRaster*         raster; /** pointer to RwRaster with data */
    RwTexDictionary*  dict;   /* Dictionary this texture is in */
    RwLLLink          lInDictionary; /* List of textures in this dictionary */

    char              tex_name[128];  /* Name of the texture */
    char              mask_name[128];  /* Name of the textures mask */

    /* 31 [xxxxxxxx xxxxxxxx vvvvuuuu ffffffff] 0 */
    DWORD             filterAddressing; /* Filtering & addressing mode flags */

    DWORD             refCount; /* Reference count, surprisingly enough */

    //Useless Th3 Extension
    DWORD uAddress;//why is this 8 bytes when in original RW Engine it's 4???
    DWORD vAddress;
    //LevelMod Extension
    ShaderObject2 shader;
};
//typedef BOOL(*RxNodeInitFn) (RxNodeDefinition* self);

/**
 * \ingroup rwcoregeneric
 * \ref RxNodeTermFn
 * is the callback to be called,
 * for the owning node definition, the last time an \ref RxPipeline
 * referencing that node definition is destroyed or locked.
 *
 * \param  self   A pointer to the node definition
 *
 * \return None
 *
 * \see RxNodeMethods
 */
//typedef void   (*RxNodeTermFn) (RxNodeDefinition* self);

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeInitFn
 * is the callback to be called, for the owning pipeline node, whenever a
 * \ref RxPipeline containing that that pipeline node is unlocked.
 *
 * \param  self   A pointer to the pipeline node
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RxNodeMethods
 */
typedef BOOL(*RxPipelineNodeInitFn) (RxPipelineNode* self);

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeTermFn
 * is the callback to be called, for the owning pipeline node, whenever a
 * \ref RxPipeline containing that that pipeline node is locked or
 * destroyed.
 *
 * \param  self   A pointer to the pipeline node
 *
 * \return None
 *
 * \see RxNodeMethods
 */
typedef void   (*RxPipelineNodeTermFn) (RxPipelineNode* self);
typedef struct RxPipeline;

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeConfigFn
 * is the callback to be called, for the owning pipeline node, whenever a
 * \ref RxPipeline containing that that pipeline node is unlocked,
 * *after* all \ref RxPipelineNodeInitFn's have been called for the
 * pipeline in question. This func is to be used as described in
 * RxPipelineNodeSendConfigMsg.
 *
 * \param  self       A pointer to the pipeline node
 * \param  pipeline   A pointer to the containing pipeline
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RxNodeMethods
 */
typedef BOOL(*RxPipelineNodeConfigFn) (RxPipelineNode* self,
    RxPipeline* pipeline);

/**
 * \ingroup rwcoregeneric
 * \ref RxConfigMsgHandlerFn
 * is the callback to be called, for the owning pipeline node, whenever
 * a message is sent to it by the \ref RxPipelineNodeConfigFn of another
 * pipeline node in the same pipeline. See \ref RxPipelineNodeSendConfigMsg.
 *
 * \param  self   A pointer to the pipeline node
 * \param  msg   Message ID
 * \param  intparam   Meaning is message-specific
 * \param  ptrparam   Meaning is message-specific
 *
 * \return A RwInt32 value, 0: unserviced; -ve: error; +ve: informative success
 *
 * \see RxNodeMethods
 */
typedef DWORD(*RxConfigMsgHandlerFn) (RxPipelineNode* self,
    DWORD msg,
    DWORD intparam,
    void* ptrparam);

enum RxNodeDefEditable
{
    /**<This node definition should not be freed */
    rxNODEDEFCONST = FALSE,
    /**< This node definition is a temporary,
     * modified copy of another and can be freed */
     rxNODEDEFEDITABLE = TRUE,
     rxNODEDEFEDITABLEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RxNodeDefEditable RxNodeDefEditable;

/**
 * \ingroup rwcoregeneric
 * \struct RxNodeMethods
 * A structure describing a set
 * of node methods
 *
 * \see RxNodeBodyFn
 * \see RxNodeInitFn
 * \see RxNodeTermFn
 * \see RxPipelineNodeInitFn
 * \see RxPipelineNodeTermFn
 * \see RxPipelineNodeConfigFn
 * \see RxConfigMsgHandlerFn
 */
struct RxNodeMethods
{
    RxNodeBodyFn         nodeBody;              /**< Node body function */
    void*                nodeInit;              /**< Node initialization function */
    void*                nodeTerm;              /**< Node termination function */
    RxPipelineNodeInitFn pipelineNodeInit;      /**< Pipeline node initialization function */
    RxPipelineNodeTermFn pipelineNodeTerm;      /**< Pipeline node termination function */
    RxPipelineNodeConfigFn pipelineNodeConfig;  /**< Pipleline node configuation function */
    RxConfigMsgHandlerFn configMsgHandler;      /**< Configuaraton message handler function */
};


struct RxNodeDefinition
{
    char*               name;                   /**< Name */
    RxNodeMethods       nodeMethods;            /**< Node methods */
    RxIoSpec            io;                     /**< Input/output specification */
    DWORD               pipelineNodePrivateDataSize; /**< Size in bytes of node's private data  */
    RxNodeDefEditable   editable;               /**< Flags whether a node is editable */
    DWORD               InputPipesCnt;          /**< Count of the unlocked pipelines containing this node */
};

struct MaterialExtensionPLG
{
    BYTE unk1;
    BYTE unk2;
    BYTE unk4;
    BYTE terrain;
    void* unk5;//animation?
    DWORD color;
    DWORD ambient;
    DWORD specular;
    DWORD diffuse;
    RwTexture* texture;
};

struct RpMaterial
{
    RwTexture* texture; /**< texture */
    DWORD             color; /**< color */
    void* pipeline; /**< pipeline */ //always NULL?
    float ambient;   /**< ambient reflection coefficient */
    float specular;  /**< specular reflection coefficient */
    float diffuse;   /**< reflection coefficient */

    WORD             refCount;          /* C.f. rwsdk/world/bageomet.h:RpGeometry */

    //Th3 Extension
    BYTE flag;//8 = 2sided 0x10 = always visible
    BYTE padding;
    WORD unk1;//
    WORD unk2;//Always same as unk1?
    void* unk3;//Always NULL?
    DWORD unk4;//0x4 0x10, flag?
    DWORD unk5[4];//Always 0?
    MaterialExtensionPLG* extension;
    void* unk7;
    void* unk8;//Always NULL?

};

struct RpMaterialList
{
    RpMaterial** materials;
    DWORD        numMaterials;
    DWORD        space;//Repeating of numMaterials
};

struct SuperSector;

struct RpSector
{
    DWORD                 type;//Always NULL?
    //Th3 Extension
    DWORD unk;//Always NULL?
    SuperSector* first;
    SuperSector* last;
    DWORD padding[2];
};

typedef struct RwResEntry;

typedef void        (*RwResEntryDestroyNotify) (RwResEntry* resEntry);

struct RwResEntry
{
    RwLLLink            link;   /* Node in the list of resource elements */
    DWORD               size;   /* Size of this node */
    void* owner;  /* Owner of this node */
    RwResEntry** ownerRef; /* Pointer to pointer to this (enables de-alloc) */
    RwResEntryDestroyNotify destroyNotify; /* This is called right before destruction */
};


#define RpWorldSector SuperSector

enum RpWorldRenderOrder
{
    rpWORLDRENDERNARENDERORDER = 0,
    rpWORLDRENDERFRONT2BACK,  /**<Renders nearest sectors first */
    rpWORLDRENDERBACK2FRONT, /**<Renders furthest sectors first */
    rpWORLDRENDERORDERFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpWorldRenderOrder RpWorldRenderOrder;

typedef RpWorldSector* (*RpWorldSectorCallBackRender) (RpWorldSector*
    worldSector);
/**
 * \ingroup rpworldsub
 * \struct RpWorld
 * World object. This should be considered an opaque type.
 * Use the RpWorld API functions to access.
 */
typedef struct RpWorld RpWorld;


#if (!defined(DOXYGEN))
struct RpWorld
{
    RwObject            object;

    RpWorldRenderOrder  renderOrder;

    /* Materials */
    RpMaterialList      matList;

    /* The world stored as a BSP tree */
    RpSector* rootSector;

    /* The number of texture coordinate sets in each sector */
    DWORD             numTexCoordSets;

    /* Render frame used when last rendered */
    DWORD             numClumpsInWorld;
    RwLLLink* currentClumpLink;

    /* All the clumps in the world */
    RwLinkList          clumpList;

    /* All of the lights in the world */
    RwLinkList          lightList;

    /* Directional lights in the world */
    RwLinkList          directionalLightList;

    /* The worlds origin offset */
    Vertex               worldOrigin;

    /* Bounding box around the whole world */
    RwBBox              boundingBox;

    /* The callbacks functions */
    RpWorldSectorCallBackRender renderCallBack;

    void* pipeline;

    RpWorld* Instance()
    {
        return *(RpWorld**)0x008E1E78;
    }
};
#endif /* (!defined(DOXYGEN)) */

struct RwSphere
{
    Vertex center;   /**< Sphere center */
    float radius;    /**< Sphere radius */
};

typedef struct RpClump;

typedef RpClump* (*RpClumpCallBack) (RpClump* clump, void* data);

struct RpClump
{
    RwObject            object;

    /* Information about all the Atomics */
    RwLinkList          atomicList;

    /* Lists of lights and cameras */
    RwLinkList          lightList;
    RwLinkList          cameraList;

    /* The clump in a world */
    RwLLLink            inWorldLink;

    /* Clump frustum callback */
    RpClumpCallBack     callback;
    
    //Th3 extension
    DWORD unk[2];
    RpWorld* world;
};

typedef struct RwObjectHasFrame RwObjectHasFrame;
typedef RwObjectHasFrame* (*RwObjectHasFrameSyncFunction)(RwObjectHasFrame* object);

struct RwObjectHasFrame
{
    RwObject                     object;
    RwLLLink                     lFrame;
    RwObjectHasFrameSyncFunction sync;
};

typedef struct RpAtomic;

typedef RpAtomic* (*RpAtomicCallBackRender) (RpAtomic* atomic);

struct RpAtomic
{
    RwObjectHasFrame    object;

    /* Information for an instance */
    RwResEntry* repEntry;

    /* Triangles making the object */
    RwGeometry* geometry;

    /* Interpolated bounding sphere (in object space and world space) */
    RwSphere            boundingSphere;
    RwSphere            worldBoundingSphere;

    /* Connections to other atomics */
    RpClump* clump;
    RwLLLink            inClumpLink;

    /* callbacks */
    RpAtomicCallBackRender renderCallBack;

    DWORD unk;//Always 1?
    DWORD unk2[2];//Always 0?
    float unk3[2];//Always 1.0?
    DWORD unk4;//Always 0?

    /* Counter for checks of "render has occurred already" */
    WORD            renderFrame;
    WORD            pad;

    /* Connections to sectors */
    RwLinkList          llWorldSectorsInAtomic;//Not sure about this...

    /* The Atomic object pipeline for this Atomic */
    void* pipeline;//Always NULL, ie default pipeline
};

struct RpAtomicContainer
{
    RpAtomic* atomic;
    DWORD checksum;
};

class BaseClass
{
public:
    void* operator new(size_t size)
    {
        typedef void*(__cdecl* const pMalloc)(size_t size);
        void* data = pMalloc(*(DWORD*)0x0058D204)(size);//pNew(0x00410460)(size);
        ZeroMemory(data, size);
    }

    void operator delete(void* ptr)
    {
        typedef void* (__cdecl* const pFree)();
        pFree(*(DWORD*)0x0058D1F4)();
    }
};

class SkaterCam : BaseClass
{
    BYTE unk[0x48];
    //Skater to follow
    Skater* skater;
    BYTE unk2[0x12C];
public:

    void SetSkater(Skater* follow)
    {
        skater = follow;
    }

    SkaterCam(int unk, RpWorld* world)
    {
        typedef SkaterCam* (__thiscall* const pSkaterCam)(SkaterCam* pThis, int unk, RpWorld* world);
        pSkaterCam(0x004B7FF0)(this, unk, world);
    }

    void AttachCamera(Gfx::Camera* camera)
    {
        typedef void(__thiscall* const pAttachCamera)(SkaterCam* pThis, Gfx::Camera*);
        pAttachCamera(0x004B8200)(this, camera);
    }

    void SetMode(DWORD mode, float unk)
    {
        typedef void(__thiscall* const pSetMode)(SkaterCam* pThis, DWORD mode, float unk);
        pSetMode(0x004B82B0)(this, mode, unk);
    }
};

class RwViewer
{
    BYTE unk[0x6C];
    Gfx::Camera* camera[8];//Not sure how many`?
    BYTE unk2[0x18];
    RpWorld* world;

public:
    Gfx::Camera* GetCamera(int index)
    {
        return camera[index];
    }

    RpWorld* GetCurrentWorld()
    {
        return world;
    }

    static RwViewer* Instance()
    {
        return *(RwViewer**)0x008E1E78;
    }
};
