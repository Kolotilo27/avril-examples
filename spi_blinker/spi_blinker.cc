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
#include "avrlib/gpio.h"
#include "avrlib/spi.h"

using namespace avrlib;

SpiMaster<NumberedGpio<10>, MSB_FIRST, 16> spi_interface;

int main(void) {
  Boot(false);
  
  spi_interface.Init();
  uint8_t counter = 0;
  uint8_t sequence[] = { 1, 2, 3, 2, 1, 2, 1, 2 };

  while (1) {
    spi_interface.Write(sequence[counter++ & 0x7]);
    ConstantDelay(200);
    spi_interface.Write(0x00);
    ConstantDelay(200);
  }
}
