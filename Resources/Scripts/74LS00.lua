-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS00, 4x 2 input NOR gate block
-- /red - 2026-05-15
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup(pinStates)

    pinStates[1].pin = 2; -- 1 is floating (input)
    pinStates[2].pin = 2; -- 2 is floating (input)
    pinStates[3].pin = 0; -- 3 is low (output)

    pinStates[4].pin = 2; -- 4 is floating (input)
    pinStates[5].pin = 2; -- 5 is floating (input)
    pinStates[6].pin = 0; -- 6 is low (output)

    pinStates[9] .pin = 2; --  9 is floating (input)
    pinStates[10].pin = 2; -- 10 is floating (input)
    pinStates[8] .pin = 0; --  8 is low (output)

    pinStates[12].pin = 2; -- 12 is floating (input)
    pinStates[13].pin = 2; -- 13 is floating (input)
    pinStates[11].pin = 0; -- 11 is low (output)

    return pinStates;
end

function Step(pinStates)

    pinStates[3] .pin = nand(pinStates[1] .wire, pinStates[2] .wire);
    pinStates[6] .pin = nand(pinStates[4] .wire, pinStates[5] .wire);
    pinStates[8] .pin = nand(pinStates[9] .wire, pinStates[10].wire);
    pinStates[11].pin = nand(pinStates[12].wire, pinStates[13].wire);

    return pinStates;
end

function nand(a, b)
    if not (a == 1 and b == 1) then
        return 1;
    else
        return 0;
    end
end