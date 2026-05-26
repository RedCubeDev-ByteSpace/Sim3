-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS32, 4x 2 input OR gate block
-- /red - 2026-05-25
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup()

    -- power pins!! just set them as input idek
    pins.VCC.pin = PIN_INPUT;
    pins.GND.pin = PIN_INPUT;

    -- all 4 for them them logical gates
    -- (each with input A and B and output Y)
    for i = 1, 4 do
        pins["A" .. i].pin = PIN_INPUT;
        pins["B" .. i].pin = PIN_INPUT;
        pins["Y" .. i].pin = PIN_OUTPUT;
    end

end

function Step()

    -- go through all gate pairs and do the logic
    for i = 1, 4 do
        pins["Y" .. i].pin = OR(
                pins["A" .. i].wire,
                pins["B" .. i].wire
        );
    end

end

function OR(a, b)
    return a == PIN_HIGH or b == PIN_HIGH;
end