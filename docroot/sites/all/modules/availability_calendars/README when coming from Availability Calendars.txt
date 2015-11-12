/**
 * Special README for users coming form Availability Calendars module.
 *
 * This file describes the changes between the already existing Availability
   Calendars and this new fields based Availability Calendar module.
 */

The availability calendar module defines an availability calendar field. It is
a complete fields-based rewrite of version 7.x-2.x of the Availability
Calendars module.


Upgrading from Availability Calendars 7.x-2.x or earlier
--------------------------------------------------------
To Drupal this is a different module from the already existing Availability
Calendars module. This makes upgrading via update.php a bit tricky. Therefore, a
separate update module has been created. This module can be found in the latest
7.x-2.x package. So install that version as well. The Availability Calendars
update module contains an UPGRADE.txt with more detailed information about
upgrading.


Compatibility with version 2 of Availability Calendars
------------------------------------------------------
A lot of settings has been changed, moved, removed or added. Below you can find
an overview of what happened with all settings:


Old                                    With fields
--------------------------------------------------------------------------------------------------
Enable for content types               Add an availability calendar field to the content type.

System wide and per node settings:
 * Show automatically on node page     Define on Manage display page of a content type.
 * Allow to override per node          Obsolete, we now have field and field instance setting.
 * Show calendars in teasers           Define on Manage display page of a content type.
 * Show a key for the calendars        Available as a block. Use the block reference module
                                       (http://drupal.org/project/blockreference) to put it in
                                       between other fields.
 * Show an edit link per month         Removed. For now, the only way of editing an a calendar is
                                       the interactive calendar. Different widgets and formatters
                                       can be added on request.
 * Edit calendar on its own tab/page   Removed for now.
 * Show a note before each week        Removed for now. Not sure about the UI for this as the per
                                       month editing has been removed.
 * Show only the first letter ...      Can be set separately in "Manage display" (show calendar)
                                       and "Manage fields" (edit calendar).
 * Do not show dates in the past       Removed, defaults to not showing states in the past.
 * Allow split day statuses            Renamed to "show split state", can be set separately in
                                       "Manage display" (show calendar) and "Manage fields" (edit
                                       calendar). This setting has become a setting that only
                                       influences display, not storage.
 * Number of months to display         "Manage display", thus per view mode (teaser, full, ...).
 * Number of months for editors        "Manage fields".

States
 * 1 set of states on admin page       1 global set of states on admin page, with the option to
                                       define a subset of allowed states per field instance.
 * 1 default state on admin page       Default state per field instance.

Styling
 * Basic styling on admin page         Basic styling on admin page.


New settings
------------
 * Show week number before each week  Can be set separately in "Manage display" (show calendar)
                                      and "Manage fields" (edit calendar).
 * Enable/disable calendar            Per node, as part of the field value. However at instance
                                      setting level you can disable the disabling per node.


New render options
------------------
 * viewport (with settings for horizontal and vertical number of months, and
   months to scroll).


New features
------------
 * Show a booking formlet on the (node/entity) page that receives the arrival
   and departure dates that can subsequently be posted to a webform.
