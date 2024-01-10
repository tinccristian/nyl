#include"shader.h"

#include "Log.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		// Log the error if the file couldn't be opened
		NYL_CORE_ERROR("Failed to open file: {0}", filename);
		return ""; // Return an empty string to signify failure
	}

	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();

	return contents;
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Check Vertex Shader Compilation Status
	GLint vertexCompiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
	if (vertexCompiled == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::vector<GLchar> vertexLog(logLength);
			glGetShaderInfoLog(vertexShader, logLength, nullptr, vertexLog.data());

			// Log or print the compilation error for the vertex shader
			std::string errorStr(vertexLog.data(), logLength);
			NYL_CORE_ERROR("Vertex shader compilation error: {0}", errorStr);
		}
		else
		{
			NYL_CORE_ERROR("Vertex shader compilation error: Unknown error occurred.");
		}
	}



	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);                         // Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);                           // Attach Fragment Shader source to the Fragment Shader Object
	glGetError();                                                                       // This will clear any previous errors
	glCompileShader(fragmentShader);                                                    // Compile the Fragment Shader into machine code

	GLint fragmentCompiled;                                                             // Check the compilation status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);

	if (fragmentCompiled != GL_TRUE) {                                                  // Check if compilation was successful
		GLint infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<GLchar> fragmentInfoLog(infoLogLength + 1);                         // Retrieve the error log
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &fragmentInfoLog[0]);
		printf("Fragment shader compilation error: %s\n", &fragmentInfoLog[0]);         // Output the error log (you can replace this with your logging system)
	}

	ID = glCreateProgram();                                                             // Create Shader Program Object and get its reference
	
	glAttachShader(ID, vertexShader);                                                   // Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);                                                                  // Wrap-up/Link all the shaders together into the Shader Program

	#pragma region Check_Shader_Program_Linking_Status
	GLint programLinked;
	glGetProgramiv(ID, GL_LINK_STATUS, &programLinked);
	if (programLinked != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::vector<GLchar> programLog(logLength);
			glGetProgramInfoLog(ID, logLength, nullptr, programLog.data());

			// Log or print the linking error for the shader program
			std::string errorStr(programLog.data(), logLength);
			NYL_CORE_ERROR("Shader program linking error: {0}", errorStr);
		}
		else
		{
			NYL_CORE_ERROR("Shader program linking error: Unknown error occurred.");
		}
	}
	#pragma endregion

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}