
Composite Views Filter
====

Allows you to create one filter condition that enables a bunch of filter
conditions. Sometimes one condition isn't enough for your filter, but you
still want the user to be able to change the filter (group).

For example
----

You have a View that lists human relationships. You might want a filter that
shows all Males in love with Females. That would require 2 conditions: 1) side
A = male and 2) side B = female. This would be a group. You'll also want the
opposite filter: Females in love with Males. That would be another group. You
might also want to show all gay relationships where both sides are older than
60. This would require several conditions (and be another group).

How?
----

You add a special filter: "Global: Composite Filter". Be sure to expose it.
You can define groups in this filter's settings. For instance:

	m2f|Boys loving girls
	f2m|Girls loving boys

These groups are then available when adding other filters.

You will create ALL filters the same way as usual: a whole list of conditions
under the "Filter criteria" header in a View. You can add every condition to
one particular group (or to none, which means it's always active).

The result will be a dropdown with your group names (and a general option to
show all results). If you add more exposed filters (not within a group!),
those will show up the usual way. No problem.

How advanced?
----

The sky is the limit?
