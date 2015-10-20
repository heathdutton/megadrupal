Description
-----------

This module allows you to create 100% valid ATOM when using Views 3. It mentions
NZ in the name of the modules as New Zealand Government namespaces are outputted
into the metadata of the ATOM feed. This module can still be used in other
countries, as the metadata can be ignored (and will not cause your feed to
become invalid), or removed through theming.

Features
--------

* Integration with the NZ Govt 'information types' schema, based on the current
  node's content type. See the NZ e-govt website for more information.
* Makes use of ATOM enclosures to embed file attachments
* Adds each taxonomy term as metadata for each ATOM entry
* Proper escaping of HTML entities to XML entities, to ensure a valid feed
  everytime
* Theme functions and templates for both the feed and row output, for those
  that wish to extend and alter the output
* Feed configuration options for:
  * The subtitle
  * The author name and URI
  * The license
* Row configuration options for:
  * The amount of content for each node, either full text, teaser text, or title
    only
  * The ability to add the links to the bottom of the content
  * The ability to add the service links to the bottom of the content (depends
    on the service links module being installed)
* Created valid RFC 4151 tag URI's for each node and the feed itself
* Also as this module simply provides Views plugins, the resulting views
  creating with this module can be stored in code using features.

TODO
----

* Integration with the NZ Govt 'regions' schema. Not too sure where this data
  should come from, perhaps a taxonomy term?

Requirements
------------

Views 3 module is installed and correctly configured

Installation
------------

Copy the 'atom_nzgovt' module directory in to your Drupal sites/all/modules
directory as usual.