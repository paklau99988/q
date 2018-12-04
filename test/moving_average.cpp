/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <q/literals.hpp>
#include <q/synth.hpp>
#include <q_io/audio_file.hpp>
#include <q/notes.hpp>
#include <q/sfx.hpp>
#include <vector>

namespace q = cycfi::q;
using namespace q::literals;
using namespace q::notes;

constexpr auto sps = 48000;

int main()
{
   ////////////////////////////////////////////////////////////////////////////
   // Synthesize a 1-second square wave, then apply 3 successive moving-
   // average filters over the waveform. Print the result to a wav file
   // for inspection.

   constexpr auto size = sps * 1;
   constexpr auto n_channels = 4;
   std::vector<float> out(size * n_channels);      // The output buffer

   constexpr auto incr = q::phase(440_Hz, sps);    // Phase increment
   auto ph = q::phase();                           // Phase accumulator
   auto square = q::basic_square;                  // Square synth

   // Moving average filters
   constexpr auto n = 16;
   auto ma1 = q::moving_average{ n };
   auto ma2 = q::moving_average{ n };
   auto ma3 = q::moving_average{ n };

   for (auto i = 0; i != size; ++i, ph += incr)
   {
      auto pos = i * n_channels;
      auto ch1 = pos;
      auto ch2 = pos+1;
      auto ch3 = pos+2;
      auto ch4 = pos+3;

      out[ch1] = square(ph);
      out[ch2] = ma1(out[ch1]);
      out[ch3] = ma2(out[ch2]);
      out[ch4] = ma3(out[ch3]);
   }

   ////////////////////////////////////////////////////////////////////////////
   // Write to a wav file

   auto wav = q::wav_writer{
      "results/moving_average.wav", n_channels, sps // mono, 48000 sps
   };
   wav.write(out);

   return 0;
}