#include "Scene_Data.h"

#include "Utilities/My_Assert.h"
#include "Utilities/Typedefs.h"
#include "middleware/arcsynthesis/framework/rapidxml.hpp"
#include "middleware/arcsynthesis/framework/rapidxml_helpers.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory> // for std::shared_ptr
#include "Rendering/Renderer.h"
#include "Rendering/ParticleRenderer.h"

// for generating things from the file data
#include "Entities/Entity.h"
#include "Entities/Components/AABB_Component.h"
#include "Entities/Components/Physics_Component.h"
#include "Entities/Components/Controller_Component.h"
#include "Shapes/Shape_Data.h"
#include "Shapes/Geometry_Creation/Shape_Generator.h"
#include "Shapes/Geometry.h"
#include "Math/F_Quat.h"
#include "Math/F_Dual_Quat.h"

// for updating particle managers (they need delta time)
#include "Timing/Game_Clock.h"

// for loading an object from Blender3d
#include "Shapes/Shape_Data.h"
#include "Shapes/My_Vertex.h"
#include "Shapes/Index_Meta_Data.h"

#include <iostream>
using std::cout;
using std::endl;


// empty namespace for helper functions
namespace
{
    void helper_open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path)
    {
        // this function is heavily influenced by the arcsynthesis framework's Mesh 
        // class constructor, and some lines are copied verbatim except for the 
        // camel-case-to-underscore notaion change
        std::ifstream file_stream(file_path);
        if (!file_stream.is_open())
        {
            throw std::runtime_error("Could not find the mesh file: " + file_path);
        }

        //std::vector<char> file_data;

        // reserve memory
        // Note: Reserving memory ensures contiguency of memory, which allows the 
        // rapidxml parser to run through the data with just a pointer.
        file_data.reserve(2 * 1024 * 1024);
        file_data.insert(file_data.end(), std::istreambuf_iterator<char>(file_stream),
            std::istreambuf_iterator<char>());
        file_data.push_back('\0');

        try
        {
            (*put_parsed_xml_document_here).parse<0>(&(file_data[0]));
        }
        catch (rapidxml::parse_error &e)
        {
            // this is a special rapidxml exception, so handle it here
            std::cout << file_path << ": Parse error in the mesh file." << std::endl;
            std::cout << e.what() << std::endl << e.where<char>() << std::endl;
            throw;
        }
    }

    Shapes::Geometry *helper_load_geometry(const rapidxml::xml_node<> *geometry_node_ptr, Scene::Scene_Data *load_into_this_scene)
    {
        if (0 == geometry_node_ptr || 0 == load_into_this_scene)
        {
            // return a null pointer
            return 0;
        }

        // grab the child shape node out of the parent geometry node
        const rapidxml::xml_node<> *shape_node_ptr = geometry_node_ptr->first_node("shape");

        // pick apart the node to figure out what shape to make, and then make it
        std::string shape_type_str = rapidxml::get_attrib_string(*shape_node_ptr, "id");
        std::shared_ptr<Shapes::Shape_Data> new_shape_ptr(nullptr);
        Shapes::Geometry_Creation::Shape_Generator& shape_generator_ref = Shapes::Geometry_Creation::Shape_Generator::get_instance();
        float f_return_value_if_parameter_not_found = 0.0f;
        int i_return_value_if_parameter_not_found = 0;

        if ("triangle" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_triangle();
            cout << "loading triangle" << endl;
        }
        else if ("plane" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_plane(
                rapidxml::get_attrib_float(*shape_node_ptr, "width", f_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "length", f_return_value_if_parameter_not_found),
                rapidxml::get_attrib_int(*shape_node_ptr, "width_segments", i_return_value_if_parameter_not_found),
                rapidxml::get_attrib_int(*shape_node_ptr, "length_segments", i_return_value_if_parameter_not_found));
            cout << "loading plane" << endl;
        }
        else if ("box" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_box(
                rapidxml::get_attrib_float(*shape_node_ptr, "width", f_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "length", f_return_value_if_parameter_not_found));
            cout << "loading box" << endl;
        }
        else if ("circle" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_circle(
                rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found));
            cout << "loading circle" << endl;
        }
        else if ("cube" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_cube();
            cout << "loading cube" << endl;
        }
        else if ("cylinder" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_cylinder(
                rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found),
                rapidxml::get_attrib_int(*shape_node_ptr, "num_vertical_segments", i_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "height", f_return_value_if_parameter_not_found));

            cout << "loading cylinder" << endl;
        }
        else if ("sphere" == shape_type_str)
        {
            new_shape_ptr = shape_generator_ref.generate_sphere(
                rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
                rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found),
                rapidxml::get_attrib_int(*shape_node_ptr, "num_vertical_segments", i_return_value_if_parameter_not_found));

            cout << "loading sphere" << endl;
        }
        else
        {
            // not a valid shape
            cout << "'" << shape_type_str << "' is not recognized and cannot be generated" << endl;
            return 0;
        }

        // get the name of the geometry out of the geometry node
        std::string geometry_id_str = rapidxml::get_attrib_string(*geometry_node_ptr, "id");

        // make a new geometry
        // Note: If the geometry name already exists, then a 0 (null pointer) will be returned.
        // In this event, let the shape object die and return a 0 to the calling function.
        Shapes::Geometry *geometry_ptr = load_into_this_scene->new_geometry(new_shape_ptr, geometry_id_str);
        if (0 == geometry_ptr)
        {
            cout << "Geometry loading: geometry id '" << geometry_id_str << "' already exists." << endl;
        }

        return geometry_ptr;
    }

    Math::F_Dual_Quat helper_get_transform(const rapidxml::xml_node<> *transform_node_ptr)
    {
        if (0 == transform_node_ptr)
        {
            // bad!
            // TODO: do something about the bad
            cout << "no transform node" << endl;
        }

        if ("dual_quaternion" == rapidxml::get_attrib_string(*transform_node_ptr, "type"))
        {
            // "r" == "real", "d" == "dual"
            float ret_val_in_case_of_failure = 0.0f;
            float rw = rapidxml::get_attrib_float(*transform_node_ptr, "rw", ret_val_in_case_of_failure);
            float rx = rapidxml::get_attrib_float(*transform_node_ptr, "rx", ret_val_in_case_of_failure);
            float ry = rapidxml::get_attrib_float(*transform_node_ptr, "ry", ret_val_in_case_of_failure);
            float rz = rapidxml::get_attrib_float(*transform_node_ptr, "rz", ret_val_in_case_of_failure);
            float dw = rapidxml::get_attrib_float(*transform_node_ptr, "dw", ret_val_in_case_of_failure);
            float dx = rapidxml::get_attrib_float(*transform_node_ptr, "dx", ret_val_in_case_of_failure);
            float dy = rapidxml::get_attrib_float(*transform_node_ptr, "dy", ret_val_in_case_of_failure);
            float dz = rapidxml::get_attrib_float(*transform_node_ptr, "dz", ret_val_in_case_of_failure);

            return Math::F_Dual_Quat(
                Math::F_Quat(rw, glm::vec3(rx, ry, rz)),
                Math::F_Quat(dw, glm::vec3(dx, dy, dz)));
        }
        else
        {
            // ??support another type??
        }

        // return a null transform
        return Math::F_Dual_Quat();
    }

    bool helper_load_components(const rapidxml::xml_node<> *entity_node_ptr, Entities::Entity *load_into_this_entity_ptr, Scene::Scene_Data *store_components_here_ptr)
    {
        const rapidxml::xml_node<> *component_node_ptr = entity_node_ptr->first_node("component");
        while (0 != component_node_ptr)
        {
            std::string component_type_str = rapidxml::get_attrib_string(*component_node_ptr, "type");
            if ("physics" == component_type_str)
            {
                Entities::Physics_Component *physics_ptr = store_components_here_ptr->new_physics_component();
                float f_return_value_in_case_of_failure = 0.0;

                const rapidxml::xml_node<> *sustained_force_vector_node_ptr = component_node_ptr->first_node("sustained_force_vector");
                if (0 != sustained_force_vector_node_ptr)
                {
                    glm::vec3 force_vector(
                        rapidxml::get_attrib_float(*sustained_force_vector_node_ptr, "x", f_return_value_in_case_of_failure),
                        rapidxml::get_attrib_float(*sustained_force_vector_node_ptr, "y", f_return_value_in_case_of_failure),
                        rapidxml::get_attrib_float(*sustained_force_vector_node_ptr, "z", f_return_value_in_case_of_failure));

                    physics_ptr->add_sustained_force_vector(force_vector);
                }

                const rapidxml::xml_node<> *mass_node_ptr = component_node_ptr->first_node("mass");
                if (0 != mass_node_ptr)
                {
                    float mass = rapidxml::get_attrib_float(*mass_node_ptr, "value", f_return_value_in_case_of_failure);

                    physics_ptr->add_mass(mass);
                }

                // do NOT look for immediate force vectors because those are resolved 
                // frame by frame and therefore should not be in the file

                // add the component
                load_into_this_entity_ptr->add_component(physics_ptr);
            }
            else if ("AABB" == component_type_str)
            {
                // the AABB relies on the meta data for a shape, so start by looking 
                // for the geometry that this bounding box will cover
                std::string geo_id_str = rapidxml::get_attrib_string(*component_node_ptr, "geometry_id");
                Shapes::Geometry *geo_ptr = store_components_here_ptr->find_geometry(geo_id_str);
                if (0 == geo_ptr)
                {
                    // bad; bounding box wants geometry that doesn't exist (or that doesn't exist yet, which shouldn't happen because the geometry is supposed to load before the components)
                    return false;
                }

                Entities::AABB_Component *aabb_ptr = store_components_here_ptr->new_AABB_component();
                aabb_ptr->calculate_default_boundaries(geo_ptr->get_shape_meta_data());

                // add the component
                load_into_this_entity_ptr->add_component(aabb_ptr);
            }
            else if ("controller" == component_type_str)
            {
                Entities::Controller_Component *controller_ptr = store_components_here_ptr->new_controller_component();
                controller_ptr->set_key_binding(Input::SUPPORTED_BINDINGS::KEYBOARD);

                load_into_this_entity_ptr->add_component(controller_ptr);
            }
            else
            {
                //TODO: ??be more harsh with bad component construction
                cout << "component type '" << component_type_str << "' unknown" << endl;
            }

            // get the next component
            component_node_ptr = component_node_ptr->next_sibling("component");
        }

        return true;
    }
}

