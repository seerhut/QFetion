#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QHash>
#include <QHashIterator>
#include <QIcon>
#include <QSize>
#include <QLabel>
#include <QThread>
#include <QTreeView>
#include "contactlistitem.h"
#include "mainwindow.h"
#include "console.h"
#include "chatwindow.h"
#include "utilities.h"
#include "contact.h"
#include "aboutdialog.h"
#include "sysconfig.h"
#include "proxysetting.h"

MainWindow::MainWindow( QWidget *parent, Qt::WFlags flags ) :
	QMainWindow(parent, flags)
{
	//	qDebug() << "MainWindow Class Loading... ";
	//at first load self ui and connnet internal s/s
	main_ui.setupUi(this);

	main_ui.statusBar->setEnabled(false);
	main_ui.loginStatusLB->setText("Offline");
	main_ui.loginStatusPB->setVisible(false);
	main_ui.contactsTR->setColumnCount(1);
	main_ui.contactsTR->setHeaderHidden(true);

	//load console and connect the addtoLog service for self and others
	console = new Console(this);
	console->setWindowFlags(Qt::Window);
	//	console->show();

	//load control service
	control = new Control(this);
	//	ctr_driver = new ControlDriver(this);
	//	ctr_driver->start();
	//	control = ctr_driver->getControl();
	//	control->setMainWindow(this);
	//	control->setContactsTree(main_ui.contactsTR);

	px = new ProxySetting(this);
	px->hide();
	px->setSysconfig(control->syscfg);

	//load tray icon
	tray = new QSystemTrayIcon(QIcon(":/pet dis"), this);
	tray_menu = new QMenu("Menu", this);
	tray_menu->setObjectName("Tray Menu");
	tray_menu->addAction(main_ui.actionShow);
	tray_menu->addAction(main_ui.actionHide);
	tray_menu->addAction(main_ui.actionExit);
	tray->setContextMenu(tray_menu);

	//load end
	connect(main_ui.actionAbout, SIGNAL(activated()), this, SLOT(about()));
	connect(main_ui.actionShow, SIGNAL(activated()), this, SLOT(show()));
	connect(main_ui.actionHide, SIGNAL(activated()), this, SLOT(hideall()));
	connect(main_ui.actionAbout_Qt, SIGNAL(activated()), this, SLOT(about()));

	connect(this, SIGNAL(addtoLog(const QString &)), console,
			SLOT(appendLog(const QString &)));

	connect(control, SIGNAL(addtoLog(const QString &)), this,
			SIGNAL(addtoLog(const QString &)));
	connect(control, SIGNAL(sbMsg(const QString &, int)), this,
			SLOT(statusbar_msg(const QString &, int)));

	connect(control, SIGNAL(loginStatusChanged(Control::loginStatus)), this,
			SLOT(handle_loginStatusChanged(Control::loginStatus)));

	connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
			SLOT(TrayClicked(QSystemTrayIcon::ActivationReason)));

	this->statusBar()->showMessage("Not Login", 0);
	ad = 0;
	guyi_logout=false;
	tray->show();

	initLoginPage();
	main_ui.centralStack->setCurrentIndex(1);// to login page
	qDebug() << "MainWindow Class loaded";
}

MainWindow::~MainWindow()
{
	qDebug() << "MainWindow class unloaded";
}

void MainWindow::closeEvent( QCloseEvent *event )
{
	event->ignore();
	hideall();
}

void MainWindow::about()
{
	if( !ad )
	{
		ad = new AboutDialog(this);
	}
	ad->show();
}

void MainWindow::hideall()
{
	this->hide();
	console->hide();
}

void MainWindow::showall()
{

}

void MainWindow::on_actionOpen_Console_activated()
{
	if( console->isVisible() )
	{
		console->hide();
	} else
	{
		console->show();
	}
}

void MainWindow::statusbar_msg( const QString &msg, int timeout )
{
	this->statusBar()->showMessage(msg, timeout);
}

