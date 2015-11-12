CONTENTS OF THIS FILE
---------------------

 * About Cue Field
 * TODOs
 * Supported players
 * Incompatibility

ABOUT CUE FIELD
---------------

Cue Field allows to make use of field collections to provide chaptering
for media players.

TODOs
-----

 * Provide an api function to build the fragmented url of a chapter.
 * Deal with more than one player on one single page.
 * A player like jPlayer or jw_player might be added to the page using
   any module. How can we support all those different ways of including
   the same player?

SUPPORTED PLAYERS
-----------------

 * jwPlayer rendered by the jw_player module.
 * jPlayer rendered by the audiofield module.

INCOMPATIBILITIES
-----------------

The platform this module was initially targeted to was not suitable to
make use of the uuid module. Still cue field needs uuids to identify
chapters uniquely and long term persistent. For that reason cue_field
contains parts of the schema and entity altering that are part of the
uuid. It is to be expected that Cue Field does not play well in
combination with the uuid module.
