#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include <vector>
#include <memory>   // for shared_ptr<...>
#include <Utilities\Typedefs.h>

// for storing the shape's data and meta data
#include <Shapes\Shape_Data.h>
#include <Shapes\Shape_Meta_Data.h>
#include <Shapes\Index_Meta_Data.h>

// for cleaning up openGL objects that are unique for each geometry object
#include <Utilities\Include_Helpers\GL_Version.h>

namespace Shapes
{
    // this is a data storage class for the renderer
    class __declspec(dllexport) Geometry
    {
    public:
        // makes a local copy of the shape data pointer, set up the 
        // openGL binding data, and analyze it to create meta data
        Geometry(const std::shared_ptr<Shape_Data> new_shape_data_ptr,
            const std::string& new_geometry_id_str);

        // public destructor for cleaning up when this object is destroyed
        ~Geometry()
        {
            // smart pointers delete themselves

            glDeleteVertexArrays(1, &m_VAO_ID);
            glDeleteBuffers(1, &m_vertex_buffer_ID);
            glDeleteBuffers(1, &m_element_buffer_ID);
        }

        // get the Vertex Array Object ID for the VAO that holds this geomety's vertices 
        // and indices and attribute configurations
        const GLuint get_VAO_ID() const;

        // get a constant reference to the shape's meta data (min/max on all axes, 
        // center of vertices)
        const Shape_Meta_Data& get_shape_meta_data() const;

        // get a constant reference to the collection of index meta data contained 
        // within the shape data (draw mode, number of indices to draw with this mode)
        // ??is there a way to provide access to this collection without requiring this header to include <vector>??
        const std::vector<Index_Meta_Data>& get_index_meta_data_list() const;

        // just make it public for now
        // TODO: ??make it private with getter and setter gates??
        std::string m_id;

    private:
        // hide the default constructor so that the user has to supply shape data
        Geometry() {}

        // private helper functions
        // Note: These are non-static private methods because they must have access to 
        // non-static private members.  Prevously, these were static methods in the 
        // Geometry_Loader class and took a pointer to a Geometry instance.  They should 
        // not be static now that they are methods of the Geometry class, and they should
        // also not take a Geometry argument pointer for the same reason.
        void initialize_attributes();
        void calculate_shape_meta_data();

        // these will be cleaned up by the destructor
        GLuint m_VAO_ID;
        GLuint m_vertex_buffer_ID;
        GLuint m_element_buffer_ID;

        // the vertex and index data of the shape
        const std::shared_ptr<Shape_Data> m_shape_data_ptr;

        // shape-specific meta data, like min/max on X, Y, and Z
        Shape_Meta_Data m_shape_meta_data;
    };
}

#endif
