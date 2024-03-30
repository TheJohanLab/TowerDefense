#include "UI.h"
#include "../Controller/TextureLoader.h"
#include <algorithm>
#include <string>


UI* UI::instance = nullptr;




UI* UI::getInstance()
{
	if (!instance) {
		instance = new UI();
	}
	return instance;
}

UI::~UI()
{
}





void UI::draw(SDL_Renderer* renderer) const
{
	drawBackground(renderer);
	drawGems(renderer);
	drawHearts(renderer);
	drawItems(renderer);
}


void UI::initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight, int UIWidth, int UIHeight, 
	const uint8_t* playerLifePoints, const Shop* shop)
{
	m_Health = playerLifePoints;
	m_Shop = shop;

	m_uiFont = TTF_OpenFont("C:/_Projets/Cpp/TowerDefenseTutorial/TowerDefenseTutorial/Dependencies/Fonts/sunny_spells_basic/Sunny Spells Basic.ttf", 24);
	if (m_uiFont == nullptr) {
		SDL_Log("Erreur lors du chargement de la police de caractères : %s", TTF_GetError());
		TTF_Quit();
		return;
	}


	m_renderer = renderer;
	m_WindowWidh = windowsWidth;
	m_WindowHeight = windowsHeight;
	m_UIWidth = UIWidth;
	m_UIHeight = UIHeight;

	m_SelectedItem = itemEnum::WallItem;

	if (renderer != nullptr)
	{
		m_BackgroundTexture = TextureLoader::loadTexture(m_renderer, "UI.bmp");
		m_WallTexture = TextureLoader::loadTexture(m_renderer, "WallUI.bmp");
		m_TurretTexture = TextureLoader::loadTexture(m_renderer, "TurretUI.bmp");
		m_EmptyHeartTexture = TextureLoader::loadTexture(m_renderer, "EmptyHeart.bmp");
		m_HalfHeartTexture = TextureLoader::loadTexture(m_renderer, "HalfHeart.bmp");
		m_FullHeartTexture = TextureLoader::loadTexture(m_renderer, "FullHeart.bmp");
		m_GemTexture = TextureLoader::loadTexture(m_renderer, "Gem.bmp");
		m_EmptyGemTexture = TextureLoader::loadTexture(m_renderer, "EmptyGem.bmp");

		m_UIItems.push_back({ m_WallTexture, m_Shop->getItemPrice(itemEnum::WallItem) });
		m_UIItems.push_back({ m_TurretTexture, m_Shop->getItemPrice(itemEnum::TurretItem) });
		m_UIItems.push_back({ m_TurretTexture, m_Shop->getItemPrice(itemEnum::ExplosionItem) });
	}

}

void UI::drawBackground(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{

		int w, h;
		SDL_QueryTexture(m_BackgroundTexture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			0 ,
			m_WindowHeight - m_UIHeight,
			w,
			h
		};

		SDL_RenderCopy(renderer, m_BackgroundTexture, NULL, &rect);

		//Draw hearts containers
		SDL_QueryTexture(m_EmptyHeartTexture, NULL, NULL, &w, &h);
		for (int i = 0; i < 5; i++)
		{
			SDL_Rect rect =
			{
				(int)(m_WindowWidh / 2 + w + (i * 80)),
				(int)((m_WindowHeight - m_UIHeight) + h / 2),
				w,
				h
			};

			SDL_RenderCopy(renderer, m_EmptyHeartTexture, NULL, &rect);
		}

		SDL_QueryTexture(m_EmptyGemTexture, NULL, NULL, &w, &h);
		//Draw gem containers
		for (int i = 0; i < 10; i++)
		{
			SDL_Rect rect =
			{
				(int)(m_WindowWidh / 2 + w * 1.1 + (i * 42)),
				(int)((m_WindowHeight - m_UIHeight) + h * 1.7),
				w,
				h
			};

			SDL_RenderCopy(renderer, m_EmptyGemTexture, NULL, &rect);
		}
	}
}

void UI::drawItems(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{

		int height = (int)(m_WindowHeight - (m_UIHeight / 2));
		int width = (int)(((m_WindowWidh / 2) / 3));
		int i = 0;
		for (const auto& item : m_UIItems)
		{
			int w, h;
			SDL_QueryTexture(item.texture, NULL, NULL, &w, &h);
			SDL_Rect rect =
			{
				(int)width - (w * 3) + i * 150,
				(int)height - h ,
				w * 2,
				h * 2
			};
			SDL_RenderCopy(renderer, item.texture, NULL, &rect);

			drawItemPrice(renderer, item, rect);
			i++;
		}

	}
}

void UI::drawItemPrice(SDL_Renderer* renderer, UIItem item, SDL_Rect itemPos) const
{
	int w, h;
	SDL_QueryTexture(m_GemTexture, NULL, NULL, &w, &h);

	for (int i = 0; i < item.price; i++)
	{

		int w, h;
		SDL_QueryTexture(item.texture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			itemPos.x - itemPos.w/2 + ((i+1.0f)/(item.price+1.0f))*(itemPos.w*1.75),
			itemPos.y - (w * 1 / 3),
			w/2 ,
			h/2
		};
		SDL_RenderCopy(renderer, m_GemTexture, NULL, &rect);

	}
}

void UI::drawHearts(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{
		int w, h;
		SDL_QueryTexture(m_EmptyHeartTexture, NULL, NULL, &w, &h);
		for (int i = 0; i < *m_Health; i++)
		{

			SDL_Rect rect =
			{
				(int)(m_WindowWidh / 2 + w) + (int)(i / 2) * 80,
				(int)((m_WindowHeight - m_UIHeight) + h / 2),
				w,
				h
			};
			SDL_RenderCopy(renderer, i % 2 == 0 ? m_HalfHeartTexture : m_FullHeartTexture, NULL, &rect);
		}

	}
}

void UI::drawGems(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{
		int w, h;
		SDL_QueryTexture(m_GemTexture, NULL, NULL, &w, &h);

		for (int i = 0; i < m_Shop->getMoneyAmount(); i++)
		{
			SDL_Rect rect =
			{
				(int)(m_WindowWidh / 2 + w * 1.1 + (i * 42)),
				(int)((m_WindowHeight - m_UIHeight) + h * 1.7),
				w,
				h
			};

			SDL_RenderCopy(renderer, m_GemTexture, NULL, &rect);
		}
	}

}


void UI::selectItem(itemEnum selectedItem, int x, int y)
{
	m_SelectedItem = selectedItem;
}



void UI::setItemSelectionZone(const itemEnum& itemEnum, const ItemSelectionZone& zone)
{
	m_mapItemSelectionZone[itemEnum] = zone;
}

const ItemSelectionZone& UI::getItemSelectionZone(itemEnum itemKey)
{
	if (m_mapItemSelectionZone.count(itemKey))
		return m_mapItemSelectionZone.at(itemKey);
}

itemEnum* UI::getSelectedItem()
{
	return &m_SelectedItem;
}
