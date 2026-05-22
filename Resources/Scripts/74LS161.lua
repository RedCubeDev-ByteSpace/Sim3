-- ---------------------------------------------------------------------------------------------------------------------
-- Logic Implementation for the 74LS161, 4-Bit Binary Counter
-- /red - 2026-05-21
-- ---------------------------------------------------------------------------------------------------------------------
function PinSetup()

    -- power pins are just gonna be inputs
    pins.VCC.pin = PIN_INPUT;
    pins.GND.pin = PIN_INPUT;

    -- all them inputs
    pins.CLR .pin = PIN_INPUT;
    pins.CLK .pin = PIN_INPUT;
    pins.A   .pin = PIN_INPUT;
    pins.B   .pin = PIN_INPUT;
    pins.C   .pin = PIN_INPUT;
    pins.D   .pin = PIN_INPUT;
    pins.ENP .pin = PIN_INPUT;
    pins.LOAD.pin = PIN_INPUT;
    pins.ENT .pin = PIN_INPUT;

    -- all them outputs
    pins.QA  .pin = PIN_OUTPUT;
    pins.QB  .pin = PIN_OUTPUT;
    pins.QC  .pin = PIN_OUTPUT;
    pins.QD  .pin = PIN_OUTPUT;
    pins.RCO .pin = PIN_OUTPUT;

end

-- ---------------------------------------------------------------------------------------------------------------------
num = 0
rippleCarry = false
-- ---------------------------------------------------------------------------------------------------------------------

function Step()

    -- if CLR low -> instantly clear the state and output
    if pins.CLR.wire == PIN_LOW then
        num = 0;
        rippleCarry = false;
    else
        -- if the output is 1111 and the pin ENT is high -> turn on ripple carry
        rippleCarry = num == 15 and pins.ENT.wire == 1;
    end

    updatePins();
end

-- ---------------------------------------------------------------------------------------------------------------------

function StepRising()

    -- if CLR low -> instantly clear the state and output
    -- this has the highest priority! nothing else comes after
    if pins.CLR.wire == PIN_LOW then

        -- just clear the state, then end this step
        num = 0;
        rippleCarry = false;
        updatePins();

        return;
    end

    -- if LOAD is low
    -- -> load the value from the inputs
    if pins.LOAD.wire == PIN_LOW then
        num = BinToNum({
            pins.D.wire,
            pins.C.wire,
            pins.B.wire,
            pins.A.wire,
        });

        updatePins();
        return;
    end

    -- if ENP and ENT are high
    -- -> counting is enabled, count!
    if pins.ENP.wire == PIN_HIGH and pins.ENT.wire == PIN_HIGH then
        num = num + 1;

        -- have we overflown our 4 bits?
        if num > 15 then
            num = 0      -- roll over
        end
    end

    updatePins();
end

-- ---------------------------------------------------------------------------------------------------------------------

function updatePins()

    state = NumToBin(num, 4);
    pins.QA .pin = state[4];
    pins.QB .pin = state[3];
    pins.QC .pin = state[2];
    pins.QD .pin = state[1];
    pins.RCO.pin = rippleCarry;
end

-- ---------------------------------------------------------------------------------------------------------------------

function LoadState(state)
    num         = state["num"];
    rippleCarry = state["rippleCarry"];
end

function SaveState()
    return {
        num = num,
        rippleCarry = rippleCarry
    };
end