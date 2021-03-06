/**
 \file 		psi_phasing.cpp
 \author	michael.zohner@ec-spride.de
 \copyright	ABY - A Framework for Efficient Mixed-protocol Secure Two-party Computation
			Copyright (C) 2015 Engineering Cryptographic Protocols Group, TU Darmstadt
			This program is free software: you can redistribute it and/or modify
			it under the terms of the GNU Affero General Public License as published
			by the Free Software Foundation, either version 3 of the License, or
			(at your option) any later version.
			This program is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
			GNU Affero General Public License for more details.
			You should have received a copy of the GNU Affero General Public License
			along with this program. If not, see <http://www.gnu.org/licenses/>.
 \brief		Private Set Intersection Test class implementation.
 */

//Utility libs
#include "../../abycore/util/crypto/crypto.h"
#include "../../abycore/util/parse_options.h"
//ABY Party class
#include "../../abycore/aby/abyparty.h"

#include "common/phasing_circuit.h"

int32_t read_test_options(int32_t* argcp, char*** argvp, e_role* role,
		uint32_t* bitlen, uint32_t* neles, uint32_t* secparam, string* address,
		uint16_t* port, int32_t* test_op, double* epsilon, bool* useyao) {

	uint32_t int_role = 0, int_port = 0;
	bool useffc = false;

	parsing_ctx options[] =
			{ { (void*) &int_role, T_NUM, 'r', "Role: 0/1", true, false },
			  {	(void*) neles, T_NUM, 'n',	"Number of elements", true, false },
			  {	(void*) bitlen, T_NUM, 'b', "Bit-length", true, false },
			  {	(void*) epsilon, T_DOUBLE, 'e', "Epsilon for Cuckoo hashing (default: 1.2)", false, false },
			  { (void*) secparam, T_NUM, 's', "Symmetric Security Bits, default: 128", false, false },
			  {	(void*) address, T_STR, 'a', "IP-address, default: localhost", false, false },
			  {	(void*) &int_port, T_NUM, 'p', "Port, default: 7766", false, false },
			  {	(void*) useyao, T_FLAG, 'y', "Use Yao's garbled circuits, default: false", false, false }
			};

	if (!parse_options(argcp, argvp, options,
			sizeof(options) / sizeof(parsing_ctx))) {
		print_usage(*argvp[0], options, sizeof(options) / sizeof(parsing_ctx));
		cout << "Exiting" << endl;
		exit(0);
	}

	assert(int_role < 2);
	*role = (e_role) int_role;

	if (int_port != 0) {
		assert(int_port < 1 << (sizeof(uint16_t) * 8));
		*port = (uint16_t) int_port;
	}

	assert(*epsilon >= 1);

	//delete options;

	return 1;
}

int main(int argc, char** argv) {

	e_role role;
	uint32_t bitlen = 32, neles = 31, secparam = 128, nthreads = 1;
	uint16_t port = 7766;
	string address = "127.0.0.1";
	int32_t test_op = -1;
	e_mt_gen_alg mt_alg = MT_OT;
	double epsilon = 1.2;
	bool useyao=false;

	read_test_options(&argc, &argv, &role, &bitlen, &neles, &secparam, &address,
			&port, &test_op, &epsilon, &useyao);

	seclvl seclvl = get_sec_lvl(secparam);


	if(useyao) {
		test_phasing_circuit(role, (char*) address.c_str(), seclvl, neles, bitlen,
				epsilon, nthreads, mt_alg, S_YAO);
	} else {
		test_phasing_circuit(role, (char*) address.c_str(), seclvl, neles, bitlen,
				epsilon, nthreads, mt_alg, S_BOOL);
	}



	cout << "PSI circuit successfully executed" << endl;

	return 0;
}

