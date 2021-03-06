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

#include "avrlib/boot.h"
#include "avrlib/deprecated/devices/input_array.h"
#include "avrlib/serial.h"
#include "avrlib/time.h"
#include "avrlib/output_stream.h"

using namespace avrlib;

typedef AdcInputScanner AnalogInputs;
typedef Serial<SerialPort0, 9600, DISABLED, POLLED> Debug;
typedef InputArray<AnalogInputs, 4, 4> Pots;

PwmOutput<12> a;
PwmOutput<13> b;
PwmOutput<3> c;
PwmOutput<4> d;

TIMER_0_TICK {
  TickSystemClock();
}

int main(void) {
  uint8_t idle;
  OutputStream<Debug> debug_output;
  Pots pots;

  Boot(true);
  Debug::Init();
  pots.Init();
  a.Init();
  b.Init();
  c.Init();
  d.Init();

  Timer<1>::set_prescaler(1);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);

  idle = 0;
  debug_output << "Let's do some scanning" << endl;
  while (1) {
    Pots::Event event = pots.Read();
    if (event.event == EVENT_NONE) {
      if (event.time > 1500) {
        if (!idle) {
          debug_output << "Idle..." << endl;
        }
        idle = 1;
      }
    } else {
      idle = 0;
      debug_output << int(event.id) << ":" << event.value << endl;
      switch (event.id) {
        case 0:
          a.Write(event.value >> 2);
          break;
        case 1:
          b.Write(event.value >> 2);
          break;
        case 2:
          c.Write(event.value >> 2);
          break;
        case 3:
          d.Write(event.value >> 2);
          break;
      }
    }
    AnalogInputs::Scan();
  }
}
