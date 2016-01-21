<?php
/**
 * @file
 */
?>
<?php echo theme('image', array('path' => $info['screenshot'])); ?>
<h3><?php echo $info['name']; ?></h3>
<?php if ($description) : ?>
  <div class='description'><?php echo $info['description']; ?></div>
<?php endif; ?>
