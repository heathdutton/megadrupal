Contextual Elements, v7.x-1.x
================================================================================
Contextual Elements provides an interface to allow administrators to add HTML
elements to the headers and footers of their site given certain criteria.

The use case that spawned this module is the need to implement a Google
Analytics like module that was capable of handling multiple multiple GA codes.
The filtering system was modeled from the GA module.


INSTALLATION
--------------------------------------------------------------------------------
1. Download the Contextual Elements module and place it in your site's "modules"
   directory. (usually "sites/all/modules/contrib")

2. In your site, enable the module.

3. Enable permissions for the allowed roles.  BE EXTREMELY CAUTIOUS!  This
   module permits users with the proper permissions to embed ANY code into the
   header and the footer of the site!

4. Go to Configuration > Search and metadata > Contextual Elements to begin
   creating elements.


ROADMAP
--------------------------------------------------------------------------------
In future versions, I'd like to accomplish the following:

1. Store elements as entities.
2. Add default element settings for things such as meta or css elements.
3. Improve security around the headers and footers.
4. Integrate with the Context module.
5. Open an API setting to allow other modules to implement their own contextual
   elements.
6. Integrate into Features or at least make them exportable.

If you would like to help, patches are always welcomed.


TODO
--------------------------------------------------------------------------------
1. Backport to D6.


CONTACT
--------------------------------------------------------------------------------
Chris Albrecht
http://drupal.org/user/176328

http://twitter.com/ChrisAlbrecht
