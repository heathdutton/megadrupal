Description
------------------
Collapsible comments is a module that allows the collapsing and expanding of
commemnt threads through the use of jQuery. It helps improve usability and
readability of the comment threads by removing the visual noise of deep
threaded conversations.


How it works
------------------
Collapsible comments adds a small js file (collapsible_comments.js) to every
page load.

Note that the js file could be added only if there are comments in the
page, but it would mean making your users have download different
bundles of js files, and in the end hurting frontend performance.


How to use it
------------------
Simply install the module as usual
http://drupal.org/documentation/install/modules-themes/modules-7

Go to admin/config/content/collapsible_comments to configure the options

The selector for the comments is hardcoded to '#comments div.comment',
which is what Drupal renders by default. I normaly never see this
rendered any other way, so for now we don't include a setting to
configure this. Feel free to open a feature request if necessary.


Theming
------------------
You can theme the link to show/hide comment threads in your theme script file,
just make sure to keep a.comment-thread-expand if you override this theme
function:

Drupal.theme.prototype.commentCollapseLink

See collapsible_comments.js and the Drupal.org js handbook page:
https://drupal.org/node/171213

The js also sets some classes for themers to use, use firebug to check it out.
