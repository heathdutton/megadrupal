WHAT IS CONTENT PAGE
--------------------

The Content Page module allows requests to pages in your Drupal web site to be
rendered with just the content area visible. This allows pages to be displayed
in an iframe using modules like Shadowbox without too much work.

MODULE USAGE
------------

* Copy the module to the usual place and enable it

* Depending on your theme, adjust the CSS selectors to style content that will
  appear in the content only mode. There is a body class, content-page-content-only,
  that is added when only the content is rendered.

* Visit a node with $_GET['ajax_request'] set, eg http://yoursite.com/node/1?ajax_request

FUTURE DEVELOPMENT
------------------

Currently, the module works off a hardcoded check for $_GET['ajax_request']. In
the near future this will be modified to work with a hook call to allow other
modules to request their paths to be rendered with just the content.

There are also plans to work on a Content Page UI module that will allow site
administrators to setup rules to set when a request should be rendered with
just the content visible.
