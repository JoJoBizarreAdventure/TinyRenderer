function updateCPath(luasocketCPath, mobdebugCPath)
    package.path = package.path .. ";" .. mobdebugCPath .. "/?.lua"

    package.path = package.path .. ";" .. luasocketCPath .. "/?.lua"
    package.path = package.path .. ";" .. luasocketCPath .. "/socket/?.lua"
    package.cpath = package.cpath .. ";" .. luasocketCPath .. "/?.dll"

    if luaDebug then
        require("mobdebug").start()
    end
end

function main()
    local l1 = require("lesson1")
    if enable[1] then
        l1.lineDrawing()
        l1.wireframeRendering()
    end

    local l2 = require("lesson2")
    if enable[2] then
        l2.fillingTriangles()
        l2.flatShadingRender()
    end

    local l3 = require("lesson3")
    if enable[3] then
        l3.triangleWithZBuffer()
        l3.pixelInterpolate()
        l3.applyTexture()
    end

    local l4 = require("lesson4")
    if enable[4] then
        l4.perspectiveProjection()
    end

    local l5 = require("lesson5")
    if enable[5] then
        l5.movingCamera()
    end

    local l6 = require("lesson6")
    if enable[6] then
        l6.GouraundShading()
        l6.certainIntensityLevel()
        l6.PhongShading()
        l6.normalMapping()
        l6.specularMapping()
        l6.normalTangent()
    end

    local l7 = require("lesson7")
    if enable[7] then
        l7.shadowMapping()
    end

    local l8 = require("lesson8")
    if enable[8] then
        l8.screenSpaceAmbientOcclusion()
    end
end