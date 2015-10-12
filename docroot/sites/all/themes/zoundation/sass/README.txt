ZOUNDATION AND SASS
------------------------------

This directory includes all the sass files used to create the CSS of the Zoundation theme.

Sass is another language in which to write CSS. It includes more features to write css smarter and more efficiently. Sass supports variables, nesting, mixins, selector inheritance, functions etc.

When you write Sass you while have a helper application to convert the Sass to css.

Learn more about Sass: http://sass-lang.com/
Learn about Sass in Foundation: http://foundation.zurb.com/docs/sass.php



USING COMPASS IN ZOUNDATION
------------------------------

To use Zoundation, you will need to install Compass http://compass-style.org/install/ 
or use a GUI tool such as CodeKit Project: http://incident57.com/codekit/index.php

Foundation does not work well with LiveReload and has some configuration issues with Compass.app

From Foundation Docs - http://foundation.zurb.com/docs/compass.php - Bottom of the page.

      LiveReload
      While LiveReload works great within CodeKit, it doesn't work well on its own because you can't update it to use your system Sass and Compass directories. Do not use this application to compile Foundation.

      Compass.app
      You can get Compass.app to work with Foundation, but it isn't that easy. Read documentation on how to get Foundation working with this app, here: https://github.com/handlino/CompassApp/wiki/Use-Compass-Extensions.


After Compass is installed, install the zurb-foundation Compass gem.

These docs will step you through how to install:
http://foundation.zurb.com/docs/compass.php