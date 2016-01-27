CONTENTS
--------

  * Introduction
  * Using The Grid
  * Design Decisions


INTRODUCTION
------------

Golden Grid is an HTML5 base theme implementation, adapted from Joni Korpi's
Golden Grid System. Designed to be as lightweight as possible while adhering to
Drupal's standards and best practices, it provides themers looking to rapidly
create fluid, responsive designs in Drupal with a possible starting point.

This theme divides the viewport into columns that fold into each other as
resolution decreases: from sixteen, to eight, and finally to four columns. It
should be noted that browsers that do not support media queries (such as dated
versions of Internet Explorer) will always revert to the mobile layout.

Furthermore, Golden Grid solves the problem of drifting content as the screen
is being resized through the use of elastic gutters. By declaring gutters in
ems rather than in relation to the viewport, they always stay proportional to
the content as they are being resized.

Finally, the Golden Grid theme establishes all of its vertical measurements in 
terms of ems, allowing the baseline grid to be zoomed at certain resolutions to
improve readability without breaking vertical rhythm. The "Golden Gridlet"
baseline grid overlay (packaged with the original Golden Grid System) will be
made available to developers and themers as a separate module.

Development of this theme is sponsored by the Experimental Media and Performing
Arts Center at Rensselaer Polytechnic Institute.


USING THE GRID
--------------

Golden Grid, like the boilerplate it was based on, uses a series of 18 folding
columns in its design. The first and last columns are reserved for the grid's
margins, which leaves 16 columns to work with.

As resolution decreases, these columns fold into each other, down to 8 for
common tablet resolutions and 4 for common mobile resolutions. It would be a
mistake, however, to call these "tablet" and "mobile" layouts, as the whole 
point of responsive design is to accommodate any device of any resolution and
orientation according to its own capabilities.

This is best illustrated in the Golden Gridlet helper module, available
at http://drupal.org/sandbox/davidwatson/1301684, which provides a customizable
baseline grid overlay in which these columns can be seen collapsing as the
resolution changes.

Golden Grid is split into five CSS files:

  - baseline.css: CSS defining the particulars of the baseline grid.
  - reset.css: A CSS reset, dealing with inconsistencies in browser defaults.
  - default.css: 4-column layout that doesn't use a media query. Processed by
    IE6-8 and devices with a low resolution.
  - medium.css: 8-column layout, displayed at >= 45ems (720px) by default.
  - wide.css: 16-column layout, displayed at >= 117ems (1872px) by default.

The percentage widths of each column are noted at the top of each CSS file.

When the grid system's elastic gutters are desired around an element, simply
apply the .wrapper class to them in your markup. This can be done in any number
of ways, the easiest being to apply them to entire containers in your theme's
template overrides (contained in /templates). Using the elastic gutters in
browsers that support them (safe in all browsers that support media queries)
allows for percentage-based containers with padding defined in ems, preventing
the perception of content "drifting" as resolutions change.

It should be noted that these CSS files are loaded in succession, so as the
resolution increases, each CSS file is processed with all of the rules of those
declared before. Therefore, it is prudent to do as much layout as possible in
default.css, and make minor changes in medium.css and wide.css as appropriate.


DESIGN DECISIONS
----------------

Q:  Why isn't the media query info file syntax followed? Why is it defined in
    the CSS instead?

A:  It is possible to add stylesheets with media queries in the following form:

    stylesheets[screen and (max-width: 45em)][] = medium.css

    However, any additional stylesheets added in this manner in the subtheme
    will always appear *after* those added in the base theme, even if they were
    overridden in the subtheme. To avoid any confusion on this point, themers
    are encouraged to stick to the convention outlined in the base theme for the
    time being, and define media queries directly in CSS.

Q:  IE6-8 receives the default (mobile) layout. Why?
A:  In short, because they don't understand media queries. The layout (assuming
    a mobile-first strategy) should still be functional in IE6-8, but will not
    be identical to that in a modern browser. Given how dated these browsers
    are, this was deemed acceptable in the original Golden Grid System, and that
    design decision was carried over here.

Q:  What about Drupal's default CSS?
A:  Much of Drupal's default CSS is disabled by default, in an effort to give
    the themer maximum control over the layout. This can be changed in
    template.php's hook_css_alter() if so desired.
