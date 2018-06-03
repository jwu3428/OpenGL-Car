#ifndef OBJLOADER_H
#define OBJLOADER_H

class vec3 {
	public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class vec2 {
	public:
	GLfloat x;
	GLfloat y;
};

bool loadOBJ(
	const char *path,
	std::vector<vec3> &out_vertices,
	std::vector<vec2> &out_uvs,
	std::vector<vec3> &out_normals
);

#endif
