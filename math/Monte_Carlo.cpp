#include <iostream>

#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time() to seed srand()
#define MAX_POINTS 1000
using namespace std;

int main()
{
    // put random seed
    srand(static_cast<unsigned int>(time(0)));
    int point_inside_circle = 0;
    float random_x = 0.0f;
    float random_y = 0.0f;
    for (int i = 0; i < MAX_POINTS; i++)
    {
        // rand float in [-1,1]
        float random_x = -1.0f + 2.0f * (float(rand()) / RAND_MAX);
        float random_y = -1.0f + 2.0f * (float(rand()) / RAND_MAX);
        cout << "Point " << i + 1 << ": (" << random_x << "," << random_y << ")\n";
        float point_dist = random_x * random_x + random_y * random_y;
        if (point_dist <= 1)
        {
            // pint in the circle
            point_inside_circle++;
        }
    }
    cout <<"total exist " <<point_inside_circle <<" point in unit circle"<< "\n";
    cout << "Estimated Pi = " << (4.0f * point_inside_circle) / MAX_POINTS << endl;
    return 0;
}