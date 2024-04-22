/* -*- c++ -*- */
/*
 * Copyright 2024 a.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "calc_impl.h"
#include <gnuradio/io_signature.h>
#include <cmath>

namespace gr {
  namespace customModule {

    calc_impl::calc_impl(bool processing_set, bool restart_set)
    : sync_block("calc",
                 io_signature::make(2, 2, sizeof(uint8_t)),
                 io_signature::make(3, 3, sizeof(float))),
      errors(0),
      total(0),
      processing(processing_set),
      restart(restart_set) {}

calc_impl::~calc_impl() {}

// Setters for the 'processing' and 'restart' parameters
void calc_impl::set_processing(bool new_processing) {
    processing = new_processing;
}

void calc_impl::set_restart(bool new_restart) {
    restart = new_restart;
    if (restart) {
        errors = 0;
        total = 0;
        restart = false; // reset the flag
    }
}


int calc_impl::work(int noutput_items,
                    gr_vector_const_void_star& input_items,
                    gr_vector_void_star& output_items)
{
    // Retrieve input pointers
    const auto received_bit_stream = static_cast<const uint8_t*>(input_items[0]);
    const auto control_bit_stream = static_cast<const uint8_t*>(input_items[1]);

    // Retrieve output pointers
    auto num_errors = static_cast<float*>(output_items[0]);
    auto ber_value = static_cast<float*>(output_items[1]);
    auto total_bits = static_cast<float*>(output_items[2]);

    // Reset errors and total if restart flag is set
    if (restart) {
        errors = 0;
        total = 0;
        restart = false; // You might want to add this line to reset the flag
    }

    if (processing) {
        for (int i = 0; i < noutput_items; i++) {
            // Assuming 8 bits per byte for each item
            for (int bit = 0; bit < 8; ++bit) {
                bool received_bit = (received_bit_stream[i] >> bit) & 1;
                bool control_bit = (control_bit_stream[i] >> bit) & 1;
                if (received_bit != control_bit) {
                    errors++;
                }
                total++;
            }
        }
    }

    // Calculate BER, avoid log if errors are zero
    float my_ber = (errors > 0) ? log10(static_cast<float>(errors) / total*8) : 0.0f;

    // Assign values to output ports
    num_errors[0] = static_cast<float>(errors);
    ber_value[0] = my_ber;
    total_bits[0] = static_cast<float>(total)/8;

    // Tell the runtime system how many output items were produced
    return noutput_items;
}
    calc::sptr calc::make(bool processing, bool restart) {
    return gnuradio::make_block_sptr<calc_impl>(processing, restart);
}

  } /* namespace customModule */ 
} /* namespace gr */
