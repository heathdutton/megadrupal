This theme is a "Responsive Base Theme" that was inspired by the 960 Grid 
system created by Nathan Smith and various articles on responsive design by 
Ethan Marcotte and others.
http://960.gs/
http://sonspring.com/journal/960-grid-system
http://www.alistapart.com/articles/responsive-web-design/

The existing Drupal 960 theme has also been a source of inspiration.
http://drupal.org/project/ninesixty

The idea behind Detamo is to design your layout on three separate grids, one 
each for Desktop, Tablet and Mobile. Media queries are used to switch between 
grids at the appropriate points and as each grid is fluid the layout will also 
scale within each grid.

This is a pragmatic approach to quickly achieving a responsive layout. The 
theme is lightweight and requires the page template(s) to be re-written as part
of creating the sub-theme. Detamo is not a base theme that is configured 
through the admin!

The three grids have the following (nominal) configurations:
Desktop: 960px - 12 60px columns with 10px gutters on left and right
Tablet: 640px - 8 60px columns with 10px gutters on left and right
Mobile: 320px - 4 60px columns with 10px gutters on left and right
Notice that the columns are the same size on all grids but there are fewer 
columns as the width reduces.

As an example consider a typical 3 column layout with a wide content area and 
two sidebars. (This example will be easier to follow if you have some 
understanding of how the original 960 system works).

On a desktop the layout might be something like:

  |         main content area             |    sidebar 1    |    sidebar 2    |
  |            7 columns                  |    3 columns    |    2 columns    |
  (12 columns in total)

 On a tablet this could be as follows:

  |         main content area                  |
  |             8 columns                      |      (8 columns in total)
  +------------------------------------------+
  |    sidebar 1          |      sidebar 2     |
  |    4 columns          |      4 columns     |      (8 columns in total)

 Whereas on mobile it might be:

  |  main content area    |
  |    4 columns          |      (4 columns in total)
  +-----------------------+
  |    sidebar 1          |
  |    4 columns          |      (4 columns in total)
  +-----------------------+
  |    sidebar 2          |
  |    4 columns          |      (4 columns in total)


Using the original 960 grid system, to achieve the desktop layout, the markup 
would include the classes as follows:
for the content - class="grid-7"
for sidebar 1 - class="grid-3"
for sidebar 2 - class="grid-2"

Using Detamo the desktop layout would simply be:
for the content - class="desktop-7"
for sidebar 1 - class="desktop-3"
for sidebar 2 - class="desktop-2"

Adding the tablet and mobile classes would give:
for the content - class="desktop-7 tablet-8 mobile-4"
for sidebar 1 - class="desktop-3 tablet-4 mobile-4"
for sidebar 2 - class="desktop-2 tablet-4 mobile-4"

When the media queries switch between the grids, the typography can also be 
changed so text size etc can be tweaked as required.

The "push and pull" classes of the 960 system are also available to alter the 
presentation order of the content. This would allow the desktop layout above 
to have the more standard presentation order: 
 | sidebar 1  |    content     | sidebar 2 |

Push and pull classes work separately on each grid so to swap the first two 
columns on the desktop the classes would then be:
for the content - class="desktop-push-3 desktop-7 tablet-8 mobile-4"
for sidebar 1 - class="desktop-pull-7 desktop-3 tablet-4 mobile-4"
for sidebar 2 - class="desktop-2 tablet-4 mobile-4"
Swapping the columns on the desktop won't affect the tablet or mobile layout. 

Note: there is less opportunity to alter the content order on the mobile layout
as often each item of content is stacked vertically ocupying the full width of 
the device. The push and pull classes only affect the "row" ordering of the 
content. This typically isn't an issue as your actual content ordering should 
be the most important things first - which is usually what you want on a 
mobile device.

Detamo also has "prefix and suffix" classes to add blank columns into a layout 
in a similar way to the 960 system. Again these are on an individual grid 
basis, so to reduce the width of the second sidebar on the tablet layout a 
blank column could be added to the left of it (using a prefix class) or to the 
right (using a suffix class). That is, change:
sidebar 2 - class="desktop-2 tablet-4 mobile-4"
to sidebar 2 - class="desktop-2 tablet-prefix-1 tablet-3 mobile-4"
or sidebar 2 - class="desktop-2 tablet-suffix-1 tablet-3 mobile-4"
As before altering the tablet layout won't affect the desktop or mobile layout.

CONVERTING A SITE FROM 960 TO DETAMO
====================================

You can convert an existing theme, which is a twelve column subtheme of 960, to 
be responsive using Detamo as follows:

1) Install Detamo

2) Update your theme's .info file to be a sub-theme of Detamo

3) Convert your page template to use Detamo's desktop classes
    - replace "container-12" with "full"
    - replace the grid classes with the equivanlt desktop classes
      e.g. grid-8 with desktop-8 or push-3 with desktop-push-3

    At this point you can view your theme - it should not have changed 
    appreciably but will now be a fluid desktop theme.

4) Decide how you want your design to look as an 8 column tablet layout. Start 
   from the top of your desktop layout and decide how many columns to give 
   each area of content. You may    want to create an 8-column wireframe.

5) Add the tablet classes alongside the desktop classes to create your 
   tablet layout.

6) Repeat the above for mobile design and classes.

7) Examine how the layout looks at various widths and make changes to the 
   CSS to ensure the font sizes are appropriate.

Troubleshooting the layout
==========================

A number of things may cause problems as follows:
- Images should scale, keeping the correct aspect ratio, as the browser width 
  is reduced. However if the image has a height and width this may not work 
  correctly. Ideally images should be sized correctly for the desktop layout.
- Any styles that include a fixed "height" will prevent horizontal scaling as 
  the browser width is changed (similar to above but may affect more than 
  just images).
- Sometimes a "tablet-clear" class needs to be added for content areas which 
  don't clear the content above them. For example, if the desktop layout has 
  four 3-column areas in a row, and this is replaced by two rows of two 
  4-column areas on the tablet - the third area will need a tablet-clear class 
  added to ensure it clears both the areas above it.
- Note. IE does not support media queries and so will only show the 
  desktop layout.
- As is the case with 960, adding margins, padding or borders to the areas with 
  grid classes will break the layout. Instead inner divs should be used to 
  achieve this type of styling.
