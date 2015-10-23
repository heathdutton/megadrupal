http://drupal.org/project/mailhandler_medialink

LICENCE: See LICENCE.txt
INSTALLATION: See INSTALL.txt
CONFIGURATION: See INSTALL.txt

Mailhandler Media Link Mailhandler Media Link Command provides a CTools plugin
that works with the Mailhandler module to expose links to videos found in the
body of emails as Feeds mapping sources when importing content with Feeds and
Mailhandler.

This module will parse the body of emails for links. It will then use Media
module to determine if the links are to video providers. If they are, the links
are exposed as a mapping source. NOTE

At this time, the links extracted from emails cannot not be mapped directly to a
media field. This is due to the nature of how media module and media fields
work. However, if you're clever you can write code to bridge this gap.