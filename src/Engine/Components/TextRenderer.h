#ifndef ENGINE_TEXT_RENDERER_H_INCLUDED
#define ENGINE_TEXT_RENDERER_H_INCLUDED

#include "define.h"
#include "UIBase.h"
#include <Structs.h>
#include <Font.h>

namespace gce {

DECLARE_COMPONENT( TextRenderer, UiBase )
{
	std::wstring text;
	Font* pFont;
	UiBrush* pBrush;
	RectanglePosF* rectPosF;

	gce::Vector2f32 textSize = { 1.0f, 1.0f };

	void SetFont(Font * font);

	static Font* GetDefaultFont();

protected:
	void Init() override;

	static Font* defaultFont;

	friend class UiSystem;
};
}

#endif