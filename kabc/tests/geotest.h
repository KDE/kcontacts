#ifndef GEOTEST_H
#define GEOTEST_H

#include <QtCore/QObject>

class GeoTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS:
    void emptyConstructor();
    void constructor();
    void isValid();
    void setData();
    void equals();
    void differs();
    void serialization();
};

#endif
