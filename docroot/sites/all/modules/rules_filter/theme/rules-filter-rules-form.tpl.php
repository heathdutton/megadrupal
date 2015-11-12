<?php

/**
 * @file
 * Theme the Rules table with tabs.
 *
 * Available variables:
 * - $rules_filter_tabs: Rules tabs.
 * - $rules_filter_form: Filter form.
 * - $rules_filter_table: Rules table.
 *
 * @see rules_filter_preprocess_rules_filter_rules_form()
 */
?>

<div id="rules-filter-wrapper">
  <div id="rules-filter-tabs">
    <?php print drupal_render($rules_filter_tabs); ?>
  </div>

  <div id="rules-filter-rules">
    <?php print drupal_render($rules_filter_form) . drupal_render($rules_filter_table); ?>
  </div>
</div>
