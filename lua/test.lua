

--[[ ret = device.math_abs(-1)
print(ret) ]]

a = {}

meta_a = {
    __index = {
        name = "balck",
        age = 17,
        sex = 1,
    }
}

setmetatable(a,meta_a)
-- print(a.name)

local base = {}
function base:test()
    -- body
    print("base::test")
end

function base:new(instance)
    -- body
    if not instance then
        instance = {}
    end
    setmetatable(instance,{__index = self})
    return instance
end

--base:test()

--b = base:new()
--b:test()

person = {}
function person:test()
    -- body
    print("person:test")
end

function person:new(instance)
    -- body
    if not instance then
        instance = {}
    end
    setmetatable(instance,{__index = self})

    print(self)
    return instance
end

man = person:new()
function man:test_man()
    -- body
    print("man:test_man")
end

function man:test_man2()
    -- body
    print("man:test_man2")
end

b = man:new()

b:test_man()
b:test()



