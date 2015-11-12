<?php

/**
 * @file
 * Default view template to display content using Salvattore.
 */
?>

<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div id="salvattore-grid" data-columns>
<?php foreach ($rows as $id => $row): ?>
  <div <?php if ($classes_array[$id]) print 'class="' . $classes_array[$id] . '"'; ?>>
    <?php print $row; ?>
  </div>
<?php endforeach; ?>
</div>
