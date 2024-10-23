/*
* This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
* Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "result_component.h"
#include "utils/random_var.h"

/**
 * Storing Generic 1D Random variable (Sampling Configuration)
 *
 */
class RandomVar1D : public ResultComponent {

public:
    explicit RandomVar1D(int size) : ResultComponent("Random Var 1D"), R(size, 1) {
        m_size = size;
    }

    RandomVar GetRV(){
        return R;
    }

    void SetRV(RandomVar &r) {
        R = r;
    }

private:
    int m_size;
    RandomVar R;
};

