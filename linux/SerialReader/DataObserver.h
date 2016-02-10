#ifndef DATAOBSERVER_H
#define DATAOBSERVER_H

#include "common.h"

class DataNotifier;

class DataObserver
{
public:
    virtual void NotifyAboutData(const Data& data) = 0;
    void SubscribeOn(DataNotifier &dataNotifier);
};

class DataNotifier
{
    std::vector<DataObserver*> observers_;

public:
    void RegisterDataObserver(DataObserver &observer);
    void NotifyAboutData(const Data& data) const;
};

#endif // DATAOBSERVER_H
