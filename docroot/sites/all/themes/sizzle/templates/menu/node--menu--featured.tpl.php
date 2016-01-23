<?php
/**
 * @file
 * Template for Menu node in Featured view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <div class="menu__wrapper">
    <?php if (!empty($content['field_menu_images'])): ?>
      <div class="menu__image">
        <?php print render($content['field_menu_images'][0]); ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($title)): ?>
      <a href="<?php print $node_url; ?>" class="menu__title link--overlay">
        <h3><?php print $title; ?></h3>

    <?php if (!empty($content['field_menu_one_liner'])): ?>
      <small class="menu__one-liner"><?php print render($content['field_menu_one_liner']); ?></small>
    <?php endif; ?>
      </a>
    <?php endif; ?>
  </div>

  <?php print render($extra); ?>
</article>
