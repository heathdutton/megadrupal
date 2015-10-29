OVERVIEW
This module helps with integrating the Revive Adserver
(http://www.revive-adserver.com)  - formerly known as OpenX and before that
Openads) into Drupal. Especially it helps with seperating code (where to
place ads in templates) and content (which ads to serve). This module will
generate all the JavaScript required to invoke Revive Adserver for the
Javascript ad delivery method but also supports the remote, local, and
XML-RPC delivery methods.

UPDATES
2007/2/28 - Initial release

2014/12/2 - Updated to work with Revive Adserver 3.0.x and updated all
mentions of OpenX to reflect the spinoff of the open source ad server from
OpenX to Revive Adserver.

DEPENDENCIES
This module works in Drupal 6 and 7.  It requires no other modules.

INSTALLATION
* Place the openads folder in your modules direcotry.

* In your site, navigate to Administer -> Modules (admin/modules),
and enable the openads module. It is located in the "Advertising" section.

CONFIGURATION
Go to Administration -> Configuration -> Revive Adserver configuration
(admin/config/media/openads) to set configuration options. Configuration options
are split into three parts:

* Adserver Settings
Every Revive Adserver server can be configured differently regarding delivery
methods, domains, and script names. Therefore, you need to provide in the URLs
and script names to be called. You can copy and paste the values from the
adserver's site Configuration -> Global Settings in the administrator panel.
If you don't have administrator access on the adserver, ask your administrator
for the appropriate values.

* Affiliate Settings
Every website is known as a affiliate in the Revive Adserver server.
Copy the affiliate ID and the number of zones for this site here.

* Zones
Now you are ready to enter your zones. Every zone has a unique number and an
internal code, you can copy this information from your adserver generated
invocation code.

Aditionally you may give your zone a name, to access this zone within
template code. You may also for every user role determine, if ads
are shown or not.

BLOCKS
This module offers as many blocks as zones are defined. The blocks are named
after the zones that is "Revive Adserver Zone 0", "Revive Adserver Zone 1" etc.
You may enable or disable blocks under Adminstration -> Structure -> Blocks
(admin/structure/block).

MANUAL INVOCATION
If you want to place ads within your templates manually, you may use the
following code:

<?php print openads_invoke([index]); ?>

Where [index] is the number of one of your zones, that is 0, 1, etc.
You may also invoke a zone by its name, like this:

<?php print openads_invoke("leaderboard top"); ?>

The module will take care to generate the according JavaScript after
validate if to show ads to the user logged in

SPONSORS
This module was sponsored by Ambiweb GmbH, http://www.ambiweb.de

CONTRIBUTORS
Gerd Riesselmann (openads@gerd-riesselmann.net) - this module
Jeff Warrington (jeff@qoolio.org)
