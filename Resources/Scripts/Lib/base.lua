-- ---------------------------------------------------------------------------------------------------------------------
-- Base64 enconding and decoding
-- ---------------------------------------------------------------------------------------------------------------------
-- Source - https://stackoverflow.com/a/35303321
-- Posted by TheCrimulo
-- Retrieved 2026-05-21, License - CC BY-SA 3.0

local b='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/' -- You will need this for encoding/decoding
-- encoding
function __b64_enc(data)
    return ((data:gsub('.', function(x)
        local r,b='',x:byte()
        for i=8,1,-1 do r=r..(b%2^i-b%2^(i-1)>0 and '1' or '0') end
        return r;
    end)..'0000'):gsub('%d%d%d?%d?%d?%d?', function(x)
        if (#x < 6) then return '' end
        local c=0
        for i=1,6 do c=c+(x:sub(i,i)=='1' and 2^(6-i) or 0) end
        return b:sub(c+1,c+1)
    end)..({ '', '==', '=' })[#data%3+1])
end

-- decoding
function __b64_dec(data)
    data = string.gsub(data, '[^'..b..'=]', '')
    return (data:gsub('.', function(x)
        if (x == '=') then return '' end
        local r,f='',(b:find(x)-1)
        for i=6,1,-1 do r=r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c=0
        for i=1,8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end
        return string.char(c)
    end))
end

-- ---------------------------------------------------------------------------------------------------------------------
-- Serialize and Deserialize MessagePack for chip state storage
-- ---------------------------------------------------------------------------------------------------------------------
function __serialize(data)
    local mp = require("MessagePack");
    return __b64_enc(mp.pack(data));
end

function __deserialize(binary)
    local mp = require("MessagePack");
    return mp.unpack(__b64_dec(binary));
end

-- ---------------------------------------------------------------------------------------------------------------------
-- Helpers
-- ---------------------------------------------------------------------------------------------------------------------
function NumToBin(num, bits)
    bits = bits or math.max(1, select(2, math.frexp(num)))
    local t = {} -- will contain the bits
    for b = bits, 1, -1 do
        t[b] = math.fmod(num, 2)
        num = math.floor((num - t[b]) / 2)
    end
    return t
end

function BinToNum(data)
    len = #data;
    num = 0;

    for i = 1, len do
        if data[i] ~= 0 then
            data[i] = 1;
        end

        num = num + data[i] * 2 ^ (len - i);
    end

    return num
end

-- ---------------------------------------------------------------------------------------------------------------------
-- Some values
-- ---------------------------------------------------------------------------------------------------------------------
PIN_LOW = 0
PIN_HIGH = 1
PIN_FLOATING = 2
PIN_INPUT = PIN_FLOATING
PIN_OUTPUT = PIN_LOW