namespace Scene
{
    bool Scene_Data::Init(const std::string& file_path)
    {
        m_entity_ptrs.clear();
        m_geometry_ptrs.clear();

        //std::string file_path = "C:/Users/John/Documents/GitHub/Simple_Game_Engine_2/scene_save_exp.xml";

        // open the file
        rapidxml::xml_document<> doc;
        std::vector<char> file_data;
        helper_open_xml_file(&doc, file_data, file_path);

        if (!load_geometries(&doc))
        {
            return false;
        }

        if (!load_particle_management())
        {
            return false;
        }

        if (!load_entities(&doc))
        {
            return false;
        }

        if (!load_camera(&doc))
        {
            return false;
        }

        return true;
    }

    // call AFTER init or you'll have nothing
    void Scene_Data::ConfigureGeometryRenderer(Rendering::Renderer *pRenderer)
    {
        pRenderer->SetCameraToRender(&m_camera);
        for (size_t i = 0; i < m_entity_geometry_pairings.size(); i++)
        {
            const ENTITY_GEOMETRY_PAIRS &pairRef = m_entity_geometry_pairings[i];
            pRenderer->ConfigureNewRenderable(pairRef.first, pairRef.second);
        }
    }

    // call AGTER init or you'll have nothing
    void Scene_Data::ConfigureParticleRenderer(Rendering::ParticleRenderer *pRenderer)
    {
        // TODO: one of the following:
        // (1) make the particle renderer keep a collection of pointers to particle managers, 
        //  not unlike the geometry renderer keeping a collection of renderables
        // (2) Give the particle managers or renderers (or both) some kind of ID that describes 
        //  what kind of particles they render, thus allowing different kinds of particle 
        //  rendering while still letting the scene keep the particle emitters nicely contained 
        //  and updateable
        // Note: For now, just hard code this single particle manager.
        // Also Note: The get() function returns the raw pointer to the particle manager, which
        // is what the particle renderer expects.
        pRenderer->SetCameraToRender(&m_camera);
        pRenderer->SetParticleManager(m_particle_managers[0].get());
    }