void MainWindow::TrayClicked( QSystemTrayIcon::ActivationReason reason )
{
	switch( reason )
	{
		case QSystemTrayIcon::DoubleClick:
			if( this->isVisible() ) hideall();
			else this->show();
			break;
		case QSystemTrayIcon::Trigger:

			this->show();
			break;
		case QSystemTrayIcon::MiddleClick:
			tray->showMessage("Hello", "This is a Fetion Clone",
					QSystemTrayIcon::Information, 2000);
			break;
		case QSystemTrayIcon::Context:

			break;
		default:
			return;
	}
}

void MainWindow::unread_msg_arrived( const QString &from_uri )
{
	//TODO blink tray

	//TODO blink contact item

}

void MainWindow::on_actionTest1_activated()
{
	main_ui.centralStack->setCurrentIndex(0);

}

void MainWindow::on_actionTest2_activated()
{

	control->test();
	initContactsPage();
	//	main_ui.nickLB->setText("hello,long long name");
}

void MainWindow::on_actionLogout_activated()
{
	//TODO:
	guyi_logout=true;

	control->tologout();
	initLoginPage();
	main_ui.centralStack->setCurrentIndex(1);

}

void MainWindow::on_actionExit_activated()
{
	guyi_logout=true;
	control->syscfg->saveToConfig();
	tray->hide();
	exit(0);
}

void MainWindow::on_savePassCKB_stateChanged( int state )
{
	switch( state )
	{
		case Qt::Checked:
			control->userinfo->setSavePasswd(true);
			break;
		case Qt::Unchecked:
			control->userinfo->setSavePasswd(false);
			break;
	}

}

void MainWindow::on_centralStack_currentChanged( int i )
{//do nothing
	if( i == 0 )
	{//to contacts list page
		qDebug() << "realy login-ed ?";
	}
	if( i == 1 )
	{//to login page
		//		initLoginPage();
	}
}

void MainWindow::initLoginPage()
{
	main_ui.nameLE->setEnabled(true);
	main_ui.passLE->setEnabled(true);
	main_ui.statusBar->setEnabled(false);
	main_ui.loginBTN->setEnabled(true);
	main_ui.loginStatusLB->setText("Offline");
	main_ui.loginStatusPB->setVisible(false);
	if( Utilities::isValidMobNo(control->syscfg->getDefaultAcount()) )
	{//fill default acount's info
		main_ui.nameLE->setText(control->syscfg->getDefaultAcount());//will trigger mobno parser
	} else
	{//empty fields
		main_ui.nameLE->setFocus();
	}
}

void MainWindow::initUserInfoPage()
{
	main_ui.nickLB->setText(control->userinfo->getNickname());
	main_ui.complaintLB->setText(control->userinfo->getComplaint());
	switch( control->userinfo->getPresence() )
	{
		case UserInfo::online:
			main_ui.presenceLB->setText("Online");
			break;
		case UserInfo::offline:
			main_ui.presenceLB->setText("Offline");
			break;
		case UserInfo::away:
			main_ui.presenceLB->setText("Away");
			break;
		case UserInfo::busy:
			main_ui.presenceLB->setText("Busy");
			break;
		default:
			main_ui.presenceLB->setText("Unknown");
			break;
	}
	//	main_ui.userIconLB->setPixmap(QPixmap(control->userinfo->getPortrait()));
}

