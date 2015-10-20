
Description
-----------

The go module adds the option to add redirected urls. It's much like the url
alias system except that it uses redirects to go to the real url. The visits
to the 'go' urls are tracked, so you can see how often a certain go url has
been visited.

The go redirects can be automatically created by enabling the filter. To create
a go url the user only has to add a tag like:

  <go href="http://example.org/test/page">An example</go>

This creates the go url:

  http://drupal.org/go/example.org/test/page

And the HTML code:

  <a href="http://drupal.org/go/example.org/test/page">An example</a>

Or with a custom go label:

  <go href="http://example.org/test/page" title="my label">An example</go>

This creates the go url:

  http://drupal.org/go/my-label

And the HTML code:

  <a href="http://drupal.org/go/my-label" title="my label">An example</a>

A disclaimers can be added before the redirect by checking the "Disclaimer"
checkbox in the administrative area.

Requirements
------------

This module requires Drupal 7.x


Installation
------------

To use the <go> tag you must activate the filter for the input formats.
Be sure to give the go filter a lower weight than the HTML filter.


Configuration
-------------

Go can be configured at Configuration -> Go settings
The list of created redirects is available in Structure -> Go redirects


Authors
-------

  * Michiel Hendriks <elmuerte@drunksnipers.com> (Original Creator)
  * Jacob Redding <jacob@wiredgeek.com> (Dislaimer Code, port to 5.x)
  * Alexander Hass (http://drupal.org/user/85918, port to 6.x, 7.x)

Thanks To
-------
  * Robin Rainton (drupal.org/user/27877)

