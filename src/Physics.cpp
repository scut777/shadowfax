/*******************************************************************************
 * This file is part of Shadowfax
 * Copyright (C) 2015 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * Shadowfax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shadowfax is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Shadowfax. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file Physics.cpp
 *
 * @brief Physical constants used in the simulation: implementation
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@ugent.be)
 */
#include "Physics.hpp"
#include "ParameterFile.hpp"
#include "io/PhysicalConstant.hpp"  // for PhysicalConstant
#include "io/Unit.hpp"              // for Unit
#include "io/UnitDefinitions.hpp"
#include <string>  // for string
using namespace std;

/**
 * @brief Constructor
 *
 * Initialize the physical constants in SI-units and store their values in
 * simulation units
 *
 * @param units UnitSet used internally during the simulation
 * @param mean_mol_weight Mean molecular weight
 * @param real_units Flag indicating whether physical values or idealized values
 * should be used for the physical constants
 */
Physics::Physics(UnitSet& units, double mean_mol_weight, bool real_units) {
    Unit Gunit("length*length*length/mass/time/time", "Gunit", 1.);
    double Gval;

    if(real_units) {
        // hard coded values in SI-units
        // gravitational constant
        Gval = 6.674e-11;
    } else {
        // hard coded idealized values (1 in most cases)
        // gravitational constant
        Gval = 1.;
    }
    PhysicalConstant G(Gval, Gunit);
    _G = G.get_value(units);

    Unit HubbleUnit("1/time", "HubbleUnit", 1.);
    PhysicalConstant H(3.24077929e-18, HubbleUnit);
    _H0 = H.get_value(units);

    // convert mean_mol_weight to amu
    Unit amu_unit = UnitDefinitions::get_unit("amu");
    UnitConverter mmw_converter(units.get_mass_unit(), amu_unit);
    _mean_mol_weight = mmw_converter.convert(mean_mol_weight);

    double kval = 1.38064852e-23;
    Unit kunit = Unit("energy", "J", 1.) / Unit("temperature", "K", 1.);
    PhysicalConstant k(kval, kunit);
    _boltzmann_k = k.get_value(units);
}

/**
 * @brief ParameterFile constructor
 *
 * @param units Internal units
 * @param parameters ParameterFile containing the desired parameter values
 */
Physics::Physics(UnitSet& units, ParameterFile* parameters)
        : Physics(units, parameters->check_parameter("Physics.RealPhysics"),
                  parameters->get_quantity("Physics.MeanMolWeight", "mass",
                                           PHYSICS_DEFAULT_MEANMOLWEIGHT)) {}

/**
 * @brief Get the value of the gravitational constant in simulation units
 *
 * @return The value of the gravitational constant in simulation units
 */
double Physics::get_gravitational_constant() {
    return _G;
}

/**
 * @brief Get the value of the Hubble constant in the given units, assuming a
 * value of 100 km/s/Mpc
 *
 * @return 100 km/s/Mpc in internal units
 */
double Physics::get_Hubble_constant() {
    return _H0;
}

/**
 * @brief Get the mean molecular weight
 *
 * @return Mean molecular weight
 */
double Physics::get_mean_mol_weight() {
    return _mean_mol_weight;
}

/**
 * @brief Get the Boltzmann constant k.
 *
 * @return Boltzmann constant k.
 */
double Physics::get_boltzmann_k() {
    return _boltzmann_k;
}

/**
 * @brief Dump the object to the given RestartFile
 *
 * @param rfile RestartFile to write to
 */
void Physics::dump(RestartFile& rfile) {
    rfile.write(_G);
    rfile.write(_H0);
    rfile.write(_mean_mol_weight);
    rfile.write(_boltzmann_k);
}

/**
 * @brief Restart constructor
 *
 * @param rfile RestartFile to read from
 */
Physics::Physics(RestartFile& rfile) {
    rfile.read(_G);
    rfile.read(_H0);
    rfile.read(_mean_mol_weight);
    rfile.read(_boltzmann_k);
}
