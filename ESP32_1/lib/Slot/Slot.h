// Slot.h

#ifndef SLOT_H
#define SLOT_H

#include <Arduino.h>


class Slot
{
private:
    String m_location;
    String m_slotID;
    String m_status;
    int m_greenPin;
    int m_redPin;
public:
    Slot(String m_location, String m_slotID, String m_status, int m_greenPin, int m_redPin);
    void Update();
    void Print();
    String getSlotID();
    void updateStatus(String newStatus);
    String getStatus();
    void updateStatusFromWeb();
};


#endif