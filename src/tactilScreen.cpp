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
    if (ts.tirqTouched() && ts.touched())
    {

        TS_Point p = ts.getPoint();
        // Valeurs brutes du XPT2046 (0-4095) -> coordonnées écran (320x240, rotation 1)
        // A calibrer : affiche p.x / p.y en Serial et touche les coins de l'écran.
        int x = map(p.x, 200, 3800, 320, 0);
        int y = map(p.y, 200, 3800, 240, 0);


        // bouger le sliders dfe l avmc et de l eclairage
        if (getCurrentScreen() == 5 && x >= 162 && x <= 266 &&
            ((y >= 74 && y <= 92) || (y >= 138 && y <= 156)))
        {
            digitalWrite(CS_TACTIL, HIGH);
            digitalWrite(TFT_CS, LOW);
            moveSlider(x, y);
            digitalWrite(TFT_CS, HIGH);
            return;
        }

        for (int i = 0; i < screenElement.count; i++)
        {
            Button &b = screenElement.button[i];
            if (b.scene == getCurrentScreen() &&
                x >= b.x && x <= b.x + b.width &&
                y >= b.y && y <= b.y + b.height)
            {
                b.action();
                break;
            }
        }
    }
}
