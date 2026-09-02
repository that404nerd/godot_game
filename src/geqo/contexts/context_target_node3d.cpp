#include "../contexts/context_target_node3d.h"

void ContextTargetNode3D::_ready()
{
	target_node = get_node<Node3D>(target_node_path);
}

void ContextTargetNode3D::set_target_node_path(NodePath target) {
	target_node_path = target;
}

Array ContextTargetNode3D::get_context(Ref<QueryInstance3D> query_instance) {
	Array result = Array();
	if (target_node == nullptr) {
		print_error("ContextTargetNode3D: No target node.");
		return Array();
	}
	result.append(target_node);
	return result;
}
void ContextTargetNode3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_node_path"), &ContextTargetNode3D::get_target_node_path);
	ClassDB::bind_method(D_METHOD("set_target_node_path", "target"), &ContextTargetNode3D::set_target_node_path);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_node_path"), "set_target_node_path", "get_target_node_path");
}