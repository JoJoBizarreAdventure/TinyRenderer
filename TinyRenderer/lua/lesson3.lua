local _M = {}

function _M.triangleWithZBuffer()
    local width = 1000
    local height = 1000
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")

    local faces, nFaces = model:faces()
    for i = 1, nFaces do
        local f = faces[i]
        local screenCoords = {}
        local avgZ = 0
        for j = 1, 3 do
            local worldCoords = model:vert(f[j])
            local z = (worldCoords:z() + 1.) / 2.
            screenCoords[j] = render.vec3f {
                (worldCoords:x() + 1.) * width / 2.,
                (worldCoords:y() + 1.) * height / 2.,
                z
            }
            avgZ = avgZ + z
        end
        avgZ = avgZ / 3
        image:triangleL3(screenCoords,
                function()
                    local c = math.floor(255 * avgZ)
                    return { c, c, c, 255 }
                end
        )
    end

    model:unload()
    image:save("triangleWithZBuffer.tga")
end

function _M.pixelInterpolate()
    local width = 1000
    local height = 1000
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")

    local faces, nFaces = model:faces()
    for i = 1, nFaces do
        local f = faces[i]
        local screenCoords = {}
        for j = 1, 3 do
            local worldCoords = model:vert(f[j])
            screenCoords[j] = render.vec3f {
                (worldCoords:x() + 1.) * width / 2.,
                (worldCoords:y() + 1.) * height / 2.,
                (worldCoords:z() + 1.) / 2.
            }
        end
        image:triangleL3(screenCoords,
                function(u, v)
                    local z = (1. - u - v) * screenCoords[1]:z() + u * screenCoords[2]:z() + v * screenCoords[3]:z()
                    local c = math.floor(255 * z)
                    return { c, c, c, 255 }
                end
        )
    end

    model:unload()
    image:save("pixelInterpolate.tga")
end

function _M.applyTexture()
    local width = 1024
    local height = 1024 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")

    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()
    for i = 1, nFaces do
        local f = faces[i]
        local uvIndex = uvIndexes[i]
        local screenCoords = {}
        local samplePoint = {}
        for j = 1, 3 do
            local worldCoords = model:vert(f[j])
            screenCoords[j] = render.vec3f {
                (worldCoords:x() + 1.) * width / 2.,
                (worldCoords:y() + 1.) * height / 2.,
                (worldCoords:z() + 1.) / 2.
            }

            samplePoint[j] = model:uv(uvIndex[j])
        end
        image:triangleL3(screenCoords,
                function(u, v)
                    local interpolateUV = samplePoint[1] * (1 - u - v) + samplePoint[2] * u + samplePoint[3] * v

                    return textureQuery(interpolateUV:x(), interpolateUV:y())
                end
        )
    end

    model:unload()
    image:save("applyTexture.tga")
end

return _M