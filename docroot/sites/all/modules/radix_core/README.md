Radix Core
====
Brings all the Radix components (theme and modules) together.

### Which version do I need?

* For Radix 7.x-2.x, use Radix Core 7.x-2.x.
* For Radix 7.x-3.x, use Radix Core 7.x-3.x.

### Where is Radix Core used?

Radix Core is meant for your distribution's Drush make file.

If you are building a distribution with Radix as the base theme, you can include Radix Core in your make file and it will pull all the required modules and themes. See an example below:

    ; Radix
    projects[radix_core][version] = 3.x-dev
    projects[radix_core][subdir] = contrib

### Read more

See the project page at http://drupal.org/project/radix_core.
