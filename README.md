# QtQuickPlayer
===============
A simple application demonstrating integration of gstreamer witin QML/QtQuick.
Uses qmlglsink from gst-plugins-bad to render video inside a QML application

# Prerequisites

Qt5, gstreamer, gstreamer-plugins-bad, possibly others. On Mac OS X w/macports
```
sudo port install qt5 qt5-creator \
                  gstreamer1 gstreamer1-gst-libav gstreamer1-gst-plugins-good
sudo port install -k gstreamer1-gst-plugins-bad +qt (*)
```

gst-plugins-bad requires special attention. 

First, macports needs to include qt support, which is accomplished specifying 
+qt as a variant. 

Second, qmlglsink is broken in gst-plugins-bad 1.8.2 so it needs to be patched
around a little. This could be done by telling macports to leave the build 
folder after it is complete, using the -k switch, then manually patching the 
source and rebuilding libgstqtsink.so:

```
cd /opt/local/var/macports/build/_opt_local_var_macports_sources_rsync.macports.org_release_tarballs_ports_gnome_gstreamer1-gst-plugins-bad/gstreamer1-gst-plugins-bad/work/gst-plugins-bad-1.8.2/ext/qt
sudo sed -i '' 's/GST_GL_HAVE_PLATFORM_COCOA//' qtitem.cc
sudo make
sudo cp .libs/libgstqtsink.so /opt/local/lib/gstreamer-1.0/libgstqtsink.so
```

If build fails finding gstgldisplay_cocoa.h, adjust it's path like so:
```
-#include <gst/gl/coaoa/gstgldisplay_cocoa.h>
+#include "../../gst-libs/gst/gl/cocoa/gstgldisplay_cocoa.h"
```

This seems to have already been fixed in upstream gstreamer-1.9.1 and hopefully
this  workaround will not be necessary in the future.

# Building

If all the prerequisites are correctly installed, just do:
```
qmake
make
```
Execute and enjoy ;)

