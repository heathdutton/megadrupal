<?php

/**
 * @file
 * Default views template for displaying a simple node archive.
 *
 * - $view: The View object.
 * - $options: Settings for the active style.
 * - $rows: The rows output from the View.
 * - $title: The title of this group of rows. May be empty.
 *
 * @ingroup views_templates
 */
?>

<?php if (!empty($archive_block)): ?>
  <div class="simple-node-archvie">
    <?php print $archive_block; ?>
  </div>
<?php endif; ?>
