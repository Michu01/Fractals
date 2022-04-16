#pragma once
#include <complex>
#include <algorithm>
#include <execution>
#include <functional>

#include <SFML/Graphics.hpp>

namespace sf
{
	using Vector2d = Vector2<double>;
}

class Mandelbrot
{
private:
	static inline const sf::Vector2<double> defaultPosition{ -2, -1.5 };
	static inline const sf::Vector2d defaultSize{ 3, 3 };

	static inline constexpr size_t defaultMaxIterations{ 50 };

private:
	sf::Vector2d position{ defaultPosition };
	sf::Vector2d size{ defaultSize };
	
	size_t maxIterations{ defaultMaxIterations };

	sf::Vector2d offset{ 0.0, 0.0 };
	sf::Vector2d scale{ 1.0, 1.0 };

	double imageFactor{ 1.0 };

private:
	size_t CheckConvergence(std::complex<double> x) const;

	double CalculateContinousIteration(std::complex<double> x) const;

	std::vector<std::complex<double>> MakeComplexVector(sf::Vector2u pixelSize) const;

	std::vector<size_t> CalculateIterations(const std::vector<std::complex<double>>& values) const;

	std::vector<double> CalculateContinousIterations(const std::vector<std::complex<double>>& values) const;

	std::vector<sf::Color> CalculateColors(const std::vector<size_t>& values) const;

	std::vector<sf::Color> CalculateContinousColors(const std::vector<double>& values) const;

	std::vector<sf::Uint8> CalculatePixels(const std::vector<sf::Color>& values) const;
	
	sf::Color CalculateColor(double iteration) const;

public:
	static sf::Vector2d ConvertPixelSize(sf::Vector2u pixelSize, double factor);

	//Setters

	void SetPosition(sf::Vector2d position);

	void SetSize(sf::Vector2d size);

	void SetMaxIterations(size_t maxIterations);

	void SetOffset(sf::Vector2d offset);

	void SetScale(sf::Vector2d scale);

	void SetImageFactor(double imageFactor);

	void Move(sf::Vector2d offset);

	void Zoom(double factor);

	//Getters

	size_t& GetMaxIterations();

	const size_t& GetMaxIterations() const;

	sf::Vector2d& GetSize();

	const sf::Vector2d& GetSize() const;

	//Generate image

	sf::Image GenerateImage() const;
};

