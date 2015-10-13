<?php
/**
 * Template file for status messages
 * printed as layout blocks.
 *
 * This is needed to get sane messages
 * out from Drupal.
 *
 * Since Drupal messages doesn't really
 * play nice with renderable array
 * especially if devel module dsm() or dpm()
 * function used.
 */
?>
<?php if (!empty($messages)) : ?>
  <div <?php print $attributes;?>>
    <?php print $messages; ?>
  </div>
<?php endif; ?>