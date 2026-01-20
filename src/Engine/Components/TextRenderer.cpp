#include "pch.h"
#include "TextRenderer.h"
#include <Font.h>
#include <Structs.h>
#include <FrameResources.h>

namespace gce {
	Font* TextRenderer::defaultFont = nullptr;

	Font* TextRenderer::GetDefaultFont()
	{
		if (!defaultFont)
		{
			defaultFont = new Font(L"Arial");
		}
		return defaultFont;
	}

	void TextRenderer::Init()
	{
		pFont = GetDefaultFont();
		pBrush = new ColorBrush(Color::White);
		text = L"Hello, World!";
		rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
	}

	void TextRenderer::SetFont(Font* font)
	{
		pFont = font;
	}

} // namespace gce