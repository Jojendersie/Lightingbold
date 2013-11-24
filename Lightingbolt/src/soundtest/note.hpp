#pragma once

namespace Soundtest
{
	class Note
	{
	public:
		Note(int note=0, float value=1, bool played=true);

		int m_note;
		float m_value;
		bool m_played;
	};
}