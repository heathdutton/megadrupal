<?php
/**
 * @file views-view-fields--gallery.tpl.php
 * Override of views-view-fields.tpl used for two views:
 * - gallery_collections, and
 * - visualizations
 *
 * Globals available:
 * - $view: The view in use.
 * - $fields: an array of $field objects.
 * - $row: The raw result object from the query, with all data it fetched.
 */
// node title (visualizations) is called title
// term name (gallery_collections) is called name
if ($view->name == 'visualizations') {
  $title_id = 'title';
  $uri_prefix = 'node/';
  $row_id = 'nid';
  // load individual visualizations with colorbox
  $extra_classes = ' lightbox';
}
else { // assume $view->name == 'gallery_collections'
  $title_id = 'name';
  $uri_prefix = 'taxonomy/term/';
  $row_id = 'tid';
  $extra_classes = '';
}

foreach ($fields as $id => $field):
  if (!empty($field->separator)):
    print $field->separator;
  endif;
  if ($id === $title_id):
    // Use an 'a' instead of div as the wrapper, so we can make the whole
    // thumbnail & bezel clickable.
    print l($field->content, $uri_prefix . $row->{$row_id}, array(
      'html' => TRUE,
      'attributes' => array(
        'class' => $field->handler->options['element_wrapper_class'] . $extra_classes,
      ),
    ));
  else:
    print $field->wrapper_prefix;
      print $field->label_html;
      print $field->content;
    print $field->wrapper_suffix;
  endif;
endforeach;
