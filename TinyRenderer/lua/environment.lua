package.path = package.path .. ";" .. "./lua/?.lua" -- update for ubuntu path

render = require('render')
utils = require("utils")

white = { 255, 255, 255, 255 }
red = { 0, 0, 255, 255 }
green = { 0, 255, 0, 255 }

enable = {
    false, -- lesson 1
    false, -- lesson 2
    false, -- lesson 3
    false, -- lesson 4
    false, -- lesson 5
    false, -- lesson 6
    false, -- lesson 7
    true, -- lesson 8
}

--luaDebug = true