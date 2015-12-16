<?php
/**
 * @file
 * Template for View content: Blog Posts.
 */
?>
<?php $rows = array_chunk($rows, 1); ?>
<?php foreach ($rows as $cols): ?>
  <div class="view__row row">
    <?php foreach ($cols as $col): ?>
      <div class="view__col col-md-12">
        <?php print $col; ?>
      </div>
    <?php endforeach; ?>
  </div>
<?php endforeach; ?>
