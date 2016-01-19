<?php
/**
 * @file
 * Template override to allow a plain text export of lesson dates, without any
 * of the extra stuff normally added by Views Data Export.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $rows: An array of row items. Each row is an array of content
 *   keyed by field ID.
 * - $header: an array of haeaders(labels) for fields.
 * - $themed_rows: a array of rows with themed fields.
 * @ingroup views_templates
 */

foreach ($themed_rows as $count => $row):
  foreach ($row as $field => $content):
?>
<?php print strip_tags($content); // strip html so its plain txt. ?>
<?php endforeach; ?>

<?php endforeach;