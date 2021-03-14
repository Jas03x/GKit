#ifndef GK_COLLADA_PARSER_HPP
#define GK_COLLADA_PARSER_HPP

#include <gk/3d/common/string.hpp>
#include <gk/core/importer/collada_format.hpp>
#include <gk/core/io/xml.hpp>
#include <gk/core/utility/string_buffer.hpp>

namespace Collada
{
    class Parser
    {
    public:
        typedef std::map<std::string, Geometry*>    GeometryLibrary;
        typedef std::map<std::string, Controller*>  ControllerLibrary;
        typedef std::map<std::string, VisualScene*> SceneLibrary;
        typedef std::map<std::string, Image*>       ImageLibrary;

    private:
        bool m_status;

        const XML* m_file;
        
        StringBuffer m_strbuf;

        std::vector<float> m_float_buffer;
        std::vector<unsigned short> m_ushort_buffer;
        std::vector<std::string> m_string_buffer;

        AssetInfo m_info;

        GeometryLibrary m_geometry_library;
        ControllerLibrary m_controller_library;
        SceneLibrary m_scene_library;
        ImageLibrary m_image_library;

    private:
        const XML::Node* find_child(const XML::Node* node, const std::string& name);
        const XML::ChildList* find_children(const XML::Node* node, const std::string& name);
        const std::string* find_attribute(const XML::Node* node, const std::string& name);
        
        void parse_float_array(const std::string& text);
        void parse_ushort_array(const std::string& text);
        void parse_name_array(const std::string& text);

        Source* read_source(const XML::Node* node);

        void read_asset_info(const XML::Node* node);
        void read_input(const XML::Node* node, Input& input);
        void read_vertex_array(const XML::Node* node, VertexArray* array);
        void read_param(const XML::Node* node, Param& param);
        void read_accessor(const XML::Node* node, Accessor& accessor);
        void read_technique(const XML::Node* node, Technique& technique);
        void read_triangle_array(const XML::Node* node, TriangleArray* array);
        void read_mesh(const XML::Node* node, Mesh& mesh);
        void read_joints(const XML::Node* node, Joints& joints);
        void read_vertex_weights(const XML::Node* node, VertexWeights& weights);
        void read_skin(const XML::Node* node, Skin& skin);
        void read_geometry(const XML::Node* node);
        void read_controller(const XML::Node* node);
        void read_extra_data(const XML::Node* node, Extra& extra);
        void read_node(const XML::Node* node, Node* parent, VisualScene* scene);
        void read_visual_scene(const XML::Node* node);
        void read_image(const XML::Node* node);

        void read_geometry_library(const XML::Node* node);
        void read_controller_library(const XML::Node* node);
        void read_visual_scene_library(const XML::Node* node);
        void read_image_library(const XML::Node* node);
        
        void process(const XML::Node* root);

    public:
        Parser();
        ~Parser();

        bool parse(const char* path);

        const AssetInfo& GetAssetInfo();
        const GeometryLibrary& GetGeometryLibrary();
        const ControllerLibrary& GetControllerLibrary();
        const SceneLibrary& GetSceneLibrary();
        const ImageLibrary& GetImageLibrary();
    };
}

#endif // GK_COLLADA_PARSER_HPP
