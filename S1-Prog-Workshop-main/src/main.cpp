#include <sil/sil.hpp>
#include "random.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <complex>
#include <cmath>
#include <iostream>

void keep_green_only(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void channels_swap(sil::Image &image)
{

    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
}

void black_and_white(sil::Image &image)
{

    for (glm::vec3 &color : image.pixels())
    {
        float moy{};
        moy = (color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722) / 3;
        color.r = moy;
        color.g = moy;
        color.b = moy;
    }
}

void negatif(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    }
}

void degrade(sil::Image &image)
{
    glm::vec3 color = {0, 0, 0};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x, y) = color;
        }
        color += 1.f / float(image.width());
    }
}

void mirror(sil::Image &image)
{
    for (int x{0}; x < image.width() / 2; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::swap(image.pixel(x, y), image.pixel(image.width() - 1 - x, y));
        }
    }
}

void noise(sil::Image &image)
{
    int totalPix = image.width() * image.height();
    int noisePix = totalPix * 0.1f;

    for (int i = 0; i < noisePix; ++i)
    {
        int randX = random_int(0, image.width());
        int randY = random_int(0, image.height());

        glm::vec3 pixel = {random_float(0, 1), random_float(0, 1), random_float(0, 1)};
        image.pixel(randX, randY) = pixel;
    }
}

void rotate90(sil::Image &image)
{
    sil::Image new_image{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            new_image.pixel(image.height() - y - 1, x) = image.pixel(x, y);
        }
    }

    image = new_image;
}

void rgbSplit(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    for (int x{0}; x < image.width() - 1; x++)
    {
        for (int y{0}; y < image.height() - 1; y++)
        {
            if (x <= 30)
            {
                new_image.pixel(x, y).r = 0.f;
                new_image.pixel(x, y).g = image.pixel(x, y).g;
                new_image.pixel(x, y).b = image.pixel(x + 30, y).b;
            }

            else if (x >= image.width() - 30)
            {
                new_image.pixel(x, y).r = image.pixel(x - 30, y).r;
                new_image.pixel(x, y).g = image.pixel(x, y).g;
                new_image.pixel(x, y).b = 0.f;
            }

            else
            {
                new_image.pixel(x, y).r = image.pixel(x - 30, y).r;
                new_image.pixel(x, y).g = image.pixel(x, y).g;
                new_image.pixel(x, y).b = image.pixel(x + 30, y).b;
            }
        }
    }
    image = new_image;
}

bool estDansDisque(int x, int y, int cx, int cy, int radius)
{
    int dx = x - cx;
    int dy = y - cy;

    return (dx * dx + dy * dy) <= radius * radius;
}

void disque(sil::Image &image, int cx, int cy)
{
    int radius = 100;

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            bool dansDisque = estDansDisque(x, y, cx, cy, radius);

            if (dansDisque)
            {
                image.pixel(x, y) = {1.f, 1.f, 1.f};
            }
        }
    }
}

void cercle(sil::Image &image, int cx, int cy)
{
    int radius = 100;
    int epaisseur = 2;

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            int dx = x - cx;
            int dy = y - cy;

            int dist2 = dx * dx + dy * dy;

            if (dist2 >= (radius - epaisseur) * (radius - epaisseur) &&
                dist2 <= (radius + epaisseur) * (radius + epaisseur))
            {
                image.pixel(x, y) = {1.f, 1.f, 1.f};
            }
            else
            {
                image.pixel(x, y) = {0.f, 0.f, 0.f};
            }
        }
    }
}

void animation()
{
    const int width = 500;
    const int height = 500;
    const int frames = 60;

    int startX = 50;
    int endX = width - 50;
    int cy = height / 2;

    for (int i = 0; i < frames; ++i)
    {
        sil::Image image(width, height);

        int cx = startX + (endX - startX) * i / (frames - 1);

        disque(image, cx, cy);

        image.save("output/frame_" + std::to_string(i) + ".png");
    }
}

