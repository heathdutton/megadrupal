
Organic groups timeframe
--------------------------------------------------------------------------------

Put a time frame on posting and editing content in organic groups.

og_timeframe.module
  The API used to restrict access. Doesn't do anything by itself. Modules have
  to extend OgTimeframeHandler or implement its own OgTimeframeHandlerInterface,
  then register this with hook_og_timefield_handler_info().

og_timeframe_datefield.module
  This uses date fields attached to groups as time frame for group content. This
  module is two-fold: It has an base class (not really useful without extending)
  that provides a time frame handler for date fields, and an extended class used
  for date fields attached to groups.


Usage
--------------------------------------------------------------------------------

Activate 'og_timeframe_datefield', add a date field to the group type and set
what group content type it should serve as a time frame for. By enabling
"Collect end date" the date field serves as a 'from – to' window, and without
enabling it, it serves as a simle 'due' date.

Edit your group node/entity and set the date the group content should be allowed
access within. Group content of selected type are now only editable in the time
frame specified in the group's date field.


BUGS
--------------------------------------------------------------------------------
* https://drupal.org/node/1858112
  Problem: Date fields with finest granularity set to 'days' or lower will cause
           DateObject::difference() to report the wrong difference when crossing
	   years, causing og_timeframe to fail.
  Affects: <= date-7.x-2.6.
  Solution: Upgrade to latest date-7.x-2.x-dev or next stable release after 2.6
