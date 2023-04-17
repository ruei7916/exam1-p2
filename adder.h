#ifndef _ADDER_H_
#define _ADDER_H_

#include <iostream>
using namespace std;

#define SC_INCLUDE_FX
#include <systemc>
using namespace sc_dt;
using namespace sc_core;



class filter : public sc_module {
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;

  sc_fifo_in<sc_ufixed_fast<20,1> > i_a_port;
  sc_fifo_out<sc_ufixed_fast<20,1> > o_sum_port;

  void do_filter() {
    while (true) {
        buffer[1] = i_a_port.read();
        //cout<<buffer[1].to_dec();
        buffer[2] = i_a_port.read();
        _o_sum = buffer[0]*(h[0]) + buffer[1]*(h[1]) + buffer[2]*(h[2]);
        //cout<<_o_sum.to_dec();
        o_sum_port.write(_o_sum);
        buffer[0] = buffer[2];
        wait();
    }
  }

  SC_HAS_PROCESS(filter);
  filter(sc_module_name name, int i) : id(i), index(0){ 
    buffer[0] = 0.0;
    h[0]=1/6.0;
    h[1]=1/3.0;
    h[2]=1/2.0;
    SC_THREAD(do_filter);  
    sensitive << i_clk.pos();
    dont_initialize();
    //reset_signal_is(i_rst, false); 
  }

  sc_ufixed_fast<20,1> i_a() { return _i_a; }

  sc_ufixed_fast<20,1> o_sum() { return _o_sum; }

private:
  const int id;
  int index;
  sc_ufixed_fast<20,1> h[3];
  sc_ufixed_fast<20,1> buffer[3];
  sc_ufixed_fast<20,1> _i_a;
  sc_ufixed_fast<20,1> _o_sum;

 
};

#endif
