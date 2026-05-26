-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS48, Binary-Coded-Decimal to seven segment display converter
-- /red - 2026-05-24
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup()

    -- power pins!! just set them as input idek
    pins.VCC.pin = PIN_INPUT;
    pins.GND.pin = PIN_INPUT;

    -- all BDC Pins are all inputs
    pins.A.pin = PIN_INPUT;
    pins.B.pin = PIN_INPUT;
    pins.C.pin = PIN_INPUT;
    pins.D.pin = PIN_INPUT;

    -- LampTest is also a input
    pins.LT.pin = PIN_INPUT;

    -- BlankingInput / RippleBlankingOutput is a little special because its both input and output
    -- -> by default its a floating input, but turns into a low output once RippleBlankingInput is low
    pins.BIRB.pin = PIN_INPUT;

    -- RippleBlankingInput is always an input
    pins.RBI.pin = PIN_INPUT;

    -- the rest are the seven segment outputs
    pins.a.pin = PIN_OUTPUT;
    pins.b.pin = PIN_OUTPUT;
    pins.c.pin = PIN_OUTPUT;
    pins.d.pin = PIN_OUTPUT;
    pins.e.pin = PIN_OUTPUT;
    pins.f.pin = PIN_OUTPUT;
    pins.g.pin = PIN_OUTPUT;
end

function Step()

    -- BlankingInput has priority
    if pins.BIRB.wire == PIN_LOW and pins.BIRB.pin ~= PIN_LOW then
        setPinsFor(15);
        return;
    end

    -- Next up: LampTest
    if pins.LT.wire == PIN_LOW then
        -- turn on all lamps, meaning: display an 8
        setPinsFor(8);
        return;
    end

    -- decode the number thats currently set on the inputs
    num = BinToNum({
        pins.D.wire, pins.C.wire, pins.B.wire, pins.A.wire
    });

    -- Then: process RippleBlankingInput (+ Leading/Trailing Zero suppression)
    if pins.RBI.wire == PIN_LOW and num == 0 then

        -- turn off all lamps
        setPinsFor(15);

        -- ripple it down the line
        pins.BIRB.pin = PIN_LOW;

        return;
    end

    -- last but not least, just output the number
    setPinsFor(num);

    -- also reset RBO if theres no blanking going on anymore
    pins.BIRB.pin = PIN_INPUT;
end

-- for some reason this chip supports outputting unique shapes for all
-- combinations from 0 to 15 - everything after 9 does not resemble a digit tho
-- may still be very helpful for debugging hex things
lookupTable = {
    {1, 1, 1, 1, 1, 1, 0}, -- 0
    {0, 1, 1, 0, 0, 0, 0}, -- 1
    {1, 1, 0, 1, 1, 0, 1}, -- 2
    {1, 1, 1, 1, 0, 0, 1}, -- 3
    {0, 1, 1, 0, 0, 1, 1}, -- 4
    {1, 0, 1, 1, 0, 1, 1}, -- 5
    {0, 0, 1, 1, 1, 1, 1}, -- 6 -- i didnt make a mistake, the 6 on this thing looks like a lowercase b
    {1, 1, 1, 0, 0, 0, 0}, -- 7
    {1, 1, 1, 1, 1, 1, 1}, -- 8
    {1, 1, 1, 0, 0, 1, 1}, -- 9 -- again, slightly weird way of showing a 9, looks like a q, i blame the americans

    {0, 0, 0, 1, 1, 0, 1}, -- 10
    {0, 0, 1, 1, 0, 0, 1}, -- 11
    {0, 1, 0, 0, 0, 1, 1}, -- 12
    {1, 0, 0, 1, 0, 1, 1}, -- 13
    {0, 0, 0, 1, 1, 1, 1}, -- 14
    {0, 0, 0, 0, 0, 0, 0}, -- 15 -- entirely blank
};

function setPinsFor(num)
    setPinsToTable(lookupTable[num + 1]);
end

function setPinsToTable(states)
    pins.a.pin = states[1];
    pins.b.pin = states[2];
    pins.c.pin = states[3];
    pins.d.pin = states[4];
    pins.e.pin = states[5];
    pins.f.pin = states[6];
    pins.g.pin = states[7];
end