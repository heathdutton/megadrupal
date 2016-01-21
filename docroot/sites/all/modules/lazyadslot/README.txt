Module: Lazy Loading Ad Slots

Description
===========
The module is using the Doubleclick for Publishers (DFP) as a base for
the google slot declaration/display JavaScript.
It provides a context reaction which is allowing us to configuration bundles.

Installation
============
Copy the module directory in to your Drupal:
/sites/all/modules directory as usual.

Configuration
=============
There is no special configuration page for the module.
All we need to do is create the context that represents
bundles of configurations.
A context reaction will have following parameters:

DFP TAG             -- Allows you to choose which DFP tag to use for this
                       bundle of configurations
IDENTIFIER          -- Allows you to specify a string in order to recognize
                       the bundle when using in a custom implementation
ADD ON ONSCROLL     -- Whether to add the Ad on page instantly(on page load)
                       or onscroll;
Top pixels 	        -- How many pixels before arriving into the viewable Ad
                       should request be fired;
AD PLACEMENT        -- Allows to input CSS selectors(per line) which determine
                       where the ad will be added;
                       Please note that a valid selector has to end up into one
                       match. For example p:nth-child(3n+0) will not work due to
                       technical limitation and desiring to not overcomplicate
                       the JavaScript implementation.
                       If you specify more than one selectors(per line) - this
                       is totally fine as the Ad Slot is going to be cloned and
                       will generate a new Slot ID;
ATTACH HOW	        -- Whether to add the Ad After or Before specified CSS
                       selectors;
Don't render the Ad -- This option can be used when we want to send all the
                       parameters in Drupal.settings but not calling the
                       JavaScript implementation in order to be handled in
                       custom implementation;
Async rendering     -- Wrap the Slot declaration into cmd.push() to be
                       handled asynchronously
