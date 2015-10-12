Panels - Why So Slow?
---------------------
This module adds a new Timer renderer that tracks how long it takes each pane to
render and displays the information. This can be very helpful for figuring out
which content in a panel is causing a page to be slow to render.

When enabled, the module will automatically modify the standard pipeline to use
the new renderer, i.e. as soon as the module is enabled, anywhere the standard
renderer is used will start showing timing information for all users.

Warning: Don't enable this module on a production server!


Features
------------------------------------------------------------------------------
The primary features include:

* Supports most of the Panels ecosystem - Panels pages, Panelizer, Panels
  Everywhere.
  Note: does not currently support Mini Panels.

* Indicates on the page how long each pane takes to render, in milliseconds.

* No additional permissions needed to control the output, once it's enabled all
  users will be able to see the information.


Configuration
------------------------------------------------------------------------------
The only way to configure the module is via variables:

  panels_why_so_slow_enabled - Simple boolean to turn on/off the functionality.
    Defaults to TRUE.

  panels_why_so_slow_skip_cache - Disables any Panels caching, to see the raw
    performance numbers on each pane. Defaults to FALSE.

  panels_why_so_slow_not_good - If a pane takes these many milliseconds to
    render, or more, it will be considered "not good" and be given a yellow
    background pattern. Defaults to 15, i.e. 15 milliseconds.

  panels_why_so_slow_really_bad - If a pane takes these many milliseconds to
    render, or more, it will be considered "really bad" and be given a red
    background pattern. Defaults to 100, i.e. 100 milliseconds.


Credits / Contact
------------------------------------------------------------------------------
Originally written by Andrew "drewish" Morton [1]. Currently maintained by
Damien McKenna [2].

Development is currently sponsored by Mediacurrent [3].

The best way to contact the author is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/panels_why_so_slow


References
------------------------------------------------------------------------------
1: https://www.drupal.org/u/drewish
2: https://www.drupal.org/u/damienmckenna
3: http://www.mediacurrent.com/
