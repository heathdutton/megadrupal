Context Mobile Switch module, README.txt

Add conditional contexts based on the Mobile Switch device detection.

DEPENDENCIES

Drupal 7.
Mobile Switch module version 2 - http://drupal.org/project/mobile_switch.
Context module - http://drupal.org/project/context.

INSTALL

1)
Copy the Context Mobile Switch folder to the modules folder in
your installation. Usually this is /sites/all/modules.
Or use the UI and install it via admin/modules/install.

2)
In your Drupal site, enable the module under Administration -> Modules.
admin/modules

ADMINISTER

No administration UI available.

USAGE

Edit the contexts under Administration -> Structure
admin/structure/context

If you edit a context,
you can use a 'Condition' named with 'Mobile detection'.

TROUBLESHOOTING

Check the 'Reactions' configurations after Mobile Switch operating mode
switches. A example to do this is the switch
from the operating mode 'detect only' to 'theme switch'.

The operating mode 'Do not use' is not supported.
