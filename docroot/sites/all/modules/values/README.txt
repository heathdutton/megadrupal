
Summary
=======
The Values module is an interface for managing reusable sets of value-label
pairs. The purpose is to allow lists of values to be reused across the
website, without having the configure them in multiple locations.

As a functional example, this module includes the Values Field module, which allows the
admin to assign a reusable value set to a field, instead of using the
allowed values. This way, if you have a form that have, say, multiple Yes/No
fields, you only have to configure it once, and then select it from the Values
list for each field.

To Use
======
To use this module, go into the Values administration page, at
admin/content/values, and you will see a table with the currently configured
value sets.

To add a new value set, click on Add and fill out the information. You will be
able to define the value-label pair for each option.

Dependencies
============
This module can be used by itself. However, exporting capabilities are provided
by enabling the Chaos tools suite module (ctools).

To do
=====
For the Values Field sub-module, implement a database update routine that will
update the stored value whenever you edit the value set information.

Credits
=======
Victor Kareh (vkareh) - http://www.vkareh.net