    void Scene_Data::update()
    {
        for (uint index_counter = 0; index_counter < m_entity_ptrs.size(); index_counter++)
        {
            m_entity_ptrs[index_counter]->update();
        }

        for (size_t i = 0; i < m_particle_managers.size(); i++)
        {
            float deltaT = (float)Timing::Game_Clock::get_instance().get_delta_time_last_frame();
            m_particle_managers[i]->Update(deltaT);
        }

        m_camera.update();
    }

    bool Scene_Data::load_from_blender_obj(const std::string& file_path)
    {
        // save some dereferencing
        using namespace std;
        using namespace Shapes;

        // open the Blender object file and check that it is, in fact, an object file
        ifstream file_stream(file_path, ios::in);
        if (!file_stream.is_open())
        {
            cout << "Could not find the object file: " + file_path << endl;
            return false;
        }

        // first line should say what kind of Blender save file it is
        std::string line;
        std::getline(file_stream, line);
        size_t posOfSubstring = line.find("OBJ");
        if (posOfSubstring == -1)
        {
            cout << "File '" << file_path << "'is not a Blender OBJ file" << endl;
            return false;
        }

        std::string objName;
        std::vector<glm::vec3> vertPositions;
        // texture coordinates not supported (yet)
        std::vector<glm::vec3> vertNormals;

        // position, texture coord, and normal for each vertex
        std::vector<glm::uvec3> faceElements;

        // it is a Blender OBJ file, so now parse it line by line
        std::string objNameLineHeader("o ");
        std::string vertPosLineHeader("v ");
        std::string vertNormLineHeader("vn ");
        std::string index_line_header("f ");
        while (std::getline(file_stream, line))
        {
            if (line.substr(0, objNameLineHeader.length()) == objNameLineHeader)
            {
                // new object
                objName = line.substr(objNameLineHeader.length());
                cout << "loading " << objName << " from Blender3D obj file" << endl;
            }
            if (line.substr(0, vertPosLineHeader.length()) == vertPosLineHeader)
            {
                float vx = 0.0f;
                float vy = 0.0f;
                float vz = 0.0f;
                sscanf(line.substr(vertPosLineHeader.length()).c_str(), "%f %f %f", &vx, &vy, &vz);
                vertPositions.push_back(glm::vec3(vx, vy, vz));
                //printf("vertex line: '%s'\n", line.c_str());
                //printf("vertex parsed: vx = %f, vy = %f, vz = %f\n", vx, vy, vz);
            }

            else if (line.substr(0, vertNormLineHeader.length()) == vertNormLineHeader)
            {
                float vx = 0.0f;
                float vy = 0.0f;
                float vz = 0.0f;
                sscanf(line.substr(vertPosLineHeader.length()).c_str(), "%f %f %f", &vx, &vy, &vz);
                vertNormals.push_back(glm::vec3(vx, vy, vz));
                //printf("normal line: '%s'\n", line.c_str());
                //printf("normal parsed: vx = %f, vy = %f, vz = %f\n", vx, vy, vz);
            }
            else if (line.substr(0, index_line_header.length()) == index_line_header)
            {
                unsigned short v_index1 = 0;
                unsigned short vt_index1 = 0;
                unsigned short vn_index1 = 0;
                unsigned short v_index2 = 0;
                unsigned short vt_index2 = 0;
                unsigned short vn_index2 = 0;
                unsigned short v_index3 = 0;
                unsigned short vt_index3 = 0;
                unsigned short vn_index3 = 0;
                sscanf(line.substr(vertPosLineHeader.length()).c_str(), "%hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd",
                    &v_index1, &vt_index1, &vn_index1,
                    &v_index2, &vt_index2, &vn_index2,
                    &v_index3, &vt_index3, &vn_index3);
                if (vt_index1 == 0.0f || vt_index2 == 0.0f || vt_index2 == 0.0f)
                {
                    // no texture coordinates, so run the parse again
                    sscanf(line.substr(vertPosLineHeader.length()).c_str(), "%hd//%hd %hd//%hd %hd//%hd",
                        &v_index1, &vn_index1,
                        &v_index2, &vn_index2,
                        &v_index3, &vn_index3);
                }
                faceElements.push_back(glm::uvec3(v_index1, vt_index1, vn_index1));
                faceElements.push_back(glm::uvec3(v_index2, vt_index2, vn_index2));
                faceElements.push_back(glm::uvec3(v_index3, vt_index3, vn_index3));
                //printf("index line: '%s'\n", line.c_str());
                //printf("index triangle 1 parsed: v1 = %f, vt1 = %f, vn1 = %f\n", v_index1, vt_index1, vn_index1);
                //printf("index triangle 1 parsed: v2 = %f, vt2 = %f, vn2 = %f\n", v_index2, vt_index2, vn_index2);
                //printf("index triangle 1 parsed: v3 = %f, vt3 = %f, vn3 = %f\n", v_index3, vt_index3, vn_index3);
            }
        }

        // every single one of the vertex position-normal pairs in the faces in the OBJ file are
        // unique, so the only way forward is to make a unique vertex for each pair and make a 
        // new set of indices that go from 0 until the number of verts
        std::vector<My_Vertex> verts;
        std::vector<unsigned short> indices;
        int newIndicesCounter = 0;

        for (size_t faceIndices = 0; faceIndices < faceElements.size(); faceIndices++)
        {
            // SO MANY THINGS CALLED "INDEX"
            // Note: -1 because the indices in the OBJ file start at 1, not 0.
            short objVertPosIndex = faceElements[faceIndices].x - 1;
            //short objVertTexIndex = faceElements[faceIndices].y - 1;
            short objVertNorIndex = faceElements[faceIndices].z - 1;

            // create it
            My_Vertex newVert;
            newVert.position = vertPositions[objVertPosIndex];
            newVert.color = glm::vec3(0.75f, 0.25f, 0.25f);
            newVert.normal = vertNormals[objVertNorIndex];

            // put it into a vertex-index world specific for this sim
            verts.push_back(newVert);
            indices.push_back(newIndicesCounter);

            // next!
            newIndicesCounter++;
        }

        // create new storage structures that will persist beyond the function
        // Note: the std::vector<...> class is great for creating the vertices when it is not 
        // known ahead of time how many vertices will be needed.  But OpenGL uses arrays, so 
        // once all the vertices and indices are created, stuff them into arrays.
        std::shared_ptr<Shape_Data> pShape = std::make_shared<Shape_Data>();
        pShape->m_verts = new My_Vertex[verts.size()];
        pShape->m_indices = new unsigned short[indices.size()];

        // copy vertices from std::vector<...> to persistant array
        for (size_t vertCount = 0; vertCount < verts.size(); vertCount++)
        {
            pShape->m_verts[vertCount] = verts[vertCount];
        }
        pShape->m_num_verts = verts.size();

        // do the same for the indices
        for (size_t indicesCount = 0; indicesCount < indices.size(); indicesCount++)
        {
            pShape->m_indices[indicesCount] = indices[indicesCount];
        }
        pShape->m_num_total_indices = indices.size();

        // will tell OpenGL the number of vertices to summon for drawing and how to draw them
        Index_Meta_Data iMetaData(GL_TRIANGLES, indices.size());
        pShape->m_index_meta_data.push_back(iMetaData);

        //// finally, create a new renderable
        //Shape_Data *pShape =
        //Shapes::Geometry_Creation::Shape_Generator::get_instance().generate_cube();

        //std::string objName("CubeeMcCube");
        Shapes::Geometry *pGeometry = this->new_geometry(pShape, objName);
        Entities::Entity *pNewEntity = this->new_entity(objName);
        pNewEntity->m_where_and_which_way = Math::F_Dual_Quat(
            Math::F_Quat(1.0f, glm::vec3()),
            Math::F_Quat(0.0f, glm::vec3(0.0f, 2.0f, 0.0f)));
        //pNewEntity->m_where_and_which_way = Math::F_Dual_Quat(
        //    Math::F_Quat(0.83f, glm::vec3(-0.4f, 0.0f, 0.4f)),
        //    Math::F_Quat(-0.80f, glm::vec3(-0.65f, 2.86f, 1.00f)));

        this->m_entity_geometry_pairings.push_back(std::pair<const Entities::Entity *, const Shapes::Geometry*>(pNewEntity, pGeometry));

        //m_renderer_ptr->configure_new_renderable(pNewEntity, pGeometry);

        return true;
    }

