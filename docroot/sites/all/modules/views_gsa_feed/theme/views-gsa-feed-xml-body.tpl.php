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
?>

<?php foreach ($themed_rows as $count => $row): ?>
  <record url="<?php print $record_url[$count];?>" mimetype="text/html">
    <metadata>
<?php foreach ($row as $field => $content): ?>
    <?php if (!empty($xml_tag[$field]) && !empty($content)):?>
      <meta name="<?php print $xml_tag[$field];?>" content="<?php print $content;?>"/>
    <?php endif; ?>
<?php endforeach; ?>
    </metadata>
  </record>
<?php endforeach; ?>
