
// YepView.cpp : implementation of the CYepView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Yep.h"
#endif

#include "YepDoc.h"
#include "YepView.h"

using namespace icc::MineSweeper;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CYepView, CFormView)

BEGIN_MESSAGE_MAP(CYepView, CFormView)

	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

	ON_WM_DESTROY()
END_MESSAGE_MAP()


CYepView::CYepView()
	: CFormView(IDD_YEP_FORM),
	gameField(nullptr), gameFieldViewOffset{20,20},
	pressedCell(nullptr), isMouseOver(false)
{
	gameField = std::make_shared<GameField>(DEFAULT_FIELD_WIDTH, DEFAULT_FIELD_HEIGHT, DEFAULT_COUNT_MINES);

	const auto mInstance = AfxGetApp()->m_hInstance;
	
	flagedCellBitmap = (HBITMAP)LoadImage(mInstance, MAKEINTRESOURCE(IDB_BITMAP_FLAGED), IMAGE_BITMAP, 0, 0, 0);
	closedCellBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_CLOSED), IMAGE_BITMAP, 0, 0, 0);
	minedCellBitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MINED), IMAGE_BITMAP, 0, 0, 0);

	const LPWSTR arrIds[] = { MAKEINTRESOURCE(IDB_BITMAP_EMPTY), 
				MAKEINTRESOURCE(IDB_BITMAP_1), MAKEINTRESOURCE(IDB_BITMAP_2), MAKEINTRESOURCE(IDB_BITMAP_3),
				MAKEINTRESOURCE(IDB_BITMAP_4), MAKEINTRESOURCE(IDB_BITMAP_5), MAKEINTRESOURCE(IDB_BITMAP_6),
				MAKEINTRESOURCE(IDB_BITMAP_7), MAKEINTRESOURCE(IDB_BITMAP_8)};
	for (auto i = 0; i < sizeof(arrIds) / sizeof(arrIds[0]); ++i)
		countedBitmaps[i] = (HBITMAP)LoadImage(mInstance, arrIds[i], IMAGE_BITMAP, 0, 0, 0);
}

CYepView::~CYepView()
{
	ClearField();
}

void CYepView::OnDraw(CDC* pDC)
{
	HDC hdcMem = CreateCompatibleDC(pDC->m_hDC);
	HGDIOBJ oldBmp = nullptr;

	for (auto& cell : allFieldStates)
	{
		//const RECT destDesc = { workingImageViewOffset.x, workingImageViewOffset.y, workingImageWidth, workingImageHeight };
		switch (cell.state)
		{
		case EFieldState::Opened:
			SelectObject(hdcMem, cell.bitmap);
			break;
		case EFieldState::Clicked:
			SelectObject(hdcMem, emptyCellBitmap);
			break;
		case EFieldState::Closed:
			SelectObject(hdcMem, closedCellBitmap);
			break;
		case EFieldState::Flaged:
			SelectObject(hdcMem, flagedCellBitmap);
			break;
		default:
			break;
		}

		BitBlt(pDC->m_hDC, cell.guiCoord.x, cell.guiCoord.y,
			DEFAULT_FIELD_CELL_SIZE, DEFAULT_FIELD_CELL_SIZE, hdcMem, 0, 0, SRCCOPY);
	}
	DeleteDC(hdcMem);

	/*	if (cell.isChanged)
		{
			cell.isChanged = false;

			cell.but->ModifyStyle(0, BS_BITMAP);
			switch (cell.state)
			{
			case EFieldState::Clear:
			case EFieldState::Clicked:
				cell.but->SetBitmap(clearCellBitmap);
				break;
			case EFieldState::Closed:
				cell.but->SetBitmap(closedCellBitmap);
				break;
			case EFieldState::Flaged:
				cell.but->SetBitmap(flagedCellBitmap);
				break;
			default:
				break;
			}
		}*/
}


void CYepView::CreateField()
{
	const auto fieldHeight = gameField->GetHeight();
	const auto fieldWidth = gameField->GetWidth();

	if (!allFieldStates.empty())
		ClearField();

	//You can call DestroyWindow() and then Create() again.Of course, this will only destroy and recreate 
	//	the button control, not the underlying C++ object, but that would be unnecessary anyway.Another 
	//	question is why you need to delete the button object - which of its properties do you want to 
	//	change which can't be changed while it exists? Or do you just want to remove it from the UI? 
	//	Then use ShowWindow instead.

	//size_t nextID = _APS_NEXT_CONTROL_VALUE + 1;
	{
		size_t nextID = 2001;
		const auto butSize = DEFAULT_FIELD_CELL_SIZE;
		const auto butFlags = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		for (size_t y = 0; y < fieldHeight; ++y)
		{
			for (size_t x = 0; x < fieldWidth; ++x)
			{
				const auto left = butSize * x + gameFieldViewOffset.x;
				const auto top = butSize * y + gameFieldViewOffset.y;

				FieldCellView cell;
				cell.cellIndex = y * fieldWidth + x; //TODO передалать независимо

				cell.guiCoord.x = left;
				cell.guiCoord.y = top;

				allFieldStates.push_back(cell);
			}
		}
	}

	gameFieldRect.left = gameFieldViewOffset.x;
	gameFieldRect.top = gameFieldViewOffset.y;
	gameFieldRect.right = gameFieldRect.left + fieldWidth * DEFAULT_FIELD_CELL_SIZE;
	gameFieldRect.bottom = gameFieldRect.top + fieldHeight * DEFAULT_FIELD_CELL_SIZE;
}


