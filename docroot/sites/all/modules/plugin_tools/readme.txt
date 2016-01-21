About
-----

Plugin tools is a small developer/administrator utility which allows you to
browse the various plugins provided by different modules. This information is
normally hidden in the code layer. The aim is to provide some useful, extra
tools to developers and interested maintainers.

This may be desirable for several reasons:

- To see which modules are providing plugins
- To check and debug plugins during development

The Drupal 7 version of this module works with Chaos Tools plugins. A possible
future Drupal 8 version will utilise core plugins.

Writing compatible plugins
--------------------------

The aim of Plugin tools is to merely wrap existing plugin paradigms without
requiring additions to each plugin, however Plugin tools does expect plugin
settings arrays to provide some common keys...

name: The machine name of the plugin. Used when another title cant be found
title: Used in the Plugin tools UI as the Plugin's title. If not found, 'label',
    will also be tried, before falling back on the machine name.
description: If provided, will be used as the description in the Plugin tools UI

Using the API
-------------

A module can add a small plugin dictionary to its own configuration screens by
using the following:

<?php
if (module_exists('plugin_tools')) {
  $output = plugin_tools_render_overview($my_module, $my_plugin_type)
}
?>