void rosace(sil::Image &image)
{
    int rayon = 50;
    int distance = 50;
    int epaisseur = 2;

    int cx = image.width() / 2;
    int cy = image.height() / 2;

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            // fond noir
            image.pixel(x, y) = {0.f, 0.f, 0.f};

            int dx0 = x - cx;
            int dy0 = y - cy;
            int d0 = dx0 * dx0 + dy0 * dy0;

            if (d0 >= (rayon - epaisseur) * (rayon - epaisseur) &&
                d0 <= (rayon + epaisseur) * (rayon + epaisseur))
            {
                image.pixel(x, y) = {1.f, 1.f, 1.f};
            }

            for (int i = 0; i < 6; ++i)
            {
                float theta = i * 3.14 / 3.f;

                int cxi = cx + distance * std::cos(theta);
                int cyi = cy + distance * std::sin(theta);

                int dx = x - cxi;
                int dy = y - cyi;
                int d = dx * dx + dy * dy;

                if (d >= (rayon - epaisseur) * (rayon - epaisseur) &&
                    d <= (rayon + epaisseur) * (rayon + epaisseur))
                {
                    image.pixel(x, y) = {1.f, 1.f, 1.f};
                }
            }
        }
    }
}

void mosaic(sil::Image &image)
{
    sil::Image new_image{image.width() * 5, image.height() * 5};

    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            new_image.pixel(x, y) = image.pixel(x % image.width(), y % image.height());
        }
    }
    image = new_image;
}

void mosaicMirror(sil::Image &image)
{
    sil::Image new_image{image.width() * 5, image.height() * 5};

    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {

            int mirrorX = x % image.width();
            int mirrorY = y % image.height();

            // miroir horizontal
            if (x / image.width() % 2 != 0)
            {
                mirrorX = image.width() - 1 - mirrorX;
            }

            // miroir vertical
            if (y / image.height() % 2 != 0)
            {
                mirrorY = image.height() - 1 - mirrorY;
            }

            new_image.pixel(x, y) = image.pixel(mirrorX, mirrorY);
        }
    }
    image = new_image;
}

void glitch(sil::Image &image)
{
    int minH = 1;
    int maxH = 6;
    int minW = 10;
    int maxW = 20;
    int iterations = 100;

    for (int i = 0; i < iterations; ++i)
    {
        int rectW = random_int(minW, maxW);
        int rectH = random_int(minH, maxH);

        int x1 = random_int(0, image.width() - rectW - 1);
        int y1 = random_int(0, image.height() - rectH - 1);

        int x2 = random_int(0, image.width() - rectW - 1);
        int y2 = random_int(0, image.height() - rectH - 1);

        for (int dx = 0; dx < rectW; ++dx)
        {
            for (int dy = 0; dy < rectH; ++dy)
            {
                std::swap(
                    image.pixel(x1 + dx, y1 + dy),
                    image.pixel(x2 + dx, y2 + dy));
            }
        }
    }
}

float get_brightness(const glm::vec3 &p)
{
    return 0.2126f * p.r + 0.7152f * p.g + 0.0722f * p.b;
}

void sort_pixels(sil::Image &image)
{
    int iterations = 100;

    for (int i = 0; i < iterations; ++i)
    {
        int y = random_int(0, image.height() - 1);
        int x = random_int(0, image.width() - 1);
        int largeur = random_int(10, 100);

        std::vector<glm::vec3> tab;

        for (int j = 0; j < largeur; ++j)
        {
            if (x + j < image.width())
            {
                tab.push_back(image.pixel(x + j, y));
            }
        }

        std::sort(tab.begin(), tab.end(),
                  [](const glm::vec3 &color1, const glm::vec3 &color2)
                  {
                      return get_brightness(color1) < get_brightness(color2);
                  });

        for (int k = 0; k < tab.size(); ++k)
        {
            image.pixel(x + k, y) = tab[k];
        }
    }
}

