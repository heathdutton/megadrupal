<?php
// $Id$

/**
 * @file
 * Renders the block that shows the user's current toon, and allows the user to select from other toons.
 *
 */
 
 ?>
<div class="summary-popup <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
	<img src="<?php echo $avatar->render['avatar']; ?>" width="48" height="48" class="avatar select-avatar-block-select-avatar" />

	<div class="popup-frame">
  	<div class="popup-text-wrapper">
  	  <div class="<?php print $title_link_class; ?>"><?php print $title; ?></div>
      <div class="shadow select-avatar-block-select-class color-c<?php echo $avatar->classId; ?>"><?php echo $avatar->level; ?> <?php echo $avatar->race; ?> <?php echo $avatar->class; ?></div>
  		<div class="select-avatar-block-select-realm"><?php echo $avatar->fullrealm; ?></div>
  	</div>
  </div>
</div>


