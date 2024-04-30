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

-- shader part
-- - image pass
local imagePassShader = {
    varyingScreenCoords = {},
    varyingUV = {},
    shadowMap = 0
}
imagePassShader.viewMatrix = utils.cameraTransformation({ 30, 30, 50 }, { 0, 0, 0 }, { 0., 1., 0 })
imagePassShader.MV = imagePassShader.viewMatrix:multiplyMatrix(model.modelMatrix)
imagePassShader.MVP = projectionMatrix:multiplyMatrix(imagePassShader.MV)
imagePassShader.InverseTranspose = imagePassShader.MV:inverse():transpose()

function imagePassShader:vertex(ithFace, jthVert)
    self.varyingUV[jthVert] = model:uv(ithFace, jthVert)

    local coords = model:vert(ithFace, jthVert):toVec4f(1.)
    local worldCoords = self.MVP:multiplyVector(coords)
    local viewCoords = viewportTransform:multiplyVector(worldCoords):toVec3f(true)
    self.varyingScreenCoords[jthVert] = viewCoords
end
function imagePassShader:fragment(u, v)
    local uv = self.varyingUV[1] * (1 - u - v) + self.varyingUV[2] * u + self.varyingUV[3] * v

    local output = model:texture(uv:x(), uv:y())

    --return { 255, 255, 255, 255 }

    return {
        math.floor(math.min(255., output:x())),
        math.floor(math.min(255., output:y())),
        math.floor(math.min(255., output:z())),
        255
    }
end

-- main part
function _M.screenSpaceAmbientOcclusion()
    local image = render.createImage(imageWidth, imageHeight, true)
    for i = 1, model.nFaces do
        for j = 1, 3 do
            imagePassShader:vertex(i, j)
        end
        image:triangleL3(imagePassShader.varyingScreenCoords, function(u, v)
            return imagePassShader:fragment(u, v)
        end)
    end

    local function queryZBuffer(x, y)
        return image:getZBufferAt(math.floor(x), math.floor(y))
    end

    local function maxElevationAngle(p, dir)
        local maxAngle = 0.
        local pDepth = queryZBuffer(p:x(), p:y())
        for t = 1, 19 do
            local cur = p + dir * t
            if cur:x() >= imageWidth or cur:y() >= imageHeight or cur:x() < 0 or cur:y() < 0 then
                return maxAngle
            end

            local elevation = queryZBuffer(cur:x(), cur:y()) - pDepth
            if elevation > 0 then
                maxAngle = math.max(maxAngle, math.atan(elevation, t))
            end
        end
        return maxAngle
    end

    local radius = math.pi / 4
    local dirs = {}
    for a = 0, 7 do
        dirs[a + 1] = render.vec2f { math.cos(radius * a), math.sin(radius * a) }
    end

    for x = 0, imageWidth - 1 do
        print("x=" .. x)
        for y = 0, imageHeight - 1 do
            if image:getZBufferAt(x, y) >= -1e5 then
                local total = 0.
                for a = 1, 8 do
                    total = total + math.pi / 2 - maxElevationAngle(render.vec2f { x, y }, dirs[a])
                end
                total = total / 8 -- average
                total = total / (math.pi / 2) -- to [0,1]
                total = total ^ 100 -- increase the contrast
                local c = image:getColorAt(x, y)
                image:setPixel(x, y, { math.floor(c[1] * total), math.floor(c[2] * total), math.floor(c[3] * total), 255 })
            end
        end
    end

    model.model:unload()
    image:save("ambientOcclusion.tga")
end

return _M