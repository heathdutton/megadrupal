The Whiteboard module can be used to draw by dragging the mouse inside the canvas.

The following DIV attributes are not necessary to draw, but are required to view saved whiteboard:

1) position
2) overflow
3) left
4) top
5) width
6) height
7) background-color
8) z-index

These attributes will be elimintated by the default Drupal filter. 'Full HTML' allows whiteboard to be saved but should be used with caution.

Specifying file upload size is recommended as this value can be over 1 MB for large whiteboard.

Enable JavaScript in browser for full functionality.

Demonstration viewable at http://demo.tetragy.com/whiteboard

INSTALLATION

1. Download whiteboard module in zip or tar format from Drupal whiteboard project website: http://drupal.org/project/whiteboard

2. Decompress the tarball in the recommended path sites/all/modules.

3. Download and unzip jsDraw2D library. Recommended location: sites/all/libraries/jsDraw2D/jsDraw2D.js. For your convenience, the URL of the library is given here:

http://jsdraw2d.jsfiction.com/

4. Install the module from admin/build/modules. Check that installation success messages are printed.

5. Go to admin/settings/whiteboard. Specify the location of the library. You should also set whiteboard upload size restrictions there.

6. Adjust filters and permissions as mentioned above.

Thank you for your interest in the Whiteboard module. Please send comments and requests for support to the issue queue.

UNINSTALL

1. Go to admin/modules. Deselect the Whiteboard module, and then press 'Save Configuration'.

2. Go to admin/modules/uninstall. Select the Whiteboard module, and submit the form.

--
Tetragy Limited

http://tetragy.com
