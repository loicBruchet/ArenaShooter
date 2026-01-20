bl_info = {
    "name": "Auto Scene Export for Game Engine",
    "author": "SHARKstudio, ChatGPT",
    "version": (1, 2),
    "blender": (4, 5, 0),
    "location": "View3D > Sidebar > Scene Export",
    "description": "Automatically exports the scene to a game-engine-readable format on save",
    "category": "Import-Export",
}

import bpy
import json
import os


# -------------------------------------------------------------------
# Addon Preferences
# -------------------------------------------------------------------

class SceneExportAddonPreferences(bpy.types.AddonPreferences):
    bl_idname = __name__

    file_extension: bpy.props.StringProperty(
        name="Scene File Extension",
        description="Exported scene extension (json, scene, level, etc.)",
        default="scene"
    )

    def draw(self, context):
        layout = self.layout
        layout.label(text="Global Scene Export Settings")
        layout.prop(self, "file_extension")


def get_addon_prefs():
    return bpy.context.preferences.addons[__name__].preferences


def get_scene_extension():
    prefs = get_addon_prefs()
    ext = prefs.file_extension.strip().lower()
    if not ext.startswith("."):
        ext = "." + ext
    return ext


# -------------------------------------------------------------------
# Utilities
# -------------------------------------------------------------------

def get_custom_properties(obj):
    """
    Returns a dictionary of custom properties for the given object.
    Filters out unsupported properties and avoids errors related to IDProperties.
    """
    props = {}
    if hasattr(obj, "keys"):  # Check if the object has keys() method (i.e. it's a data block with properties)
        for prop in obj.keys():
            try:
                if prop != "_RNA_UI":  # Avoid internal Blender UI properties
                    props[prop] = obj[prop]
            except Exception as e:
                # Skip any properties that can't be accessed
                print(f"Skipping property {prop} on object {obj.name}: {e}")
    return props


def export_mesh_data(obj):
    if obj.type != "MESH":
        return None

    # Get evaluated mesh (modifiers applied)
    deps = bpy.context.evaluated_depsgraph_get()
    eval_obj = obj.evaluated_get(deps)
    mesh = eval_obj.to_mesh()

    # Force triangulation for consistent indices
    mesh.calc_loop_triangles()

    vertices = []
    indices = []
    uvs = []
    normals = []

    # Fetch attributes
    uv_layer = mesh.uv_layers.active.data if mesh.uv_layers.active else None
    normal_attr = mesh.attributes.get("normal")

    idx_counter = 0

    for tri in mesh.loop_triangles:
        for corner in range(3):
            loop_index = tri.loops[corner]
            vert_index = tri.vertices[corner]

            # POSITION
            v = mesh.vertices[vert_index].co
            vertices.append([float(v.x), float(v.y), float(v.z)])

            # NORMAL (loop normal, stored as attribute in Blender 4.x)
            if normal_attr and normal_attr.domain == 'CORNER':
                n = normal_attr.data[loop_index].vector
                normals.append([float(n.x), float(n.y), float(n.z)])
            else:
                n = mesh.vertices[vert_index].normal
                normals.append([float(n.x), float(n.y), float(n.z)])

            # UV
            if uv_layer:
                uv = uv_layer[loop_index].uv
                uvs.append([float(uv.x), float(uv.y)])
            else:
                uvs.append([0.0, 0.0])

            indices.append(idx_counter)
            idx_counter += 1

    eval_obj.to_mesh_clear()

    return {
        "vertices": vertices,
        "indices": indices,
        "uvs": uvs,
        "normals": normals,
        "custom_properties": get_custom_properties(obj)
    }



def get_scene_filename():
    blend_path = bpy.data.filepath
    if not blend_path:
        return None

    base = os.path.splitext(os.path.basename(blend_path))[0]
    return base + get_scene_extension()


def export_engine_scene(directory):
    scene_data = {"entities": []}

    for obj in bpy.context.scene.objects:
        rot = obj.matrix_world.to_quaternion()

        # Store position, rotation, and scale under "transform"
        scene_data["entities"].append({
            "name": obj.name,
            "entity_type": obj.type,
            "transform": {
                "position": list(obj.matrix_world.to_translation()),
                "rotation": [rot.x, rot.y, rot.z, rot.w],
                "scale": list(obj.matrix_world.to_scale())
            },
            "parent": obj.parent.name if obj.parent else None,
            "material": obj.active_material.name if obj.active_material else None,
            "properties": get_custom_properties(obj),
            "mesh": export_mesh_data(obj)
        })

    filename = get_scene_filename()
    if not filename:
        print("Scene export failed: .blend file has not been saved yet.")
        return

    full_path = os.path.join(directory, filename)

    try:
        with open(full_path, "w") as f:
            json.dump(scene_data, f, indent=4)
        print("Scene exported to:", full_path)
    except Exception as e:
        print(f"Error writing the scene data to file: {e}")


# -------------------------------------------------------------------
# Property Group
# -------------------------------------------------------------------

class SceneExportSettings(bpy.types.PropertyGroup):
    export_dir: bpy.props.StringProperty(
        name="Export Directory",
        description="Folder where the scene file is saved",
        subtype="DIR_PATH"
    )


# -------------------------------------------------------------------
# UI Panel
# -------------------------------------------------------------------

class VIEW3D_PT_scene_export(bpy.types.Panel):
    bl_label = "Scene Export"
    bl_idname = "VIEW3D_PT_SCENE_EXPORT"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "Game Scene"

    def draw(self, context):
        layout = self.layout
        settings = context.scene.scene_export_settings

        layout.prop(settings, "export_dir")

        name = get_scene_filename()
        if name:
            layout.label(text=f"Scene file: {name}")
        else:
            layout.label(text="Save the .blend file first")

        if settings.export_dir:
            layout.label(text="Using custom export directory")
        else:
            layout.label(text="Using .blend directory")


# -------------------------------------------------------------------
# Auto Export Handler
# -------------------------------------------------------------------

def auto_export_scene(dummy):
    settings = bpy.context.scene.scene_export_settings
    blend_path = bpy.data.filepath

    if not blend_path:
        print("Scene export skipped: file not saved")
        return

    if settings.export_dir and os.path.exists(settings.export_dir):
        export_dir = settings.export_dir
    else:
        export_dir = os.path.dirname(blend_path)

    try:
        export_engine_scene(export_dir)
    except Exception as e:
        print("Scene export failed:", e)


# -------------------------------------------------------------------
# Registration
# -------------------------------------------------------------------

classes = (
    SceneExportAddonPreferences,
    SceneExportSettings,
    VIEW3D_PT_scene_export,
)


def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.Scene.scene_export_settings = bpy.props.PointerProperty(type=SceneExportSettings)

    # Register the save_post handler to trigger scene export on save
    if auto_export_scene not in bpy.app.handlers.save_post:
        bpy.app.handlers.save_post.append(auto_export_scene)


def unregister():
    if auto_export_scene in bpy.app.handlers.save_post:
        bpy.app.handlers.save_post.remove(auto_export_scene)

    del bpy.types.Scene.scene_export_settings

    for cls in reversed(classes):
        bpy.utils.unregister_class(cls)


if __name__ == "__main__":
    register()
