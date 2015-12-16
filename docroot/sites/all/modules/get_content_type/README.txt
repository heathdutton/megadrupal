Get_Content_Type Module

The get_content_type module fills an oversight by the D5 developers.
When they moved the part of CCK (sometimes called CCK-Lite) into core
for creating new content types, they forgot the analog to
"taxonomy/term/xxx," that is "node/type/xxx." This simple module
provides that function.

It may be invoked with "node/type/content-type/teaser/number-per-page"
or "node_get_by_type(content-type, teaser, number-per-page)" where:
* content-type is the (internal) name of the type;
* teaser is TRUE or FALSE to show in teaser format (TRUE is default);
* number-per-page is the number of nodes to show per page (default is
the same as the post settings).

The "sticky" attribute is honored (as is sticky-encoded weighting).

For example: assume that post settings is at the default of 10 per page,
"http://mysite.com/node/type/story" will show all the "story" type nodes
as teasers with 10 per page.

"http://mysite.com/node/type/ed-classified/FALSE/5" will show 5 classified ads per page as full nodes.