void CYepView::ClearField()
{	
	allFieldStates.clear();
}

void CYepView::EndGame()
{
	OutputDebugString(L"БДЫЩ\n");
	AfxMessageBox(L"End game");
}

CYepView::FieldCellView* CYepView::GetFieldCell(CPoint coordInClientRect)
{
	if (coordInClientRect.x >= gameFieldRect.left && coordInClientRect.x < gameFieldRect.right)
	{
		if (coordInClientRect.y >= gameFieldRect.top && coordInClientRect.y < gameFieldRect.bottom)
		{
			CPoint coordInGameField = coordInClientRect - gameFieldViewOffset;
			const size_t x = coordInGameField.x / DEFAULT_FIELD_CELL_SIZE;
			const size_t y = coordInGameField.y / DEFAULT_FIELD_CELL_SIZE;

			const auto index = y * gameField->GetWidth() + x;
			return index < allFieldStates.size()? &allFieldStates[index] : nullptr;
		}
		else
			return nullptr;
	}
	else
		return nullptr;
}

#pragma region Обвязка
void CYepView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CYepView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CYepView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CreateField();
}

#ifdef _DEBUG
void CYepView::AssertValid() const
{
	CFormView::AssertValid();
}

void CYepView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CYepDoc* CYepView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYepDoc)));
	return (CYepDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion

#pragma region Mouse Events
void CYepView::OnRButtonDown(UINT /* nFlags */, CPoint point)
{
	OutputDebugString(L"R BUTTON DOWN\n");

	auto cell = GetFieldCell(point);
	if (cell != nullptr)
	{
		if (cell->IsFlaged())
		{
			cell->TrySetAsClosed();
			OutputDebugString(L"set as closed\n");
		}
		else
		{
			cell->TrySetAsFlaged();
			OutputDebugString(L"set as flaged\n");
		}

		Invalidate(FALSE);
	}
}

void CYepView::OnLButtonDown(UINT nFlags, CPoint point)
{
	auto cell = GetFieldCell(point);
	if (cell != nullptr)
	{
		if (cell->TrySetAsClicked())
		{
			pressedCell = cell;
			OutputDebugString(L"set as clicked\n");
		}

		Invalidate(FALSE);
	}
}

void CYepView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pressedCell != nullptr)
	{
		if (isMouseOver)
		{
			std::vector<OPENED_CELL_INFO> openedCells;
			if (gameField->OpenCell(pressedCell->cellIndex, openedCells) == EOpenCellResult::EndGame)
				EndGame();
				
			for (auto& cellInfo : openedCells)
			{
				if (cellInfo.hasMine)
					allFieldStates[cellInfo.index].SetAsOpened(minedCellBitmap);
				else
					allFieldStates[cellInfo.index].SetAsOpened(countedBitmaps[cellInfo.countMinesAround]);
			}
		}
		else
		{
			pressedCell->TrySetAsClosed();
			OutputDebugString(L"set as closed\n");
		}

		pressedCell = nullptr;

		Invalidate(FALSE);
	}
}

LRESULT CYepView::OnMouseLeave(WPARAM, LPARAM)
{
	isMouseOver = false;
	return 0;
}

LRESULT CYepView::OnMouseHover(WPARAM, LPARAM)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent(&tme);

	return 0;
}

void CYepView::OnMouseMove(UINT nFlags, CPoint point)
{
	//wchar_t arr[30];
	//int f = _snwprintf_s(arr, 30, 30, L"x:%d y:%d!\n", point.x, point.y);
	//arr[f] = '\n';
	//OutputDebugString(arr);

	if (pressedCell != nullptr)
	{
		auto newPressedCell = GetFieldCell(point);
		if (newPressedCell != nullptr && newPressedCell != pressedCell)
		{
			pressedCell->TrySetAsClosed();
			pressedCell = newPressedCell;
			pressedCell->TrySetAsClicked();

			Invalidate(FALSE);
		}
	}

	currMousePosition = point;

	if (!isMouseOver)
	{
		isMouseOver = true;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
	}
}
#pragma endregion

void CYepView::OnDestroy()
{
	//for (auto& cell : allFieldStates)
	//	cell.but->DestroyWindow();
}
