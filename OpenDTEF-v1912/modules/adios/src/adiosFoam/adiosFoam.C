/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "adiosFoam.H"
#include "OSspecific.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const Foam::word Foam::adiosFoam::defaultDirectory("adiosData");
const Foam::string Foam::adiosFoam::foamAttribute("/openfoam");
const Foam::string Foam::adiosFoam::timeAttribute("/time");


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

Foam::instantList Foam::adiosFoam::findTimes
(
    const fileName& directory,
    const word& constantName
)
{
    // Read directory entries into a list
    fileNameList dirEntries(readDir(directory, fileName::FILE, false));

    // Initialise instant list
    instantList Times(dirEntries.size() + 1);
    label nTimes = 0;

    // Check for "constant" - not yet useful
    bool haveConstant = false;
    for (const fileName& dirEntry : dirEntries)
    {
        if (dirEntry.hasExt("bp") && dirEntry.lessExt() == constantName)
        {
            Times[nTimes].value() = 0;
            Times[nTimes].name()  = word(dirEntry);
            ++nTimes;
            haveConstant = true;
            break;
        }
    }

    // Read and parse all the entries in the directory
    for (const fileName& dirEntry : dirEntries)
    {
        if (dirEntry.hasExt("bp"))
        {
            scalar val;
            const word dirValue(dirEntry.lessExt());

            if (readScalar(dirValue, val))
            {
                Times[nTimes].value() = val;
                Times[nTimes].name()  = word(dirEntry);
                ++nTimes;
            }
        }
    }

    // Reset the length of the times list
    Times.setSize(nTimes);

    if (haveConstant)
    {
        if (nTimes > 2)
        {
            std::sort(&Times[1], Times.end(), instant::less());
        }
    }
    else if (nTimes > 1)
    {
        std::sort(&Times[0], Times.end(), instant::less());
    }

    return Times;
}


// ************************************************************************* //
