Introduction
-----------
Device Detector is a simple, PHP-based browser and device
(Desktop & Mobile) feature-detection module that can detect
devices & browsers on its own without the need to pull from
a central database of browser information and adds
configuration classes to "" tag.

  - This module provides an admin configuration section,
    where the user can provide different class names for
    different conditions(browser & device wise).
  - This class names are then added to the html “” tag,
    while the page gets rendered from the server end.
  - The class names for browsers are rendered based on 
    the the admin configurations, using a browser’s(mainly)
    unique user-agent string as a key.
  - Mobile Detect Class, included in in the module itself,
    is used to collect and record any useful information's
    (like OS or device name) the user-agent string may contain,
    for rendering classes for device(Desktop & Mobile).

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire device detector directory to the
   Drupal "sites/all/modules" directory.

2. Login as an administrator. 
   Enable the module in the "Administer" -> "Modules" -> "Device Detector".

3. (Optional) Edit the settings under ::
    1. "Administer" -> "Modules" -> "Device Detector" -> "Configure".
    OR
    2. "Administer" -> "Configuration" -> "Device detector Settings"
       -> "Device Detector Configuration".
    OR
    2. Directly go to -> "admin/config/device-detector".

Maintainers
-----------
Current maintainers:
 * Monoj Nath - https://www.drupal.org/u/monojnath
