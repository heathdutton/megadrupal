POPUP FIELD GROUP
=================

Extends field_group module with an option to make a 'Popup' group.

This module does not mess around with the structure of the form or the DOM
particularly. Rather the children of the group are surrounded in a <div>, which
is then styled as a very simple position-fixed popup.

A button is then rendered on the form which toggles the popup open/closed.


THEMING THE POPUP
-----------------

For convenience this module allows you to choose arbitrary additional CSS files
to be included on the page. This allows you to easily customize the styling of
the popup, even though it may be being viewed in an admin theme.

The system_stream_wrapper module must be installed to use this feature.
