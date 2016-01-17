Mobile Switch module, README.txt

Provides various functionalities to develop mobile ready websites.

This readme is valid for the branch 7.x-2+.
For other branches and their versions, please read the appropriate
README.txt files.

The module allows to work with 4 different operating modes.

DEPENDENCIES

Drupal 7.
The Mobile Detect PHP class - http://mobiledetect.net.
Libraries API module, branch 2+, - http://drupal.org/project/libraries.

REQUIRED

Views module branch 3+ - http://drupal.org/project/views,
to work with the Views filter criteria 'Mobile switch'.

RECOMMENDED MODULES

Mobile Detect Import - sub-module of Mobile Switch.
Mobile Switch Panels - http://drupal.org/project/mobile_switch_panels.
Context Mobile Switch - http://drupal.org/project/context_mobile_switch.
Mobile Switch Block - http://drupal.org/project/mobile_switch_block.
Mobile Switch Blocks - http://drupal.org/project/mobile_switch_blocks.

Not all of these modules are useful in each operating mode.

HOW WORKS - OPERATING MODES

1) Do not use
Only the Mobile Detect PHP class will be loaded.

2) No theme switch - detect only
Work with a single site and a responsive theme.

3) No theme switch - redirect to website
Work with a multisite installation.

IMPORTANT: In this mode, define default themes in the setting.php files.

Please read the HELP.txt for an example setup for a shared content multisite.

4) A theme name
This is the operating mode 'theme switch'.
Work with a single site and two themes.

IMPORTANT: In this mode, do NOT define default themes in the setting.php files.

LIMITATION

- Caching

It is not possible to work with 'Cache pages for anonymous users'.

INSTALL

1)
Make sure that the library directory exists for Mobile Detect.

Typically and recommended, this is:

sites/all/libraries/Mobile_Detect

or

sites/default/libraries/Mobile_Detect
sites/example.com/libraries/Mobile_Detect

If this directory does not exist it should be created.

Note: The directory name Mobile_Detect is case sensitive.

Multisite installation

Execute this step only on the default site.

For a multisite with shared content is is required to install the
Mobile Detect class on the default multisite in sites/all/libraries.

Please read the HELP.txt.

2)
Download the Mobile Detect class file and install it as a library.

Exists the library directory from step 1 and exists the class file

Mobile_Detect.php (with exactly this name)

you can skip this installation step.

Note: The file name Mobile_Detect.php is case sensitive!

Download the class file optionally from

a) https://raw.github.com/serbanghita/Mobile-Detect/tags

or

b) https://raw.github.com/serbanghita/Mobile-Detect/master/Mobile_Detect.php

The download from b) usually provides a higher version of the
Mobile Detect class.
The Mobile Detect Import module uses the link b).

The result of proper installation of the Mobile Detect class as library:

sites/all/libraries/Mobile_Detect/Mobile_Detect.php

or

sites/default/libraries/Mobile_Detect/Mobile_Detect.php
sites/example.com/libraries/Mobile_Detect/Mobile_Detect.php

Ultimately, this structure must exist in order to find the
Libraries API the Mobile Detect class.

- Multisite installation

Execute this step only on the default site.

For a multisite with shared content is is required to install the
Mobile Detect class in sites/all/libraries/Mobile_Detect.

Read the HELP.txt.

3)
Copy the Libraries API folder to the modules folder in your installation.
Usually this is sites/all/modules.
Or use the UI and install it via admin/modules/install.

Note:
Mobile Switch branch 2+ requires Libraries API 2+.

4)
Copy the Mobile Switch folder to the modules folder in your installation.
Usually this is sites/all/modules.
Or use the UI and install it via admin/modules/install.

5)
Enable the modules

Important:
As first step, enable ONLY the Libraries API module!
As second step, enable the modules Mobile Switch and Mobile Detect Import.

Enable modules under Administration -> Modules.
admin/modules

UPDATE FROM 1 TO 2 BRANCH

1)
Visit the Mobile Switch administration and inspect the configuration
parameters.
Make sure that you can remember all the configuration details.

2)
Disable and uninstall Mobile Switch and if used, the
Mobile Switch Block module.

Disable and uninstall the Browscap module when it is no longer
necessary (by other modules, or for logging of user agents).

3)
Follow the steps described in the INSTALL section above.

Install and enable the Mobile Switch Block module if it was uninstalled
in the update step 2.

4)
Administer the Mobile Switch module and reconfigure all parameter
to the previous settings.

If again the Mobile Switch block module enabled in the update step 3,
then enable the block 'Mobile switch' in the desktop and mobile theme.

ADMINISTER

- Administer single site

This section only describes the administration when using
the operating mode 'theme switch'!

Other operating modes:
Not all settings are available in each operating mode.

To get extended informations about the operating mode 'redirect to website'
please read the HELP.txt.

1)
Administer themes under Appearance.
admin/appearance

Enable the prefered mobile theme (NOT use 'Enable and set default').
If the prefered mobile theme already enabled not use 'Set default'.

2)
Administer the Mobile Switch module under:
Administration -> Configuration -> User interface
admin/config/user-interface/mobile-switch

Choose your mobile theme.

If a theme used as mobile theme, their displayed informations on the
Appearance administration page are altered for better visualisation.

A Mobile Switch mobile theme is default not available in the maintenance mode.

- Mobile theme on administration pages

Configure the 'Administration usage' in the module 'Basic settings' to
enable the use of the mobile theme on administration pages.

- Mobile device prevention

It is possible to bypass the automatic switching to the mobile theme for
mobile devices.
This is useful, for example, to exclude large tablets for the theme
switching.

To use this,

administer the 'Advanced' settings under:
Administration -> Configuration -> User interface -> Mobile Switch
admin/config/user-interface/mobile-switch/advanced

enable the 'Use preventing' option and configure the user agent strings
for such devices.

To test this feature, without a real mobile device, it is a good
solution to use the desktop browser with a user agent switcher extension
and custom defined user agents.

- For the development of a web site

Administer the 'Development' settings under:
Administration -> Configuration -> User interface -> Mobile Switch
admin/config/user-interface/mobile-switch/development

a) Enable/disable the 'Developer modus'.

If a desktop mobile emulator not detected from Mobile Detect it is
possible to configure additional user agents.

b) Enable/disable desktop browser usage of the mobile theme.

3)
Administer the Mobile Detect Import module.

See README.txt of the module.

- Administer shared content multisite

For a complete administration of both sites (the desktop and the mobile site)
with all aspects of the configuration, you need two browsers (on desktop and
mobile device) or a browser with a user-agent switcher on your device.
You can emulate the mobile device with the 'Opera Mobile Emulator'.

DEVELOPMENT

Development with Mobile Switch.

Possible is the usage of the

@code
mobile_switch_mobile_detect()
@endcode

function. Please read the inline code documentation of this function.

Full access to the loaded Mobile Detect PHP class:

@code
global $msw_detect;
$msw_detect->isMobile();
$msw_detect->isTablet();
@endcode

The loaded class is also available in the operating mode 'Do not use'.

MODULE DEVELOPMENT

The module is only tested in a Apache web server environment.

EXTERNAL RECOURCES

Mobile Emulators & Simulators: The Ultimate Guide
http://www.mobilexweb.com/emulators
