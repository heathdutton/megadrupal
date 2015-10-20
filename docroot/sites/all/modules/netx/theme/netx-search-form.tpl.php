<?php

/**
 * @file
 * This template handles the layout of the NetX search form. Emulates views
 * exposed filter to reuse css styles.
 *
 * Variables available:
 * - $form: Form array.
 */
?>

<div class="views-exposed-form">
  <div class="views-exposed-widgets clearfix">
    <?php foreach (array('search', 'sort', 'sort_type', 'apply') as $key): ?>
      <div class="views-exposed-widget">
        <?php print drupal_render($form[$key]); ?>
      </div>
    <?php endforeach; ?>
  </div>
</div>
<?php print drupal_render_children($form); ?>
