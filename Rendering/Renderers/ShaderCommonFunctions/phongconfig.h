#ifndef PHONGCONFIG_H
#define PHONGCONFIG_H

#include <QWidget>
#include <glm/glm.hpp>
namespace Ui {
class PhongConfig;
}
struct LightInfo {
	public:
		glm::vec4 Position; // Light position in eye coords.
		glm::vec3 La; // Ambient light intensity
		glm::vec3 Ld; // Diffuse light intensity
		glm::vec3 Ls; // Specular light intensity
};
struct MaterialInfo {
	public:
		glm::vec3 Ka; // Ambient reflectivity
		glm::vec3 Kd; // Diffuse reflectivity
		glm::vec3 Ks; // Specular reflectivity
		float Shininess; // Specular shininess factor
};
class PhongConfig : public QWidget
{
		Q_OBJECT
		
	public:
		explicit PhongConfig(QWidget *parent = 0);
		~PhongConfig();
		void readConfig();
		MaterialInfo material;
		LightInfo light;
	private:
		Ui::PhongConfig *ui;
};

#endif // PHONGCONFIG_H