    bool Scene_Data::save(const std::string& file_path)
    {
        return true;
    }

    Entities::Entity *Scene_Data::new_entity(const std::string& new_entity_id_str)
    {
        //TODO: check if the entity already exists, and if it does, ??do something??
        m_entity_ptrs.push_back(std::unique_ptr<Entities::Entity>(new Entities::Entity(new_entity_id_str)));

        // return a pointer to the newly created entity
        return m_entity_ptrs[m_entity_ptrs.size() - 1].get();
    }

    Entities::Physics_Component *Scene_Data::new_physics_component()
    {
        m_physics_components.push_back(std::unique_ptr<Entities::Physics_Component>(new Entities::Physics_Component));

        return m_physics_components[m_physics_components.size() - 1].get();
    }

    Entities::AABB_Component *Scene_Data::new_AABB_component()
    {
        m_AABB_components.push_back(std::unique_ptr<Entities::AABB_Component>(new Entities::AABB_Component));

        return m_AABB_components[m_AABB_components.size() - 1].get();
    }

    Entities::Controller_Component *Scene_Data::new_controller_component()
    {
        m_controller_components.push_back(std::unique_ptr<Entities::Controller_Component>(new Entities::Controller_Component));

        return m_controller_components[m_controller_components.size() - 1].get();
    }


