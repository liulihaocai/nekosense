#include "clantagchanger.h"

#include <sstream>
#include "../settings.h"
#include "../interfaces.h"
float lastTime = 1.0f;

static bool ClanTagChanged = false;

static std::vector<std::wstring> splitWords(std::wstring text)
{
	std::wistringstream stream(text);
	std::wstring word;
	std::vector<std::wstring> words;
	while (stream >> word)
	{
		words.push_back(word);
	}

	return words;
}

static ClanTagChanger::Animation Letters(std::string name, std::wstring text)
{
	// Outputs a letter incrementing animation
	std::vector<ClanTagChanger::Frame> frames;
	for (unsigned long i = 1; i <= text.length(); i++)
	{
		frames.push_back(ClanTagChanger::Frame(text.substr(0, i), Settings::ClanTagChanger::animationSpeed));
	}

	for (unsigned long i = text.length() - 2; i > 0; i--)
	{
		frames.push_back(ClanTagChanger::Frame(frames[i].text, Settings::ClanTagChanger::animationSpeed));
	}

	return ClanTagChanger::Animation(name, frames, ClanTagChanger::ANIM_LOOP);
}

static ClanTagChanger::Animation Marquee(std::string name, std::wstring text, int width = 15)
{
	text.erase(std::remove(text.begin(), text.end(), '\0'), text.end());

	std::wstring cropString = std::wstring(width, ' ') + text + std::wstring(width - 1, ' ');

	std::vector<ClanTagChanger::Frame> frames;
	for (unsigned long i = 0; i < text.length() + width; i++)
	{
		frames.push_back(
			ClanTagChanger::Frame(cropString.substr(i, width + i), Settings::ClanTagChanger::animationSpeed));
	}

	return ClanTagChanger::Animation(name, frames, ClanTagChanger::ANIM_LOOP);
}

static ClanTagChanger::Animation Words(std::string name, std::wstring text)
{
	// Outputs a word by word animation
	std::vector<std::wstring> words = splitWords(text);
	std::vector<ClanTagChanger::Frame> frames;
	for (unsigned long i = 0; i < words.size(); i++)
	{
		frames.push_back(ClanTagChanger::Frame(words[i], Settings::ClanTagChanger::animationSpeed));
	}

	return ClanTagChanger::Animation(name, frames, ClanTagChanger::ANIM_LOOP);
}

static ClanTagChanger::Animation Rotate(std::string name, std::wstring text)
{
	std::vector<ClanTagChanger::Frame> frames;
	text.push_back(' ');
	for (unsigned long i = 0; i < text.length(); i++)
	{
		std::rotate(text.begin(), text.begin() + 1, text.end());
		frames.push_back(ClanTagChanger::Frame(text, Settings::ClanTagChanger::animationSpeed));
	}

	return ClanTagChanger::Animation(name, frames, ClanTagChanger::ANIM_LOOP);
}

static ClanTagChanger::Animation Underline(std::string name, std::wstring text)
{
	std::vector<ClanTagChanger::Frame> frames;

	frames.push_back(ClanTagChanger::Frame(text, Settings::ClanTagChanger::animationSpeed));
	for (unsigned long i = 0; i < text.length(); i++)
	{
		// copy the string
		std::wstring line = text;
		line[i] = line[i] == ' ' ? ' ' : '_';
		frames.push_back(ClanTagChanger::Frame(line, Settings::ClanTagChanger::animationSpeed));
	}

	return ClanTagChanger::Animation(name, frames, ClanTagChanger::ANIM_LOOP);
}

std::vector<ClanTagChanger::Animation> ClanTagChanger::animations = {
	Marquee("--", L"--"),
	Words("--", L"--"),
	Letters("--", L"--"),
	Rotate("--", L"--"),
	Underline("--", L"--"),
};
ClanTagChanger::Animation *ClanTagChanger::animation = &ClanTagChanger::animations[0];

void ClanTagChanger::UpdateClanTagCallback()
{
	if (strlen(Settings::ClanTagChanger::value) > 0 && Settings::ClanTagChanger::type > ClanTagType::STATIC)
	{
		std::wstring wc = Util::StringToWstring(Settings::ClanTagChanger::value);

		switch (Settings::ClanTagChanger::type)
		{
		case ClanTagType::MARQUEE:
			*ClanTagChanger::animation = Marquee("------", wc);
			break;
		case ClanTagType::WORDS:
			*ClanTagChanger::animation = Words("------", wc);
			break;
		case ClanTagType::LETTERS:
			*ClanTagChanger::animation = Letters("------", wc);
			break;
		case ClanTagType::ROTATE:
			*ClanTagChanger::animation = Rotate("------", wc);
			break;
		case ClanTagType::UNDERLINE:
			*ClanTagChanger::animation = Underline("------", wc);
			break;
		default:
			break;
		}

		return;
	}

	ClanTagChanger::animations = {
		Marquee("--", L"--"),
		Words("--", L"--"),
		Letters("--", L"--"),
		Rotate("--", L"--"),
		Underline("--", L"--"),
	};

	int current_animation = (int)Settings::ClanTagChanger::type - 1;
	if (current_animation >= 0)
	{
		ClanTagChanger::animation = &ClanTagChanger::animations[current_animation];
	}
}

void ClanTagChanger::BeginFrame(float frameTime)
{
	if (!Settings::ClanTagChanger::enabled)
	{
		return;
	}

	if (!engine->IsInGame())
	{
		ClanTagChanged = false;
		return;
	}

	if (Settings::ClanTagChanger::type == ClanTagType::STATIC)
	{

		std::string ctWithEscapesProcessed = std::string(Settings::ClanTagChanger::value);
		// Util::StdReplaceStr(ctWithEscapesProcessed, "\\n", "\n"); // compute time impact? also, referential so i assume RAII builtin cleans it up...
		if (!ctWithEscapesProcessed.empty() && !ClanTagChanged)
		{
			SendClanTag(ctWithEscapesProcessed.c_str(), ctWithEscapesProcessed.c_str());
			ClanTagChanged = true;
		}
	}
	else
	{
		long currentTime_ms = Util::GetEpochTime();
		static long timeStamp = currentTime_ms;

		if (currentTime_ms - timeStamp > ClanTagChanger::animation->GetCurrentFrame().time)
		{
			timeStamp = currentTime_ms;
			ClanTagChanger::animation->NextFrame();
			if (!ClanTagChanger::animation->GetCurrentFrame().text.empty())
			{
				SendClanTag(Util::WstringToString(ClanTagChanger::animation->GetCurrentFrame().text).c_str(),
							Util::WstringToString(ClanTagChanger::animation->GetCurrentFrame().text).c_str());
			}
		}

		// SendClanTag((char*)ClanTagChanger::animation->GetCurrentFrame().text.c_str(), (char*)ClanTagChanger::animation->GetCurrentFrame().text.c_str());
	}
}
