#ifndef EDITORMAINWINDOW_HPP_INCLUDED
#define EDITORMAINWINDOW_HPP_INCLUDED

#include "ui_mainwindow.hpp"

class EditorMainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		EditorMainWindow(QWidget *parent = 0); 
		bool openMap(char *file);
	
	public slots:
		void openRequested();
		void saveRequested();

	private:
		Ui::MainWindow ui;
		hex::camera *camera_;
		hex::gamemap *map_;
		bool opened_;
};

#endif
