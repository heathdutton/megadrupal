<?php

/**
 * @file
 * Theme the views table with tabs.
 *
 * Available variables:
 * - $tabs: Views tabs.
 * - $form: Filter form.
 * - $table: Rules table.
 */
?>

<div id="ctools-export-ui-list-items" class="views-admin views-docket">
  <div id="docket-wrapper">
    <div id="docket-tabs">
      <?php print drupal_render($tabs); ?>
    </div>

    <div id="docket-list">
      <?php print drupal_render($table); ?>
    </div>
  </div>
</div>
