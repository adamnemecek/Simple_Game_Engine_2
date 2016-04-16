#include "Renderer.h"

#include <Utilities/My_Assert.h>
#include <Utilities/Shader_Maker.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

// for glm::perspective
#include <Utilities/Include_Helpers/GLM_Mat_Transform.h>
#include <Utilities/Include_Helpers/GL_Version.h>

// include this last to avoid errors when this includes gl.h 
// (apparently this needs to be included after the glm and glload and other libraries)
#include <GL/freeglut.h>

#include <Shapes\Geometry.h>
#include <Rendering\Camera.h>

#include <Utilities\Printer_Helper.h>
#include <iostream>
using std::cout;
using std::endl;


namespace Rendering
{
    Renderer::Renderer()
        :
        m_camera_ptr(0),
        _renderProgramId(0),
        m_unif_loc_model_to_camera_matrix(0),
        m_unif_loc_camera_to_clip_matrix(0),
        m_unif_loc_light_1_pos_cs(0),
        m_unif_loc_light_1_intensity(0),
        m_unif_loc_light_2_pos_cs(0),
        m_unif_loc_light_2_intensity(0)
    {
    }
    bool Renderer::initialize(const GLuint programId)
    {
        _renderProgramId = programId;

        std::string uniform_name_str = "unif_model_to_camera_matrix";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_model_to_camera_matrix)) 
        { glUseProgram(0); return false; }

        uniform_name_str = "unif_camera_to_clip_matrix";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_camera_to_clip_matrix)) 
        { glUseProgram(0); return false; }

        uniform_name_str = "unif_light_1_pos_cs";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_light_1_pos_cs)) 
        { glUseProgram(0); return false; }

        uniform_name_str = "unif_light_1_intensity";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_light_1_intensity)) 
        { glUseProgram(0); return false; }

        uniform_name_str = "unif_light_2_pos_cs";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_light_2_pos_cs)) 
        { glUseProgram(0); return false; }

        uniform_name_str = "unif_light_2_intensity";
        if (!find_uniform(_renderProgramId, uniform_name_str, &m_unif_loc_light_2_intensity)) 
        { glUseProgram(0); return false; }

        m_num_current_renderables = 0;

        return true;
    }

    bool Renderer::shutdown()
    {
        return true;
    }

    void Renderer::configure_new_renderable(const Entities::Entity *e_ptr, const Shapes::Geometry *g_ptr)
    {
        MY_ASSERT(m_num_current_renderables != m_MAX_RENDERABLES);
        MY_ASSERT(0 != g_ptr && 0 != e_ptr);
        Renderable &r = m_renderables[m_num_current_renderables++];
        r.set_geometry(g_ptr);
        r.set_entity(e_ptr);
    }

    void Renderer::set_viewport(GLsizei width, GLsizei height)
    {
        glViewport(0, 0, width, height);

        float fov_radians = (1.0f / 4.0f) * 3.14159f;   // 1/2 pi
        float aspect_ratio = (float)width / height;
        float near_plane_dist = 0.1f;
        float far_plane_dist = 100.0f;
        m_perspective_mat = glm::perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);

        cout << "resizing to aspect ratio " << aspect_ratio << endl;
    }

    void Renderer::set_camera_to_render(Camera *camera_ptr)
    {
        MY_ASSERT(camera_ptr != 0);
        m_camera_ptr = camera_ptr;
    }

    void Renderer::render_scene()
    {
        // we can't render without a camera
        MY_ASSERT(m_camera_ptr != 0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(_renderProgramId);

        // for each renderable:
        // - bind the geometry VAO
        // - send "full transform" and "orientation only" matrices to GPU
        // - draw elements 

        // the camera position is independent of each renderable
        glm::mat4 camera_mat = m_camera_ptr->get_world_to_view_matrix();

        // the lights are independent of each renderable
        // Note: The light locations that are hard-coded here are in world space, so I only need
        // to transform them with the camera matrix to get them into camera space.
        glm::vec3 light_1_location = glm::vec3(camera_mat * glm::vec4(+5.0f, +6.0f, -7.0f, 1.0f));
        float light_1_intensity = 90.0f;
        glm::vec3 light_2_location = glm::vec3(camera_mat * glm::vec4(-5.0f, +3.0f, +5.0f, 1.0f));
        float light_2_intensity = 30.0f;
        glUniform3fv(m_unif_loc_light_1_pos_cs, 1, glm::value_ptr(light_1_location));
        glUniform1f(m_unif_loc_light_1_intensity, light_1_intensity);
        glUniform3fv(m_unif_loc_light_2_pos_cs, 1, glm::value_ptr(light_2_location));
        glUniform1f(m_unif_loc_light_2_intensity, light_2_intensity);

        glm::mat4 m = m_perspective_mat;
        // the camera-to-clip matrix is independent of each renderable
        glUniformMatrix4fv(m_unif_loc_camera_to_clip_matrix, 1, GL_FALSE, glm::value_ptr(m_perspective_mat));

        for (uint renderable_count = 0; renderable_count < m_num_current_renderables; renderable_count++)
        {
            Renderable &r = m_renderables[renderable_count];
            glBindVertexArray(r.get_VAO_ID());

            glm::mat4 model_to_camera = camera_mat * r.get_model_to_world_matrix();

            glUniformMatrix4fv(m_unif_loc_model_to_camera_matrix, 1, GL_FALSE, glm::value_ptr(model_to_camera));

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
                GLenum err = glGetError();
                if (GL_NO_ERROR != err)
                {
                    break;
                }

                indices_drawn_so_far += index_meta_data_ref.m_num_indices_this_mode;
            }
        }

        // courteous cleanup
        glUseProgram(0);
    }


    // Private functions

    // throws an "out of range" exception if the uniform's location is < 0
    bool Renderer::find_uniform(const GLuint program_ID, const std::string &name, GLint *put_location_here)
    {
        GLint temp_location = glGetUniformLocation(program_ID, name.c_str());
        if (temp_location < 0)
        {
            printf("find_uniform(...): could not find uniform '%s' in program '%u'; the requested uniform is either mispelled or unused in the program", name.c_str(), program_ID);
            *put_location_here = -1;
            return false;
        }

        *put_location_here = temp_location;

        return true;
    }

    // throws an "out of range" exception if the attr location is < 0
    bool Renderer::find_attribute(const GLuint program_ID, const std::string &name, GLint *put_location_here)
    {
        GLint temp_location = glGetAttribLocation(program_ID, name.c_str());
        if (temp_location < 0)
        {
            printf("find_uniform(...): could not find uniform '%s' in program '%u'; the requested uniform is either mispelled or unused in the program", name.c_str(), program_ID);
            *put_location_here = -1;
            return false;
        }

        *put_location_here = temp_location;

        return true;
    }
}


