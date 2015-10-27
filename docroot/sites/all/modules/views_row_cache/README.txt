
-- SUMMARY --

View Row Cache implements a Views cache plugin that allows the output of single
rows to be cached independently.

* This makes it possible to reuse row cache even for authenticated users that
  are seeing different result sets. Since this is the primary use case this
  module was created for, no result caching is implemented.
* The bits of markup that may vary per-user are rendered in the post render
  phase and replaced through placeholders. This way cache entries can be reused
  for any user, without needing cid user granularity.
* Cache entries identifiers are based on a hash of the row content, this way no
  specific invalidation strategy is needed: as soon the row content changes a
  new id is computed. Cache entries can be purged by a cron run if an expiration
  time different from "Permanent" is configured.
* Cache entries identifiers include the row base field value, so
  business-logic-specific invalidation strategies can be implemented on top of
  the generic system.


-- REQUIREMENTS --

Views.


-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.


-- USAGE --

To enable Views Row Cache on a View, just edit it and enable the "Row-based"
caching strategy. You can configure also an expiration time for cache entries.

Views Row Cache works by slicing the View output after identifying row markup
and caching it. For this to actually work you need to inject two variables in
the view style template, see the provided "views-view-table--example.tpl.php"
for more details. Views Row Cache supports the four style plugins natively
provided by Views, if you need to support other ones you can call
"views_row_cache_views_style_plugin_preprocess()" from the template preprocess
function, see "views_row_cache_preprocess_views_view_grid()" for an example.

If you have user-specific output in your markup, you can inject placeholders in
the row output: cache entries will be post-processed and placeholders will be
replaced on the fly by the actual values provided by the specified callback
function. A placeholder comes in the following form:

[views_row_cache:mymodule_replacement_callback:row_id:arg1:arg2:...:argN]

where "views_row_cache" is a fixed prefix, "my_module_replacement_callback" is
the callback function name, "row_id" is the row identifier, and "arg1" - "argN"
are additional parameters to be passed to the callback function. For instance:

function mymodule_preprocess_views_view_field(&$variables) {
  $row_id = views_row_cache_get_row_id($variables['view'], $variables['row']);
  $variables['output'] .= ' [views_row_cache:mymodule_replacement_callback:' . $row_id . ':foo:bar]';
}

function mymodule_replacement_callback($view, $row, $row_id, $args) {
  $foo = $args[0];
  $bar = $args[1];
  $entity = mymodule_entity_load($row_id);
  return mymodule_get_user_data($GLOBALS['user'], $entity, $foo, $bar);
}

See "views_row_cache_replacement__table__active_column()" for a real-life
example.
