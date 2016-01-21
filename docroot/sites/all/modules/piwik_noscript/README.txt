Piwik noscript
==============

Some sites have a strict privacy policy which prohibits tracking of browser
metadata tracked by the Piwik JavaScript client (piwik.js).

In addition, some sites have a significant user base using noscript and other
browser extensions which limit ability to execute JavaScript.

This module uses an alternative syntax to setup Piwik tracking code via an image
tag rather than loading piwik.js.

This module adds a noscript tag containing the Piwik image tag to the bottom of
every page.

In addition, if Piwik module is not enabled, this module also adds an image tag
with some JavaScript to track the referrer URL.

To use this module you'll need to configure your Piwik settings by either
installing and configuring the Drupal Piwik module, or by adding these lines to
your settings.php file:

$conf['piwik_site_id'] = 1;
$conf['piwik_url_https'] = 'https://piwik.example.org/';
