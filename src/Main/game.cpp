#if 0
#include "../src/sparky.h"

using namespace sparky;
using namespace graphics;

class Game : public Sparky
{
private:
	Window* window;
	Layer* layer;
	Label* fps;
	Sprite* sprite;
	Shader* shader;
public:
	Game()
	{

	}

	~Game()
	{
		delete layer;
	}

	void init() override
	{
		using namespace audio;

		window = createWindow("Test Game", 960, 540);
		FontManager::get()->setScale(window->getWidth() / 960.0f, window->getHeight() / 540.0f);
#ifdef SPARKY_EMSCRIPTEN
		shader = new Shader("res/shaders/basic.es3.vert", "res/shaders/basic.es3.frag");
#else
		shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
#endif
		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(0, 960, 0, 540, -1.0f, 1.0f));
#ifdef SPARKY_EMSCRIPTEN
		sprite = new Sprite(0.0f, 0.0f, 4, 4, new Texture("res/tb.png"));
#else
		sprite = new Sprite(50.0f, 50.0f, 64, 64, new Texture("tb.png"));
#endif
		layer->add(sprite);

		fps = new Label("", 50, 500, 0xffffffff);
		layer->add(fps);

		layer->setLineThickness(0.05f);

		SoundManager::add(new Sound("Cherno", "Cherno.ogg"));
	}

	void tick() override
	{
		fps->text = std::to_string(getFPS()) + " fps";
		std::cout << getUPS() << " ups, " << getFPS() << " fps" << std::endl;
	}

	void update() override
	{
		float speed = 10;
		if (window->isKeyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->isKeyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;
		if (window->isKeyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;
		else if (window->isKeyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;

		if (window->isKeyPressed(GLFW_KEY_SPACE))
			audio::SoundManager::get("Cherno")->play();

		std::cout << sprite->getPosition() << std::endl;

		double x, y;
		window->getMousePosition(x, y);
		//shader->setUniform2f("light_pos", maths::vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
	}

	void render() override
	{
		//layer->drawLine(maths::vec3(-2, -4, 0), maths::vec3(-1.5f, -2.5f, 0), 0xffff3020);
		layer->render();
	}
};
#if 0
int main()
{
	Game game;
	game.start();
	return 0;
}
#endif
#endif