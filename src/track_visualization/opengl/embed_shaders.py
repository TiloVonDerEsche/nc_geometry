import os

def convert_to_c_string(glsl_path, var_name):
    lines = []
    with open(glsl_path, 'r') as f:
        for line in f:
            # Escape backslashes and quotes, strip newline
            escaped = line.replace('\\', '\\\\').replace('"', '\\"').rstrip('\r\n')
            lines.append(f'    "{escaped}\\n"')

    joined_lines = "\n".join(lines)
    return f"const char* {var_name} =\n{joined_lines};\n"

def main():
    vs_code = convert_to_c_string("vertex.glsl", "vertexShaderSource")
    fs_code = convert_to_c_string("fragment.glsl", "fragmentShaderSource")

    header_content = f"""// ==========================================
// AUTO-GENERATED FILE. DO NOT EDIT DIRECTLY.
// Generated from vertex.glsl and fragment.glsl
// ==========================================

#ifndef SHADERS_EMBEDDED_H
#define SHADERS_EMBEDDED_H

{vs_code}
{fs_code}

#endif // SHADERS_EMBEDDED_H
"""

    with open("shaders_embedded.h", "w") as f:
        f.write(header_content)
    print("SUCCESS: Shaders embedded into shaders_embedded.h")

if __name__ == "__main__":
    main()
