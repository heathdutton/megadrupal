A Feeds parser plugin that extracts the title and content of a webpage using a
PHP port of Readability.

Readability was originally developed as a Javascript bookmarklet to ease online
reading by automatically extracting the content and title of a webpage,
stripping out sidebars, advertisements, and other navigational cruft. The
algorithm is now commonly found built in to the Safari web browser -- just look
for the "Reader" icon that appears in the location bar when you browse to an
article on a news site for an example of this in action.

This parser plugin runs the Readability algorithm on a fetched webpage, and
provides three mapping sources for your Feeds importer: title, content, and
image.

Try using this plugin in combination with Feeds Self Node Processor and
FileField Sources to automatically fetch the remote article's image to an image
field on your target node.

To install:

- Install and enable Libraries >= 2.0, and
- Download and copy Readability.php and JSLikeHTMLElement.php from
  http://code.fivefilters.org/php-readability/src into
  sites/all/libraries/php-readability
