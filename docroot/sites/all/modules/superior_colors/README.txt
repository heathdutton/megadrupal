This module allows the user to set site colors using a colorpicker interface

Available at admin/config/media/colors

The module supports both themes and modules.

Look at the examples in the 'example_theme' and 'example_module' folders.

This documentation is for themes but does also apply for modules.

The color selection form is based on the colors defined in the .info file for
the theme, and any defined colors are inherited from parent base themes, but
clashing color names are overwritten by the selected theme.

Colors and color groups are defined like this:

  superior_colors[color_groups][page]=Page
  superior_colors[color_groups][content]=Content
  superior_colors[colors][content][content_summary_text][name]=Content Summary Text
  superior_colors[colors][content][content_summary_text][value]=444444
  superior_colors[colors][page][page_body_text][name]=Page body text
  superior_colors[colors][page][page_body_text][value]=000000

Groups have a key (machine name) and a name

Colors are placed in groups (based on groups' machine names) and have a machine
name, a name and a default value. All values have to be 6 hexadecimal digits,
WITHOUT the #. 3-digit colors like FFF are not accepted and should be written
as FFFFFF.

To use the colors on the page, a color temlate style sheet has to be created.

In the theme .info file, add a line with the path of the color_css template,
and use the theme name as key 

  superior_colors[color_css][themename][]=css/colortemplate.css

This will add the file "css/colortemplate.css" from the theme "themename" to
the list of color templates, that are added as extra CSS to the page views.

You will also have to specify a type which could be either "theme" or "module"
and allows the system find the path of the CSS file:

  superior_colors[color_css][themename][type]=theme

or

  superior_colors[color_css][themename][type]=module

In the color template, custom colors can be referred like this:

  .node-event .event-properties .event-property {
    border-color:[[content_event_property_border]];
  }

  .node-event .event-properties .field-label{
    color: [[content_event_properties_label_text]];
  }

Remember the double square brackets, and leave out the # - it will be added
automatically. 

To alter the data before generating the CSS file, use this hook:

  hook_superior_colors_generate_alter(&$colors)

IMPORTANT:

This module requires the 'jquery_colorpicker' module to be installed - which
requires the colorpicker library from http://www.eyecon.ro/colorpicker/ to be
installed in order to work.

To save your own time, use the supplied make file to download it all, by typing
the following command from the root of your site (make sure sites/default is
writable):

  drush make sites/all/modules/superior_colors/superior_colors.make --no-core

And then enable the module. This requires drush and drush make to be installed.
You may adjust the path to match the actual module location.