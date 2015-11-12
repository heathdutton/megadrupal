INTRODUCTION
------------

This module provides comment widgets, and xautoload integration for the official
Disqus PHP library.

REQUIREMENTS
------------
libraries  https://drupal.org/project/libraries
xautoload  https://drupal.org/project/xautoload
Disqus PHP https://github.com/disqus/disqus-php

INSTALLATION
------------

 1. Install as usual, see http://drupal.org/node/70151 for further information.

 2. Download the Disqus PHP library from https://github.com/disqus/disqus-php and
    install into sites/all/libraries/disqus-php.

    git clone git://github.com/disqus/disqus-php.git sites/all/libraries/disqus-php

 3. Add the following code to the start of the disqusapi.php file after the
    initial opening php tag:

    namespace Disqus;
    use \Exception;

 4. Configure the module at admin/config/services/disqus

USAGE
-----

Use disqus_widgets_disqus() to get a Disqus API object, or use the available
blocks as designed.

See http://disqus.com/api/docs/ for API information.


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
