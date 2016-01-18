/**
 * @author: Daniel Honrade 
 *          http://drupal.org/user/351112
 *
 * References: 
 * http://lesscss.org/
 * http://leafo.net/lessphp/docs/
 * See also less_examples.less
 *
 */
 
A base theme dependent on LESS CSS preprocessor for hardcore themers. 
This base theme gives you full control over your css, no more tons of 
css overriding, and write less css with lessphp.

Features
=====================================================================================
    -- Using LESS CSS preprocessor, for more organized and less css
    -- Inspired by Tao, removed most core styling by default, for lesser 
       default css overriding
    -- Using Eric Meyer's HTML5 latest reset.css, to reduce browser inconsistencies.


Installation
=====================================================================================
    Module
        -- Install LESS module
        -- Install Libraries API module.
        -- Download the third party library, LESSphp to:
           /sites/all/libraries/
        -- The folder should appear like this:
           /sites/all/libraries/lessphp/lessc.inc.php
        -- Make sure you check LESS developer mode when developing your custom theme:
           (site base url) /admin/config/development/less
    Theme
        -- Install baseless theme to:
           /sites/all/themes/
        -- Copy baseless_subtheme folder from:
           /sites/all/themes/baseless/(baseless_subtheme).
        -- Put it here:
           /sites/all/themes/
        -- Rename it and it should appear like this, (Ex. the new name is My Theme):
           /sites/all/themes/my_theme/my_theme.info
        -- Open my_theme.info and also change "name" to "My Theme", 
           so that the name will appear correctly on:
           (site base url) /admin/appearance
 
