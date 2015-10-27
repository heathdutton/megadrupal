
=============
== Summary ==
=============
This module provides links to post or "tweet" pages to websites like Twitter.
Clicking the links will open a new window or tab for the relevant site. The
tweet will be in focus and will contain customizable text which can include the
relevant URL, title, and (if the tweet link appears on a node) taxonomy terms
and the teaser. The Shorten URLs module is used to shorten the URLs if it is
installed.

URLs and titles will be for either the node which is being displayed as a
teaser or for the current page. Multiple links can appear on the same page, as
on a View of teasers. By default, links appear in the Links section when
viewing full nodes or teasers.

Administrators can choose whether to show the link as an icon, an icon and
text, or just text.  Options can be chosen separately for nodes and teasers.
Administrators can also choose which node types the links should appear on, or
could choose not to have links show up on nodes at all.  If the module is
configured not to display links automatically, administrators can display their
own links wherever they like by calling tweet_to_twitter().  A more complete
explanation is below.

=====================
== Development/API ==
=====================
If you are using this module to display links to twitter arbitrarily, you will
probably be using the tweet_to_twitter() function.  This constructs the link
you need according to the following arguments.  All arguments are optional
unless otherwise noted.  If no arguments are passed the link constructed will
be for the current page according to your settings.

If you want more control, the _tweet_to_twitter() function takes the same
arguments and returns an array in the format required by hook_link()
(http://api.drupal.org/api/function/hook_link).

-- tweet_to_twitter($site = 'Twitter', $type = '', $format = '', $nid = '') --
$site
  Specifies for which site the link will be generated. Twitter is the default.
  See tweet_sites() for a list of allowed sites.
$type
  Specifies what will show up in the link: the twitter icon, the twitter icon
  and text, or just text. Pass 'icon' to show just the icon, 'icon_text' to
  show the icon and text, and 'text' to show just the text. Required if display
  options for nodes are set to 'none' on the settings page.
$format
  A string representing the tweet text, optionally with the case-sensitive
  tokens [url], [title], [node-tags], and [node-teaser]. If not passed, the
  format from the settings page will be used.
$nid
  The NID of the node for which the twitter link should be constructed, or
  the absolute URL of the page for which the twitter link should be
  constructed. If the URL given is not the current URL, and if $nid is not a
  NID, the title must be set manually (instead of using the [title] token) or
  it could be incorrect.


If you are building a module for general use, you will probably want to use the
tweet_sites() function to get a list of sites for which Tweet can build links.

-- tweet_sites($enabled_only = FALSE, $reset = FALSE) --
$enabled_only
  If TRUE, only enabled sites are returned. Otherwise, all defined sites are
  returned.
$reset
  Reset and rebuild the static cache.


----
If you want to add another site for the Tweet module to use, you should
implement hook_tweet_sites(). That hook returns an associative array specifying
information that Tweet will use to build tweet links. See tweet_tweet_sites()
for an example. The key of each element of the array should be the name of the
website; the value should be another associative array. The elements of the
inner array include:

path
  The base URL where Tweet will send users who click on the tweet link.
  Ex.: 'http://twitter.com/home'
query_key
  The query key that specifies the text which will appear in the tweet textarea.
  For Twitter, this is 'status,' generating a link like
  http://twitter.com/home?status=[tweet_text].
image
  The location of the image file associated with the site. Your module may want
  to offer a setting to override this path if you want your users to be able to
  use custom icons.
  Ex.: drupal_get_path('module', 'tweet') .'/twitter.png'

If you just want to modify the settings a module provides for an existing site,
use hook_tweet_sites_alter(&$sites), where $sites is the array of sites
returned by module_invoke_all('tweet_sites').

==================
== Installation ==
==================
  1. Install this module as usual (FTP the files to sites/all/modules, enable 
     at admin/build/modules).  See http://drupal.org/node/176044 for help.
  2. If you want to use shortened URLs in your tweets, also install the
      Shorten URLs module by the same author (see the Links section below).
  3. If you want, go to admin/settings/tweet to change the module's settings.

===========
== Links ==
===========
Visit the module page for more information.

Module Page: http://drupal.org/project/tweet
Issue Queue: http://drupal.org/project/issues/tweet
Enable Module: http://example.com/?q=admin/build/modules
Settings Page: http://example.com/?q=admin/settings/tweet
Shorten URLs: http://drupal.org/project/shorten