    Shapes::Geometry *Scene_Data::new_geometry(const std::shared_ptr<Shapes::Shape_Data> new_shape_data_ptr, const std::string& new_geometry_id_str)
    {
        // check if the name is already taken
        if (0 != geometry_already_loaded(new_geometry_id_str))
        {
            // this name already exists, so don't load anything and let the shape instance go out of scope and die
            return 0;
        }

        // geometry doesn't exist, so create it
        // Note: I understand that this push_back(...) back call is long, but it is 
        // necessary in order to create a unique pointer to it.  The unique pointer
        // has no copy constructor, but unlike other objects, no destructor is called
        // if I pass in the constructor.  ??why? I won't argue??
        m_geometry_ptrs.push_back(std::unique_ptr<Shapes::Geometry>(new Shapes::Geometry(new_shape_data_ptr, new_geometry_id_str)));

        // return a pointer to the newly created geometry
        return m_geometry_ptrs[m_geometry_ptrs.size() - 1].get();
    }

    void Scene_Data::new_entity_geometry_pairing(const Entities::Entity *entity_ptr, const Shapes::Geometry *geo_ptr)
    {
        m_entity_geometry_pairings.push_back(std::pair<const Entities::Entity *, const Shapes::Geometry *>(entity_ptr, geo_ptr));
    }

