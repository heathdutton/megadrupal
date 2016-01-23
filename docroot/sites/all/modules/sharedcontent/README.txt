CONTENTS OF THIS FILE
---------------------

 * About Shared Content
 * Documentations
 * TODOs
 * Incompatibility
 * SEO
 * Maintainers
 * Links


ABOUT SHARED CONTENT
--------------------

The project Shared Content introduces technology to build a content sharing
network. Different to classical content sharing that duplicate full content
items across multiple servers, this project is first about notifying system
about content availability and building references to relevant/related content
from the network.

The architecture introduces two key content concepts:

  * Local content
  * Remote content

The shared content architecture thus provides two main components:

  * Shared Content Server
  * Shared Content Client

DOCUMENTATIONS
--------------

  * Concept
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-concept-20130312.pdf
  * Technical Specification
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-technicalspecification-20130312.pdf
  * Operations Manual
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-operationsmanual-20130312.pdf
  * ShareBoard User Manual
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-shareboardusermanual-20130312.pdf
  * RichMedia - Detail Specification
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-richmedia-detailspecification-20130312.pdf
  * Chapters - Detail Specification
    http://www.md-systems.ch/sites/default/files/sharedcontent-documentation-chapters-detailspecification-20130312.pdf


TODOs AND KNOWN ISSUES
----------------------

See issue queue at http://drupal.org/project/issues/sharedcontent.


INCOMPATIBILITIES
-----------------

The platform this module was initially targeted to was not suitable to make use
of the uuid module. Still all Shared Content entities needs uuids to identify
them uniquely across multiple systems. For that reason all Shared Content
entities has an uuid attribute and maintains proper values for them by itself.
It is to be expected that Shared Content does not play well in combination with
the uuid module.

Since Services version 3.4, services added CSRF protection. Services Client does
not support that. If you use session authentication you must use Services 3.3.

SUPPORTED AUTHENTICATION MECHANISMS
-----------------------------------

The service endpoint can be queried both with http basic and session
authentication. To make use of http basic authentication on the client, the
Services client module needs to be patched. See https://drupal.org/node/2023089.

SEO
---

Shared Content uses a http query attribute to detect if the site should be
rendered with an alternative theme. This causes the same content to be available
through two urls. This can cause ranking penalties at search engines. In order
to prevent this the following rule should be added to the robots.txt file.

  Disallow: /*sc[overlay]


MAINTAINERS
-----------

 * Miro "miro_dietiker" Dietiker, concept and lead architect.
 * Sascha "Berdir" Grossenbacher, architect.
 * Christian "corvus_ch" Häusler, developer


LINKS
-----

  * http://drupal.org/project/sharedcontent
  * http://www.md-systems.ch/projekt/2013/shared-content