local _M = {}
-- screen part
local imageWidth = 1000
local imageHeight = 1000
local viewportTransform = utils.viewportTransformation(imageWidth, imageHeight)

local projectionMatrix = utils.projectionTransformation()

-- model part
local model = {
    model = render.loadModel("obj/african_head.obj"),
    textureQuery = utils.getTextureQueryMethod("obj/african_head_diffuse.tga"),
    normalQuery = utils.getTextureQueryMethod("obj/african_head_nm.tga"),
    specularQuery = utils.getTextureQueryMethod("obj/african_head_spec.tga"),

    modelMatrix = utils.modelingTransformation(10, { 0, 0, -5 })
}
model.faces, model.nFaces = model.model:faces()
model.uvIndexes, _ = model.model:uvIndexes()
function model:uv(i, j)
    return self.model:uv(self.uvIndexes[i][j])
end
function model:vert(i, j)
    return self.model:vert(self.faces[i][j])
end
function model:texture(u, v)
    return render.vec3f(self.textureQuery(u, v))
end
function model:normal(u, v)
    local queryResult = self.normalQuery(u, v)
    local normal = render.vec3f({ queryResult[3], queryResult[2], queryResult[1] })
    normal = normal / 255. * 2. - 1.
    return normal:normalized()
end
function model:specular(u, v)
    return self.specularQuery(u, v)[1]
end

-- shader part
-- - light pass
local lightPassShader = {
    screenCoords = {},
}
local light = {
    lightAt = { 30, 30, 50 },
    pointTo = { 0, 0, 0 }
}
lightPassShader.viewMatrix = utils.cameraTransformation(light.lightAt, light.pointTo, { 0, 1, 0 })
lightPassShader.MVP = projectionMatrix:multiplyMatrix(lightPassShader.viewMatrix:multiplyMatrix(model.modelMatrix))
function lightPassShader:vertex(ithFace, jthVert)
    local coords = model:vert(ithFace, jthVert):toVec4f(1.)
    local worldCoords = self.MVP:multiplyVector(coords)
    self.screenCoords[jthVert] = viewportTransform:multiplyVector(worldCoords):toVec3f(true)
end
function lightPassShader:fragment(u, v)
    local z = (1. - u - v) * self.screenCoords[1]:z() + u * self.screenCoords[2]:z() + v * self.screenCoords[3]:z()
    z = math.max(0., math.min(z, 1.))
    local c = math.floor(255 * z)
    return { c, c, c, 255 }
end
-- - image pass
local imagePassShader = {
    varyingScreenCoords = {},
    varyingUV = {},
    shadowMap = 0
}
imagePassShader.viewMatrix = utils.cameraTransformation({ 50, 0, 50 }, { 0, 0, 0 }, { 0., 1., 0 })
imagePassShader.MV = imagePassShader.viewMatrix:multiplyMatrix(model.modelMatrix)
imagePassShader.MVP = projectionMatrix:multiplyMatrix(imagePassShader.MV)
imagePassShader.InverseTranspose = imagePassShader.MV:inverse():transpose()
imagePassShader.toSM = viewportTransform:multiplyMatrix(lightPassShader.MVP:multiplyMatrix(imagePassShader.MVP:inverse():multiplyMatrix(viewportTransform:inverse())))
local lightDir = (render.vec3f(light.lightAt) - render.vec3f(light.pointTo)):normalized()
function imagePassShader:vertex(ithFace, jthVert)
    self.varyingUV[jthVert] = model:uv(ithFace, jthVert)

    local coords = model:vert(ithFace, jthVert):toVec4f(1.)
    local worldCoords = self.MVP:multiplyVector(coords)
    self.varyingScreenCoords[jthVert] = viewportTransform:multiplyVector(worldCoords):toVec3f(true)
end
local function round(num)
    return math.floor(num + 0.5)
end
local EPS = 1e-3
function imagePassShader:fragment(u, v)
    local uv = self.varyingUV[1] * (1 - u - v) + self.varyingUV[2] * u + self.varyingUV[3] * v
    local screenCoords = self.varyingScreenCoords[1] * (1 - u - v) + self.varyingScreenCoords[2] * u + self.varyingScreenCoords[3] * v

    local shadowMapCoords = self.toSM:multiplyVector(screenCoords:toVec4f(1.)):toVec3f(true)

    local normal = model:normal(uv:x(), uv:y()):toVec4f(0.)
    normal = self.InverseTranspose:multiplyVector(normal):toVec3f():normalized()

    local diffuse = math.max(0., normal:dot(lightDir))

    local lightOut = (normal * (normal:dot(lightDir) * 2.) - lightDir):normalized()
    local specular = math.max(lightOut:z(), 0.) ^ model:specular(uv:x(), uv:y())

    local visibility = self.shadowMap:getZBufferAt(round(shadowMapCoords:x()), round(shadowMapCoords:y())) <= shadowMapCoords:z() + EPS
    visibility = (visibility and 1.) or 0.3

    local baseColor = model:texture(uv:x(), uv:y())
    local output = baseColor * (diffuse + 0.6 * specular) * visibility + 5

    return {
        math.floor(math.min(255., output:x())),
        math.floor(math.min(255., output:y())),
        math.floor(math.min(255., output:z())),
        255
    }
end

-- main part
function _M.shadowMapping()
    local shadowMap = render.createImage(imageWidth, imageHeight, true)
    for i = 1, model.nFaces do
        for j = 1, 3 do
            lightPassShader:vertex(i, j)
        end
        shadowMap:triangleL3(lightPassShader.screenCoords, function(u, v)
            return lightPassShader:fragment(u, v)
        end)
    end
    imagePassShader.shadowMap = shadowMap

    local image = render.createImage(imageWidth, imageHeight, true)
    for i = 1, model.nFaces do
        for j = 1, 3 do
            imagePassShader:vertex(i, j)
        end
        image:triangleL3(imagePassShader.varyingScreenCoords, function(u, v)
            return imagePassShader:fragment(u, v)
        end)
    end

    model.model:unload()
    shadowMap:save("shadowMapping_shadowMap.tga")
    image:save("shadowMapping_image.tga")
end

return _M