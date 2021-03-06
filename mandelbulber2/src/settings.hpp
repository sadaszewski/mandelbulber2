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
 * cSettings class - loading / saving settings
 *
 * cSettings can transpose program internal settings to settings string and vice versa.
 * It has also methods to [load / save] [from / to] [clipboard / string / file]
 */

#ifndef MANDELBULBER2_SRC_SETTINGS_HPP_
#define MANDELBULBER2_SRC_SETTINGS_HPP_

#include <QtCore>

// forward declarations
class cParameterContainer;
class cFractalContainer;
class cAnimationFrames;
class cKeyframes;

class cSettings
{
public:
	enum enumFormat
	{
		formatFullText,
		formatCondensedText,
		formatAppSettings,
		formatNetRender
	};

	cSettings(enumFormat _format);
	size_t CreateText(const cParameterContainer *par, const cFractalContainer *fractPar,
		cAnimationFrames *frames = NULL, cKeyframes *keyframes = NULL);
	bool SaveToFile(QString filename);
	void SaveToClipboard();
	bool LoadFromFile(QString filename);
	bool LoadFromString(const QString &_settingsText);
	bool LoadFromClipboard();
	bool Decode(cParameterContainer *par, cFractalContainer *fractPar,
		cAnimationFrames *frames = NULL, cKeyframes *keyframes = NULL);
	QString GetHashCode() { return hash.toHex(); }
	void BeQuiet(bool _quiet) { quiet = _quiet; }
	QString GetSettingsText();

private:
	QString CreateHeader(void);
	void DecodeHeader(QStringList &separatedText);
	QString CreateOneLine(const cParameterContainer *par, QString name);
	bool DecodeOneLine(cParameterContainer *par, QString line);
	bool CheckSection(QString text, QString &section);
	void Compatibility(QString &name, QString &value);
	void Compatibility2(cParameterContainer *par, cFractalContainer *fract);
	void CreateAnimationString(
		QString &text, const QString &headerText, const cAnimationFrames *frames);

	bool DecodeFramesHeader(
		QString line, cParameterContainer *par, cFractalContainer *fractPar, cAnimationFrames *frames);
	bool DecodeFramesLine(
		QString line, cParameterContainer *par, cFractalContainer *fractPar, cAnimationFrames *frames);

	QString everyLocaleDouble(QString txt);

	bool CheckIfMaterialsAreDefined(cParameterContainer *par);

	enumFormat format;
	QString settingsText;

	bool textPrepared;
	bool quiet;
	double appVersion;
	double fileVersion;
	QByteArray hash;
	int csvNoOfColumns;
	QStringList listOfLoadedPrimitives;
};

#endif /* MANDELBULBER2_SRC_SETTINGS_HPP_ */
