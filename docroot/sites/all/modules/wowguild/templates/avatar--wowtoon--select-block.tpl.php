<?php
// $Id$

/**
 * @file
 * Renders the block that shows the user's current toon, and allows the user to select from other toons.
 *
 */?>
<div class="<?php print $classes; ?> <?php echo $faction_class; ?>">
  <!--  block width: 210  inset: 230x116  -->
  <div id="select-avatar-block-inset" style="background: url('<?php echo $avatar->render['inset']; ?>') no-repeat center; padding-top: 116px;"></div>
  <div id="select-avatar-block-current-toon">
    <div id="select-avatar-block-name" class="">
    <div id="select-avatar-block-char-arrow"></div>
      <?php echo $fullname_link; ?>
    </div>
    <div id="select-avatar-block-class" class="shadow <?php echo $class_color; ?>">
    <span id="select-avatar-block-level"><?php echo $level; ?></span>
    <?php echo $race; ?>
    <?php echo $class; ?>
    </div>
    <div id="select-avatar-block-guild"><?php echo $guild_name; ?></div>
    <div id="select-avatar-block-realm"><?php echo $fullrealm; ?></div>
    
  </div>
</div>