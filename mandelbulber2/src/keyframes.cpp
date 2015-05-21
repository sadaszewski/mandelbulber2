/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * Class to store settings for animation keyframes
 *
 * Copyright (C) 2014 Krzysztof Marczak
 *
 * This file is part of Mandelbulber.
 *
 * Mandelbulber is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Mandelbulber is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com), Sebastian Jennen (jenzebas@gmail.com)
 */

#include "global_data.hpp"
#include "keyframes.hpp"
#include "animation_frames.hpp"

cKeyframes::cKeyframes() : cAnimationFrames()
{
	framesPerKeyframe = 5;
}

cKeyframes::~cKeyframes()
{
	qDeleteAll(morph);
	morph.clear();
}

cAnimationFrames::sAnimationFrame cKeyframes::GetInterpolatedFrame(int index)
{
	int keyframe = index / framesPerKeyframe;
	int subindex = index % framesPerKeyframe;

	if(subindex == 0)
	{
		// no need to interpolate
		return GetFrame(keyframe);
	}

	sAnimationFrame interpolated;
	QList<QString> parameterList = frames.at(keyframe).parameters.GetListOfParameters();
	for (int i = 0; i < parameterList.size(); i++)
	{
		// prepare interpolator
		if(morph.size() <= i)
		{
			morph.append(new cMorph());
		}
		for(int k = fmax(0, keyframe - 2); k <= fmin(frames.size() - 1, keyframe + 3); k++)
		{
			if(morph[i]->findInMorph(k) == -1)
			{
				morph[i]->AddData(k, frames.at(k).parameters.GetAsOneParameter(parameterList.at(i)));
			}
		}
		// interpolate each parameter and write back
		interpolated.parameters.AddParamFromOneParameter(parameterList.at(i), morph[i]->Interpolate(keyframe, 1.0 * subindex / framesPerKeyframe));
	}
	return interpolated;
}

void cKeyframes::GetInterpolatedFrameAndConsolidate(int index, cParameterContainer *params, cFractalContainer *fractal)
{
	if(index >= 0 && index < frames.count() * framesPerKeyframe)
	{
		sAnimationFrame frame = GetInterpolatedFrame(index);

		for(int i=0; i < listOfParameters.size(); ++i)
		{
			cParameterContainer *container = ContainerSelector(listOfParameters[i].containerName, params, fractal);
			QString parameterName = listOfParameters[i].parameterName;
			cOneParameter oneParameter = frame.parameters.GetAsOneParameter(listOfParameters[i].containerName + "_" + parameterName);
			container->SetFromOneParameter(parameterName, oneParameter);
		}
	}
	else
	{
		qWarning() << "cAnimationFrames::GetInterpolatedFrameAndConsolidate(int index): wrong index" << index;
	}
}

int cKeyframes::GetUnrenderedTotal()
{
		return GetUnrenderedTillIndex(frames.count() * GetFramesPerKeyframe() - 1);
}

int cKeyframes::GetUnrenderedTillIndex(int index)
{
	if(index >= 0 && index < frames.count() * GetFramesPerKeyframe())
	{
		int keyframe = index / GetFramesPerKeyframe();
		int subindex = index % GetFramesPerKeyframe();
		int count = 0;
		for(int index = 0; index < keyframe; ++index)
		{
			cAnimationFrames::sAnimationFrame frame = GetFrame(index);
			int subindexMax = (index == keyframe - 1 ? subindex : GetFramesPerKeyframe());
			for(int subindex = 0; subindex < subindexMax; subindex++)
			{
				QString filename = GetKeyframeFilename(index, subindex);
				if(frame.alreadyRenderedSubFrames[subindex]) count++;
			}
			ModifyFrame(index, frame);
		}
		return count;
	}
	else
	{
		qWarning() << "cAnimationFrames::GetUnrenderedTillIndex(int index): wrong index" << index;
		return 0;
	}
}

QString cKeyframes::GetKeyframeFilename(int index, int subindex)
{
	int frameIndex = index * GetFramesPerKeyframe() + subindex;
	return gPar->Get<QString>("anim_flight_dir") + "frame_" + QString("%1").arg(frameIndex, 5, 10, QChar('0')) + QString(".jpg");
}
