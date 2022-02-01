#ifndef OBJ_JUNCTIONWHEEL_H
#define OBJ_JUNCTIONWHEEL_H

#include "SonicMania.h"

// Object Class
struct ObjectJunctionWheel {
    RSDK_OBJECT
    Hitbox hitboxEnteredWheel;
    Hitbox hitboxSolid2;
    Hitbox hitboxSolid;
    Hitbox hitboxSolid3;
    Hitbox hitboxEntry1;
    Hitbox hitboxEntry5;
    uint16 aniFrames;
};

// Entity Class
struct EntityJunctionWheel {
    RSDK_ENTITY
    Animator slotAnimator;
    Animator maskAnimator;
    uint8 spinDir;
};

// Object Struct
extern ObjectJunctionWheel *JunctionWheel;

// Standard Entity Events
void JunctionWheel_Update(void);
void JunctionWheel_LateUpdate(void);
void JunctionWheel_StaticUpdate(void);
void JunctionWheel_Draw(void);
void JunctionWheel_Create(void* data);
void JunctionWheel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void JunctionWheel_EditorDraw(void);
void JunctionWheel_EditorLoad(void);
#endif
void JunctionWheel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JUNCTIONWHEEL_H
