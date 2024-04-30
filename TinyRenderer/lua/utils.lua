local _M = {}

function _M.modelingTransformation(scale, translation)
    scale = scale or 1.
    translation = translation or { 0, 0, 0 }

    local scaleMatrix = render.matrix4x4({
        { scale, 0., 0., 0. },
        { 0., scale, 0., 0. },
        { 0., 0., scale, 0. },
        { 0., 0., 0., 1. }
    })
    local transportMatrix = render.matrix4x4({
        { 1., 0., 0., translation[1] },
        { 0., 1., 0., translation[2] },
        { 0., 0., 1., translation[3] },
        { 0., 0., 0., 1. }
    })
    return transportMatrix:multiplyMatrix(scaleMatrix)
end

function _M.cameraTransformation(cameraAt, pointTo, up)
    cameraAt = cameraAt or { 0, 0, 1 }
    pointTo = pointTo or { 0, 0, 0 }
    up = up or { 0, 1, 0 }

    local translate = render.matrix4x4 {
        { 1., 0., 0., -cameraAt[1] },
        { 0., 1., 0., -cameraAt[2] },
        { 0., 0., 1., -cameraAt[3] },
        { 0., 0., 0., 1. }
    }

    local lookAt = render.vec3f({ pointTo[1] - cameraAt[1], pointTo[2] - cameraAt[2], pointTo[3] - cameraAt[3] }):normalized() -- direction of -z
    up = render.vec3f(up)
    up = up - lookAt * lookAt:dot(up)
    up = up:normalized()-- direction of y
    local dirX = lookAt:cross(up):normalized()

    local changeBasis = render.matrix4x4 {
        { dirX:x(), dirX:y(), dirX:z(), 0. },
        { up:x(), up:y(), up:z(), 0. },
        { -lookAt:x(), -lookAt:y(), -lookAt:z(), 0. },
        { 0., 0., 0., 1. }
    }
    return changeBasis:multiplyMatrix(translate)
end

function _M.projectionTransformation(zNear, zFar, eye_fov, aspect_ratio)
    zNear = zNear or -10
    zFar = zFar or -300.
    eye_fov = eye_fov or math.pi / 6.
    aspect_ratio = aspect_ratio or 1.

    local perspect2ortho = render.matrix4x4 {
        { zNear, 0., 0., 0. },
        { 0., zNear, 0., 0. },
        { 0., 0., zNear + zFar, -zFar * zNear },
        { 0., 0., 1., 0. }
    }

    local yTop = -zNear * math.tan(eye_fov / 2)
    local xRight = yTop * aspect_ratio
    local ortho = render.matrix4x4 {
        { 1. / xRight, 0., 0., 0. },
        { 0., 1. / yTop, 0., 0. },
        { 0., 0., 2. / (zNear - zFar), -(zNear + zFar) / (zNear - zFar) },
        { 0., 0., 0., 1. }
    }
    return ortho:multiplyMatrix(perspect2ortho)
end

function _M.viewportTransformation(width, height)
    return render.matrix4x4 {
        { width / 2., 0., 0., width / 2. },
        { 0., height / 2., 0., height / 2. },
        { 0., 0., 0.5, 0.5 },
        { 0., 0., 0., 1. }
    }
end

function _M.getTextureQueryMethod(path)
    local texture, textureWidth, textureHeight = render.openImage(path)
    return function(u, v)
        return texture:getColorAt(math.floor(u * textureWidth), math.floor(v * textureHeight))
    end
end

return _M