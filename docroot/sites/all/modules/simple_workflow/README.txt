
Simple Workflow module
-----------------
by Jean Valverde, jean@dev-drupal.com

This module adds a new "ready" publishing option on node bundles and
fine-grained permissions on publishing options.
It gives you a simple worflow:
  Ready not checked (draft)
  Ready checked (ready to be published)
  Published checked (and surely ready!)

If you need a more suitable and customised workflow, you better look at Worflow
module: http://drupal.org/project/workflow

CONTENTS OF THIS FILE
---------------------

 * Requirements
 * Installation
 * Configure and use

************************************************************************

REQUIREMENTS
------------

This module require at least version 7.x of Drupal

INSTALLATION
------------

See http://drupal.org/documentation/install/modules-themes/modules-7

CONFIGURE AND USE
----------------- 

You first need to visit permissions (Administration > People > permissions) to
adapt access on each node bundles under "Simple workflow" section.
You need to add permissions after each node bundle creation. On bundle creation
you need to unchecked default published option.

This module adds a complementary admin view which is disabled by default but
could be a good base for a more convenient content administration.
Enable the view at Administration > Structure > Views, and flush menu cache.
Views Bulk Operations (https://drupal.org/project/views_bulk_operations) is a
recommended module to add "Update actions" to this view and get a full
replacement for Drupal core admin content page.

For default node templates (node.tpl.php), you have now access to a $ready
variable, a boolean indicating ready status and a new class: node-ready or
node-noready.

************************************************************************

This module has been developed by Jean Valverde
http://www.developpeur-drupal.com

I provide paid development service, visit my website to get more info.

Jean Valverde.
jean@dev-drupal.com