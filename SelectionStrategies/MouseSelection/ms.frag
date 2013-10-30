#version 400

flat in uint fcolor;
out uint outputColor;

void main()
{
    //outputColor = outputColor|fcolor;
    outputColor = fcolor;
}
