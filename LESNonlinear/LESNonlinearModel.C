/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "LESNonlinearModel.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(LESNonlinearModel, 0);
defineRunTimeSelectionTable(LESNonlinearModel, dictionary);
addToRunTimeSelectionTable(turbulenceModel, LESNonlinearModel, turbulenceModel);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void LESNonlinearModel::printCoeffs()
{
    if (printCoeffs_)
    {
        Info<< type() << "Coeffs" << coeffDict_ << endl;
    }
}


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

LESNonlinearModel::LESNonlinearModel
(
    const word& type,
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& transport,
    const word& turbulenceModelName
)
:
    turbulenceModel(U, phi, transport, turbulenceModelName),

    IOdictionary
    (
        IOobject
        (
            "LESNLProperties",
            U.time().constant(),
            U.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),

    printCoeffs_(lookupOrDefault<Switch>("printCoeffs", false)),
    coeffDict_(subOrEmptyDict(type + "Coeffs")),

    kMin_("kMin", sqr(dimVelocity), SMALL),
    delta_(LESdelta::New("delta", U.mesh(), *this))
{
    kMin_.readIfPresent(*this);

    // Force the construction of the mesh deltaCoeffs which may be needed
    // for the construction of the derived models and BCs
    mesh_.deltaCoeffs();
}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

autoPtr<LESNonlinearModel> LESNonlinearModel::New
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& transport,
    const word& turbulenceModelName
)
{
    //For some reason, this was missing and somehow was meant to be
    //automagically called from elsewhere
    constructdictionaryConstructorTables();

    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    const word modelType
    (
        IOdictionary
        (
            IOobject
            (
                "LESNLProperties",
                U.time().constant(),
                U.db(),
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("LESNonlinearModel")
    );

    Info<< "Selecting LES turbulence model " << modelType << endl;

    Info << dictionaryConstructorTablePtr_ << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "LESNonlinearModel::New"
            "("
                "const volVectorField&, "
                "const surfaceScalarField& ,"
                "transportModel&, "
                "const word&"
            ")"
        )   << "Unknown LESNonlinearModel type "
            << modelType << nl << nl
            << "Valid LESNonlinearModel types:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<LESNonlinearModel>
    (
        cstrIter()(U, phi, transport, turbulenceModelName)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void LESNonlinearModel::correct(const tmp<volTensorField>&)
{
    turbulenceModel::correct();
    delta_().correct();
}


void LESNonlinearModel::correct()
{
    correct(fvc::grad(U_));
}


bool LESNonlinearModel::read()
{
    //if (regIOobject::read())

    // Bit of trickery : we are both IOdictionary ('RASProperties') and
    // an regIOobject from the turbulenceModel level. Problem is to distinguish
    // between the two - we only want to reread the IOdictionary.

    bool ok = IOdictionary::readData
    (
        IOdictionary::readStream
        (
            IOdictionary::type()
        )
    );
    IOdictionary::close();

    if (ok)
    {
        if (const dictionary* dictPtr = subDictPtr(type() + "Coeffs"))
        {
            coeffDict_ <<= *dictPtr;
        }

        delta_().read(*this);

        kMin_.readIfPresent(*this);

        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace incompressible
} // End namespace Foam

// ************************************************************************* //
