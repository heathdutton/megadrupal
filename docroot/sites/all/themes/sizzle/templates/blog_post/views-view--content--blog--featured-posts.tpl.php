<?php
/**
 * @file
 * Template for Featured Posts.
 */
?>
<?php $rows = array_chunk($rows, 2); ?>
<?php foreach ($rows as $cols): ?>
  <div class="view__row row">
    <?php foreach ($cols as $col): ?>
      <div class="view__col col-sm-6">
        <?php print $col; ?>
      </div>
    <?php endforeach; ?>
  </div>
<?php endforeach; ?>
