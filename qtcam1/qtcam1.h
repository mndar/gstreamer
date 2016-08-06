/*
 * qtcam1.h - Video Overlay using GStreamer 1.0.5 with Qt
 * Copyright (c) 2014 Mandar Joshi (emailmandar at gmail)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <QtWidgets>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

class QtCam1: public QWidget {
	Q_OBJECT
	public:
		QtCam1 (QWidget *parent=0);
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
		GstElement *pipeline, *source, *sink;
	  	GstBus *bus;
		GstCaps *caps;
		static GstBusSyncReply bus_sync_handler (GstBus *, GstMessage *, gpointer);
		static guintptr cam_window_handle;
	private slots:
		void start_pipeline();
		void stop_pipeline();
		void take_snap();
		void save_snap();
};

