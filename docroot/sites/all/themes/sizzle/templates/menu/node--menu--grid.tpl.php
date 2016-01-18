<?php
/**
 * @file
 * Template for Menu node in Grid view mode.
 */
?>
<article class="<?php print $classes; ?>">
  <div class="menu__wrapper">
    <?php if (!empty($content['featured'])): ?>
      <?php print render($content['featured']); ?>
    <?php endif; ?>

    <div class="row">
      <div class="col-xs-5 col-sm-12">
        <?php if (!empty($content['field_menu_images'])): ?>
          <div class="menu__image">
            <?php print render($content['field_menu_images'][0]); ?>
          </div>
        <?php endif; ?>
      </div>

      <div class="col-xs-7 visible-xs">
        <?php if (!empty($title)): ?>
          <h4 class="menu__title margin--xs--bottom"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h4>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_nutrition_types'])): ?>
          <div class="menu__nutrition-types margin--sm--top"><?php print render($content['field_menu_nutrition_types']); ?></div>
        <?php endif; ?>
      </div>

      <div class="col-xs-12">
        <?php if (!empty($title)): ?>
          <h4 class="menu__title hidden-xs"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h4>
        <?php endif; ?>

        <?php if (!empty($content['field_menu_nutrition_types'])): ?>
          <div class="menu__nutrition-types margin--sm--top hidden-xs"><?php print render($content['field_menu_nutrition_types']); ?></div>
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
