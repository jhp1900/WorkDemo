#pragma once

#include "stdafx.h"

enum UILIB_RESOURCETYPE
{
	UILIB_FILE = 1,				// ���Դ����ļ�
	UILIB_ZIP,						// ���Դ���zipѹ����
	UILIB_RESOURCE,			  // ������Դ
	UILIB_ZIPRESOURCE,	  // ������Դ��zipѹ����
};

class WindowImplBase : public DuiLib::CDuiWindowImpl
{
public:
	WindowImplBase() {}
	virtual ~WindowImplBase() {}

	virtual LRESULT OnInit() { return 0; }  		// �����ʼ��
	virtual void OnFinalMessage(HWND hWnd) override;	// ���Ĵ���һ�����ͷ���Դʹ��
	virtual void Notify(TNotifyUI& msg) override;		// DuiLib �ؼ���Ϣ·��
	virtual BOOL OnSysClick(TNotifyUI& msg);			// ϵͳ��ť�����Ӧ
	virtual void OnUserClick(const TNotifyUI& msg) {}			// 	�û��ؼ������Ӧ -- ���ڸ�д

protected:
	virtual CDuiString GetSkinFolder() { return xml_path_; }
	virtual CDuiString GetSkinFile() { return xml_name_; }
	virtual LPCTSTR GetWindowClassName(void) const { return _T("WindowImplBase"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

public:
	virtual UINT GetClassStyle() const override;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;	// WM_ ��Ϣ·�ɴ�����
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	// �����Զ��� WM_ ��Ϣ
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetSkinFolder(CDuiString xml_path) { xml_path_ = xml_path; }
	void SetSkinFile(CDuiString xml_name) { xml_name_ = xml_name; }

protected:
	static LPBYTE m_lpResourceZIPBuffer;
	CPaintManagerUI m_PaintManager;
	CDuiString xml_path_;
	CDuiString xml_name_;
	CDuiString win_class_naem_;
};

