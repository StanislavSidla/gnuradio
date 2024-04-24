/* -*- c++ -*- */
/*
 * Copyright 2024 a.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_CUSTOMMODULE_CALC_IMPL_H
#define INCLUDED_CUSTOMMODULE_CALC_IMPL_H

#include <gnuradio/customModule/calc.h>

namespace gr {
namespace customModule {

class calc_impl : public calc
{
private:
    int errors;
    int total;
    bool processing;
    bool restart;

public:
    calc_impl(bool processing = false, bool restart = false);
    ~calc_impl();

    void set_processing(bool new_processing);
    void set_restart(bool new_restart);
    
    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace customModule
} // namespace gr

#endif /* INCLUDED_CUSTOMMODULE_CALC_IMPL_H */