void MainWindow::initContactsPage()
{
	QTreeWidget * tree = main_ui.contactsTR;
	QTreeWidgetItem * item;

	tree->clear();
	item = new QTreeWidgetItem(tree, QStringList(tr("New Contacts")));
	tree->insertTopLevelItem(0, item);

	qDebug() << "add group " << 0 << ":" << "New Contacts";
	QHashIterator<int, QString> ig(control->getGroupsHash());
	while( ig.hasNext() )
	{
		ig.next();
		item = new QTreeWidgetItem(tree, QStringList(ig.value()));
		tree->insertTopLevelItem(ig.key(), item);
		item->setExpanded(false);
		qDebug() << "add group " << ig.key() << ":" << ig.value();

	}

	int gid;
	Contact * contact;
	QHashIterator<QString, Contact *> ic(control->getContactsHash());
	while( ic.hasNext() )
	{
		ic.next();
		contact = ic.value();
		gid = contact->getTeam();
		item = new QTreeWidgetItem(tree->topLevelItem(gid));
		tree->setItemWidget(item, 0, contact->getListItem());
		//		connect(item, SIGNAL(), contact, SLOT(getChatWindow()));
		qDebug() << "add contact " << gid << ":" << contact->getSid();

	}
	tree->expandAll();

}

void MainWindow::on_passLE_returnPressed()
{
	main_ui.loginBTN->click();

}

void MainWindow::on_loginBTN_clicked()
{
	if( Utilities::isValidMobNo(main_ui.nameLE->text())
			&& !main_ui.passLE->text().isEmpty() )
	{
		control->userinfo->setPasswd(main_ui.passLE->text());//always using
		control->userinfo->setPresence(UserInfo::online);
		main_ui.nameLE->setEnabled(false);
		main_ui.passLE->setEnabled(false);
		control->tologin();
	}
	guyi_logout=false;

}

void MainWindow::on_proxyBTN_clicked()
{
	px->show();
}

void MainWindow::on_nameLE_textChanged()
{
	if( Utilities::isValidMobNo(main_ui.nameLE->text()) )
	{
		qDebug() << "valid mobno";
		main_ui.loginBTN->setEnabled(true);
		control ->userinfo->initFromConfig(main_ui.nameLE->text());
		if( control->userinfo->isSavePasswd() )
		{
			if( !control->userinfo->getPasswd().isEmpty() )
			{
				main_ui.passLE->setText(control->userinfo->getPasswd());
				main_ui.savePassCKB->setChecked(true);
				main_ui.loginBTN->setFocus();
			} else
			{
				main_ui.savePassCKB->setChecked(true);
				main_ui.passLE->setText("");
				main_ui.passLE->setFocus();
			}
		} else
		{
			main_ui.savePassCKB->setChecked(false);
			main_ui.passLE->setText("");
			main_ui.passLE->setFocus();
		}
	} else
	{
		main_ui.loginBTN->setEnabled(false);
		main_ui.passLE->setText("");
		main_ui.savePassCKB->setChecked(false);
		//		main_ui.autoLoginCKB->setChecked(false);
	}
}

