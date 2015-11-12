<?php
// $Id$

/**
 * @file
 * Renders the block that shows the user's current toon, and allows the user to select from other toons.
 *
 */
 
 ?>
<div class="select-avatar-block-select-name">
  <img src="<?php echo $avatar->render['avatar']; ?>" width="48" height="48" class="select-avatar-block-select-avatar" />
  <a href="<?php print $url; ?>" rel="<?php print $title_link_rel; ?>" class="<?php print $title_link_class; ?>"><?php print $title; ?></a>
</div>
<div class="shadow select-avatar-block-select-class color-c<?php echo $avatar->classId; ?>"><?php echo $avatar->level; ?> <?php echo $avatar->race; ?> <?php echo $avatar->class; ?></div>
<div class="select-avatar-block-select-realm"><?php echo $avatar->fullrealm; ?></div>
