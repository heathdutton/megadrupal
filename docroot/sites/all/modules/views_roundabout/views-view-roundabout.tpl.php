<?php
/**
 * @file
 * Displays the the view-rows and appends the clss views-roundabout.
 */
?>
<?php if (!empty($title)): ?>
  <h3 class="<?php print $view_roundabout_id; ?>"><a href="#"><?php print $title; ?></a></h3>
<?php endif; ?>

<?php foreach ($rows as $id => $row): ?>
  <div class="<?php print $classes_array[$id]; ?> views-roundabout">
    <?php print $row; ?>
  </div>
<?php endforeach; ?>