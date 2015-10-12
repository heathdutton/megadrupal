Mobile Switch Panels module, README.txt

Mobile Switch Panels provides an control plugin to show panel pages
in context of mobile device detection.

DEPENDENCIES

Drupal 7.
Mobile Switch module version 2 - http://drupal.org/project/mobile_switch.
Panels module - http://drupal.org/project/panels.

RECOMMENDED

CacheExclude module - http://drupal.org/project/cacheexclude

INSTALL

1)
Copy the Mobile Switch Panels folder to the modules folder in
your installation. Usually this is sites/all/modules.
Or use the UI and install it via admin/modules/install.

2)
In your Drupal site, enable the module under Administration -> Modules.
admin/modules

ADMINISTER

No administration UI available.

USAGE

- Single variant panel pages and panels

Visit your Page/Panel 'Content' tab.
Under 'Visibility rules' for a pane select 'Add new rule'.
Check 'Mobile Switch: Mobile detection', click Next.
Choose one of the two options. Don't use the 'Reverse (NOT)' setting.
Click Save.

- Multiple variant panel pages and panels

The usage of two page/panel variants makes it possible to control the
complete visibility in context of mobile device detection. As example:
1)
The first page/panel variant uses the Selection rule
'Detected device is not mobile'.
2)
The second page/panel variant uses the Selection rule
'Detected device is mobile'.

Create two page/panel variants.
Visit the 'Selection rules' tab in a variant.
Add the criteria 'Mobile Switch: Mobile detection'.
Choose one of the two options. Don not use the 'Reverse (NOT)' setting.
For the two page/panel variants insert only the content which will be
shown in the variant.

- Mini panels

Operating mode 'theme switch':
For the visibility control of the complete mini panel in context of mobile
device detection you can use the Mobile Switch Blocks module:
http://drupal.org/project/mobile_switch_blocks

LIMITATIONS

1)
The use as Access rule is possible. But, as a result can be displayed
the Access denied page.

2)
The use as Selection rule is possible. But, as a result can be displayed
the page 'Page not found'.

The solution

Work with two page/panel variants.
