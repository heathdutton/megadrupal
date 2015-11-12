Modules Form Batch Process modules submits the Drupal's Module form as a
Batch Process. This is to avoid timeouts when a number of modules are
Activated or Deactivated together. Each module is enabled one after the
other, and dependencies are treated like wise. The module will also
check if it is disabling or uninstalling itself and skip the
batch creation.

Installation
------------
Copy the module to your module directory and then enable on the admin
modules page.  Once enabled it will take up all submissions
to the Modules form and run them as a batch process


Author
------
naughty_david	
arjun@athousandnodes.com
