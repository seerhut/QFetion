#ifndef TEST_H
#define TEST_H

#include <QtGui/QWidget>

namespace Ui {
    class Test;
}

class Test : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Test)
public:
    explicit Test(QWidget *parent = 0);
    virtual ~Test();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Test *m_ui;
};

#endif // TEST_H