void mandelbrot(sil::Image &image, int maxIter = 100)
{
    float xMin = -2.0f;
    float xMax = 2.0f;
    float yMin = -2.0f;
    float yMax = 2.0f;

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            float x0 = xMin + (xMax - xMin) * x / float(image.width());
            float y0 = yMin + (yMax - yMin) * y / float(image.height());
            std::complex<float> c{x0, y0};
            std::complex<float> z{0.f, 0.f};

            int iter = 0;
            while (std::abs(z) <= 2.0f && iter < maxIter)
            {
                z = z * z + c;
                iter++;
            }

            float t = iter / float(maxIter);

            float r = 0.5f * (1.0f - t);
            float g = 0.0f;
            float b = 0.5f + 0.5f * t;

            if (iter < maxIter)
            {
                r = 0.0f;
                g = 0.0f;
                b = 0.0f;
            }

            image.pixel(x, y) = {r, g, b};
        }
    }
}

void tramage(sil::Image &image)
{
    const int bayer_n = 4;
    float bayer_matrix_4x4[][bayer_n] = {
        {-0.5, 0, -0.375, 0.125},
        {0.25, -0.25, 0.375, -0.125},
        {-0.3125, 0.1875, -0.4375, 0.0625},
        {0.4375, -0.0625, 0.3125, -0.1875},
    };

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {

            float brightness = get_brightness(image.pixel(x, y));
            float bayer_value = bayer_matrix_4x4[y % bayer_n][x % bayer_n];
            float birghtness_offset = brightness + bayer_value;

            if (birghtness_offset < 0.5f)
                image.pixel(x, y) = glm::vec3(0.0f);
            else
                image.pixel(x, y) = glm::vec3(1.0f);
        }
    }
}

void norm_histogramme(sil::Image &image)
{
    float minBrightness = 1.0f;
    float maxBrightness = 0.0f;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float currentBrightness = get_brightness(image.pixel(x, y));

            minBrightness = std::min(minBrightness, currentBrightness);
            maxBrightness = std::max(maxBrightness, currentBrightness);
        }
    }

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            glm::vec3 &color = image.pixel(x, y);
            color = (color - minBrightness) / (maxBrightness - minBrightness);
            image.pixel(x, y) = color;
        }
    }
}

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

void vortex(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};

    glm::vec2 center = glm::vec2(image.width() / 2, image.height() / 2);

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            glm::vec2 point{x, y};

            float dist = glm::distance(center, point);
            float angle = dist / 10;

            glm::vec2 newPoint = rotated(point, center, angle);
            if (newPoint.x < image.width() && newPoint.y >= 0 && newPoint.y < image.height() && newPoint.x >= 0)
            {
                new_image.pixel(x, y) = image.pixel(newPoint.x, newPoint.y);
            }
            else
            {
                image.pixel(x, y) = glm::vec3(0.0f);
            }
        }
    }
    image = new_image;
}

void vortexEye(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};

    glm::vec2 center = glm::vec2(image.width() / 2, image.height() / 2);

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            glm::vec2 point{x, y};

            float dist = glm::distance(center, point);
            float angle = dist * 10;

            glm::vec2 newPoint = rotated(point, center, angle);
            if (newPoint.x < image.width() && newPoint.y >= 0 && newPoint.y < image.height() && newPoint.x >= 0)
            {
                new_image.pixel(x, y) = image.pixel(newPoint.x, newPoint.y);
            }
            else
            {
                image.pixel(x, y) = glm::vec3(0.0f);
            }
        }
    }
    image = new_image;
}

void convolution(sil::Image &image)
{

    sil::Image new_image{image.width(), image.height()};

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            glm::vec3 color{0};

            for (int x2 = x - 3; x2 < x + 4; ++x2)
            {
                for (int y2 = y - 3; y2 < y + 4; ++y2)
                {
                    if (x2 > 0 && x2 < image.width() && y2 > 0 && y2 < image.height())
                    {
                        color += image.pixel(x2, y2);
                    }
                }
            }
            color /= 49;

            new_image.pixel(x, y) = color;
        }
    }
    image = new_image;
}

