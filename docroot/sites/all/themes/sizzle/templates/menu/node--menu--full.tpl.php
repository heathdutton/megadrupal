<?php
/**
 * @file
 * Template for Menu node in Full view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <?php if (!empty($title)): ?>
    <h1 class="menu__title clear-margin--top margin--md--bottom visible-xs">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h1>
  <?php endif; ?>

  <div class="row">
    <div class="col-sm-5">
      <?php if (!empty($content['field_menu_images'])): ?>
        <div class="menu__image margin--sm--bottom">
          <?php if (!empty($content['featured'])): ?>
            <?php print render($content['featured']); ?>
          <?php endif; ?>
          <?php print render($content['field_menu_images']); ?>
        </div>
      <?php endif; ?>
    </div>
    <div class="col-sm-7">
      <?php if (!empty($title)): ?>
        <h1 class="menu__title clear-margin--top hidden-xs">
          <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
        </h1>
      <?php endif; ?>

      <?php if (!empty($content['field_menu_nutrition_types'])): ?>
        <div class="menu__nutrition-types"><?php print render($content['field_menu_nutrition_types']); ?></div>
      <?php endif; ?>

      <?php if (!empty($content['body'])): ?>
        <div class="menu__text margin--sm--top"><?php print render($content['body']); ?></div>
      <?php endif; ?>

      <?php if (!empty($content['field_menu_one_liner'])): ?>
        <small class="menu__one-liner"><?php print render($content['field_menu_one_liner']); ?></small>
      <?php endif; ?>

      <?php if (!empty($content['field_menu_variants'])): ?>
        <div class="menu__variants border--xs--top"><?php print render($content['field_menu_variants']); ?></div>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
