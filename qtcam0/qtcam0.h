/*
 * qtcam0.h - XOverlay using GStreamer 0.10.36 with Qt
 * Copyright (c) 2014 Mandar Joshi (emailmandar at gmail)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <QtGui>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

class QtCam0: public QWidget {
	Q_OBJECT
	public:
		QtCam0 (QWidget *parent=0);
	private:
		QGraphicsScene *scene;
		QWidget *cam_window;
		QLabel *still_image_window;
		QPushButton *start_button, *take_snap_button, *stop_button, *save_snap_button;
		QHBoxLayout *main_hbox;
		QVBoxLayout *button_vbox;
		QGridLayout *main_grid;
		QLineEdit *filename_entry;
		WId cam_winid;		
		GstElement *playbin2, *source, *sink;
	  	GstBus *bus;
		GstCaps *caps;
		static GstBusSyncReply bus_sync_handler (GstBus * bus, GstMessage * message, gpointer data);
	private slots:
		void start_pipeline();
		void stop_pipeline();
		void take_snap();
		void save_snap();
};

