#include "SceneManager.h"
#include "SceneBase.h"

/// <summary>
/// ƒV[ƒ“‚ÌØ‚è‘Ö‚¦
/// </summary>
/// <param name="scene"></param>
void SceneManager::changeScene(SceneBase* scene)
{
	while (!scenes_.empty())
	{
		delete scenes_.front();
		scenes_.pop_front();
	}
	scenes_.push_front(scene);
}

/// <summary>
/// 
/// </summary>
/// <param name="scene"></param>
void SceneManager::pushScene(SceneBase* scene)
{
	scenes_.push_front(scene);
}

void SceneManager::popScene()
{
	if (scenes_.size() > 1) {
		delete scenes_.front();
		scenes_.pop_front();
	}
}

void SceneManager::update(const InputState& input)
{
	scenes_.front()->update(input);
}

void SceneManager::draw()
{
	for (int i = scenes_.size() - 1; i >= 0; --i) {
		scenes_[i]->draw();
	}
}