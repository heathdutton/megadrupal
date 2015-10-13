Views Filter Option Limit
=========================

This module allows Views exposed filters that are on Entityreference fields to
have their visible options limited according to the values of other filters on
the targeted entity type.

It is related to the Reference field option limit module, though does not
require it. (https://drupal.org/project/reference_option_limit)

Example
-------

Suppose you have article nodes which have a reference field to the city node
they are about. Each city node has a field for its country. You can use this
module to create a view of article nodes with exposed filters for the country
and city, where the city options are only those cities in the currently selected
countries.

The user of this view has this workflow:
- load the view
- select one or more countries from the country filter and submit
- the view updates, and the city filter now shows only relevant cities
- select one or more cities and submit

The steps to build this view are:
- configure the entityreference field on article that points to cities to
  provide the Views filter handler with limited options
- build the basic view showing article nodes
- add a relationship to city nodes
- add an exposed filter on the city relationship for the country field
- add an exposed filter for the city field. Configure it to be limited by the
  country field.

TODO
----

This module does not yet support:

- contextual filters with multiple values and an AND operator, i.e. where the
  argument is of the form '1,2,3' rather than '1+2+3'.
- default values on an exposed filter.
- updating of the restricted filter options via AJAX.
- having the restricted filter itself be on a chain of relationship, if these
  cause the relationship ID to be different from the underlying field name.
- having limiting filters be on relationships itself on the relationship that
  corresponds to the restricted field.

If you require any of these features, please consider either providing a patch
or sponsoring development.
