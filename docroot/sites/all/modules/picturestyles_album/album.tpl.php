<?php
/**
 *@file
 *Template file for Album element
*/
?>
<div class="album">
  <?php if(isset($images[0])):?>
    <?php print $images[0];?>
  <?php endif;?>
  <?php if (isset($images[1])):?>
    <?php print $images[1];?>
  <?php endif;?>
  <?php if (isset($images[2])):?>
    <?php print $images[2];?>
  <?php endif;?>
</div>