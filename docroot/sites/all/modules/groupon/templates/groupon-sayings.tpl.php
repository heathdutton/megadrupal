<?php
/**
 * @file
 * Default theme implementation to display groupon sayings.
 *
 * Available variables:
 * - $sayings: The array of groupon sayings.
 */
?>
<div id='groupon-wrapper'>
  <?php
    foreach ($sayings as $key => $value) :
      $title = check_plain($value['title']);
      $desc = $value['desc'];
  ?>
    <div class='groupon-row'>
      <div class='groupon-says-title'><h2><?php echo $title;?></h2></div>
      <div class='groupon-says-desc'><?php echo $desc;?></div>
    </div>
  <?php endforeach; ?>
</div>
