$Id$

CONTENTS
--------

1. Introduction
2. Installation
3. Known issues

INTRODUCTION
------------

Location chooser module simplifies node entry by providing an easy way to choose
locations from existing nodes, user locations, or a default location entered on
the module's administration pages.  The following functionality is included:

- Adds a select box on node entry forms for chosen content types with location
  items from configurable sources including nodes, the current user, and the
  default location.
- Choosing a location from the select box will automatically set the location
  fields for the FIRST location on the node.
- Locations chosen from the select box will have the same location id (lid) as
  the source node.  This could be useful for grouping in views, etc.
- Options to completely hide the standard location entry form or wrap it in a
  fieldset (for use with the vertical tabs module, for example).
- Settings for a default location to be given to new nodes.
- Optional mode to reuse location lids when changing a source node's location;
  so for example if one venue moves, all events attached to that venue will move
  automatically.  Disabled by default - by default, changing a source location
  will not change all attached events.

INSTALLATION
------------

1. Follow installation instructions for the location module, also enabling
   location chooser module.

2. Enter a default location at /admin/config/content/location/site-default if desired.

3. Configure the module at /admin/config/content/location/location-chooser:
   - choose "target" content types on which to show chooser select box
   - choose "sources," including content types, user locations, and the default.

KNOWN ISSUES
------------

1. Version 1.x will ONLY work with Location 3.x, and will only allow choosing
   locations on Drupal 7 node bundles.  It does not utilize the field api, nor 
   does it function with other fieldable entities.

2. In general, ALL source locations can be seen by any user with permissions
   to EDIT ANY target nodetypes, since the node edit form will contain location
   data from whatever location has been chosen.  This behavior IGNORES security
   features provided by node access modules and permissions settings such as 
   "view all user locations".  Therefore, if source locations must truly never
   be seen by your editors, you must only allow them "edit own [target] content"
   permissions for nodetypes that are location chooser targets.

3. This module will not work for content types with multiple locations.

4. It is currently not possible to position the location chooser field separate
   from the location fieldset in the CCK content type administration interface;
   the location chooser will have the same weight as the location fieldset.
