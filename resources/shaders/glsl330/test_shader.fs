// #version 330
//
//
// out vec4 fragColor; // Final pixel color
//
// uniform sampler2D texture0; // Main texture (provided by Raylib)
// uniform vec4 tintColor; // Tint color (provided by Raylib)
//
// void main() {
//     vec4 texColor = texture(texture0, fragTexCoord);
//     fragColor = texColor * tintColor;
// }

#version 330

in vec2 fragTexCoord; // Received from vertex shader
out vec4 fragColor; // Final pixel color

void main() {
    // fragColor = vec4(0.0, 1.0, 0.0, 0.5); // Solid green color (RGBA)
    if ((fragTexCoord.x / 10) % 5 == 0) {
        fragColor = vec4(0.0, 1.0, 0.0, 0.5); // Solid green color (RGBA)
    } else {
        fragColor = vec4(0.0, 1.0, 0.0, 0.5); // Solid green color (RGBA)
        // fragColor = vec4(1.0, 0.0, 0.0, 0.5); // Solid green color (RGBA)
    }
}
