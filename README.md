avr-power-meter
======

Device for monitoring DC energy usage (bidirectional) with an external shunt resistor like [this](https://www.allelectronics.com/mas_assets/cache/image/5/e/f/b/24315.Jpg).

This version is designed for monitoring 24V systems (2 voltage inputs, 1 shunt input).

It is galvanically isolated from the power supply, which allows using it in the battery stack without risks of creating ground loops.

It exposes Modbus RTU protocol over RS485 line. Modules can be daisy-chained using onboard angled pin headers.

The core of the board is `INA262` chip with `ATmega8` microcontroller.

### History

See the v1 (test) version in [[v1]](https://github.com/KrystianD/avr-power-meter/tree/v1) branch.

# Board

<a href=".docs/board/schematic.png"><img src=".docs/board/schematic.png" width="500" /></a>
<br/>
<a href=".docs/board/top.png"><img src=".docs/board/top.png" width="250" /></a>
<a href=".docs/board/bottom.png"><img src=".docs/board/bottom.png" width="250" /></a>

[More board images](.docs/board)

# Photos

<a href=".docs/photos/photo1.jpg"><img src=".docs/photos/photo1.jpg" height="250" /></a>
<a href=".docs/photos/photo2.jpg"><img src=".docs/photos/photo2.jpg" height="250" /></a>