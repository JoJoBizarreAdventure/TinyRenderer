local _M = {}

function _M.lineDrawing()
    local image = render.createImage(100, 100)

    image:line(13, 20, 80, 40, white);
    image:line(20, 13, 40, 80, red)
    image:line(80, 40, 13, 20, red)

    image:save("lineDrawing.tga")
end

function _M.wireframeRendering()
    local width = 1000
    local height = 1000
    local image = render.createImage(width, height)

    local model = render.loadModel("obj/african_head.obj")

    local faces, nFaces = model:faces()
    for i = 1, nFaces do
        local f = faces[i]
        for j = 1, 3 do
            local v0 = model:vert(f[j])
            local v1 = model:vert(f[j % 3 + 1])
            local x0 = math.floor((v0:x() + 1.) * width / 2.);
            local y0 = math.floor((v0:y() + 1.) * height / 2.);
            local x1 = math.floor((v1:x() + 1.) * width / 2.);
            local y1 = math.floor((v1:y() + 1.) * height / 2.);
            image:line(x0, y0, x1, y1, white);
        end
    end

    model:unload()
    image:save("wireframeRendering.tga")
end

return _M