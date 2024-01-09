//#include<Renderer.h>
//
//namespace Nyl
//{
//
//	glGenBuffers(1, &vertex_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//	glCompileShader(vertex_shader);
//
//	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//	glCompileShader(fragment_shader);
//
//	program = glCreateProgram();
//	glAttachShader(program, vertex_shader);
//	glAttachShader(program, fragment_shader);
//	glLinkProgram(program);
//
//	mvp_location = glGetUniformLocation(program, "MVP");
//	vpos_location = glGetAttribLocation(program, "vPos");
//	vcol_location = glGetAttribLocation(program, "vCol");
//
//	glEnableVertexAttribArray(vpos_location);
//	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//		sizeof(vertices[0]), (void*)0);
//	glEnableVertexAttribArray(vcol_location);
//	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//		sizeof(vertices[0]), (void*)(sizeof(float) * 2));
//}