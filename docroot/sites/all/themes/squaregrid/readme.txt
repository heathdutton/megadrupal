# About This Project #

Square Grid theme is intended as a very lean, lightweight grid theme that provides a framework for you page layout ... and that's it. It incorporates responsive design principles that are configurable via your theme's settings, to present usable layouts for various screen sizes. That's it. We want your theme to load quickly, with as little grid framework css as possible.

The Square Grid theme is inspired by the Square Grid CSS Framework by Avraham Cornfeld.

## Version 3.x ##

This is the HTML5 branch, with a content-first, mobile-first, responsive layout. The grid layout is 100% fluid width. Themers upgrading from Version 2.x should plan on potentially significant child theme adjustments.


# Description #

The concept of this theme is simple: To provide a base theme defined by the Square Grid framework, for designers and front-end developers to use as a parent theme for a Square Grid-based design.

Grids are fabulous tools for rapid design and development of interactive CSS-driven interfaces. There are many grids out there. The Square Grid is relatively new.

The Square Grid has some nice advantages over other grids:

* Its 35-column breakdown of space allows great flexibility in grid selection, including popular favorites 960 and Golden Ratio.

* It has wider gutters between "columns," providing more whitespace.

* Its algorithm is easily adapted to other screen sizes.


# Installation #

1. Install the Square Grid theme in your Drupal site under /sites/all/themes (or, if on a multisite configuration, /sites/[example.com]/themes, as appropriate).

2. Create a new theme. In your new theme's info file, declare 'squaregrid' as the base theme.

3. Visit your new theme's configuration settings (/appearance/settings/YOURTHEMENAME) to set site width, break point and grid size/placement settings.

4. Copy in and modify templates and create stylesheets as desired within your own theme's folders.


# General Tips 

* Be sure to visit your theme's configration settings to set the Square Grid configurations available to your theme.

* Use the Sqaure Grid templates to provide the basis for your design. This will provide the basis for quick conversion from flat mock-up to html components. Square Grid templates for design can be downloaded at http://thesquaregrid.com. There are 35 squares across to define the grid. In this approach, a grid square can serve to define the width of a block element.

* The theme utilizes two kinds of classes: "sg-x" and "push-x". This approach is used to enable visual display of sidebar content to the left of main content without having to rearrange semantic order of content in your page.tpl.php template.

  - "sg-x" defines the width of a region. The integer value of x determines how may grid squares wide the region will display.
  
  - "push-x" defines a "left" position value in numbers of grid squares. Define a region's push value by the number of grid squares you want to *add* to the left of the region.
  
@TODO: Revise the following:
* You can define your own region widths and positions by copying "example.template.php.txt" to your own theme, renaming it "template.php" and replacing in the function names "YOURTHEMENAME" with your theme name, then editing the sg-x, push-x values.


# More Information

* The Square Grid framework website for information, design templates, and other tips on working with the Square Grid framework: http://thesquaregrid.com

* Square Grid theme documentation page: http://drupal.org/node/1264768


# Credits

Theme by Laura Scott http://drupal.org/user/18973.

Development and maintenance sponsored by PINGV http://pingv.com.
