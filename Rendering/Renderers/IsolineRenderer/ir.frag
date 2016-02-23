#version 400

smooth in float scalarValue;
in float slope;

uniform float[50] IsolinesSteps;
uniform int IsolinesStepsN;
uniform vec4 GradientStartColor;
uniform vec4 GradientEndColor;

out vec4 FragColor;

void main()
{
    for(int i=0; i<IsolinesStepsN;i++) {
        if(abs(scalarValue-IsolinesSteps[i]) < 0.0003*slope) {
            float colorMult = (IsolinesSteps[i]-IsolinesSteps[0])/(IsolinesSteps[IsolinesStepsN-1]-IsolinesSteps[0]);
            FragColor = colorMult*(GradientEndColor-GradientStartColor)+GradientStartColor;
            //FragColor = vec4(1.0,0.0,0.0,1.0);
            return;
        }
    }
    //FragColor = vec4(clamp(vec3(1.0)*slope,0.0,1.0),1.0);
    FragColor = vec4(0.0,0.0,0.0,0.0);
}
