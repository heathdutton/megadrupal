Display exposed filter values in your view.

What?
-----

Use this field if you want to pass data from exposed filters as context filters
into a view_field. Hide the filterfield from display and use its rendered token
in the arguments string for the viewfield.

You can configure the field to render multiple filter values as 1+2+3 to use
in an OR query or as 1,2,3 for an AND query. Strings can be convered to
uppercase or lowercase if needed.

An empty filter value will be rendered as 'all'.


How?
----

Extract the module to a modules directory under sites or profiles, then hit
your module admin page and enable it. You can find it in the "Views" section.


Why?
----

I needed to run two count queries for each row inside a view that calculates an
average. The http://drupal.stackexchange.com/questions/28435/views3-and-subqueries
solution worked, until the count queries needed to inherit exposed filters from
the parent view.

With the views_filterfield module I could create the count query as a view and
make it easier to manage the context filters from within Drupal.


Caveats
-------

- The raw field tokens do not work.
- You'll end up running quite a few queries. Don't hurt your SQL server.
