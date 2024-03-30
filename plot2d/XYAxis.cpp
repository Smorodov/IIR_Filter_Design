#pragma once

#include "blend2d.h"
#include "XYRect.h"
#include "XYAxis.h"
#include <vector>
#include <string>
#include <cmath>
Axis::Axis()
{

}
Axis::~Axis()
{

}
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
	void Axis::computeAxis(
		XYDrawingBackend* ctx,
		float startIn,
		float endIn,
		int n_steps,
		float canvasStart,
		float canvasEnd,
		// это для вычисления размера строк
		int e, // экспонента (-3 -> 1e-3 -> ##.### 3->1e3 -> ###000)		
		std::string textlsFontName,		
		float textHeight
	) {
		minValue = 0.0f;
		maxValue = 0.0f;
		step = 0.0f;
		nSteps = 0;
		values.clear();
		canvasMinValue = 0.0f;
		canvasMaxValue = 0.0f;
		canvasStep = 0.0f;
		canvasValues.clear();
		strValues.clear();
		canvasValueStringBoxes.clear();
		maxValStrW = 0;
		maxValStrH = 0;

		float multiplier = pow(10.0f, -e);
		float start = startIn * multiplier;
		float end = endIn * multiplier;
		// если начало и конец с одной стороны от нуля
		if (start * end >= 0)
		{
			float finalStart = floor(start);
			float finalEnd = ceil(end);
			float range = finalEnd - finalStart;
			float initialStep = range / float(n_steps);
			float finalStep = round(initialStep);
			float finalNSteps = floor(range / finalStep);
			if (finalStart+finalNSteps * finalStep < end)
			{
				finalNSteps += 1.0f;
			}
			float finalRange = finalNSteps * finalStep;
			step = finalStep;
			minValue = finalStart;
			maxValue = finalStart + finalRange;
			nSteps = int(finalNSteps);
		}
		else
			// если нуль между началом и концом
		{
			// посчитаем общий шаг
			float finalStart = floor(start);
			float finalEnd = ceil(end);
			float range = finalEnd - finalStart;
			float initialStep = range / float(n_steps-1);
			float finalStep = round(initialStep);
			// посчитаем параметры для положительной полуоси
			float finalNStepsPos = floor(finalEnd / finalStep);
			if (finalStart+finalNStepsPos * finalStep < end)
			{
				finalNStepsPos += 1.0f;
			}
			float finalRangePos = finalNStepsPos * finalStep;
			// посчитаем параметры для отрицательной полуоси
			float finalNStepsNeg = floor(-finalStart / finalStep);
			if (-finalNStepsNeg * finalStep > start)
			{
				finalNStepsNeg += 1.0f;
			}
			float finalRangeNeg = finalNStepsNeg * -finalStep;

			step = finalStep;
			minValue = finalRangeNeg;
			maxValue = finalRangePos;
			nSteps = int(finalNStepsPos) + int(finalNStepsNeg);
		}

		step /= multiplier;
		minValue /= multiplier;
		maxValue /= multiplier;

		float scale = (canvasEnd - canvasStart) / (maxValue - minValue);
		float offset = canvasStart - minValue * scale;
		canvasStep = step * scale + offset;
		canvasMaxValue = canvasEnd;
		canvasMinValue = canvasStart;
		
		for (int i = 0; i <= nSteps; ++i)
		{
			values.push_back(minValue + step * float(i));
			canvasValues.push_back((minValue + step * float(i)) * scale + offset);

			float v = values[i];
			char fmt[100] = { 0 };
			if (e < 0)
			{
				sprintf(fmt, "%s%df", "%.", -e);
			}
			else
			{
				sprintf(fmt, "%s%s", "%", "d");
			}
			char str[100] = { 0 };
			std::sprintf(str, fmt, v);
			strValues.push_back(str);

			// gb.setUtf8Text(str, strlen(str));
			// font.shape(gb);
			// font.getTextMetrics(gb, tm);
			float textWidth;
			float dummy;
//			float textHeight;
			ctx->getTextSize(textlsFontName, textHeight, str, textWidth, dummy);
			if (maxValStrW < textWidth)
			{
				maxValStrW = textWidth;
			}
			if (maxValStrH < textHeight)
			{
				maxValStrH = textHeight;
			}
			canvasValueStringBoxes.push_back(XYRect(0,0,textHeight,textWidth));
		}
	}
