-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS00, 4x 2 input NOR gate block
-- /red - 2026-05-15
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup(pinStates)

    pinStates[2].pin = 2;
    pinStates[3].pin = 2;
    pinStates[1].pin = 0;

    pinStates[5].pin = 2;
    pinStates[6].pin = 2;
    pinStates[4].pin = 0;

    pinStates[8] .pin = 2
    pinStates[9] .pin = 2;
    pinStates[10].pin = 0;

    pinStates[11].pin = 2
    pinStates[12].pin = 2
    pinStates[13].pin = 0

    return pinStates;
end

function Step(pinStates)

    pinStates[1] .pin = nor(pinStates[2] .wire, pinStates[3] .wire);
    pinStates[4] .pin = nor(pinStates[5] .wire, pinStates[6] .wire);
    pinStates[10].pin = nor(pinStates[8] .wire, pinStates[9] .wire);
    pinStates[13].pin = nor(pinStates[11].wire, pinStates[12].wire);

    return pinStates;
end

function nor(a, b)
    if not (a == 1 or b == 1) then
        return 1;
    else
        return 0;
    end
end