void MainWindow::handle_loginStatusChanged( Control::loginStatus status )
{

	switch( status )
	{
		case Control::notLogin:
			tray->setIcon(QIcon(":/pet dis"));
			break;
		case Control::loginToSSiPortal:
			main_ui.nameLE->setEnabled(false);
			main_ui.passLE->setEnabled(false);
			main_ui.loginBTN->setEnabled(false);
			main_ui.loginStatusPB->setVisible(true);
			main_ui.loginStatusPB->setRange(0, 100);
			main_ui.loginStatusPB->setValue(5);
			main_ui.loginStatusLB->setText("Login to SSI portal...");
			break;
		case Control::loginToSSiPortalDone:
			main_ui.loginStatusPB->setValue(10);
			main_ui.loginStatusLB->setText("Login to SSI portal done");
			break;
		case Control::getProxyHost:
			main_ui.loginStatusPB->setValue(15);
			main_ui.loginStatusLB->setText("Get Proxy Host...");
			break;
		case Control::getProxyHostDone:
			main_ui.loginStatusPB->setValue(20);
			main_ui.loginStatusLB->setText("Get Proxy Host done");
			break;
		case Control::registerToProxyConnecting:
			main_ui.loginStatusPB->setVisible(true);
			main_ui.loginStatusPB->setValue(30);
			main_ui.loginStatusLB->setText("Connecting to Proxy...");
			break;
		case Control::registerToProxyConnected:
			main_ui.loginStatusPB->setVisible(true);
			main_ui.loginStatusPB->setValue(40);
			main_ui.loginStatusLB->setText("Connecting to Proxy done");
			break;
		case Control::registerToProxyRequestNonce:
			main_ui.loginStatusPB->setVisible(true);
			main_ui.loginStatusPB->setValue(50);
			main_ui.loginStatusLB->setText("Try to Register...");
			break;
		case Control::registerToProxySentDigest:
			main_ui.loginStatusPB->setVisible(true);
			main_ui.loginStatusPB->setValue(65);
			main_ui.loginStatusLB->setText("Send the Digest...");
			break;
		case Control::registerToProxyDone:
			main_ui.loginStatusPB->setValue(80);
			main_ui.loginStatusLB->setText("Login OK");
			tray->setIcon(QIcon(":/pet ena"));
			//		main_ui.centralStack->setCurrentIndex(0);
			break;
		case Control::loginToSSiPortalError:
			main_ui.loginStatusPB->reset();
			main_ui.loginStatusPB->setVisible(false);
			main_ui.loginStatusLB->setText("Login  Error");
			main_ui.nameLE->setEnabled(true);
			main_ui.passLE->setEnabled(true);
			main_ui.loginBTN->setEnabled(true);
			break;
		case Control::getProxyHostError:
			main_ui.loginStatusPB->reset();
			main_ui.loginStatusPB->setVisible(false);
			main_ui.loginStatusLB->setText("Get Proxy Host Error");
			main_ui.nameLE->setEnabled(true);
			main_ui.passLE->setEnabled(true);
			main_ui.loginBTN->setEnabled(true);
			break;
		case Control::registerToProxyError:
			main_ui.loginStatusPB->reset();
			main_ui.loginStatusPB->setVisible(false);
			main_ui.loginStatusLB->setText("Register Error");
			main_ui.nameLE->setEnabled(true);
			main_ui.passLE->setEnabled(true);
			main_ui.loginBTN->setEnabled(true);
			break;
		case Control::getUserInfo:
			main_ui.loginStatusLB->setText("Getting User Info");
			main_ui.loginStatusPB->setValue(80);
			break;
		case Control::getUserInfoDone:
			main_ui.loginStatusLB->setText("Getting User Info Done");
			main_ui.loginStatusPB->setValue(85);
			this->statusBar()->setEnabled(true);
			statusbar_msg("Getting User info", 2000);
			initUserInfoPage();
			break;
		case Control::getContacts:
			main_ui.loginStatusLB->setText("Getting Contacts");
			main_ui.loginStatusPB->setValue(90);
			statusbar_msg("Get Buddies", 2000);
			break;
		case Control::getContactsDone:
			main_ui.loginStatusLB->setText("Getting Contacts Done");
			main_ui.loginStatusPB->setValue(100);
			main_ui.centralStack->setCurrentIndex(0);
			statusbar_msg("Get Buddies Done", 2000);
			initContactsPage();
			break;
		case Control::shouldShowContactsPage:
			main_ui.centralStack->setCurrentIndex(0);
			break;
		case Control::getUserInfoError:
			main_ui.loginStatusLB->setText("Getting User Info Error");
			main_ui.loginStatusPB->setValue(100);

			break;
		case Control::getContactsError:
			main_ui.loginStatusLB->setText("Getting Contacts Error");
			main_ui.loginStatusPB->setValue(100);

			break;
		case Control::logoutFromProxy:
			//		initContactsPage();
			main_ui.contactsTR->clear();
			tray->setIcon(QIcon(":/pet dis"));
			initLoginPage();
			break;
		case Control::serverCloseConnection:
			//		initContactsPage();
			main_ui.contactsTR->clear();
			tray->setIcon(QIcon(":/pet dis"));
			initLoginPage();
			if(!guyi_logout)on_loginBTN_clicked();
			break;
		default:

			break;
	}
}
