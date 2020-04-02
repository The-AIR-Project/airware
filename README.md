# AIRware
Software for the AIR project

#### External Libraries

LiquidCrystal_I2C (https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/blob/master/LiquidCrystal_I2C.h)

#### Volume Output Characterization

- Record volume outputs at various servo angles
- In `characterization.py`, place these values into `raw_vol` and `raw_ang`
- Take the average volume output at each servo angle, place these values into
`vol` and `ang`
- If any validation takes place, place these volumes and servo angles into
`val_vol` and `val_ang`
- Note: the `ang_to_vol` function was determined by fitting a cubic to the
volume output vs servo angle data in Excel

#### Blockage Detection Characterization

- Blockage detection is only implemented in adults right now
- Any adjustments to mechanical design requires recalibration
- To calibrate, output the volume, pressure, and `current_sum` value upon releasing the bag (around line 400 or so)
- Save this data in Excel in the format shown in `current_data.xlsx`, which contains
the most recently-used data for characterization
- Run `current.py` to fit a surface function to the data and output the
coefficients `[a, b, c, d, e, f]`
    - polynomial of the form `z = a + bx + cx^2 + dy + ey^2 + fxy`
- Apply these coefficients in  `predict_current()` function
