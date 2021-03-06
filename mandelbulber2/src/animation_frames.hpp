/**
 * Mandelbulber v2, a 3D fractal generator       ,=#MKNmMMKmmßMNWy,
 *                                             ,B" ]L,,p%%%,,,§;, "K
 * Copyright (C) 2014-16 Krzysztof Marczak     §R-==%w["'~5]m%=L.=~5N
 *                                        ,=mm=§M ]=4 yJKA"/-Nsaj  "Bw,==,,
 * This file is part of Mandelbulber.    §R.r= jw",M  Km .mM  FW ",§=ß., ,TN
 *                                     ,4R =%["w[N=7]J '"5=],""]]M,w,-; T=]M
 * Mandelbulber is free software:     §R.ß~-Q/M=,=5"v"]=Qf,'§"M= =,M.§ Rz]M"Kw
 * you can redistribute it and/or     §w "xDY.J ' -"m=====WeC=\ ""%""y=%"]"" §
 * modify it under the terms of the    "§M=M =D=4"N #"%==A%p M§ M6  R' #"=~.4M
 * GNU General Public License as        §W =, ][T"]C  §  § '§ e===~ U  !§[Z ]N
 * published by the                    4M",,Jm=,"=e~  §  §  j]]""N  BmM"py=ßM
 * Free Software Foundation,          ]§ T,M=& 'YmMMpM9MMM%=w=,,=MT]M m§;'§,
 * either version 3 of the License,    TWw [.j"5=~N[=§%=%W,T ]R,"=="Y[LFT ]N
 * or (at your option)                   TW=,-#"%=;[  =Q:["V""  ],,M.m == ]N
 * any later version.                      J§"mr"] ,=,," =="""J]= M"M"]==ß"
 *                                          §= "=C=4 §"eM "=B:m|4"]#F,§~
 * Mandelbulber is distributed in            "9w=,,]w em%wJ '"~" ,=,,ß"
 * the hope that it will be useful,                 . "K=  ,=RMMMßM"""
 * but WITHOUT ANY WARRANTY;                            .'''
 * without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * ###########################################################################
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com)
 *
 * Class to store settings for animation frames
 *
 * Handles the 2D matrix of the list of parameters / list of frames
 * and exposes functions to modify this matrix.
 */

#ifndef MANDELBULBER2_SRC_ANIMATION_FRAMES_HPP_
#define MANDELBULBER2_SRC_ANIMATION_FRAMES_HPP_

#include "parameters.hpp"
#include <QtGui>

// forward declarations
class cFractalContainer;

class cAnimationFrames
{
public:
	struct sAnimationFrame
	{
		sAnimationFrame() : alreadyRendered(false) {}

		cParameterContainer parameters;
		QImage thumbnail;
		bool alreadyRendered;
		QList<bool> alreadyRenderedSubFrames;
	};

	struct sParameterDescription
	{
		sParameterDescription(QString _parameterName, QString _containerName,
			parameterContainer::enumVarType _varType, parameterContainer::enumMorphType _morphType)
				: parameterName(_parameterName),
					containerName(_containerName),
					varType(_varType),
					morphType(_morphType)
		{
		}

		QString parameterName;
		QString containerName;
		parameterContainer::enumVarType varType;
		parameterContainer::enumMorphType morphType;
	};

	cAnimationFrames();
	virtual ~cAnimationFrames();
	void AddFrame(
		const cParameterContainer &params, const cFractalContainer &fractal, int index = -1);
	void AddFrame(const sAnimationFrame &frame);
	void ModifyFrame(int index, sAnimationFrame &frame);
	void GetFrameAndConsolidate(int index, cParameterContainer *params, cFractalContainer *fractal);
	sAnimationFrame GetFrame(int index) const;
	int GetNumberOfFrames() const;
	virtual int GetUnrenderedTillIndex(int index);
	virtual int GetUnrenderedTotal();
	void Clear();
	void ClearAll();
	virtual void AddAnimatedParameter(
		const QString &parameterName, const cOneParameter &defaultValue);
	virtual bool AddAnimatedParameter(const QString &fullParameterName,
		const cParameterContainer *param, const cFractalContainer *fractal);
	virtual void RemoveAnimatedParameter(const QString &fullParameterName);
	QList<sParameterDescription> GetListOfUsedParameters() const { return listOfParameters; }
	const cParameterContainer *ContainerSelector(QString containerName,
		const cParameterContainer *params, const cFractalContainer *fractal) const;
	cParameterContainer *ContainerSelector(
		QString containerName, cParameterContainer *params, cFractalContainer *fractal) const;
	void DeleteFrames(int begin, int end);
	void Override(QList<sAnimationFrame> _frames, QList<sParameterDescription> _listOfParameters)
	{
		frames = _frames;
		listOfParameters = _listOfParameters;
	}
	QList<sAnimationFrame> GetFrames() { return frames; }
	QList<sParameterDescription> GetListOfParameters() { return listOfParameters; }
	void SetListOfParametersAndClear(QList<sParameterDescription> _listOfParameters)
	{
		listOfParameters = _listOfParameters;
		frames.clear();
	}
	int IndexOnList(QString parameterName, QString containerName);

protected:
	QList<sAnimationFrame> frames;
	QList<sParameterDescription> listOfParameters;
};

extern cAnimationFrames *gAnimFrames;

#endif /* MANDELBULBER2_SRC_ANIMATION_FRAMES_HPP_ */
