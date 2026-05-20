-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS161, 4-Bit Binary Counter
-- /red - 2026-05-21
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup(pinStates)

    for i = 1, 10 do
        pinStates[i].pin = 2; -- all of these are inputs (floating)
    end

    for i = 11, 15 do
        pinStates[i].pin = 0; -- all of these are outputs (low)
    end

    return pinStates;
end

-- ---------------------------------------------------------------------------------------------------------------------
num = 0
carry = false

function Step(pinStates)

    -- Pin 1 = CLR
    -- -> if low -> instantly clear the state and output
    if pinStates[1].wire == 0 then
        num = 0;
    end

    updatePins(pinStates);
    return pinStates;
end

-- ---------------------------------------------------------------------------------------------------------------------

function StepRising(pinStates)

    -- reset the carry flag every step
    carry = false;

    -- if 7 (ENP) and 10 (ENT) are high
    -- -> counting is enabled, count!
    if pinStates[7].wire == 1 and pinStates[10].wire == 1 then
        num = num + 1;

        -- have we overflowed our 4 bits?
        if num > 15 then
            num = 0      -- roll around
            carry = true -- set the carry bit
        end
    end

    -- if 9 (LOAD) is low
    -- -> load the value from the inputs
    if pinStates[9].wire == 0 then
        num = pinStates[3].wire + pinStates[4].wire * 2 + pinStates[5].wire * 4 + pinStates[6].wire * 8;
    end

    return pinStates;
end

function StepFalling(pinStates)

    -- nothing to do here

    return pinStates;
end

-- ---------------------------------------------------------------------------------------------------------------------

function toBits(num,bits)
    -- returns a table of bits, most significant first.
    bits = bits or math.max(1, select(2, math.frexp(num)))
    local t = {} -- will contain the bits
    for b = bits, 1, -1 do
        t[b] = math.fmod(num, 2)
        num = math.floor((num - t[b]) / 2)
    end
    return t
end

function updatePins(pinStates)

    state = toBits(num, 4);
    pinStates[14].pin = state[4];
    pinStates[13].pin = state[3];
    pinStates[12].pin = state[2];
    pinStates[11].pin = state[1];
    pinStates[15].pin = (carry and 1 or 0)

    return pinStates;
end

-- ---------------------------------------------------------------------------------------------------------------------

function LoadState(state)

end

function SaveState()

end