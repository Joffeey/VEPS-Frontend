// Slot.cpp

#include <Slot.h>


Slot::Slot(String loc, String slo, String sta, int green, int red) :
    m_location(loc), m_slotID(slo), m_status(sta), m_greenPin(green), m_redPin(red) {
    }

void Slot::Update(){
}

void Slot::Print(){
    Serial.println("Data for the selection slot: ");
    Serial.print("Location: ");
    Serial.println(this->m_location);
    Serial.print("SlotID: ");
    Serial.println(this->m_slotID);
    Serial.print("Status: ");
    Serial.println(this->m_status);
    Serial.print("Green LED is connected to pin: ");
    Serial.println(this->m_greenPin);
    Serial.print("Red LED is connected to pin: ");
    Serial.println(this->m_redPin);
}

String Slot::getSlotID(){
    return this->m_slotID;
}

void Slot::updateStatus(String newStatus){
    if(this->m_status == newStatus){
        Serial.println("New status is the same as current, no need to update -- Leaving!");
    }else if(this->m_status != newStatus && newStatus == "Available" || newStatus == "Unavailable"){
        Serial.println("New status is NOT the same as current and is a valid one -- Updating stauts!");
        this->m_status = newStatus;
    }
    else{
        Serial.println("Invalid status fetched, failed to update status -- Leaving!");
    }
}

String Slot::getStatus(){
    return this->m_status;
}