CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Configuration
 * Module Details
 * Recommended modules
 * Configuration
 * FAQ


INTRODUCTION
------------
Republish Module allows administrators to provide a Republish button for the end
user to republish the content of a node in an overlay. When end user clicks on
republish button provided, a popup appears which contains node content as per
configurations besides republish guidelines and instructions. End user can copy
the html content provided in textarea of overlay and use that html to embed it
into another website to republish.
Basically Republish Module provides feature to put node content in overlay or
say popup. Republish module uses url context to detect node. The module is
flexible enough for better usability. From republish button text to body of
content to be republished everything is configurable.

IT IS RECOMMENDED TO UNDERSTAND CONFIGURATIONS FIRSTLY BEFORE IMPLEMENTATION.


CONFIGURATION
-------------

Republish Configurations:

* Republish Link Text -
  Enter text to be displayed on republish button.

* Republish Box Title -
  Enter Title for Republish overlay box.

* Republish Guidelines -
  Enter descriptive guideline to be displayed on the Republish overlay box above
  the main body. Supports Filtered HTML format.

* Republish Instructions -
  Enter instructions to be displayed on the Republish overlay box below the main
  body.

* Republish Main Body -
  Enter the content to be displayed in the overlay. You may use available
  tokens. To access node fields use [node:field_name].

* Readonly - Checkbox -
  When enabled the main content to be republished in the textarea box would be
  readonly. In any case, end user would be able to copy the content in the box.

* Branding Text -
  Enter the Branding text to be included in editor.



MODULE DETAILS
--------------
Republish module can be used in two ways:
1. Using Block - Simple
2. Using Ctools Plugin - Advanced

Republish - Using Block
Administrators can simply provide republish feature using block which can be
sitewidely used with simple configurations. Once Republish module is enabled,
block "Republish Block" is available for users which can be placed anywhere.
Republish Block has various configurations for its usage which are described in
configuration section above.

Republish - Using Ctools Plugin System.
Republish plugin is available as ctools plugin also. It is available under
Widgets section, when you add the republish plugin there will be a configuration
form for that plugin which has the same configurations as the block had.
Becareful while using Republish Ctools Plugin, Republish Block and Republish
Plugin are two different things, so while using plugin it is recommended to
disable Republish Block.

Major Difference between Republish Block and Ctools Plugin.
* Each republish plugin used in website will have their independent settings
  while block will be site wide and hence has only single set of configurations.
* Republish block is useful when your website is having similar content type
  (means their fields are same). So that single block configuration works for
  all content types. While Republish plugin allows user to use the republish
  feature separately for each content type.



RECOMMENDED MODULES
-------------------

* CTOOLS
  When enabled Republish module provides Republish Plugin available as ctools
  plugin under Widgets section. Republish plugin allows user to use the
  republish feature separately for each panel.


FAQ
---

Q: How can we show node content in republish overlay?

A: Republish overlay's Main body is configurable where node fields can be
accessed as [node:field_name]. Generally we access [node:body].



Q: Can we use Republish block and Republish Ctools plugin at the same time.

A: No, although the buttons can be displayed for both block and ctool plugin on
  the node page but both will show same content in overlay. So it is recommended
  to disable the Republish Block when you are using Republish Ctools Plugin.



Q: Different content types have different fields how can I use same republish
  block for different content types.

A: It is not possible using Republish Block and hence we have provided Republish
  Ctools Plugin. Each Ctool Plugin would be using separate set of
  configurations.





This project has been sponsored by:
 * QED42
  QED42 is a web development agency focussed on helping organisations and
  individuals reach their potential, most of our work is in the space of
  publishing, e-commerce, social and enterprise.
