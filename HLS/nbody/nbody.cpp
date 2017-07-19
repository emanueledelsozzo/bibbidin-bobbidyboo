//
//  nbody.cpp
//  N-Body
//
//  Authors:
//	Emanuele Del Sozzo, Lorenzo Di Tucci, Marco Rabozzi, Marco Nanni
//

#include "string.h"
#include "math.h"
#include "utils.hpp"
#include "nbody.hpp"

void core(hls::stream<my_type_48> &stream_x, hls::stream<my_type_48> &stream_y, hls::stream<my_type_48> &stream_z,
		hls::stream<my_type_48> &stream_c, my_type x_val, my_type y_val, my_type z_val, my_type EPS,
		hls::stream<my_type> &out_x, hls::stream<my_type> &out_y, hls::stream<my_type> &out_z){

	my_type acc_x[RED_1];
	my_type acc_y[RED_1];
	my_type acc_z[RED_1];

	my_type tot_acc_x[RED_0] = {0, 0, 0, 0, 0, 0, 0, 0};
	my_type tot_acc_y[RED_0] = {0, 0, 0, 0, 0, 0, 0, 0};
	my_type tot_acc_z[RED_0] = {0, 0, 0, 0, 0, 0, 0, 0};

	central:for(int j = 0; j < N_LOCAL_TILE; j++){
	#pragma HLS PIPELINE
		my_type_48 tmp_x_val = stream_x.read();
		my_type_48 tmp_y_val = stream_y.read();
		my_type_48 tmp_z_val = stream_z.read();
		my_type_48 c_val = stream_c.read();
		unsigned int idx = j % RED_0;
	parallel:for(int k = 0; k < ELEM; k++){
			my_type rx = tmp_x_val.val[k] - x_val;
			my_type ry = tmp_y_val.val[k] - y_val;
			my_type rz = tmp_z_val.val[k] - z_val;

			my_type dd = rx*rx + ry*ry + rz*rz + EPS;
			my_type d = 1/ (dd * sqrtf(dd));

			my_type s = c_val.val[k] * d;
			acc_x[k] = rx * s;
			acc_y[k] = ry * s;
			acc_z[k] = rz * s;

		}

		tot_acc_x[idx] += MY_REDUCTION(ELEM)<my_type, RED_1>(acc_x);
		tot_acc_y[idx] += MY_REDUCTION(ELEM)<my_type, RED_1>(acc_y);
		tot_acc_z[idx] += MY_REDUCTION(ELEM)<my_type, RED_1>(acc_z);
	}

	reduction_8<my_type, RED_0>(out_x, tot_acc_x);
	reduction_8<my_type, RED_0>(out_y, tot_acc_y);
	reduction_8<my_type, RED_0>(out_z, tot_acc_z);


}


void core_start(ap_uint<512> p_x[TILE_SIZE], ap_uint<512> p_y[TILE_SIZE], ap_uint<512> p_z[TILE_SIZE], ap_uint<512> c[TILE_SIZE],
		my_type x_val, my_type y_val, my_type z_val, my_type EPS, hls::stream<my_type> &out_x, hls::stream<my_type> &out_y, hls::stream<my_type> &out_z){

#pragma HLS DATAFLOW

	hls::stream<my_type_48> stream_x;
#pragma HLS STREAM variable=stream_x depth=1 dim=1
	hls::stream<my_type_48> stream_y;
#pragma HLS STREAM variable=stream_y depth=1 dim=1
	hls::stream<my_type_48> stream_z;
#pragma HLS STREAM variable=stream_z depth=1 dim=1
	hls::stream<my_type_48> stream_c;
#pragma HLS STREAM variable=stream_c depth=1 dim=1

	Axi2StreamMod<my_type_48, ap_uint<512>, my_type, TILE_SIZE, ELEM_RATIO, LOOP_TILE, AP_ELEM>(stream_x, p_x);
	Axi2StreamMod<my_type_48, ap_uint<512>, my_type, TILE_SIZE, ELEM_RATIO, LOOP_TILE, AP_ELEM>(stream_y, p_y);
	Axi2StreamMod<my_type_48, ap_uint<512>, my_type, TILE_SIZE, ELEM_RATIO, LOOP_TILE, AP_ELEM>(stream_z, p_z);
	Axi2StreamMod<my_type_48, ap_uint<512>, my_type, TILE_SIZE, ELEM_RATIO, LOOP_TILE, AP_ELEM>(stream_c, c);

	core(stream_x, stream_y, stream_z, stream_c, x_val, y_val, z_val, EPS, out_x, out_y, out_z);

}


