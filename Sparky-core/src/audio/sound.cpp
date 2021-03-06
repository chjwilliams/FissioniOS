#include "sound.h"
#include "sound_manager.h"

namespace sparky { namespace audio {
    
    void destroy_on_finish(ga_Handle* in_handle, void* in_context);
    void loop_on_finish(ga_Handle* in_handle, void* in_context);

	Sound::Sound(const std::string& name, const std::string& filename)
		: m_Name(name), m_Filename(filename), m_Playing(false), m_Count(0)
	{
		std::vector<std::string> split = split_string(m_Filename, '.');
		if (split.size() < 2)
		{
			std::cout << "[Sound] Invalid file name '" << m_Filename << "'!" << std::endl;
			return;
		}
#ifdef SPARKY_EMSCRIPTEN
// 		EM_ASM(
// 		window.SoundManager.m_Sounds[]
// 		);
#else
		m_Sound = gau_load_sound_file(filename.c_str(), split.back().c_str());
		if (m_Sound == nullptr)
			std::cout << "[Sound] Could not load file '" << m_Filename << "'!" << std::endl;
#endif

	}

	Sound::~Sound()
	{
#ifdef SPARKY_EMSCRIPTEN
#else
		ga_sound_release(m_Sound);
#endif
	}

	void Sound::play()
	{
#ifdef SPARKY_EMSCRIPTEN
		SoundManagerPlay(m_Name.c_str());
#else
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &destroy_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
		m_Count++;
#endif
		m_Playing = true;
	}

	void Sound::loop()
	{
#ifdef SPARKY_EMSCRIPTEN
		SoundManagerLoop(m_Name.c_str());
#else
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &loop_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
#endif
		m_Playing = true;
	}

	void Sound::resume()
	{
		if (m_Playing)
			return;

		m_Playing = true;
#ifdef SPARKY_EMSCRIPTEN
		SoundManagerPlay(m_Name.c_str());
#else
		ga_handle_play(m_Handle);
#endif
	}

	void Sound::pause()
	{
		if (!m_Playing)
			return;

		m_Playing = false;
#ifdef SPARKY_EMSCRIPTEN
		SoundManagerPause(m_Name.c_str());
#else
		ga_handle_stop(m_Handle);
#endif
	}

	void Sound::stop()
	{
		if (!m_Playing)
			return;

#ifdef SPARKY_EMSCRIPTEN
		SoundManagerStop(m_Name.c_str());
#else
		ga_handle_stop(m_Handle);
#endif
		m_Playing = false;
	}

	void Sound::setGain(float gain)
	{
		if (!m_Playing)
		{
			std::cout << "[Sound] Cannot set gain! Sound is not currently playing!" << std::endl;
			return;
		}
		m_Gain = gain;
#ifdef SPARKY_EMSCRIPTEN
		SoundManagerSetGain(m_Name.c_str(), gain);
#else
		ga_handle_setParamf(m_Handle, GA_HANDLE_PARAM_GAIN, gain);
#endif
	}

#ifdef SPARKY_EMSCRIPTEN
#else
	void destroy_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*)in_handle->sound;
		sound->m_Count--;
		if (sound->m_Count == 0)
			sound->stop();
	}

	void loop_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*) in_handle->sound;
		sound->loop();
		ga_handle_destroy(in_handle);
	}
#endif

} }