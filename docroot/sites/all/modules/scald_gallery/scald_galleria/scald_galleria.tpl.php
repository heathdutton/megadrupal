<?php
/**
 * @file
 * Theme implementation of the Scald Galleria player
 *
 * Variables:
 * - $options: An array of settings from the player.
 * - $galleria_id: The ID of the gallery instance.
 * - $items: An array of items to be displayed in the gallery.
 */
?>
<?php if (!empty($options['fullscreen_link'])): ?>
<div class="galleria-fullscreen"><a class="galleria-fullscreen-link">fullscreen</a></div>
<?php endif; ?>
<div class="scald-gallery <?php print $galleria_id ?>" style="width: <?php print $options['width']; ?>; height: <?php print $options['height']; ?>">
  <?php foreach ($items as $item): ?>
    <?php print $item; ?>
  <?php endforeach; ?>
</div>
