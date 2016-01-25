Viewport Classes
****************

This module provides an easy way for module/theme developers to assign class 
attribute values to the body element of all pages on a site based on the 
available user agent (web browser) viewport width.

The most obvious use for this is for using these class names in CSS selectors
that determinine the number of columns in a flexible-width layout (eg. three 
columns on large viewports, two columns on smaller ones, a single column on 
low-resolution mobile devices). You might also want to adjust other properties 
such as margins, font size, etc.

Obviously this module depends on the user agent supporting JavaScript (and 
jQuery), so it's best to have a sensible default style as well.

Usage
-----

Install and enable the module as usual. The module provides a hook that 
can be implemented in either a module or theme, eg:

/**
 * Implementation of hook_viewport_classes().
 */
function mymodule_viewport_classes() {
  return array(
    'mymodule-small' => array(
      'max width' => 799,
    ),
    'mymodule-medium' => array(
      'min width' => 800,
      'max width' => 1023,
    ),
    'mymodule-large' => array(
      'min width' => 1024,
    ),
  );
}

Or in a theme (template.php would be a sensible place to put this), like any 
theme function override:

/**
 * Implementation of hook_viewport_classes().
 */
function phptemplate_viewport_classes() {
  return array(
    'mytheme-small' => array(
      'max width' => 799,
    ),
    'mytheme-medium' => array(
      'min width' => 800,
      'max width' => 1023,
    ),
    'mytheme-large' => array(
      'min width' => 1024,
    ),
  );
}

It's best to prefix class names with the name of your module or theme to 
avoid namespace collisions (class names are not just set, but also unset based 
on viewport width - so it's not wise to set a class name called 'front' or 
'sidebar-first' in a Zen-based theme for instance). On the other hand if you 
want to override the values for a class set elsewhere, normal theming override 
rules apply; subthemes override base themes, which override theme engines, 
which override modules.

Omitting a value for 'min width' is equivalent to a value of zero, omitting 
'max width' or setting it to zero treats the maximum as infinite.
