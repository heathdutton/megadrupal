; $Id: README.txt,v 1.3 2008/07/17 21:51:21 aronnovak Exp $

Aggregator
----------

This module suite is a Google Summer of Code project to replace
the current Drupal core aggregator with a better solution.

Check out outline for more information: http://groups.drupal.org/node/12772

This module is not production ready.

Start using the module
----------------------

- Remove modules/aggregator directory from your Drupal installation.
- Enable the following modules: Syndication Parser, Aggregator.
- Let's start create a feed at node/add/feed .

If you'd like to use the node processor, turn on Aggregator Node and add a
new content-type end edit the content-type to enable Aggregator for this.
When you enable Aggregator module, a content-type is automatically created
for the other processor.

Create your first feed
----------------------

Choose Create content / <your content type> and fill in the URL field.
Click on Save.

Features
--------

The module currently offers the following features:
 - content-type based presets
 - SimpleXML-based parser (supports enclosures and namespaces)
 - feed autodetection in HTML pages (it's enough to supply
   http://example.com and the feed will be detected)
 - all of the existing aggregator features (categorization is replaced by
taxonomy)
 - input filters for non-node items
 - a processor which turns items into nodes

