#ifndef INITIALLAYEREDITOR_H
#define INITIALLAYEREDITOR_H

#include <QWidget>

class QTableWidget;

class InitialLayerEditor : public QWidget
{
    Q_OBJECT
public:
    explicit InitialLayerEditor(QWidget *parent = nullptr);
};

#endif // INITIALLAYEREDITOR_H