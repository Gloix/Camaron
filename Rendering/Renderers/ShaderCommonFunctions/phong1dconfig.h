#ifndef PHONG1DCONFIG_H
#define PHONG1DCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
namespace Ui {
class Phong1DConfig;
}
struct LightInfo1D {
	public:
		glm::vec4 Position; // Light position in eye coords.
		float La; // Ambient light intensity
		float Ld; // Diffuse light intensity
		float Ls; // Specular light intensity
};
struct MaterialInfo1D {
	public:
		float Ka; // Ambient reflectivity
		float Kd; // Diffuse reflectivity
		float Ks; // Specular reflectivity
		float Shininess; // Specular shininess factor
};
class Phong1DConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit Phong1DConfig(QWidget *parent = 0);
		~Phong1DConfig();
		void readConfig();
		MaterialInfo1D material;
		LightInfo1D light;
		
	private:
		Ui::Phong1DConfig *ui;
};

#endif // PHONG1DCONFIG_H