void nbody(ap_uint<512> *p_x, ap_uint<512> *p_y, ap_uint<512> *p_z, ap_uint<512> *a_x, ap_uint<512> *a_y, ap_uint<512> *a_z, ap_uint<512> *c, my_type EPS, unsigned int tiling_factor){
#pragma HLS INTERFACE m_axi port=p_x offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=p_y offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=p_z offset=slave bundle=gmem2
#pragma HLS INTERFACE m_axi port=a_x offset=slave bundle=gmem3
#pragma HLS INTERFACE m_axi port=a_y offset=slave bundle=gmem4
#pragma HLS INTERFACE m_axi port=a_z offset=slave bundle=gmem5
#pragma HLS INTERFACE m_axi port=c   offset=slave bundle=gmem6

#pragma HLS INTERFACE s_axilite register port=p_x bundle=control
#pragma HLS INTERFACE s_axilite register port=p_y bundle=control
#pragma HLS INTERFACE s_axilite register port=p_z bundle=control
#pragma HLS INTERFACE s_axilite register port=a_x bundle=control
#pragma HLS INTERFACE s_axilite register port=a_y bundle=control
#pragma HLS INTERFACE s_axilite register port=a_z bundle=control
#pragma HLS INTERFACE s_axilite register port=c bundle=control
#pragma HLS INTERFACE s_axilite register port=EPS bundle=control
#pragma HLS INTERFACE s_axilite register port=tiling_factor bundle=control
#pragma HLS INTERFACE s_axilite register port=return bundle=control


	ap_uint<512> outer_x[TILE_SIZE];
	ap_uint<512> outer_y[TILE_SIZE];
	ap_uint<512> outer_z[TILE_SIZE];

	ap_uint<512> inner_x[TILE_SIZE];
#pragma HLS ARRAY_PARTITION variable=inner_x cyclic factor=3 dim=1
	ap_uint<512> inner_y[TILE_SIZE];
#pragma HLS ARRAY_PARTITION variable=inner_y cyclic factor=3 dim=1
	ap_uint<512> inner_z[TILE_SIZE];
#pragma HLS ARRAY_PARTITION variable=inner_z cyclic factor=3 dim=1
	ap_uint<512> inner_c[TILE_SIZE];
#pragma HLS ARRAY_PARTITION variable=inner_c cyclic factor=3 dim=1

	ap_uint<512> local_a_x[TILE_SIZE];
	ap_uint<512> local_a_y[TILE_SIZE];
	ap_uint<512> local_a_z[TILE_SIZE];

	my_type tmp_a_x[TILE_ELEM];
	my_type tmp_a_y[TILE_ELEM];
	my_type tmp_a_z[TILE_ELEM];


	outerTile:for(int t0 = 0; t0 < tiling_factor; t0++){
#pragma HLS LOOP_FLATTEN off
		unsigned int outer_tile = t0 * TILE_SIZE;
		memcpy(outer_x, (const ap_uint<512> *)p_x + outer_tile, (TILE_SIZE) * sizeof(ap_uint<512>));
		memcpy(outer_y, (const ap_uint<512> *)p_y + outer_tile, (TILE_SIZE) * sizeof(ap_uint<512>));
		memcpy(outer_z, (const ap_uint<512> *)p_z + outer_tile, (TILE_SIZE) * sizeof(ap_uint<512>));

		myMemset<ap_uint<512>, TILE_SIZE>(local_a_x, 0);
		myMemset<ap_uint<512>, TILE_SIZE>(local_a_y, 0);
		myMemset<ap_uint<512>, TILE_SIZE>(local_a_z, 0);

		innerTile:for(int t1 = 0; t1 < tiling_factor; t1++){
#pragma HLS LOOP_FLATTEN off
			unsigned int inner_tile = t1 * TILE_SIZE;
			memcpy(inner_x, (const ap_uint<512> *)p_x + inner_tile, (TILE_SIZE) * sizeof(ap_uint<512>));
			memcpy(inner_y, (const ap_uint<512> *)p_y + inner_tile, (TILE_SIZE) * sizeof(ap_uint<512>));
			memcpy(inner_z, (const ap_uint<512> *)p_z + inner_tile, (TILE_SIZE) * sizeof(ap_uint<512>));
			memcpy(inner_c, (const ap_uint<512> *)c + inner_tile, (TILE_SIZE) * sizeof(ap_uint<512>));

			computation:for(int i = 0; i < FOR_INDEX_TILE; i++){
				unsigned int i_index = i/AP_ELEM;
				unsigned int e_index = i%AP_ELEM;
				unsigned int lower_range = 32*e_index;
				unsigned int upper_range = 32*(e_index + 1)-1;
				unsigned int x_val_tmp = outer_x[i_index].range(upper_range, lower_range);
				my_type x_val = *((my_type *)&x_val_tmp);
				unsigned int y_val_tmp = outer_y[i_index].range(upper_range, lower_range);
				my_type y_val = *((my_type *)&y_val_tmp);
				unsigned int z_val_tmp = outer_z[i_index].range(upper_range, lower_range);
				my_type z_val = *((my_type *)&z_val_tmp);

				unsigned int acc_x_tmp = local_a_x[i_index].range(upper_range, lower_range);
				my_type acc_x_val = *((my_type *)&acc_x_tmp);
				unsigned int acc_y_tmp = local_a_y[i_index].range(upper_range, lower_range);
				my_type acc_y_val = *((my_type *)&acc_y_tmp);
				unsigned int acc_z_tmp = local_a_z[i_index].range(upper_range, lower_range);
				my_type acc_z_val = *((my_type *)&acc_z_tmp);

				hls::stream<my_type> out_x;
		#pragma HLS STREAM variable=out_x depth=1 dim=1
				hls::stream<my_type> out_y;
		#pragma HLS STREAM variable=out_y depth=1 dim=1
				hls::stream<my_type> out_z;
		#pragma HLS STREAM variable=out_z depth=1 dim=1

				core_start(inner_x, inner_y, inner_z, inner_c, x_val, y_val, z_val, EPS, out_x, out_y, out_z);

				my_type x_out_tmp = out_x.read();
				my_type y_out_tmp = out_y.read();
				my_type z_out_tmp = out_z.read();

				my_type x_out_val = x_out_tmp + acc_x_val;
				my_type y_out_val = y_out_tmp + acc_y_val;
				my_type z_out_val = z_out_tmp + acc_z_val;

				local_a_x[i_index].range(upper_range, lower_range) = *((unsigned int *)&x_out_val);
				local_a_y[i_index].range(upper_range, lower_range) = *((unsigned int *)&y_out_val);
				local_a_z[i_index].range(upper_range, lower_range) = *((unsigned int *)&z_out_val);

			}
		}
		memcpy(a_x + outer_tile, (const ap_uint<512> *)local_a_x, (TILE_SIZE) * sizeof(ap_uint<512>));
		memcpy(a_y + outer_tile, (const ap_uint<512> *)local_a_y, (TILE_SIZE) * sizeof(ap_uint<512>));
		memcpy(a_z + outer_tile, (const ap_uint<512> *)local_a_z, (TILE_SIZE) * sizeof(ap_uint<512>));

	}
}

