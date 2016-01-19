
AJAX Include Pattern for Modular Content


REQUIREMENTS

- matchMedia.js and Ajax-Include-Pattern
  Download the library archives from the folllowing:

  https://github.com/paulirish/matchMedia.js/
  https://github.com/filamentgroup/Ajax-Include-Pattern/

  Extract them and rename the folders accordingly so they are available at:
  sites/../libraries/matchmedia/matchMedia.js
  sites/../libraries/matchmedia/matchMedia.addListener.js
  sites/../libraries/ajaxinclude/dist/ajaxInclude.min.js
  sites/../libraries/ajaxinclude/dist/ajaxIncludePlugins.min.js

- libraries (>=2.x)

- block.tpl.php must support Drupal attributes, content_attributes, or
  title_attributes. The title is optional.


INSTALLATION

Install the module as usual, more info can be found on:
  http://drupal.org/documentation/install/modules-themes/modules-7


USAGES

Visit admin/structure/block, and find AJAX Include settings under Visibility.
Enable it as needed, and adjust the relevant options accordingly.


FEATURES

- Flexible AJAX content placements, e.g.: before/after/replace/append the common
  block elements such as title/block/content.
- Interaction with click or mouseenter to trigger AJAX.
- Custom Target element to load AJAX content at a separate element.
- Media-qualified includes to include content based on a particular Media Query.
- Pure CSS preloaders.
- Optional noscript.
- Optional no cache.
- Option to exclude a particular block from a single concatenated request.
- Aggregated HTTP requests by default.


LIMITATION

Requires block.tpl which has attributes printed, meaning block--no-wrapper.tpl
will fail.


SIMILAR MODULES
- ajaxblocks
o AJAX Include adds a field to block table like i18n_block or block_class.
  Ajax Blocks creates a separate table.
o AJAX Include uses pure CSS preloaders.
  Ajax Blocks image-based.
o AJAX Include uses external libraries.
  Ajax Blocks none.
o AJAX Include consumes plain HTML dataType.
  Ajax Blocks JSON which means extra processing at client side.
o AJAX Include allows flexible placements via various methods and block
  elements: before, after, append, replace, in combination with block,
  content or title, e.g.: AJAX content may be placed before or after title, etc.
  Ajax Blocks only expects a block content placement.
o AJAX Include makes use Drupal roles visibility.
  Ajax Blocks allows more controls for different roles with different cache
  strategy.
o AJAX Include has not been tested against Boost nor Authcache.
  Ajax Blocks claims them both.
o AJAX Include offers simple interaction to load contents, click and mouseenter.
  Ajax Blocks none.
o AJAX Include offers loading distribution strategy to reduce lengthy delay:
  a single request for many automatically, and/or individual request if so
  configured manually.
  Ajax Blocks does the first only.

Both do not make use Drupal AJAX framework.
Both make GET requests to deliver faster than Drupal.ajax default POST.


READ MORE

https://www.filamentgroup.com/lab/ajax-includes-modular-content.html
http://adactio.com/articles/5043/
https://24ways.org/2010/speed-up-your-site-with-delayed-content


AUTHOR/MAINTAINER/CREDITS

gausarts

With the help from the community:
- https://www.drupal.org/node/2615156/committers
- CHANGELOG.txt for helpful souls with their patches, suggestions and reports.

SpinKit:
https://github.com/tobiasahlin/SpinKit
