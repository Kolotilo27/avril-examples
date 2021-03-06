// Copyright 2010 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "avrlib/devices/rotary_encoder.h"
#include "avrlib/boot.h"
#include "avrlib/serial.h"
#include "avrlib/time.h"
#include "avrlib/output_stream.h"

using namespace avrlib;

typedef Serial<SerialPort0, 9600, DISABLED, POLLED> Debug;
typedef RotaryEncoder<
    NumberedGpio<0>, NumberedGpio<1>, NumberedGpio<2> > Encoder;

TIMER_0_TICK {
  TickSystemClock();
}

int main(void) {
  OutputStream<Debug> debug_output;
  Encoder encoder;
  int16_t value = 0;

  Boot(true);
  Debug::Init();
  encoder.Init();
  while (1) {
    int8_t delta = encoder.Read();
    value += delta;
    if (delta != 0) {
      debug_output << value << " " << int(delta) << endl;
    }
    if (encoder.clicked()) {
      debug_output << "clicked" << endl;
    }
  }
}
