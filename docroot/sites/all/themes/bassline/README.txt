Bassline


    |———————————————————————|
    |—————————3h4———————————|
    |———3h5———————5—————————|
    |—0—————x———————0—2h3—2—|


A clean theme providing solid foundation for creating bespoke themes in the
shortest possible time. No styling is added to this theme, we have stripped
everything so a themer has the most possibilities.

## Features

### Bassline base theme

* HTML5 Markup.
* All styling stripped.
* Theme settings to enable / disable the breadcrumb.
* Comes with a childtheme which uses the Bootstrap 3 grid system.

### Child theme

* Comes with a basic 3 column layout.

## Usage

### Description

Bassline has been built so you have the freedom to use the markup and grid
system you prefer.

By default, Bassline strips all styling and alters a few template files to use
HTML5 markup - it's the bare bones starting point for any site.

### Installation

Place Bassline into the following location: /sites/all/themes/ & then visit the
following URL to enable the theme: /admin/appearance

#### Using Bassline childtheme

The Bassline childtheme has been created to use the Bootstrap 3 grid system
(No styling or JS files included) if you want to use this as your starting
point please see below on how best to enable:

Please note:
In the child theme we have a custom page.tpl.php which generates the 3 column
layout. To take advantage of this please rename
/sites/all/themes/your-theme-name/templates/system/__page.tpl.php
to
/sites/all/themes/your-theme-name/templates/system/page.tpl.php

Once you have done this you will need to clear the site cache.

##### Automatic (Drush)

This site takes advantage of Drush to generate child themes.
The easiest way is to run the following command:

drush bassline "My theme" --path=sites/default/themes

This will generate "My theme" (without qoutes) into the themes directory

For more options you can use the following command:

drush bassline --help

##### Manual

Copy the childtheme directory from /sites/all/themes/bassline & place into
/sites/all/themes e.g. /sites/all/themes/CHILDTHEME. Now replace all instances
of CHILDTHEME to the name of your theme.

The following files will have reference

CHILDTHEME.info.example > themename.info

#### Using your own grid system

You can setup your own grid system by creating a custom theme but in your .info
add the following line so your theme knows to use bassline as the parent:

base theme = bassline

## Maintainers

https://www.drupal.org/node/2309525/committers
