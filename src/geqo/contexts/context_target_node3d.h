#pragma once
#include "../contexts/query_context3d.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
using namespace godot;

class ContextTargetNode3D : public QueryContext3D {
	GDCLASS(ContextTargetNode3D, QueryContext3D)

private:
	Node3D *target_node = nullptr;
	NodePath target_node_path {};

public:
	ContextTargetNode3D() {}
	~ContextTargetNode3D() {}

	void _ready() override;

	void set_target_node_path(NodePath target);
	NodePath get_target_node_path() { return target_node_path; }

	Array get_context(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};
