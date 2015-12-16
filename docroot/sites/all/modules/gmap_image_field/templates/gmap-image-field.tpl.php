<?php

/**
 * @file
 * Example template file.
 */

?>

<div class="<?php echo $classes?>">

  <div id="<?php echo $map['id']?>"></div>

  <?php if ($map['enable_descriptions']):?>
  <div class="the-title">
    <?php echo $map['description']?>
  </div>
  <?php endif?>

</div>
