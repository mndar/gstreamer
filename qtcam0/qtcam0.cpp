/*
 * qtcam0.cpp - XOverlay using GStreamer 0.10.36 with Qt
 * Copyright (c) 2014 Mandar Joshi (emailmandar at gmail)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "qtcam0.h"

int global_cam_winid;
GstBusSyncReply QtCam0::bus_sync_handler (GstBus * bus, GstMessage * message, gpointer data)
{
	// ignore anything but 'prepare-xwindow-id' element messages
	if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
		return GST_BUS_PASS;
	if (!gst_structure_has_name (message->structure, "prepare-xwindow-id"))
		return GST_BUS_PASS;

	if (global_cam_winid != 0) {
		GstXOverlay *xoverlay;
		// GST_MESSAGE_SRC (message) will be the video sink element
		xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
		gst_x_overlay_set_window_handle (xoverlay, global_cam_winid);
	} else {
		g_warning ("Should have obtained video_window_xid by now!");
	}

	gst_message_unref (message);
	return GST_BUS_DROP;
}

void QtCam0::start_pipeline() {
	gst_element_set_state (playbin2, GST_STATE_PLAYING);
}

void QtCam0::stop_pipeline() {
	gst_element_set_state (playbin2, GST_STATE_PAUSED);
}

void QtCam0::take_snap() {
	still_image_window->setPixmap(QPixmap::grabWindow(cam_winid));
}

void QtCam0::save_snap() {
	QPixmap pix_map = QPixmap::grabWindow(still_image_window->winId());
	pix_map.save(filename_entry->text());
}

QtCam0::QtCam0 (QWidget *parent): QWidget(parent) {

	start_button = new QPushButton ("Start Capture");
	take_snap_button = new QPushButton ("Take Snap");
	stop_button = new QPushButton ("Stop Capture");
	save_snap_button = new QPushButton ("Save Snap");
	filename_entry = new QLineEdit();
	filename_entry->setText("abc.png");
	
	cam_window = new QWidget(this);
	cam_window->setFixedWidth(640);
	cam_window->setFixedHeight(480);
	
	button_vbox = new QVBoxLayout;
	button_vbox->addWidget(start_button);
	button_vbox->addWidget(stop_button);
	button_vbox->addWidget(take_snap_button);
	button_vbox->addWidget(filename_entry);
	button_vbox->addWidget(save_snap_button);
		
	still_image_window = new QLabel();
	still_image_window->setFixedWidth(640);
	still_image_window->setFixedHeight(480);
	
	main_grid = new QGridLayout(this);
	main_grid->addWidget(cam_window,0,0,2,2);
	main_grid->addLayout(button_vbox, 0,2,1,1);
	main_grid->addWidget(still_image_window,0,3,2,2);	
	setLayout(main_grid);

	caps = gst_caps_new_simple("video/x-raw",
				   "format", G_TYPE_STRING, "I420",
				   "width", G_TYPE_INT, 640,
				   "height", G_TYPE_INT, 480,
				    NULL);
	playbin2 = gst_parse_launch ("v4l2src ! video/x-raw-yuv,width=640,height=480 ! xvimagesink", NULL);
	/*source = gst_element_factory_make ("v4l2src", "source");
	sink = gst_element_factory_make ("autovideosink", "sink");
	
	gst_bin_add_many (GST_BIN (playbin2), source, sink, NULL);
	//gst_element_link_filtered (source, sink, caps);
	gst_element_link (source, sink);*/
	
	cam_winid = cam_window->winId();
	global_cam_winid = cam_winid;
	GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (playbin2));
  	gst_bus_set_sync_handler (bus, (GstBusSyncHandler) bus_sync_handler, NULL);
  	gst_object_unref (bus);
  	
	connect(start_button, SIGNAL(clicked()), this, SLOT(start_pipeline()));
	connect(stop_button, SIGNAL(clicked()), this, SLOT(stop_pipeline()));
	connect(take_snap_button, SIGNAL(clicked()), this, SLOT(take_snap()));
	connect(save_snap_button, SIGNAL(clicked()), this, SLOT(save_snap()));
}

