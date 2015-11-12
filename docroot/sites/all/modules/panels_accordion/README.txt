DRUPAL panels_accordion README.txt
==============================================================================

This module adds an accordion style setting to Panels 2, more specifically
Mini Panels.

A JavaScript accordion hides all but the active area that a user clicked on.
When the user clicks the heading of an area it activates that area, the
rest will be hidden.

You need to be using Panels 2 and enable Mini Panels (for now).

  1. Go ahead and build your mini panel, fill it with 2 or more panes.
     ( http://your-domain.com/admin/structure/mini-panels ). 
  2. Click on "Display Settings"
  3. Choose "Accordion" from dropdown
  4. Click "Edit Style settings" and Select your "Action", "Effect", and "Speed"
  5. Save
  6. Build a Panel Page that contains your Mini Panel, style it with some CSS.

**NOTE**
1) Make sure your Content in the mini panels has a title.
   You have the option to override that title when building the Panel Page
2) By default first pane inside accordion mini panel would be in expanded mode. You 
   can turn off by selecting "Collapsed by default" option in pane options => Style.
   Make sure you have applied Accordion style for pane.

***What is New in Drupal 7 Version ***   
The Drupal 7 version now supports:-
 1. Default panel pane markup
 2. Configurable header tags e.g h2, h3 .. h6
 3. Default & Custom CSS classes for panel panes. (Better control on accordion elements)
 
 ***Future Implementations?***
 Add your ideas in Project Issue Queue: 
 https://drupal.org/project/issues/panels_accordion
