local _M = {}

function _M.GouraundShading()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    --local uvIndexes, _ = model:uvIndexes()
    local normalIndexes, _ = model:normalIndexes()

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 40, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 1, 1, 0 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingIntensity = {}

        local function vertex(ithFace, jthVert)
            local normal = model:normal(normalIndexes[ithFace][jthVert])
            varyingIntensity[jthVert] = math.max(0., normal:dot(lightDir))

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local worldCoords = MVP:multiplyVector(coords)
            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local interpolateIntensity = (1 - u - v) * varyingIntensity[1] + u * varyingIntensity[2] + v * varyingIntensity[3]
            local c = math.floor(255 * interpolateIntensity)
            return { c, c, c, 255 }
        end

        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("GouraundShading.tga")
end

function _M.certainIntensityLevel()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    --local uvIndexes, _ = model:uvIndexes()
    local normalIndexes, _ = model:normalIndexes()

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 40, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 1, 1, 0 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingIntensity = {}

        local function vertex(ithFace, jthVert)
            local normal = model:normal(normalIndexes[ithFace][jthVert])
            varyingIntensity[jthVert] = math.max(0., normal:dot(lightDir))

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local worldCoords = MVP:multiplyVector(coords)
            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local intensity = (1 - u - v) * varyingIntensity[1] + u * varyingIntensity[2] + v * varyingIntensity[3]
            if (intensity > .85) then
                intensity = 1
            elseif (intensity > .60) then
                intensity = .80;
            elseif (intensity > .45) then
                intensity = .60;
            elseif (intensity > .30) then
                intensity = .45;
            elseif (intensity > .15) then
                intensity = .30;
            else
                intensity = 0;
            end
            local c = math.floor(255 * intensity)
            return { c, c, c, 255 }
        end
        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("certainIntensityLevel.tga")
end

function _M.PhongShading()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()
    local normalIndexes, _ = model:normalIndexes()
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 30, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 1, 0, 1 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingIntensity = {}
        local varyingUV = {}

        local function vertex(ithFace, jthVert)
            local normal = model:normal(normalIndexes[ithFace][jthVert])
            varyingIntensity[jthVert] = math.max(0., normal:dot(lightDir))

            varyingUV[jthVert] = model:uv(uvIndexes[ithFace][jthVert])

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local worldCoords = MVP:multiplyVector(coords)
            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local intensity = (1 - u - v) * varyingIntensity[1] + u * varyingIntensity[2] + v * varyingIntensity[3]

            local uv = varyingUV[1] * (1 - u - v) + varyingUV[2] * u + varyingUV[3] * v

            local textureColor = textureQuery(uv:x(), uv:y())
            return {
                math.floor(textureColor[1] * intensity),
                math.floor(textureColor[2] * intensity),
                math.floor(textureColor[3] * intensity),
                255
            }
        end
        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("PhongShading.tga")
end

function _M.normalMapping()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")
    local normalQuery = utils.getTextureQueryMethod("obj/african_head_nm.tga")

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 30, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local InverseTranspose = viewMatrix:multiplyMatrix(modelMatrix):inverse():transpose()
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 1, 0, 1 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingUV = {}

        local function vertex(ithFace, jthVert)
            varyingUV[jthVert] = model:uv(uvIndexes[ithFace][jthVert])

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local worldCoords = MVP:multiplyVector(coords)
            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local uv = varyingUV[1] * (1 - u - v) + varyingUV[2] * u + varyingUV[3] * v

            local queryResult = normalQuery(uv:x(), uv:y())
            local normal = render.vec3f({ queryResult[3], queryResult[2], queryResult[1] })
            normal = normal / 255. * 2. - 1.
            normal = normal:normalized():toVec4f(0.)
            normal = InverseTranspose:multiplyVector(normal):toVec3f():normalized()

            local intensity = math.max(0., normal:dot(lightDir))
            local textureColor = textureQuery(uv:x(), uv:y())
            return {
                math.floor(textureColor[1] * intensity),
                math.floor(textureColor[2] * intensity),
                math.floor(textureColor[3] * intensity),
                255
            }
        end
        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("normalMapping.tga")
end

function _M.specularMapping()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")
    local normalQuery = utils.getTextureQueryMethod("obj/african_head_nm.tga")
    local specularQuery = utils.getTextureQueryMethod("obj/african_head_spec.tga")

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 30, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MVP = projectionMatrix:multiplyMatrix(viewMatrix:multiplyMatrix(modelMatrix))
    local InverseTranspose = viewMatrix:multiplyMatrix(modelMatrix):inverse():transpose()
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 0, 0, 1 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingUV = {}

        local function vertex(ithFace, jthVert)
            varyingUV[jthVert] = model:uv(uvIndexes[ithFace][jthVert])

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local worldCoords = MVP:multiplyVector(coords)
            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local uv = varyingUV[1] * (1 - u - v) + varyingUV[2] * u + varyingUV[3] * v

            local queryResult = normalQuery(uv:x(), uv:y())
            local normal = render.vec3f({ queryResult[3], queryResult[2], queryResult[1] })
            normal = normal / 255. * 2. - 1.
            normal = normal:normalized():toVec4f(0.)
            normal = InverseTranspose:multiplyVector(normal):toVec3f():normalized()
            local diffuse = math.max(0., normal:dot(lightDir))

            local lightOut = (normal * (normal:dot(lightDir) * 2.) - lightDir):normalized()
            local specular = math.max(lightOut:z(), 0.) ^ specularQuery(uv:x(), uv:y())[1]

            local baseColor = render.vec3f(textureQuery(uv:x(), uv:y()))
            baseColor = baseColor * (diffuse + 0.6 * specular) + 5
            local textureColor = {
                math.min(255., baseColor:x()),
                math.min(255., baseColor:y()),
                math.min(255., baseColor:z()),
            }
            return {
                math.floor(textureColor[1]),
                math.floor(textureColor[2]),
                math.floor(textureColor[3]),
                255
            }
        end
        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("specularMapping.tga")
