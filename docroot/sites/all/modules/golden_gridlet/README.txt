CONTENTS
--------

  * Introduction
  * Using Golden Gridlet
  * Design Decisions


INTRODUCTION
------------

Golden Gridlet is a helper module intended to work in tandem with the Golden
Grid base theme, available at http://drupal.org/project/golden_grid. Golden
Grid, a responsive, HTML5 base theme, uses a series of 18 folding columns in
its design. The first and last columns are reserved for the grid's margins,
which leaves 16 columns to work with. These columns collapse into 8 columns at
common tablet resolutions, and 4 columns on resolutions commonly seen on mobile
devices.

Packaged with the original Golden Grid System project, this module provides a 
customizable, JavaScript-driven baseline grid overlay in which these columns
can be seen collapsing as the resolution changes. This allows themers to see
whether or not their subtheme conforms to the baseline grid at various display
resolutions.

Development of this module is sponsored by the Experimental Media and
Performing Arts Center at Rensselaer Polytechnic Institute.


USING THE GRIDLET
-----------------

Golden Gridlet is intended to be a development and theming aid, and as such,
should not be enabled on production sites. While enabled, it will display a
grid toggle button in the upper right corner of the screen. Clicking this
button will toggle the visibility of the grid overlay.

If you have enabled the module and cannot see the grid toggle, one of two
things needs to change: either the current user does not have permission to
view the overlay (that is, they lack the "View Golden Gridlet" permission), or
the color matches the background and must be changed in the administrative
settings.

The settings for this module can be found in admin/config/development/gridlet,
where the user may change the dimensions of the grid, the width of the elastic
gutters, and the colors and opacity of the grid itself. Please note that this
only applies to the overlay, and should reflect any changes you have made (or
intend to make) in your subtheme in order to be useful.


DESIGN DECISIONS
----------------

Q:  Why use the variable table to hold the dimensions of the grid?

A:  In a word, flexibility. First of all, keeping them in variable would allow
    themers to customize their instance of Golden Gridlet to suit their own
    individual subthemes. What's more, these settings may be exported via
    Strongarm for inclusion in a feature, or hardcoded using variable_set() in
    an installation profile should the need arise.

Q:  Why is the base font size in pixels, while all other units are in ems?

A:  This is how Golden Grid itself operates: in the event the base font size is
    increased, the grid overlay itself will expand. If the need arises to think
    in pixels, simply multiply the base font size by the measurement in ems.
