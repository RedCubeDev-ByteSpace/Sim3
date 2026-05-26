-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for a fictional clock chip
-- /red - 2026-05-15
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup()

    -- clock rate input
    pins.A.pin = PIN_INPUT;
    pins.B.pin = PIN_INPUT;
    pins.C.pin = PIN_INPUT;
    pins.D.pin = PIN_INPUT;

    -- enable pin
    pins.EN.pin = PIN_INPUT;

    -- clock output pin
    pins.OUT.pin = PIN_OUTPUT;

end

timer = 0;
outputState = false;

function Step()

    if pins.EN.wire == PIN_LOW then
        return;
    end

    timer = timer + 1;
    num = BinToNum({
        pins.D.wire,
        pins.C.wire,
        pins.B.wire,
        pins.A.wire,
    });

    if timer > num then
        timer = 0;
        outputState = not outputState;
        pins.OUT.pin = outputState;
    end

end