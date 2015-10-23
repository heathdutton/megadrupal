<?php
/**
 * @file
 * Template for Past Events.
 */
?>
<?php $rows = array_chunk($rows, 3); ?>
<?php foreach ($rows as $cols): ?>
  <div class="view__row row">
    <?php foreach ($cols as $col): ?>
      <div class="view__col col-sm-4 margin--md--bottom">
        <?php print $col; ?>
      </div>
    <?php endforeach; ?>
  </div>
<?php endforeach; ?>
