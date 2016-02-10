#include "DataObserver.h"

void DataObserver::SubscribeOn(DataNotifier &dataNotifier)
{
    dataNotifier.RegisterDataObserver(*this);
}

void DataNotifier::RegisterDataObserver(DataObserver &observer)
{
    observers_.push_back(&observer);
}

void DataNotifier::NotifyAboutData(const Data &data) const
{
    for (DataObserver* observer: observers_) {
        observer->NotifyAboutData(data);
    }
}
