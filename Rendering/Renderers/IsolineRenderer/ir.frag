#version 400

smooth in float scalarValue;

uniform float[50] IsolinesSteps;
uniform int IsolinesStepsN;
uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;

out vec4 FragColor;

void main()
{
    for(int i=0; i<IsolinesStepsN;i++) {
        if(abs(scalarValue-IsolinesSteps[i]) < 0.05) {
            FragColor = vec4(1.0,0.0,0.0,1.0);
            return;
        }
    }
    FragColor = vec4(1.0,1.0,1.0,1.0);
}
