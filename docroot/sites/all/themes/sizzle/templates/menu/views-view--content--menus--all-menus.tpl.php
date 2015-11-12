<?php
/**
 * @file
 * Template for All Menus.
 */
?>
<div class="container margin--md--bottom padding--md--bottom" data-menu-category="<?php print drupal_html_class($title); ?>">
  <?php if (!empty($title)): ?>
    <a name="<?php print drupal_html_class($title); ?>"></a>
    <h3 class="ribbon ribbon--secondary"><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $term; ?>

  <?php $rows = array_chunk($rows, 3); ?>
  <?php foreach ($rows as $cols): ?>
    <div class="view__row row">
      <?php foreach ($cols as $col): ?>
        <div class="view__col col-xs-12 col-sm-6 col-md-<?php print $col_size; ?>">
          <?php print $col; ?>
        </div>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
</div>
