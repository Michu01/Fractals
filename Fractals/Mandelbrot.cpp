#include "Mandelbrot.h"

size_t Mandelbrot::CheckConvergence(std::complex<double> x) const
{
	std::complex<double> c(x);
	size_t it{};

	while (it < maxIterations && abs(c) < 2)
	{
		c = c * c + x;
		++it;
	}

	return it;
}

double Mandelbrot::CalculateContinousIteration(std::complex<double> x) const
{
	return CheckConvergence(x) + 1 - (log(2) / abs(x)) / log(2);
}

std::vector<std::complex<double>> Mandelbrot::MakeComplexVector(sf::Vector2u pixelSize) const
{
	sf::Vector2<double> step(scale.x * size.x / pixelSize.x, scale.y * size.y / pixelSize.y);

	std::vector<std::complex<double>> vector(pixelSize.x * pixelSize.y);

	sf::Vector2<double> point(position + offset);

	for (size_t m = 0; m < pixelSize.y; ++m, point.x = position.x + offset.x, point.y += step.y)
	{
		for (size_t n = 0; n < pixelSize.x; ++n, point.x += step.x)
		{
			vector[m * pixelSize.x + n] = { point.x, point.y };
		}
	}

	return vector;
}

std::vector<size_t> Mandelbrot::CalculateIterations(const std::vector<std::complex<double>>& values) const
{
	std::vector<size_t> iterations(values.size());

	std::transform(std::execution::par_unseq, values.cbegin(), values.cend(), iterations.begin(), std::bind(std::mem_fn(&Mandelbrot::CheckConvergence), this, std::placeholders::_1));

	return iterations;
}

std::vector<double> Mandelbrot::CalculateContinousIterations(const std::vector<std::complex<double>>& values) const
{
	std::vector<double> continous(values.size());

	std::transform(std::execution::par_unseq, values.cbegin(), values.cend(), continous.begin(), std::bind(std::mem_fn(&Mandelbrot::CalculateContinousIteration), this, std::placeholders::_1));

	return continous;
}

std::vector<sf::Color> Mandelbrot::CalculateColors(const std::vector<size_t>& values) const
{
	std::vector<sf::Color> colors(values.size());

	std::transform(std::execution::par_unseq, values.cbegin(), values.cend(), colors.begin(), std::bind(std::mem_fn(&Mandelbrot::CalculateColor), this, std::placeholders::_1));

	return colors;
}

std::vector<sf::Color> Mandelbrot::CalculateContinousColors(const std::vector<double>& values) const
{
	std::vector<sf::Color> colors(values.size());

	std::transform(std::execution::par_unseq, values.cbegin(), values.cend(), colors.begin(), std::bind(std::mem_fn(&Mandelbrot::CalculateColor), this, std::placeholders::_1));

	return colors;
}

std::vector<sf::Uint8> Mandelbrot::CalculatePixels(const std::vector<sf::Color>& values) const
{
	std::vector<sf::Uint8> pixels(4 * values.size());
	size_t n{ 0 };

	std::for_each(std::execution::seq, values.cbegin(), values.cend(), [&n, &pixels](const sf::Color& color) {
		pixels[4 * n] = color.r;
		pixels[4 * n + 1] = color.g;
		pixels[4 * n + 2] = color.b;
		pixels[4 * n + 3] = color.a;
		++n;
		});

	return pixels;
}

sf::Color Mandelbrot::CalculateColor(double iteration) const
{
	sf::Color color;

	color.r = sf::Uint8(sin(1.57079632679 * iteration / maxIterations) * 255);
	color.g = sf::Uint8(sin(1.57079632679 * iteration / maxIterations) * 255);
	color.b = sf::Uint8(sin(1.57079632679 * iteration / maxIterations) * 255);

	return color;
}

sf::Vector2d Mandelbrot::ConvertPixelSize(sf::Vector2u pixelSize, double factor)
{
	return { pixelSize.x / factor, pixelSize.y / factor };
}

void Mandelbrot::SetPosition(sf::Vector2d position)
{
	this->position = position;
}

void Mandelbrot::SetSize(sf::Vector2d size)
{
	this->size = size;
}

void Mandelbrot::SetMaxIterations(size_t maxIterations)
{
	this->maxIterations = maxIterations;
}

void Mandelbrot::SetOffset(sf::Vector2d offset)
{
	this->offset = offset;
}

void Mandelbrot::SetScale(sf::Vector2d scale)
{
	this->scale = scale;
}

void Mandelbrot::SetImageFactor(double imageFactor)
{
	this->imageFactor = imageFactor;
}

void Mandelbrot::Move(sf::Vector2d offset)
{
	this->offset += { offset.x * scale.x, offset.y * scale.y };
}

void Mandelbrot::Zoom(double factor)
{
	offset -= 0.5 * factor * sf::Vector2d{ size.x * scale.x, size.y * scale.y };
	scale *= (1.0 + factor);
}

size_t& Mandelbrot::GetMaxIterations()
{
	return maxIterations;
}

const size_t& Mandelbrot::GetMaxIterations() const
{
	return maxIterations;
}

sf::Vector2d& Mandelbrot::GetSize()
{
	return size;
}

const sf::Vector2d& Mandelbrot::GetSize() const
{
	return size;
}

sf::Image Mandelbrot::GenerateImage() const
{
	sf::Vector2u pixelSize(unsigned(imageFactor * size.x), unsigned(imageFactor * size.y));
	std::vector<std::complex<double>> coordinates(MakeComplexVector(pixelSize));
	std::vector<size_t> iterations(CalculateIterations(coordinates));
	std::vector<sf::Color> colors(CalculateColors(iterations));
	std::vector<sf::Uint8> pixels(CalculatePixels(colors));

	sf::Image image;
	image.create(pixelSize.x, pixelSize.y, pixels.data());

	return image;
}