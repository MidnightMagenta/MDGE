#include "../include/windows/win32_cursor.hpp"

HCURSOR mdge::GetDefaultCursor(Cursor cursor) {
	switch (cursor) {
		case mdge::Cursor::ARROW:
			return LoadCursorA(0, IDC_ARROW);
			break;
		case mdge::Cursor::TEXT_SELECT:
			return LoadCursorA(0, IDC_IBEAM);
			break;
		case mdge::Cursor::WAIT:
			return LoadCursorA(0, IDC_WAIT);
			break;
		case mdge::Cursor::CROSS:
			return LoadCursorA(0, IDC_CROSS);
			break;
		case mdge::Cursor::UP:
			return LoadCursorA(0, IDC_UPARROW);
			break;
		case mdge::Cursor::DIAGONAL:
			return LoadCursorA(0, IDC_SIZENWSE);
			break;
		case mdge::Cursor::COUNTER_DIAGONAL:
			return LoadCursorA(0, IDC_SIZENESW);
			break;
		case mdge::Cursor::HORIZONTAL:
			return LoadCursorA(0, IDC_SIZEWE);
			break;
		case mdge::Cursor::VERTICAL:
			return LoadCursorA(0, IDC_SIZENS);
			break;
		case mdge::Cursor::ARROW_CROSS:
			return LoadCursorA(0, IDC_SIZEALL);
			break;
		case mdge::Cursor::NO:
			return LoadCursorA(0, IDC_NO);
			break;
		case mdge::Cursor::HAND:
			return LoadCursorA(0, IDC_HAND);
			break;
		case mdge::Cursor::ARROW_WAIT:
			return LoadCursorA(0, IDC_APPSTARTING);
			break;
		case mdge::Cursor::HAND_PIN:
			return LoadCursorA(0, IDC_PIN);
			break;
		case mdge::Cursor::HAND_PERSON:
			return LoadCursorA(0, IDC_PERSON);
			break;
		case mdge::Cursor::PEN:
			return LoadCursorA(0, MAKEINTRESOURCE(32631));
			break;
		case mdge::Cursor::THICK_VERTICAL:
			return LoadCursorA(0, MAKEINTRESOURCE(32652));
			break;
		case mdge::Cursor::THICK_HORIZONTAL:
			return LoadCursorA(0, MAKEINTRESOURCE(32653));
			break;
		case mdge::Cursor::THICK_ARROW_CROSS:
			return LoadCursorA(0, MAKEINTRESOURCE(32654));
			break;
		case mdge::Cursor::THICK_UP:
			return LoadCursorA(0, MAKEINTRESOURCE(32655));
			break;
		case mdge::Cursor::THICK_DOWN:
			return LoadCursorA(0, MAKEINTRESOURCE(32656));
			break;
		case mdge::Cursor::THICK_LEFT:
			return LoadCursorA(0, MAKEINTRESOURCE(32657));
			break;
		case mdge::Cursor::THICK_RIGHT:
			return LoadCursorA(0, MAKEINTRESOURCE(32658));
			break;
		case mdge::Cursor::THICK_UP_LEFT:
			return LoadCursorA(0, MAKEINTRESOURCE(32659));
			break;
		case mdge::Cursor::THICK_UP_RIGHT:
			return LoadCursorA(0, MAKEINTRESOURCE(32660));
			break;
		case mdge::Cursor::THICK_DOWN_LEFT:
			return LoadCursorA(0, MAKEINTRESOURCE(32661));
			break;
		case mdge::Cursor::THICK_DOWN_RIGHT:
			return LoadCursorA(0, MAKEINTRESOURCE(32662));
			break;
		case mdge::Cursor::ARROW_CD:
			return LoadCursorA(0, MAKEINTRESOURCE(32663));
			break;
		default:
			break;
	}
	return nullptr;
}
