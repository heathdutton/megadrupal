NORMALIZE
---------

Normalize.css is a customisable CSS file that makes browsers render all elements
more consistently and in line with modern standards. We researched the
differences between default browser styles in order to precisely target only the
styles that need normalizing.

Check out the demo at http://necolas.github.com/normalize.css/3.0.2/test.html


WHAT DOES IT DO?
----------------

* Preserves useful defaults, unlike many CSS resets.
* Normalizes styles for a wide range of elements.
* Corrects bugs and common browser inconsistencies.
* Improves usability with subtle improvements.
* Explains what code does using detailed comments.


HOW TO USE THE STYLESHEET
-------------------------

Normalize.css is intended to be used as an alternative to CSS resets.

It's suggested that you read the normalize.css file and consider customising it
to meet your needs. Alternatively, include the file in your project and override
the defaults later in your CSS.

For more information about how to use it, see the About normalize.css article at
http://nicolasgallagher.com/about-normalize-css/


HOW TO USE THE MODULE
---------------------

Nicolas Gallagher explains two approaches to use normalize.css on your website.
This project supports both methods.

1. "Include normalize.css untouched and build upon it, overriding the defaults
    later in your CSS if necessary."

    To use this approach, simply enable the Normalize module and override what
    you'd like in your theme.

2. "Use normalize.css as a starting point for your own project's base CSS,
    customising the values to match the design's requirements."

    To use this approach, download the module and copy the
    normalize.css/normalize-rtl.css or normalize.scss/normalize-rtl.scss files
    into your theme. You can then add them and modify them like normal theme
    stylesheets.

    You can optionally enable this module so that the normalize.css stylesheet
    comes before all other stylesheets. As long as your theme's version is still
    named "normalize.css", your version will be loaded instead of the module's
    original version.


BROWSER SUPPORT
---------------

* Google Chrome
* Mozilla Firefox 4+
* Apple Safari 5+
* Opera 12+
* Internet Explorer 8+ (optionally, IE 6+)


LICENSE
-------

The CSS and Sass files are licensed under MIT/GPLv2. The module code is licensed
under the GPL; see LICENSE.txt file for more information.


ACKNOWLEDGEMENTS
----------------

Normalize.css is a project by Nicolas Gallagher and Jonathan Neal available at
http://necolas.github.com/normalize.css/

The Compass port of normalize.css is a project by John Albin Wilkins available
at http://github.com/JohnAlbin/normalize.css-with-sass-or-compass

The Drupal port of the CSS and Sass files, as well as the module code, is
maintained by John Albin Wilkins and Damien Lee.
