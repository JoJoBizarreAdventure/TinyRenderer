local _M = {}

function _M.fillingTriangles()
    local image = render.createImage(200, 200)

    local t0 = {
        render.vec2i { 10, 70 }, render.vec2i { 50, 160 }, render.vec2i { 70, 80 }
    }
    local t1 = {
        render.vec2i { 180, 50 }, render.vec2i { 150, 1 }, render.vec2i { 70, 180 }
    }
    local t2 = {
        render.vec2i { 180, 150 }, render.vec2i { 120, 160 }, render.vec2i { 130, 180 }
    }

    image:triangleL2(t0, red)
    image:triangleL2(t1, white)
    image:triangleL2(t2, green)

    image:save("fillingTriangles.tga")
end

local function randomColorGenerator(seed)
    seed = seed or os.time()
    math.randomseed(seed)
    return function()
        local r = math.random(0, 255)
        local g = math.random(0, 255)
        local b = math.random(0, 255)
        return { r, g, b, 255 }
    end
end

function _M.flatShadingRender()
    local width = 1000
    local height = 1000
    local image = render.createImage(width, height)

    local model = render.loadModel("obj/african_head.obj")

    local faces, nFaces = model:faces()
    local generator = randomColorGenerator()
    for i = 1, nFaces do
        local f = faces[i]
        local screenCoords = {}
        for j = 1, 3 do
            local worldCoords = model:vert(f[j])
            screenCoords[j] = render.vec2i {
                math.floor((worldCoords:x() + 1.) * width / 2.),
                math.floor((worldCoords:y() + 1.) * height / 2.)
            }
        end
        image:triangleL2(screenCoords, generator())
    end

    model:unload()
    image:save("flatShadingRender.tga")
end

return _M