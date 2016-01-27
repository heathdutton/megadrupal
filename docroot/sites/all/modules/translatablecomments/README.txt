// $Id: README.txt,v 1.4 2010/08/23 13:16:51 davetrainer Exp $

Translatablecomments allows you to add a translator widget to different elements of pages on your Drupal site. The widget displays a series of languages, and users click on one of the languages to translate content. The content is translated immediately, without refreshing the page, and without copying and pasting, thanks to the Google AJAX Language API.

Installation
------------

Copy the translatablecomments folder to your module directory, then enable it at Administer > Site Building > Modules.

Configuration
-------------

Navigate to /admin/settings/translatablecomments

Enter css selectors, 1 per line, to configure which html elements will have the translator widget attached.

Use the checkboxes to select which languages will be displayed as options to translate content.

The translator is themeable. You can change the html structure of the widget by overriding the theme functions from jquery.translatablecomments.js in your own theme.

Contributors
------------
David Trainer
david AT minimalmedia DOT com

Randy Fay
randy AT randyfay DOT com
