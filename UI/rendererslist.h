#ifndef RENDERERSLIST_H
#define RENDERERSLIST_H

#include <QWidget>
#include <QListWidgetItem>
namespace Ui {
class RenderersList;
}
class Renderer;
class RenderersList : public QWidget
{
		Q_OBJECT
		
	public:
		explicit RenderersList(QWidget *parent = 0);
		~RenderersList();
		void addRenderer(Renderer*);
		void removeRenderer(unsigned int r);
		void removeAllRenderers();
		std::vector<Renderer*>& getRenderers();
	protected:
		void keyPressEvent(QKeyEvent* event);
	signals:
		void secondaryRendererRemoved();
	private:
		std::vector<Renderer*> renderers;
		Ui::RenderersList *ui;
};

#endif // RENDERERSLIST_H
