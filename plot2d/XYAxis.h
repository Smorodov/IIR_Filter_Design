#pragma once

#include "XYDrawingBackend.h"
#include "XYRect.h"
#include <vector>
#include <string>
#include <cmath>


class Axis
{
public:
	Axis();
	~Axis();

	float minValue = 0.0f;
	float maxValue = 0.0f;
	float step = 0.0f;
	int nSteps = 0;
	std::vector<float> values;
	float canvasMinValue = 0.0f;
	float canvasMaxValue = 0.0f;
	float canvasStep = 0.0f;
	std::vector<float> canvasValues;
	std::vector <std::string> strValues;
	std::vector <XYRect> canvasValueStringBoxes;
	float maxValStrW = 0;
	float maxValStrH = 0;

	// ----------------------------------
	// функция вычисления параметров оси.
	// --------------
	// Вход функцмм:
	// --------------
	// start : Float - минимальное значение координат по этой оси
	// end: Float - максимальное значение координат по этой оси
	// n_steps : Int - примерное желаемое значене количестап отсчетов по этой оси
	// может быть изменено после расчетов.
	// canvasStart: Float - начало оси на холсте
	// canvasEnd: Float- конец оси на холсте
	// --------------
	// Выход функцмм:
	// --------------
	// см. класс Axis
	// ----------------------------------
	void computeAxis(
		XYDrawingBackend* ctx,
		float startIn,
		float endIn,
		int n_steps,
		float canvasStart,
		float canvasEnd,
		// это для вычисления размера строк
		int e, // экспонента (-3 -> 1e-3 -> ##.### 3->1e3 -> ###000)		
		std::string textlsFontName,
		float textFontHeight
	);
};