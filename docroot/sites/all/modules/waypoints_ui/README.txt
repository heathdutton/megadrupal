Waypoints UI
==========

This module provides a user interface for adding jquery_waypoints settings to your site.
JQuery waypoints:

https://github.com/imakewebthings/waypoints
http://imakewebthings.com/waypoints

allows you to trigger javascript function(s) when a DOM element comes into the viewpoint
(e.g. when a user scrolls). A Drupal libraries module exists to include the waypoints
js library (version 1 only at this point), but there is no ui for quickly adding waypoints
functionality.

At this point the module just lets you specify a class that should toggle on an element
when the same or a different element enters the viewport. Future versions will let you
specify a js function to execute in addition to just toggling a class.

This module uses the waypoints module (https://www.drupal.org/project/waypoints) to add the
waypoints js library and adds some additional ui.

To Use:

1) Install as you would any other module
2) Navigate to /admin/config/user-interface/waypoints/ui
3) From here you can add as many waypoints as you like

Adding waypoints:

The ui lets you specify

1) Which dom element will trigger a waypoint (Waypoint Element Selector)
2) What class will toggle on the targeted element
3) Whether you want to toggle the class on the Waypoint Element, or on a different element
4) If a different element the you can specify the selector for the 'Toggle element'
5) The offset at which the waypoint will trigger. This is the % of the viewport at which
   the element must cross to trigger the waypoint. 0% == the top of the screen, 100%==
   the bottom of the screen. See http://imakewebthings.com/waypoints/api/offset-option/ for
   more info. At this point only % is supported but future versions will support more.

There is an example feature module included in this package called waypoints_ui_demo. Just enable
the module and see what the settings are in the ui. Then go to add a waypoints_ui_demo node
and the node page will show you how the waypoints work.

AUTHOR/MAINTAINER
======================
-Ted Benice, BTW Labs LLC <ted at bythewaylabs DOT com>