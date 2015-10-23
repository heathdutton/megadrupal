<?php
/**
 * @file views-view-simplyscroll.tpl.php
 *
 */
?>

<div id="<?php print $simplyscroll_id ?>" class="<?php print $classes; ?> clearfix">
  <ul class="<?php print $simplyscroll_list_class ?>">
    <?php foreach ($rows as $row): ?>
      <li><?php print $row ?></li>
    <?php endforeach; ?>
  </ul>
</div>
