<?php
/**
 * @file
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of headers(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */

print '<?xml version="1.0" encoding="UTF-8" ?>';
//TODO use *display* title instead of view title -> should be in $title
//TODO remove datasource and feedtype (defined somewhere else in the module)
//TODO set <IMAGE_SOURCE>, <title> etc. See onebox doc reference.
//dpm($view->display['gsa_onebox_provider_1']);
//dpm($view);
?>
<OneBoxResults>
  <provider><?php print $view->display[$view->current_display]->display_title; ?></provider>
