
CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation

INTRODUCTION
------------

Current Maintainer: Jeremy Lichtman <jeremy@lichtman.ca>

Currently rules that reference a site variable (i.e. some obtained via
a call to variable_get()) require inline php code.

This module allows you to create rule conditions that reference a
variable by name, and check against a value. This results in cleaner
(and potentially safer) rules, particularly when they need to be
exported via a feature.


INSTALLATION
------------

1. Download and enable the module.

2. In your rule, create a new condition, and select
'Site variable to check'.

3. Set the 'The site variable you want to check'
field to be the name of the variable you wish to check against. You do
not need to surround the variable name with quotes.

4. Set the 'The value that the site variable should have'
field to be the value that the variable should be in order for the
condition to return true.

