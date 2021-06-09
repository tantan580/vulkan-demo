#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor; //输入变量不一定必须使用相同的名称，它们将使用location指令指定的索引链接在一起
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor; //修饰符指定帧缓冲区的索引, 0代表帧缓冲区

void main() {
    outColor = vec4(texture(texSampler, fragTexCoord).rgb, 0.5);
}
