local _M = {}

function _M.perspectiveProjection()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")
    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 0, 0, 50 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local viewportTransform = utils.viewportTransformation(width, height)

    for i = 1, nFaces do
        local f = faces[i]
        local uvIndex = uvIndexes[i]
        local screenCoords = {}
        local samplePoint = {}
        for j = 1, 3 do
            local coords = model:vert(f[j]):toVec4f(1.)

            local worldCoords = MVP:multiplyVector(coords)

            screenCoords[j] = viewportTransform:multiplyVector(worldCoords):toVec3f(true)

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
    image:save("perspectiveProjection.tga")
end

return _M