    Entities::Entity *Scene_Data::find_entity(const std::string& entity_id_str)
    {
        for (uint index_counter = 0; index_counter < m_entity_ptrs.size(); index_counter++)
        {
            Entities::Entity *entity_ptr = m_entity_ptrs[index_counter].get();
            if (entity_id_str == entity_ptr->get_id())
            {
                return entity_ptr;
            }
        }

        return 0;
    }

    Shapes::Geometry *Scene_Data::find_geometry(const std::string& geometry_id_str)
    {
        for (uint index_counter = 0; index_counter < m_geometry_ptrs.size(); index_counter++)
        {
            Shapes::Geometry *geo_ptr = m_geometry_ptrs[index_counter].get();
            if (geometry_id_str == geo_ptr->m_id)
            {
                return geo_ptr;
            }
        }

        return 0;
    }

    Shapes::Geometry *Scene_Data::geometry_already_loaded(const std::string& geometry_id_str)
    {
        for (uint geometry_index_counter = 0; geometry_index_counter < m_geometry_ptrs.size(); geometry_index_counter++)
        {
            if (geometry_id_str == m_geometry_ptrs[geometry_index_counter]->m_id)
            {
                return m_geometry_ptrs[geometry_index_counter].get();
            }
        }

        return 0;
    }


    // PRIVATE
    bool Scene_Data::load_geometries(const rapidxml::xml_document<> *parsed_scene_doc)
    {
        // make sure that there is a "scene" root node (if not, let rapidxml blow up)
        rapidxml::xml_node<> *root_node_ptr = parsed_scene_doc->first_node("scene");

        // cycle through the list of geometries and generate them one by one
        const rapidxml::xml_node<> *geo_node_ptr = root_node_ptr->first_node("geometry");
        while (0 != geo_node_ptr)
        {
            // TODO:??do something with this pointer?
            Shapes::Geometry *new_geo_ptr = helper_load_geometry(geo_node_ptr, this);

            // get the next geometry in the node hierarchy
            geo_node_ptr = geo_node_ptr->next_sibling("geometry");
        }

        return true;
    }

