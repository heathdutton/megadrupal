Numeric Field Filter
------------------------
by:
 * Oleksandr Trotsenko

Description
-----------
Numeric Field Filter introduces ability to filter numeric fields attached to any
entity not only against a constant factor (like it does standard views numeric
filter), however also to filter against another numeric field attached to the
entity.

For example: you have a field "Distance to sea" and a field "Distance to the
capital". Using this filter you can build up a view that will get you all the
entities that are closer to sea than to its capital, i.e. it allows you to build
queries like ...WHERE field_distance_sea > field_distance_capital. It supports
all basic operators for number comparison like:

 * less than (<)
 * less than or equal to (<=)
 * equal (=)
 * not equal (!=)
 * greater than or equal to (>=)
 * greater than (>)
 * between
 * not between

Apart from its ability to filter against values of another field, it also
introduces ability of simple mathematical operations between fields before they
are compared. Recalling to our previous example, a slightly more advanced
version would be to get all entities that are closer to the sea at least by 1.5
times than to its capital. This kind of filtering is also possible with Numeric
Field Filter module. Thus it would build a query like
...WHERE field_distance_sea > 1.5 * field_distance_capital. Right now filter
allows you to execute the following operations on the fields before comparing
them:

 * summation (+)
 * substraction (-)
 * multiplication (*)
 * division (/)

Requirements
-------------
Numeric Field Filter module requires enabled the following modules:
 * Number (ships with Drupal core)
 * Views (http://drupal.org/project/views)

Installation
------------
 * Copy the module's directory to your modules directory and activate the
 module.
