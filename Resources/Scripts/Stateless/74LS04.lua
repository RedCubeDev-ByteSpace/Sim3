-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS04, 6x 1 input NOT gate block
-- /red - 2026-05-24
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup()

    -- power pins!! just set them as input idek
    pins.VCC.pin = PIN_INPUT;
    pins.GND.pin = PIN_INPUT;

    -- all 6!! for them them logical gates
    -- (each with input A and output Y)
    for i = 1, 6 do
        pins["A" .. i].pin = PIN_INPUT;
        pins["Y" .. i].pin = PIN_OUTPUT;
    end

end

function Step()

    -- go through all gate pairs and do the logic
    for i = 1, 6 do
        pins["Y" .. i].pin = not (pins["A" .. i].wire == PIN_HIGH);
    end

end