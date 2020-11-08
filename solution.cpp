#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Save the Planet.
 * Use less Fossil Fuel.
 **/

 const double RAD2DEG = 180 / M_PI;

 int rad2deg(double rad) {
     return rad * RAD2DEG;
 }

int main()
{
    int N; // the number of points used to draw the surface of Mars.
    cin >> N; cin.ignore();

    int startX = -1, endX = -1;
    int lastX = -1, lastY = -1;
    int landingY = -1;
    for (int i = 0; i < N; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        cin >> landX >> landY; cin.ignore();

        if (landY == lastY && startX == -1) {
            startX = lastX;
            endX = landX;
            landingY = landY;
        }

        lastX = landX;
        lastY = landY;
    }

    cerr << landingY << endl;

    const double G = 3.711;
    const int maxVSpeed = 40;
    const int maxHSpeed = 20;

    // game loop
    while (1) {
        int X, Y;
        int horSpeed, vertSpeed;
        int fuel, curAngle, curThrust;
        cin >> X >> Y;
        cin >> horSpeed >> vertSpeed;
        cin >> fuel >> curAngle >> curThrust;
        cin.ignore();

        double speed = sqrt(vertSpeed * vertSpeed + horSpeed * horSpeed);

        int angle = 0, thrust = 4;        
        if (X < startX || X > endX) {
            if ((X < startX && horSpeed < 0) || (X > endX && horSpeed > 0) ||
                abs(horSpeed) > 4 * maxHSpeed)
                angle = rad2deg(asin(horSpeed / speed));
            else if (abs(horSpeed) < 2 * maxHSpeed)
                angle = X < startX ? -45 : 45;
            else if (vertSpeed >= 0)
                thrust = 3;
        }
        else {
            if (Y < landingY + 200)
                thrust = 4;
            else if (abs(horSpeed) <= maxHSpeed - 10 && abs(vertSpeed) <= maxVSpeed - 5)
                thrust = 2;
            else
                angle = rad2deg(asin(horSpeed / speed));
        }

        // R P. R is the desired rotation angle. P is the desired thrust power.
        cout << angle << " " << thrust << endl;
    }
}