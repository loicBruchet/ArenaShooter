#pragma once

#include <Core/define.h>

/// <summary>
/// WARNING !! ONLY 32 TAGS !!
/// <para> (0 - 31) </para>
/// </summary>
#define TAG_ENUM_BEGIN(name) enum name : uint32 {
#define TAG(name, index) name = (1u << index)
#define TAG_ENUM_END };

TAG_ENUM_BEGIN(GameTags)
	TAG(TAG_DEFAULT		, 0),
	TAG(TAG_ENTITY		, 1),
	TAG(TAG_PLAYER		, 2),
	TAG(TAG_ENEMY		, 3),
	TAG(TAG_OTHER		, 4),
	TAG(TAG_PROJECTILE	, 5),
TAG_ENUM_END