end

function _M.normalTangent()
    local width = 1000
    local height = 1000 -- just apply texture size, using scale is also a choice
    local image = render.createImage(width, height, true)

    local model = render.loadModel("obj/african_head.obj")
    local faces, nFaces = model:faces()
    local uvIndexes, _ = model:uvIndexes()
    local normalIndexes, _ = model:normalIndexes()
    local textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga")
    local normalTangentQuery = utils.getTextureQueryMethod("obj/african_head_nm_tangent.tga")
    local function getNT(u, v)
        local queryResult = normalTangentQuery(u, v)
        local nt = render.vec3f { queryResult[3], queryResult[2], queryResult[1] }
        nt = nt / 255. * 2. - 1.
        return nt:normalized()
    end
    local specularQuery = utils.getTextureQueryMethod("obj/african_head_spec.tga")

    local modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
    local viewMatrix = utils.cameraTransformation({ 30, 30, 50 }, { 0, 0, 0 }, { 0., math.sqrt(3.) / 2., -0.5 })
    local projectionMatrix = utils.projectionTransformation()
    local MV = viewMatrix:multiplyMatrix(modelMatrix)
    local InverseTranspose = MV:inverse():transpose()
    local viewportTransform = utils.viewportTransformation(width, height)

    local lightDir = render.vec3f({ 1, 0, 1 }):normalized()

    for i = 1, nFaces do
        local screenCoords = {}
        local varyingUV = {}
        local viewTri = {}
        local varyingNormal = {}

        local function vertex(ithFace, jthVert)
            local uv = model:uv(uvIndexes[ithFace][jthVert])
            varyingUV[jthVert] = uv

            local coords = model:vert(faces[ithFace][jthVert]):toVec4f(1.)
            local viewCoords = MV:multiplyVector(coords)
            viewTri[jthVert] = viewCoords:toVec3f(true)
            local worldCoords = projectionMatrix:multiplyVector(viewCoords)

            local normal = model:normal(normalIndexes[ithFace][jthVert]):toVec4f(0.)
            varyingNormal[jthVert] = InverseTranspose:multiplyVector(normal):toVec3f():normalized()

            return viewportTransform:multiplyVector(worldCoords):toVec3f(true)
        end

        for j = 1, 3 do
            screenCoords[j] = vertex(i, j)
        end

        local function fragment(u, v)
            local uv = varyingUV[1] * (1 - u - v) + varyingUV[2] * u + varyingUV[3] * v

            local bn = (varyingNormal[1] * (1 - u - v) + varyingNormal[2] * u + varyingNormal[3] * v):normalized()
            local v01 = viewTri[2] - viewTri[1]
            local v02 = viewTri[3] - viewTri[1]

            local AI = (render.matrix3x3 {
                v01:asTable(),
                v02:asTable(),
                bn:asTable()
            }):inverse()

            local vecI = AI:multiplyVector(render.vec3f { varyingUV[2]:x() - varyingUV[1]:x(), varyingUV[3]:x() - varyingUV[1]:x(), 0 }):normalized()
            local vecJ = AI:multiplyVector(render.vec3f { varyingUV[2]:y() - varyingUV[1]:y(), varyingUV[3]:y() - varyingUV[1]:y(), 0 }):normalized()
            local B = (render.matrix3x3 {
                vecI:asTable(),
                vecJ:asTable(),
                bn:asTable()
            }):transpose()
            local queryResult = getNT(uv:x(), uv:y())

            local normal = B:multiplyVector(queryResult):normalized()

            local diffuse = math.max(0.0, normal:dot(lightDir))

            local lightOut = (normal * (normal:dot(lightDir) * 2.) - lightDir):normalized()

            local specular = math.max(lightOut:z(), 0.) ^ (5. + specularQuery(uv:x(), uv:y())[1])

            local baseColor = render.vec3f(textureQuery(uv:x(), uv:y()))
            baseColor = baseColor * (diffuse + specular) + 10

            local textureColor = {
                math.min(255., baseColor:x()),
                math.min(255., baseColor:y()),
                math.min(255., baseColor:z()),
            }
            return {
                math.floor(textureColor[1]),
                math.floor(textureColor[2]),
                math.floor(textureColor[3]),
                255
            }
        end
        image:triangleL3(screenCoords, fragment)
    end

    model:unload()
    image:save("normalTangent.tga")
end

return _M