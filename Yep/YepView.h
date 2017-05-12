
// YepView.h : interface of the CYepView class
//

#pragma once

#include "AppTypes.h"
#include "GameField.h"

class CYepView : public CFormView
{
protected: // create from serialization only
	CYepView();
	DECLARE_DYNCREATE(CYepView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_YEP_FORM };
#endif

// Attributes
public:
	CYepDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC);
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CYepView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg LRESULT OnMouseHover(WPARAM, LPARAM);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	enum class EFieldState
	{
		Closed,
		Flaged,
		Opened,
		Clicked,
	};

	struct FieldCellView
	{
		EFieldState	state;
		HBITMAP		bitmap;
		CPoint		guiCoord;
		size_t		cellIndex;

		FieldCellView() : state(EFieldState::Closed)
		{}

		void	SetState(EFieldState newState)
		{
			state = newState;
		}

		bool	IsFlaged() const { return state == EFieldState::Flaged; }
		bool	IsClosed() const { return state == EFieldState::Closed; }

		void	SetAsOpened(HBITMAP bitmapState)
		{
			state = EFieldState::Opened;
			bitmap = bitmapState;
		}

		//return true if state changed
		bool	TrySetAsClicked()
		{
			if (state == EFieldState::Closed)
			{
				state = EFieldState::Clicked;
				return true;
			}
			else
				return false;
		}

		bool	TrySetAsClosed()
		{
			if (state == EFieldState::Clicked)
			{
				state = EFieldState::Closed;
				return true;
			}
			else
				return false;
		}

		void	TrySetAsFlaged()
		{
			if (state == EFieldState::Closed)
				state = EFieldState::Flaged;
		}
	};

private:
	void	CreateField();
	void	ClearField();

	void	EndGame();
	FieldCellView*	GetFieldCell(CPoint coordInClientRect);
private:
	

	
	std::vector<FieldCellView>	allFieldStates;
	std::shared_ptr<icc::MineSweeper::GameField>	gameField;

	//смещение игрового поля в окне игры
	const CPoint	gameFieldViewOffset;
	//координаты прямоугольника в котором размещается игровое поле
	RECT			gameFieldRect;

	HBITMAP	flagedCellBitmap;
	//HBITMAP	openCellBitmap;
	//HBITMAP	minedCellBitmap;
	HBITMAP	closedCellBitmap;
	HBITMAP	emptyCellBitmap;
	HBITMAP	minedCellBitmap;
	HBITMAP	countedBitmaps[10];

#pragma region Mouse
	bool	isMouseOver;
	FieldCellView*	pressedCell;
	CPoint	rightButtonUpPointInImageCoordInv;
	CPoint	currMousePosition;
#pragma endregion

};

#ifndef _DEBUG  // debug version in YepView.cpp
inline CYepDoc* CYepView::GetDocument() const
   { return reinterpret_cast<CYepDoc*>(m_pDocument); }
#endif

