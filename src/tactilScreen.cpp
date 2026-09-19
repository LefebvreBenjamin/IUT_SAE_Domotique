#include "tactilScreen.h"

XPT2046_Touchscreen ts(CS_TACTIL, TIRQ_PIN);

void initTouchScreen()
{
    ts.begin();
    ts.setRotation(1);
    Serial.println("Touchscreen initialized");
}

void updateTouchScreen()
{
    if (ts.tirqTouched())
    {
        if (ts.touched())
        {
            TS_Point p = ts.getPoint();
            Serial.print("Pressure = ");
            Serial.print(p.z);
            Serial.print(", x = ");
            Serial.print(p.x);
            Serial.print(", y = ");
            Serial.print(p.y);
            Serial.println();
        }
    }
}
