ENTITY RECUR
========================

This module allows you to recur entities based on repeating criteria.
Only entities that contain date fields can be recurred. The recurring
process will automatically generate copies of the entity, changing the
date to represent the recurring pattern. This is useful for
administrators that do not want to use Date Repeat for recurrences
because separate entities are required (for whatever reason).

Dependencies
------------------------
Entity API
Replicate
Date API
Date

Installation and setup
------------------------
1. Install the module as you would any other
2. Have an entity bundle (e.g. node type) with a date field. It's recommended
that the field is required and have a maximum of one value. The field can have
an end-date, if desired.
3. Navigate to admin/config/content/entity-recur. Enable recurring for the
desired entity bundles, and set the other options as desired.
4. Toggle permissions to your liking.
5. Depending on your configuration, the recur options will be on the
entity add form, and there will be a link visible when viewing an entity
(only of the types that have recurring enabled).

Recurring options
-----------------------
There are currently two types of recurring options available.

1. Pick the days of the week
2. Every N period, where N is a number, and period can be days, weeks,
or months.
