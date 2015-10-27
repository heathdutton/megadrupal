MakeUp Image
============

Maintainer: w3wfr (<drupal@w3w.fr>)

Inspired by Color Field module proposed by targoo, this module provides a 
formatter that allows an imagefield value to be displayed through CSS.

### Notice

 * This module will not add any new html tag in the template. It only 
define a CSS declaration that put the image on the HTML selector you choose.
 * This CSS declaration can be added through 2 different formatters: **Inline CSS** 
in the HTML header or **CSS File** that can be aggregated and optimized.
 * Please note Selector field default value to "body" because I still did not
find any better value. Do not hesitate to make proposal in the issue queue.

### Features

 * Display an image through CSS declaration
 * Add options to the formatter : CSS standards for background tag, Imagecache 
preset, Additional CSS field
 * Use token to define CSS path - e.g. use tid or nid to apply properly on list 
pages


Requirements
------------

 * Drupal 7.x

### Modules

#### Dependency

 * Image

#### Compatibility

 * Token
 * Breakpoints


Installation
------------

Ordinary installation.
[https://drupal.org/documentation/install/modules-themes/modules-7](https://drupal.org/documentation/install/modules-themes/modules-7)


Setting the values
-------------------

Go on the **Manage display** page of the *Entity >> Bundle >> View mode* 
to configure.

If the imagefield to configure does not exist yet, go on the 
"Manage field" page and create one, then come back here :-)

 * Select **Inline CSS** in the select dropdown menu of your imagefield to put
 inline CSS in the header.
 * Select **CSS File** in the select dropdown menu of the imagefield to put
 CSS code in a file that will be imported and optimized by Drupal.
 * Default values will appear on the right: you will probably want to adapt them.
 * Click the Update button of the settings
 * Click the Save button of the "Manage display" page


Choosing the values
-------------------

### Working with Breakpoints

MakeUp Image is compatible with Breakpoints module. When Breakpoints 
is enabled, you'll get a formatter for each preset group.

Once you selected the formatter of the right group, you will be able
to define settings for each breakpoint.

First setting *Same as* allow you to reuse a setting: so you won't have to 
set it several times if you don't need to.

### Values

Here are the available values to set:

Where to display the image?

 * Selector: A valid CSS selector such as `.links li a`, `#logo`.

What the image should look like?

 * Image Style: Select any defined Image style preset.

Generating the CSS syntax for the "background" property

 * Color: A valid CSS color value.
 * Repeat: Horizontally / Vertically / Both / Neither / Inherit
 * Attachment: Should it be scrolling or not?
 * Horizontal position: A valid CSS vertical position value.
 * Vertical position: A valid CSS vertical position value.
 * Important: Check it to make this CSS have " !important" at the end.

### Width and Height

Two options allows to force the element size based on Image width and height.

### SEO and Accessibility

One can print an img tag with dummy `src` option for SEO and Accessibility optimization.

Examples
--------

### Displaying a pattern background on the node main page

Go on the Full content view mode, and use `#main-content as selector value.
Choose the best Image style that will suite your expectations.

You don't need background color here, you probably want to repeat the image to
have a pattern effect dedicated to this node.

You might need to use "important!" value.


### Place a photo, in Views list, on the right side of a teaser

Go on the Teaser view mode, and use the Token integration feature in selector: 
`#node-[node:nid]`. 

This make sure you address *#node-126* and *#node-237* correctly
so each one receive the right image, and don't mix up.

Transparency can be used as background color: `rgba(255,255,255,.2)`

Then: no-repeat and scroll. Positionning it right and top is usually OK. 
And verify if you need "important!".


### Full page background and Panel compatibility

Considering Drupal Commons 3, and targetting to have a beautiful image
as background when displaying the group page.

The view mode page can't be used because Commons is not using it to display this 
page. Panelizer (based on Panels) is doing the job instead. ;-)

Click on **Customize this page**, add a field in Node section: get *Field: Group 
Logo (field_group_logo)*. Do whatever you want with the Title, and select the
**CSS Declaration** formatter - click Continue.

Here default "body" can be kept as selector value, no image styling (keep the 
original size), no color, no repeat, no scroll, center and top - and  "important!". 
Click Finish. Click Save.

