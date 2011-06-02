#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "control.h"
#include "ui_mainwindow.h"

namespace Ui
{
class MainWindow;
}
class MainWindow;
class QObject;
class QWidget;
class QMainWindow;
class QString;
class QAction;
class QMenu;
class QSystemTrayIcon;
class Console;
class Control;
class AboutDialog;
class SysConfig;
class ProxySetting;
//class ControlDriver;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow( QWidget *parent = 0, Qt::WFlags flags = 0 );
	~MainWindow();

	signals:
	void show_console();
	void addtoLog( const QString & );
	//	void startLogin();


private slots:
	void about();
	void TrayClicked( QSystemTrayIcon::ActivationReason );
	void hideall();
	void showall();
	void on_actionOpen_Console_activated();
	void on_actionLogout_activated();
	void on_centralStack_currentChanged( int i );
	void on_loginBTN_clicked();
	void on_passLE_returnPressed();
	void on_nameLE_textChanged();

	void handle_loginStatusChanged( Control::loginStatus status );

	void statusbar_msg( const QString &, int );
	void on_actionTest1_activated();
	void on_actionTest2_activated();
	void on_actionExit_activated();
	void on_proxyBTN_clicked();
	void on_savePassCKB_stateChanged(int state);
	void unread_msg_arrived( const QString &from_uri );

private:
	void closeEvent( QCloseEvent *event );
	void initLoginPage();
	void initContactsPage();
	void initUserInfoPage();
	Ui::MainWindow main_ui;
	Console * console;
	Control * control;
	QSystemTrayIcon * tray;
	QMenu * tray_menu;
	AboutDialog * ad;
	ProxySetting * px;
	bool guyi_logout;
};

#endif // MAINWINDOW_H
