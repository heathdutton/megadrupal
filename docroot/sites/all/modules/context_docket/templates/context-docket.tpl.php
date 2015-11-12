<?php

/**
 * @file
 * Theme the context table with tabs.
 *
 * Available variables:
 * - $tabs: Context tabs.
 * - $form: Filter form.
 * - $table: Rules table.
 */
?>

<div id="ctools-export-ui-list-items" class="context-admin context-docket">
  <div id="docket-wrapper">
    <div id="docket-tabs">
      <?php print drupal_render($tabs); ?>
    </div>

    <div id="docket-list">
      <?php print drupal_render($table); ?>
    </div>
  </div>
</div>
