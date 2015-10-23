CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Examples
  * Installation
  * Known Issues/Shortcomings
  * Maintainers


INTRODUCTION
------------
Contextual Filters allow a View to be filtered based on arguments passed via URL
or other embedded method (e.g. the View Reference module), but contextual
filters are inflexible compared with normal filters (which allow for "contains,"
"begins with," "ends with," etc.).

This module allows contextual filter arguments to be used as replacement tokens
in filters and table arguments, so that passed-in arguments (such as from URLs
or View Reference fields) can be used as values in normal View filters.


EXAMPLES
--------
To search for a node by title containing some keywords:
  1. Create a new node-based ("Content") View: Go to admin/structure/views/add
     and under "Show", choose "Content".
  2. Add a "Global: Null" Contextual Filter (this step is required to allow the
     view to receive at least 1 argument).
  3. Add a "Content: Title" Filter: Under "Filter Criteria," click "Add."
     Choose "Content: Title," set the "Operator" to "Contains", and set the
     "Value" to "***!1***" (without the quotes). Click "Apply."
  4. Done! Any value passed as the first Contextual Filter argument will be used
     as the value for the Content: Title filter.

To search for nodes containing the same title as a specified node:
  1. Start by completing the example "to search for a node by title containing
     some keywords," as given above.
  2. Edit the "Global: Null" Contextual Filter: Enable "Specify validation
     criteria," and set "Validator" to "Content."
  3. Edit the "Content: Title (contains ***!1***)" filter: Change the "Value"
     from "***!1***" to "***%1***".
  4. Done!  If you pass in a valid NID as the first Contextual Filter argument,
     its title will be used as the search value for the Content: Title filter.

INSTALLATION
------------
Activate the module, then administer a desired View.  Add as many Contextual
Filters as you need filter values (using "Global: Null", if appropriate), then
insert corresponding tokens ("***!1***" for the first value, "***!2***" for the
second, etc.) as filter values.  Additionally, "***%1***" may be used to insert
the title for the first argument (for example, if the argument is a node), and
so on.


KNOWN ISSUES/SHORTCOMINGS
-------------------------
None, so far.


MAINTAINERS
-----------
- jay.dansand (Jay Dansand)