void convolution2(sil::Image &image)
{
    const int sizeM = 3;
    float matrice[sizeM][sizeM] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}};

    sil::Image new_image{image.width(), image.height()};

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            glm::vec3 color{0.f};

            for (int x2 = -1; x2 <= 1; ++x2)
            {
                for (int y2 = -1; y2 <= 1; ++y2)
                {
                    int ix = x + x2;
                    int iy = y + y2;
                    if (ix >= 0 && ix < image.width() && iy >= 0 && iy < image.height())
                    {
                        color += matrice[x2 + 1][y2 + 1] * image.pixel(ix, iy);
                    }
                }
            }

            new_image.pixel(x, y) = color;
        }
    }

    image = new_image;
}

void convolution3(sil::Image &image)
{
    const int sizeM = 3;
    float matrice[sizeM][sizeM] = {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}};

    sil::Image new_image{image.width(), image.height()};

    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            glm::vec3 color{0.f};

            for (int x2 = -1; x2 <= 1; ++x2)
            {
                for (int y2 = -1; y2 <= 1; ++y2)
                {
                    int ix = x + x2;
                    int iy = y + y2;
                    if (ix >= 0 && ix < image.width() && iy >= 0 && iy < image.height())
                    {
                        color += matrice[x2 + 1][y2 + 1] * image.pixel(ix, iy);
                    }
                }
            }

            new_image.pixel(x, y) = color;
        }
    }

    image = new_image;
}

// void differenceGaussienne(sil::Image &image)
// {
//     sil::Image blur1 = image;
//     sil::Image blur2 = image;

//     convolution(blur1);
//     convolution(blur2);

//     for (int x = 0; x < image.width(); ++x)
//     {
//         for (int y = 0; y < image.height(); ++y)
//         {
//             glm::vec3 c1 = blur1.pixel(x, y);
//             glm::vec3 c2 = blur2.pixel(x, y);

//             glm::vec3 diff = c1 - c2;

//             image.pixel(x, y) = diff;
//         }
//     }
// }

int main()
{
    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        keep_green_only(image);
        image.save("output/keep_green_only_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        channels_swap(image);
        image.save("output/channels_swap_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        black_and_white(image);
        image.save("output/black_and_white_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        negatif(image);
        image.save("output/negatif_logo.png");
    }

    {
        sil::Image image{300 /*width*/, 200 /*height*/};
        degrade(image);
        image.save("output/degrade.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        mirror(image);
        image.save("output/mirror_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        noise(image);
        image.save("output/noise_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        rotate90(image);
        image.save("output/rotate90_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        rgbSplit(image);
        image.save("output/rgbSplit_logo.png");
    }

    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        // TODO: modifier l'image
        disque(image, image.width() / 2, image.height() / 2);
        image.save("output/disque.png");
    }

    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        // TODO: modifier l'image
        cercle(image, 250, 250);
        image.save("output/cercle.png");
    }

    {
        animation();
    }

    {
        sil::Image image{500 /*width*/, 500 /*height*/};
        // TODO: modifier l'image
        rosace(image);
        image.save("output/rosace.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        mosaic(image);
        image.save("output/mosaic_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        mosaicMirror(image);
        image.save("output/mosaicMirror_logo.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        glitch(image);
        image.save("output/glitch_logo.png");
    }

    {
        sil::Image image(500, 500);
        mandelbrot(image, 200);
        image.save("output/mandelbrot.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        // TODO: modifier l'image
        tramage(image);
        image.save("output/tragrame.png");
    }

    {
        sil::Image image{"images/photo_faible_contraste.jpg"};
        // TODO: modifier l'image
        norm_histogramme(image);
        image.save("output/histogramme.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        vortex(image);
        image.save("output/vortex.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        vortexEye(image);
        image.save("output/vortexEye.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        convolution(image);
        image.save("output/convolution.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        sort_pixels(image);
        image.save("output/sort.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        convolution2(image);
        image.save("output/convolution2.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        convolution3(image);
        image.save("output/convolution3.png");
    }

    {
        sil::Image image{"images/logo.png"};
        // TODO: modifier l'image
        differenceGaussienne(image);
        image.save("output/diffGaussienne.png");
    }
}
