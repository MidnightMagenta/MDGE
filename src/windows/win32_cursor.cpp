#include "../include/windows/win32_cursor.hpp"

HCURSOR mdge::GetDefaultCursor(Cursor cursor) {
	LPCSTR cursorName;
	switch (cursor) {
		case mdge::Cursor::ARROW:
			cursorName = IDC_ARROW;
			break;
		case mdge::Cursor::TEXT_SELECT:
			cursorName = IDC_IBEAM;
			break;
		case mdge::Cursor::WAIT:
			cursorName = IDC_WAIT;
			break;
		case mdge::Cursor::CROSS:
			cursorName = IDC_CROSS;
			break;
		case mdge::Cursor::UP:
			cursorName = IDC_UPARROW;
			break;
		case mdge::Cursor::DIAGONAL:
			cursorName = IDC_SIZENWSE;
			break;
		case mdge::Cursor::COUNTER_DIAGONAL:
			cursorName = IDC_SIZENESW;
			break;
		case mdge::Cursor::HORIZONTAL:
			cursorName = IDC_SIZEWE;
			break;
		case mdge::Cursor::VERTICAL:
			cursorName = IDC_SIZENS;
			break;
		case mdge::Cursor::ARROW_CROSS:
			cursorName = IDC_SIZEALL;
			break;
		case mdge::Cursor::NO:
			cursorName = IDC_NO;
			break;
		case mdge::Cursor::HAND:
			cursorName = IDC_HAND;
			break;
		case mdge::Cursor::ARROW_WAIT:
			cursorName = IDC_APPSTARTING;
			break;
		case mdge::Cursor::HAND_PIN:
			cursorName = IDC_PIN;
			break;
		case mdge::Cursor::HAND_PERSON:
			cursorName = IDC_PERSON;
			break;
		case mdge::Cursor::PEN:
			cursorName = MAKEINTRESOURCE(32631);
			break;
		case mdge::Cursor::THICK_VERTICAL:
			cursorName = MAKEINTRESOURCE(32652);
			break;
		case mdge::Cursor::THICK_HORIZONTAL:
			cursorName = MAKEINTRESOURCE(32653);
			break;
		case mdge::Cursor::THICK_ARROW_CROSS:
			cursorName = MAKEINTRESOURCE(32654);
			break;
		case mdge::Cursor::THICK_UP:
			cursorName = MAKEINTRESOURCE(32655);
			break;
		case mdge::Cursor::THICK_DOWN:
			cursorName = MAKEINTRESOURCE(32656);
			break;
		case mdge::Cursor::THICK_LEFT:
			cursorName = MAKEINTRESOURCE(32657);
			break;
		case mdge::Cursor::THICK_RIGHT:
			cursorName = MAKEINTRESOURCE(32658);
			break;
		case mdge::Cursor::THICK_UP_LEFT:
			cursorName = MAKEINTRESOURCE(32659);
			break;
		case mdge::Cursor::THICK_UP_RIGHT:
			cursorName = MAKEINTRESOURCE(32660);
			break;
		case mdge::Cursor::THICK_DOWN_LEFT:
			cursorName = MAKEINTRESOURCE(32661);
			break;
		case mdge::Cursor::THICK_DOWN_RIGHT:
			cursorName = MAKEINTRESOURCE(32662);
			break;
		case mdge::Cursor::ARROW_CD:
			cursorName = MAKEINTRESOURCE(32663);
			break;
		default:
			break;
	}
	return LoadCursorA(0, cursorName);
}
