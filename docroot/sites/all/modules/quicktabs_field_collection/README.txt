INTRODUCTION
------------

The QuickTabs Field Collection module provides a field formatter for rendering
field collections as QuickTabs.


INSTALLATION
------------

 * Install as usual, see http://drupal.org/node/70151 for further information.


CONFIGURATION
-------------
 1. Go to the Manage Display page for any Entity that contains a
    field-collection field.

 2. Change the formatter to QuickTabs

 3. Click the gear icon on the right to access the formatter settings.

 4. (REQUIRED) Select the field to use for the tab values.

 5. (REQUIRED) Select the field to use for the tab contents.

 6. Optionally tick the box to strip all HTML tags from the tab content.

 7. Optionally add custom classes that will output in the field-collection
    wrapper div.

 8. Optionally configure the available QuickTabs options.

 9. Adjust formatter settings for each of the fields in the field-collection.

    The field used to output the value of the tab label and tab contents
    respects the formatter settings applied to those fields. However any HTML
    that is output get in the tab label is run through check_plain() by the
    QuickTabs module so it is recommended to choose Plain Text as the formatter
    for the tab value.


CONTACT
-------

Current maintainers:
* Steven Wichers (steven.wichers) - http://drupal.org/user/1774136

This project has been sponsored by:
* McMurry/TMG
  McMurry/TMG is a world-leading, results-focused content marketing firm. We
  leverage the power of world-class content — in the form of the broad
  categories of video, websites, print and mobile — to keep our clients’ brands
  top of mind with their customers.  Visit http://www.mcmurry.com for more
  information.
