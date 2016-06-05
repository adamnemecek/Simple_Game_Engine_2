#include "Renderer.h"

#include "Utilities/My_Assert.h"
#include "Utilities/ShaderManager.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

// for glm::perspective
#include "Utilities/Include_Helpers/GLM_Mat_Transform.h"
#include "Utilities/Include_Helpers/GL_Version.h"

// include this last to avoid errors when this includes gl.h 
// (apparently this needs to be included after the glm and glload and other libraries)
#include <GL/freeglut.h>

#include "Shapes/Geometry.h"
#include "Rendering/Camera.h"

#include "Utilities/Printer_Helper.h"
#include <iostream>
using std::cout;
using std::endl;


namespace Rendering
{
    Renderer::Renderer()
        :
        _pCamera(0),
        _renderProgramId(0),
        _unifLocModelToCameraMatrix(0),
        _unifLocCameraToClipMatrix(0),
        _unifLocLight1Intensity(0),
        _unifLocLight1PosCs(0),
        _unifLocLight2Intensity(0),
        _unifLocLight2PosCs(0)
    {
    }

    bool Renderer::Init(const GLuint programId)
    {
        _renderProgramId = programId;

        Utilities::ShaderManager &shaderManagerRef = Utilities::ShaderManager::GetInstance();

        std::string unifStr = "unif_model_to_camera_matrix";
        _unifLocModelToCameraMatrix = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocModelToCameraMatrix < 0) { glUseProgram(0); return false; }

        unifStr = "unif_camera_to_clip_matrix";
        _unifLocCameraToClipMatrix = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocCameraToClipMatrix < 0) { glUseProgram(0); return false; }

        unifStr = "unif_light_1_pos_cs";
        _unifLocLight1PosCs = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocLight1PosCs < 0) { glUseProgram(0); return false; }

        unifStr = "unif_light_1_intensity";
        _unifLocLight1Intensity = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocLight1Intensity < 0) { glUseProgram(0); return false; }

        unifStr = "unif_light_2_pos_cs";
        _unifLocLight2PosCs = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocLight2PosCs < 0) { glUseProgram(0); return false; }

        unifStr = "unif_light_2_intensity";
        _unifLocLight2Intensity = shaderManagerRef.FindUniform(_renderProgramId, unifStr);
        if (_unifLocLight2Intensity < 0) { glUseProgram(0); return false; }

        _numCurrentRenderables = 0;

        return true;
    }

    bool Renderer::Shutdown()
    {
        return true;
    }

    void Renderer::ConfigureNewRenderable(const Entities::Entity *e_ptr, const Shapes::Geometry *g_ptr)
    {
        MY_ASSERT(_numCurrentRenderables != _MAX_RENDERABLES);
        MY_ASSERT(0 != g_ptr && 0 != e_ptr);
        Renderable &r = _renderables[_numCurrentRenderables++];
        r.set_geometry(g_ptr);
        r.set_entity(e_ptr);
    }

    void Renderer::SetViewport(GLsizei width, GLsizei height)
    {
        glViewport(0, 0, width, height);

        float fov_radians = (1.0f / 4.0f) * 3.14159f;   // 1/2 pi
        float aspect_ratio = (float)width / height;
        float near_plane_dist = 0.1f;
        float far_plane_dist = 100.0f;
        _perspectiveMat = glm::perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);

        cout << "resizing to aspect ratio " << aspect_ratio << endl;
    }

    void Renderer::SetCameraToRender(Camera *camera_ptr)
    {
        MY_ASSERT(camera_ptr != 0);
        _pCamera = camera_ptr;
    }

    void Renderer::Render()
    {
        // we can't render without a camera
        MY_ASSERT(_pCamera != 0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(_renderProgramId);

        // for each renderable:
        // - bind the geometry VAO
        // - send "full transform" and "orientation only" matrices to GPU
        // - draw elements 

        // the camera position is independent of each renderable
        glm::mat4 camera_mat = _pCamera->get_world_to_view_matrix();

        // the lights are independent of each renderable
        // Note: The light locations that are hard-coded here are in world space, so I only need
        // to transform them with the camera matrix to get them into camera space.
        glm::vec3 light_1_location = glm::vec3(camera_mat * glm::vec4(+5.0f, +6.0f, -7.0f, 1.0f));
        float light_1_intensity = 90.0f;
        glm::vec3 light_2_location = glm::vec3(camera_mat * glm::vec4(-5.0f, +3.0f, +5.0f, 1.0f));
        float light_2_intensity = 30.0f;
        glUniform3fv(_unifLocLight1PosCs, 1, glm::value_ptr(light_1_location));
        glUniform1f(_unifLocLight1Intensity, light_1_intensity);
        glUniform3fv(_unifLocLight2PosCs, 1, glm::value_ptr(light_2_location));
        glUniform1f(_unifLocLight2Intensity, light_2_intensity);

        // the camera-to-clip matrix is independent of each renderable
        glUniformMatrix4fv(_unifLocCameraToClipMatrix, 1, GL_FALSE, glm::value_ptr(_perspectiveMat));

        for (uint renderable_count = 0; renderable_count < _numCurrentRenderables; renderable_count++)
        {
            Renderable &r = _renderables[renderable_count];
            glBindVertexArray(r.get_VAO_ID());

            glm::mat4 model_to_camera = camera_mat * r.get_model_to_world_matrix();
            
            //glm::mat4 worldToCamera = _perspectiveMat * model_to_camera;
            //glm::vec4 v4 = glm::vec4(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
            //v4 = worldToCamera * v4;
            glm::vec4 v4 = glm::vec4(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
            v4 = model_to_camera * v4;
            v4 = _perspectiveMat * v4;

            glUniformMatrix4fv(_unifLocModelToCameraMatrix, 1, GL_FALSE, glm::value_ptr(model_to_camera));

            // draw the indices according to their draw command
            std::vector<Shapes::Index_Meta_Data> index_meta_data_collection_ref = r.get_geometry_index_meta_data_collection();
            uint indices_drawn_so_far = 0;
            for (uint render_mode_index = 0; render_mode_index < index_meta_data_collection_ref.size(); render_mode_index++)
            {
                const Shapes::Index_Meta_Data &index_meta_data_ref = index_meta_data_collection_ref[render_mode_index];
                glDrawElements(
                    index_meta_data_ref.m_render_mode,
                    index_meta_data_ref.m_num_indices_this_mode,
                    GL_UNSIGNED_SHORT,
                    (void *)(indices_drawn_so_far * sizeof(GLushort))
                    );

                indices_drawn_so_far += index_meta_data_ref.m_num_indices_this_mode;
            }
        }
        
        //??could this really be the problem??
        glBindVertexArray(0);

        // courteous cleanup
        glUseProgram(0);
    }
}


