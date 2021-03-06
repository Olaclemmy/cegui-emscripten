/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

#include "CEGUI/RendererModules/OpenGLES2/ShaderManager.h"

#include "CEGUI/RendererModules/OpenGLES2/Shader.h"

#include "CEGUI/RendererModules/OpenGLES2/StandardShaderVert.h"
#include "CEGUI/RendererModules/OpenGLES2/StandardShaderFrag.h"

#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

#include <iostream>

namespace CEGUI
{
    OpenGLES2ShaderManager::OpenGLES2ShaderManager()
    {
        d_shadersInitialised = false;
    }

    OpenGLES2ShaderManager::~OpenGLES2ShaderManager()
    {
        deinitialiseShaders();
        d_shadersInitialised = false;
    }

    OpenGLES2Shader* OpenGLES2ShaderManager::getShader(GLuint id)
    {
        if(d_shaders.find(id) != d_shaders.end())
            return d_shaders[id];
        else
            return 0;
    }

    void OpenGLES2ShaderManager::loadShader(GLuint id, std::string vertexShader, std::string fragmentShader)
    {
        if(d_shaders.find(id) == d_shaders.end())
        {
            d_shaders[id] = CEGUI_NEW_AO OpenGLES2Shader(vertexShader, fragmentShader);
            d_shaders[id]->link();
        }
    }

    void OpenGLES2ShaderManager::initialiseShaders()
    {
        if(!d_shadersInitialised)
        {
            loadShader(SHADER_ID_STANDARDSHADER, StandardShaderVert, StandardShaderFrag);


            if(!getShader(SHADER_ID_STANDARDSHADER)->isCreatedSuccessfully())
            {
                const CEGUI::String errorString("Critical Error - One or multiple shader programs weren't created successfully");
                CEGUI_THROW(RendererException(errorString));

                return;
            }

            const CEGUI::String notify("OpenGLES2Renderer: Notification - Successfully initialised OpenGLES2Renderer shader programs.");
            if (CEGUI::Logger* logger = CEGUI::Logger::getSingletonPtr())
                logger->logEvent(notify);

            return;
        }
    }

    void OpenGLES2ShaderManager::deinitialiseShaders()
    {
        for(shaderContainerType::iterator iter = d_shaders.begin(); iter != d_shaders.end(); ++iter)
        {
            CEGUI_DELETE_AO iter->second;
        }
        d_shaders.clear();
    }

}
