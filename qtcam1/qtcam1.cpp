/*
 * qtcam1.cpp - Video Overlay using GStreamer 1.0.5 with Qt
 * Copyright (c) 2014 Mandar Joshi (emailmandar at gmail)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "qtcam1.h"

guintptr QtCam1::cam_window_handle;

GstBusSyncReply QtCam1::bus_sync_handler (GstBus *bus, GstMessage *message, gpointer user_data)
{
	if (!gst_is_video_overlay_prepare_window_handle_message (message))
		return GST_BUS_PASS;

	GstVideoOverlay *overlay;
	overlay = GST_VIDEO_OVERLAY (GST_MESSAGE_SRC (message));
	gst_video_overlay_set_window_handle (overlay, cam_window_handle);

	gst_message_unref (message);
	return GST_BUS_DROP;
}

void QtCam1::start_pipeline() {
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void QtCam1::stop_pipeline() {
	gst_element_set_state (pipeline, GST_STATE_PAUSED);
}

void QtCam1::take_snap() {
	still_image_window->setPixmap(QPixmap::grabWindow(cam_winid));
}

void QtCam1::save_snap() {
	QPixmap pix_map = QPixmap::grabWindow(still_image_window->winId());
	pix_map.save(filename_entry->text());
}

QtCam1::QtCam1 (QWidget *parent): QWidget(parent) {

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

	cam_winid = cam_window->winId();
	
	caps = gst_caps_new_simple("video/x-raw",
				   "width", G_TYPE_INT, 640,
				   "height", G_TYPE_INT, 480,
				    NULL);
	pipeline = gst_pipeline_new ("pipeline");
	source = gst_element_factory_make ("v4l2src", "source");
	sink = gst_element_factory_make ("xvimagesink", "sink");
	
	gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
	gst_element_link_filtered (source, sink, caps);
	
	/*pipeline = gst_parse_launch ("v4l2src ! video/x-raw,width=640,height=480 ! xvimagesink", NULL);*/
	cam_window_handle = cam_winid;
	GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	gst_bus_set_sync_handler (bus, (GstBusSyncHandler) bus_sync_handler, NULL, NULL);
	gst_object_unref (bus);

	connect(start_button, SIGNAL(clicked()), this, SLOT(start_pipeline()));
	connect(stop_button, SIGNAL(clicked()), this, SLOT(stop_pipeline()));
	connect(take_snap_button, SIGNAL(clicked()), this, SLOT(take_snap()));
	connect(save_snap_button, SIGNAL(clicked()), this, SLOT(save_snap()));
}

