/* -*- c++ -*- */
/*
 * Copyright 2024 a.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "calc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
  namespace customModule {

    calc_impl::calc_impl(bool processing, bool restart)
      : sync_block("calc",
                   io_signature::make(2, 2, sizeof(uint8_t)),
                   io_signature::make(3, 3, sizeof(float))),
        errors(0),
        total(0),
        processing(processing),
        restart(restart) {}

    calc_impl::~calc_impl() {}
    
    void calc_impl::set_processing(bool new_processing) {
        //std::cout << "processing set to: " << new_processing << std::endl;
        processing = new_processing;
    }

    void calc_impl::set_restart(bool new_restart) {
    	//std::cout << "restart set to: " << new_restart << std::endl;
        restart = new_restart;
        if (new_restart) {
            errors = 0;
            total = 0;
        }
    }

    int calc_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items) {
      //std::cout << "processing " << processing << ", Restart: " << restart << std::endl;
      const auto received_bit_stream = static_cast<const uint8_t*>(input_items[0]);
      const auto control_bit_stream = static_cast<const uint8_t*>(input_items[1]);
      auto num_errors = static_cast<float*>(output_items[0]);
      auto ber_value = static_cast<float*>(output_items[1]);
      auto total_bits = static_cast<float*>(output_items[2]);

      if (restart) {
          errors = 0;
          total = 0;
      }

      if (processing) {
          for (int i = 0; i < noutput_items; i++) {
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

      float my_ber = (errors > 0) ? log10(static_cast<float>(errors)*8 / total) : 0.0f;
      num_errors[0] = static_cast<float>(errors);
      ber_value[0] = my_ber;
      total_bits[0] = static_cast<float>(total)/8;

      return noutput_items;
    }

    calc::sptr calc::make(bool processing, bool restart) {
      return gnuradio::make_block_sptr<calc_impl>(processing, restart);
    }

  } /* namespace customModule */ 
} /* namespace gr */
