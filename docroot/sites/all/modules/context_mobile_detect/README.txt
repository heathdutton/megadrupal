This module is really simple and allows to show certain blocks based on context.

It gets tricky when the drupal page cache is enabled.

In this case the following line needs to be added to the end of settings.php:

include DRUPAL_ROOT . '/sites/all/modules/custom/context_mobile_detect/settings.inc';

This will ensure that the page cache is prefixed correctly.

It gets a little more tricky -- if Varnish is used. The VCL rules need to be changed as follows:

* Disable caching for the current request if no "device" cookie was sent.
* Do not strip "device" cookie
* Vary cache based on cookie content (default).

The real trick is to not deliver from cache, if no device cookie was sent. Afterwards all requests will be cached.

Enjoy!
