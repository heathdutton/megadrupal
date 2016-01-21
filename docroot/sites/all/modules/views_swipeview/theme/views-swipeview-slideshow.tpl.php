<?php
/**
 * @file
 * The SwipeView styles.
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="swipeview-slides">
  <?php foreach ($rows as $id => $row): ?>
    <div<?php if ($classes_array[$id]): ?> class="<?php print $classes_array[$id]; ?>" <?php endif; ?>>
      <?php print $row; ?>
    </div>
  <?php endforeach; ?>
</div>

<?php if ($swipeview_options['navigation'] || $swipeview_options['pager']): ?>
  <div class="swipeview-controls">
    <?php if ($swipeview_options['navigation']): ?>
      <div class="swipeview-control swipeview-navigation">
        <div class="previous navigation-link" data-navigation-type="prev">
          Previous
        </div>
        <div class="next navigation-link" data-navigation-type="next">
          Next
        </div>
      </div>
    <?php endif; ?>

    <?php if ($swipeview_options['pager']): ?>
      <div class="swipeview-control swipeview-pager">
        <?php foreach($rows as $id => $row): ?>
          <div class="pager-item" data-navigate-to-page="<?php print $id; ?>">
            <?php print $id; ?>
          </div>
        <?php endforeach; ?>
      </div>
    <?php endif; ?>
  </div>
<?php endif; ?>