    bool Scene_Data::load_particle_management()
    {
        m_particle_managers.push_back(std::unique_ptr<Particles::ParticleManager>(new Particles::ParticleManager()));
        m_particle_managers.back()->Init(50, 2, 2, 1, 2, glm::vec3(0.0f, 1.0f, 0.0f));

        return true;
    }

    bool Scene_Data::load_entities(const rapidxml::xml_document<> *parsed_scene_doc)
    {
        // make sure that there is a "scene" root node (if not, let rapidxml blow up)
        rapidxml::xml_node<> *root_node_ptr = parsed_scene_doc->first_node("scene");

        // cycle through the list of entities, generate them and their components, and
        // make renderables for them (if needed)
        const rapidxml::xml_node<> *entity_node_ptr = root_node_ptr->first_node("entity");
        while (0 != entity_node_ptr)
        {
            // before creating anything, check that there is a transform and that the 
            // entity has an id
            const rapidxml::xml_node<> *transform_node_ptr = entity_node_ptr->first_node("transform");
            std::string new_entity_id_str = rapidxml::get_attrib_string(*entity_node_ptr, "id");
            if (0 == transform_node_ptr || "" == new_entity_id_str)
            {
                // bad
                // TODO: ??be more elegant than this??
                cout << "load_entities(...): transform node doesn't exist or entity id is a blank string" << endl;
                return false;
            }

            // make a new entity
            Entities::Entity *new_entity_ptr = this->new_entity(new_entity_id_str);
            Math::F_Dual_Quat transform = helper_get_transform(transform_node_ptr);
            new_entity_ptr->m_where_and_which_way = transform;

            // check if it has a geometry, and if it does, make a renderable
            const rapidxml::xml_node<> *geo_node_ptr = entity_node_ptr->first_node("geometry");
            if (0 != geo_node_ptr)
            {
                std::string geo_id_str = rapidxml::get_attrib_string(*geo_node_ptr, "id");
                Shapes::Geometry *geo_ptr = this->find_geometry(geo_id_str);
                if (0 == geo_ptr)
                {
                    // bad; needs a geometry that doesn't exist
                    // TODO: ??delete the entity??
                    cout << "load_entities(...): geometry with id '" << geo_id_str << "' doesn't exist" << endl;
                    return 0;
                }

                // add a new pairing to the scene so that it can be saved later
                this->m_entity_geometry_pairings.push_back(std::pair<const Entities::Entity *, const Shapes::Geometry*>(new_entity_ptr, geo_ptr));

                // add a renderable for this entity-geometry pairing
                //m_renderer_ptr->configure_new_renderable(new_entity_ptr, geo_ptr);
            }

            // check for components and load any that are found
            // Note: This process requires a lot of condition checks, so I thought that
            // it would be a good choice to spin it off into a dedicated helper function.
            if (!helper_load_components(entity_node_ptr, new_entity_ptr, this))
            {
                // bad; something went wrong with component loading (TODO:??be more specific??)
                return false;
            }

            if (!new_entity_ptr->initialize())
            {
                // bad (??do something else??)
                return false;
            }

            // get the next entity in the node hierarchy
            entity_node_ptr = entity_node_ptr->next_sibling("entity");
        }

        return true;
    }

    bool Scene_Data::load_camera(const rapidxml::xml_document<> *parsed_scene_doc)
    {
        // make sure that there is a "scene" root node (if not, let rapidxml blow up)
        rapidxml::xml_node<> *root_node_ptr = parsed_scene_doc->first_node("scene");

        // look for the cameras and link them up with their corresponding entities
        // Note: For the foreseeable future, I'll only have one camera.
        const rapidxml::xml_node<> *camera_node_ptr = root_node_ptr->first_node("camera");
        while (0 != camera_node_ptr)
        {
            // ??does the camera really need a unique ID??
            std::string camera_id_str = rapidxml::get_attrib_string(*camera_node_ptr, "id");
            m_camera.set_id(camera_id_str);

            std::string followed_entitiy_id = rapidxml::get_attrib_string(*camera_node_ptr, "followed_entity_id");
            Entities::Entity *followed_entity_ptr = this->find_entity(followed_entitiy_id);

            if (0 == followed_entity_ptr)
            {
                // bad
                // ??kill anything? set camera id to blank again??
                return false;
            }

            m_camera.set_entity_to_follow(followed_entity_ptr);

            camera_node_ptr = camera_node_ptr->next_sibling("camera");
        }

        return true;
    }
}