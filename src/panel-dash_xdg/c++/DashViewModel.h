/* 
 * File:   DashViewModel.h
 * Author: akiel
 *
 * Created on November 3, 2014, 10:02 PM
 */

#ifndef DASHVIEWMODEL_H
#define	DASHVIEWMODEL_H
#include <QAbstractListModel>
#include <qt5xdg/XdgDesktopFile>
#include <QList>

class DashViewModel : public QAbstractListModel  {
    Q_OBJECT
    
public:
    explicit DashViewModel(const QList<XdgDesktopFile*>& appList,QObject* parent = 0);
//    DashViewModel(const DashViewModel& orig);
    virtual ~DashViewModel();
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    XdgDesktopFile* getDesktop(int rowIndex);
    void clear();
    
private:
    QList<XdgDesktopFile*> appList_;
};

#endif	/* DASHVIEWMODEL_H */

