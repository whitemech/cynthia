/*
 * This file is part of Cynthia.
 *
 * Cynthia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cynthia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cynthia.  If not, see <https://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file
#include <catch.hpp>

#include <cynthia/sdd.hpp>

namespace cynthia {
namespace sdd {
namespace Test {
TEST_CASE("SDD Installation", "[example]") {
    // set up vtree and manager
    SddLiteral var_count = 4;
    SddLiteral var_order[4] = {2, 1, 4, 3};
    const char* type = "balanced";
    Vtree* vtree = sdd_vtree_new_with_var_order(var_count, var_order, type);
    SddManager* manager = sdd_manager_new(vtree);
    // construct a formula (A^B)v(B^C)v(C^D)
    printf("constructing SDD ... ");
    SddNode* f_a = sdd_manager_literal(1, manager);
    SddNode* f_b = sdd_manager_literal(2, manager);
    SddNode* f_c = sdd_manager_literal(3, manager);
    SddNode* f_d = sdd_manager_literal(4, manager);
    SddNode* alpha = sdd_manager_false(manager);
    SddNode* beta;
    beta = sdd_conjoin(f_a, f_b, manager);
    alpha = sdd_disjoin(alpha, beta, manager);
    beta = sdd_conjoin(f_b, f_c, manager);
    alpha = sdd_disjoin(alpha, beta, manager);
    beta = sdd_conjoin(f_c, f_d, manager);
    alpha = sdd_disjoin(alpha, beta, manager);
    printf("done\n");
    printf("saving sdd and vtree ... ");
    sdd_save_as_dot("sdd.dot", alpha);
    sdd_vtree_save_as_dot("vtree.dot", vtree);
    printf("done\n");
    sdd_vtree_free(vtree);
    sdd_manager_free(manager);

}
} // namespace Test
} // namespace sdd
} // namespace cynthia