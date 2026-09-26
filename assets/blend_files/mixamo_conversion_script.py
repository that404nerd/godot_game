import bpy

# =========================================================
# Mixamo -> Blender 5.2 Animation Normalizer
#
# IMPORTANT:
# - BACK UP YOUR .BLEND FIRST.
# - Select the ARMATURE before running.
# - Assumes a uniform armature scale such as 0.1 or 0.01.
# =========================================================

armature = bpy.context.active_object

if armature is None:
    raise RuntimeError("No active object.")

if armature.type != 'ARMATURE':
    raise RuntimeError(
        f"Active object must be an ARMATURE, got {armature.type}"
    )

# ---------------------------------------------------------
# Get original armature scale
# ---------------------------------------------------------

scale = armature.scale.copy()

if not (
    abs(scale.x - scale.y) < 1e-6 and
    abs(scale.y - scale.z) < 1e-6
):
    raise RuntimeError(
        f"Armature scale must be uniform. Current scale: {tuple(scale)}"
    )

scale_factor = scale.x

if abs(scale_factor) < 1e-8:
    raise RuntimeError("Armature scale is effectively zero.")

for action in bpy.data.actions:

    print(f"Processing Action: {action.name}")

    for layer in action.layers:

        for strip in layer.strips:

            # We only expect keyframe strips here.
            if not hasattr(strip, "channelbags"):
                continue

            for channelbag in strip.channelbags:

                # Copy list because we're going to remove curves.
                fcurves = list(channelbag.fcurves)

                for fcurve in fcurves:

                    # Only process bone location curves.
                    if "location" not in fcurve.data_path:
                        continue

                    # -------------------------------------------------
                    # Keep Hips location animation
                    # -------------------------------------------------

                    if "Hips" in fcurve.data_path:

                        for keyframe in fcurve.keyframe_points:

                            # keyframe.co:
                            #   [0] = frame
                            #   [1] = value
                            #
                            # We ONLY modify the value.
                            keyframe.co[1] *= scale_factor

                        fcurve.update()

                    # -------------------------------------------------
                    # Remove location animation from other bones
                    # -------------------------------------------------

                    else:
                        channelbag.fcurves.remove(fcurve)


# ---------------------------------------------------------
# Clear pose-bone location offsets
# ---------------------------------------------------------

for pose_bone in armature.pose.bones:
    pose_bone.location = (0.0, 0.0, 0.0)


# ---------------------------------------------------------
# Apply armature scale
# ---------------------------------------------------------

if armature.mode != 'OBJECT':
    bpy.ops.object.mode_set(mode='OBJECT')

bpy.ops.object.select_all(action='DESELECT')

armature.select_set(True)
bpy.context.view_layer.objects.active = armature

bpy.ops.object.transform_apply(
    location=False,
    rotation=False,
    scale=True
)
