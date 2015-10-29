<?php
/**
 * @file
 * Template for Menu node in Stacked view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <div class="menu__wrapper">
    <?php if (!empty($content['featured'])): ?>
      <?php print render($content['featured']); ?>
    <?php endif; ?>

    <div class="row">
      <div class="col-md-3">
        <?php if (!empty($content['field_menu_images'])): ?>
          <div class="menu__image">
            <?php print render($content['field_menu_images'][0]); ?>
          </div>
        <?php endif; ?>
      </div>
      <div class="col-md-9">
        <?php if (!empty($title)): ?>
          <h3 class="menu__title"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h3>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_nutrition_types'])): ?>
          <div class="menu__nutrition-types"><?php print render($content['field_menu_nutrition_types']); ?></div>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_teaser_text'])): ?>
          <p class="menu__teaser-text"><?php print render($content['field_menu_teaser_text']); ?></p>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_one_liner'])): ?>
          <small class="menu__one-liner"><?php print render($content['field_menu_one_liner']); ?></small>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_variants'])): ?>
          <div class="menu__variants border--xs--top"><?php print render($content['field_menu_variants']); ?></div>
        <?php endif; ?>
      </div>
    </div>
  </div>

  <?php print render($extra); ?>
</article>
