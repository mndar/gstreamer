/*
 * main.cpp - XOverlay using GStreamer 0.10.36 with Qt
 * Copyright (c) 2014 Mandar Joshi (emailmandar at gmail)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include <QApplication>
#include "qtcam0.h"

int main (int argc, char *argv[]) {
	gst_init (&argc, &argv);
	QApplication app (argc, argv);
	QtCam0 widget;
	//widget.resize(1500,700);
	widget.show();	
	return app.exec();
}

