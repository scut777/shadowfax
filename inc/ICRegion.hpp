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
 * @file ICRegion.hpp
 *
 * @brief Region in a BlockICGenerator setup: header
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@ugent.be)
 */
#ifndef ICREGION_H
#define ICREGION_H

#include <string>
#include <vector>

#include "StateVector.hpp"
#include "Vec.hpp"

class SymbolicFunction;

/**
 * @brief Region with particle properties used by BlockICGenerator
 *
 * A region has geometrical dimensions and stores a list with a SymbolicFunction
 * for every particle property that needs to be initialized. It forms the
 * building block of the initial conditions generated by BlockICGenerator.
 *
 * If we call \f$\vec{o} = (o_x, o_y, o_z)\f$ the origin of the region,
 * \f$\vec{l} = (l_x, l_y, l_z)\f$ its sidelength and \f$e\f$ its exponents,
 * then a point with coordinates \f$\vec{x} = (x_x, x_y, x_z)\f$ lies inside the
 * region if
 * \f[\sqrt[1/e]{\left(2\frac{x_x-o_x}{l_x}\right)^e +
 *               \left(2\frac{x_y-o_y}{l_y}\right)^e +
 *               \left(2\frac{x_z-o_z}{l_z}\right)^e} \leq 1.
 * \f]
 */
class ICRegion {
  private:
    /*! \brief Exponent \f$e\f$ of the region */
    double _exponent;

    /*! \brief Origin \f$\vec{o} = (o_x, o_y, o_z)\f$ of the region */
    Vec _origin;

    /*! \brief Sidelength \f$\vec{l} = (l_x, l_y, l_z)\f$ of the region */
    Vec _sides;

    /*! \brief SymbolicFunctions used for the hydrodynamical quantities */
    std::vector<SymbolicFunction*> _hydrofunctions;

    /*! \brief SymbolicFunction used for the dark matter density */
    std::vector<SymbolicFunction*> _dmfunction;

    /*! \brief Maximal hydrodynamical density in the region */
    double _max_value_hydro;

    /*! \brief Maximal dark matter density in the region */
    double _max_value_dm;

    /*! \brief Volume of the region */
    double _volume;

    double integrate(SymbolicFunction* function, Vec origin, Vec sides,
                     double exponent);

  public:
    ICRegion(Vec origin, Vec sides, double exponent,
             std::vector<std::string> hydrofunctions,
             std::vector<std::string> dmfunction);

    ~ICRegion();

    bool inside(Vec position);
    StateVector get_hydro(Vec position);

    double get_max_value_hydro(ICRegion* cut_out_region = NULL);
    double get_max_value_dm(ICRegion* cut_out_region = NULL);

    bool accept_hydro(Vec position);
    bool accept_dm(Vec position);
};

#endif  // ICREGION_H
