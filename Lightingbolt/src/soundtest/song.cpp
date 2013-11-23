#include "song.hpp"

namespace Soundtest
{
	DynArray<Note> Song::twosonHigh()
	{
		DynArray<Note> notes;
		for(int i=0; i<2; i++)
		{
			notes.append(Note(11, 4));

			notes.append(Note(12, 2));
			notes.append(Note(9, 2));
		}

		notes.append(Note(11, 4));

		notes.append(Note(12, 4));

		notes.append(Note(12, 4));

		notes.append(Note(9, 2));
		notes.append(Note(12, 2));

		notes.append(Note(14));
		notes.append(Note(19));
		notes.append(Note(18));
		notes.append(Note(19, 1.0f/2.0f));
		notes.append(Note(14, 1.0f/2.0f));

		notes.append(Note(14, 3));
		notes.append(Note(12));

		notes.append(Note(11));
		notes.append(Note(16));
		notes.append(Note(14));
		notes.append(Note(12, 1.0f/2.0f));
		notes.append(Note(11, 1.0f/2.0f));

		notes.append(Note(11, 3));
		notes.append(Note(9));

		notes.append(Note(7));
		notes.append(Note(11));
		notes.append(Note(9, 1.0f/2.0f));
		notes.append(Note(7, 1.0f/2.0f));
		notes.append(Note(9));

		notes.append(Note(7));
		notes.append(Note(11));
		notes.append(Note(9));
		notes.append(Note(14));

		notes.append(Note(7));
		notes.append(Note(11));
		notes.append(Note(9, 1.0f/2.0f));
		notes.append(Note(7, 1.0f/2.0f));
		notes.append(Note(9));

		notes.append(Note(7));
		notes.append(Note(9, 1.0f/2.0f));
		notes.append(Note(11, 1.0f/2.0f));
		notes.append(Note(9, 2));

		notes.append(Note(14));
		notes.append(Note(19));
		notes.append(Note(18));
		notes.append(Note(19, 1.0f/2.0f));
		notes.append(Note(14, 1.0f/2.0f));

		notes.append(Note(14, 3));
		notes.append(Note(12));

		notes.append(Note(11));
		notes.append(Note(16));
		notes.append(Note(14));
		notes.append(Note(12, 1.0f/2.0f));
		notes.append(Note(11, 1.0f/2.0f));

		notes.append(Note(11, 3));
		notes.append(Note(9));

		notes.append(Note(7, 2));
		notes.append(Note(9, 2));

		notes.append(Note(11));
		notes.append(Note(7));
		notes.append(Note(9, 2));

		notes.append(Note(11));
		notes.append(Note(14));
		notes.append(Note(12, 1.0f/2.0f));
		notes.append(Note(11, 1.0f/2.0f));
		notes.append(Note(9));
		return notes;
	}

	DynArray<Note> Song::twosonLow()
	{
		DynArray<Soundtest::Note> notes;
		for(int i=0; i<4; i++)
		{
			notes.append(Soundtest::Note(0, 4, false));
		}

		notes.append(Soundtest::Note(-13));
		notes.append(Soundtest::Note(-10));
		notes.append(Soundtest::Note(-3));
		notes.append(Soundtest::Note(-1, 1.0f/2.0f));
		notes.append(Soundtest::Note(-5, 1.0f/2.0f));

		notes.append(Soundtest::Note(-5, 2.0f));
		notes.append(Soundtest::Note(-1));
		notes.append(Soundtest::Note(0, 1.0f/2.0f));
		notes.append(Soundtest::Note(2, 1.0f/2.0f));

		notes.append(Soundtest::Note(2, 2.0f));
		notes.append(Soundtest::Note(-1));
		notes.append(Soundtest::Note(0, 1.0f/2.0f));
		notes.append(Soundtest::Note(2, 1.0f/2.0f));

		notes.append(Soundtest::Note(2, 2.0f/3.0f));
		notes.append(Soundtest::Note(-3, 2.0f/3.0f));
		notes.append(Soundtest::Note(-8, 2.0f/3.0f));
		notes.append(Soundtest::Note(-10, 2));

		notes.append(Soundtest::Note(-5, 4));

		notes.append(Soundtest::Note(-6, 4));

		notes.append(Soundtest::Note(-8, 4));

		notes.append(Soundtest::Note(-9, 4));

		for(int i=0; i<2; i++)
		{
			notes.append(Soundtest::Note(-5, 2));
			notes.append(Soundtest::Note(-3, 2));

			notes.append(Soundtest::Note(-1));
			notes.append(Soundtest::Note(-5));
			notes.append(Soundtest::Note(-3, 2));
		}

		notes.append(Soundtest::Note(-5, 4));

		notes.append(Soundtest::Note(-6, 4));

		notes.append(Soundtest::Note(-8, 4));

		notes.append(Soundtest::Note(-9, 4));

		notes.append(Soundtest::Note(-5, 2));
		notes.append(Soundtest::Note(-3, 2));

		notes.append(Soundtest::Note(-1));
		notes.append(Soundtest::Note(-5));
		notes.append(Soundtest::Note(-3, 2));

		notes.append(Soundtest::Note(-1, 2));
		notes.append(Soundtest::Note());
		notes.append(Soundtest::Note(2));
		return notes;
	}
}