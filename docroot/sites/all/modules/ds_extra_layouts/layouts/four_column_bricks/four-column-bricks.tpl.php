<?php
/**
 * @file
 * Display Suite Four Column Bricks template.
 *
 * Available variables:
 *
 * Layout:
 * - $classes: String of classes that can be used to style this layout.
 * - $contextual_links: Renderable array of contextual links.
 * - $layout_wrapper: wrapper surrounding the layout.
 *
 * Regions:
 *
 * - $header: Rendered content for the "Header" region.
 * - $header_classes: String of classes that can be used to style the "Header" 
 * - region.
 *
 * - $above_first: Rendered content for the "Above First" region.
 * - $above_first_classes: String of classes that can be used to style the 
 * - "Above First" region.
 *
 * - $above_second: Rendered content for the "Above Second" region.
 * - $above_second_classes: String of classes that can be used to style the 
 * - "Above Second" region.
 *
 * - $above_third: Rendered content for the "Above Third" region.
 * - $above_third_classes: String of classes that can be used to style the 
 * - "Above Third" region.
 *
 * - $above_fourth: Rendered content for the "Above Fourth" region.
 * - $above_fourth_classes: String of classes that can be used to style the 
 * - "Above Fourth" region.
 *
 * - $middle: Rendered content for the "Middle" region.
 * - $middle_classes: String of classes that can be used to style the "Middle" 
 * - region.
 *
 * - $below_first: Rendered content for the "Below First" region.
 * - $below_first_classes: String of classes that can be used to style the 
 * - "Below First" region.
 *
 * - $below_second: Rendered content for the "Below Second" region.
 * - $below_second_classes: String of classes that can be used to style the 
 * - "Below Second" region.
 *
 * - $below_third: Rendered content for the "Below Third" region.
 * - $below_third_classes: String of classes that can be used to style the 
 * - "Below Third" region.
 *
 * - $below_fourth: Rendered content for the "Below Fourth" region.
 * - $below_fourth_classes: String of classes that can be used to style the 
 * - "Below Fourth" region.
 *
 * - $footer: Rendered content for the "Footer" region.
 * - $footer_classes: String of classes that can be used to style the "Footer" 
 * - region.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> clearfix">

  <!-- Needed to activate contextual links. -->
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

    <<?php print $header_wrapper; ?> class="ds-header<?php print $header_classes; ?>">
      <?php print $header; ?>
    </<?php print $header_wrapper; ?>>

    <<?php print $above_first_wrapper; ?> class="ds-above-first<?php print $above_first_classes; ?>">
      <?php print $above_first; ?>
    </<?php print $above_first_wrapper; ?>>

    <<?php print $above_second_wrapper; ?> class="ds-above-second<?php print $above_second_classes; ?>">
      <?php print $above_second; ?>
    </<?php print $above_second_wrapper; ?>>

    <<?php print $above_third_wrapper; ?> class="ds-above-third<?php print $above_third_classes; ?>">
      <?php print $above_third; ?>
    </<?php print $above_third_wrapper; ?>>

    <<?php print $above_fourth_wrapper; ?> class="ds-above-fourth<?php print $above_fourth_classes; ?>">
      <?php print $above_fourth; ?>
    </<?php print $above_fourth_wrapper; ?>>

    <<?php print $middle_wrapper; ?> class="ds-middle<?php print $middle_classes; ?>">
      <?php print $middle; ?>
    </<?php print $middle_wrapper; ?>>

    <<?php print $below_first_wrapper; ?> class="ds-below-first<?php print $below_first_classes; ?>">
      <?php print $below_first; ?>
    </<?php print $below_first_wrapper; ?>>

    <<?php print $below_second_wrapper; ?> class="ds-below-second<?php print $below_second_classes; ?>">
      <?php print $below_second; ?>
    </<?php print $below_second_wrapper; ?>>

    <<?php print $below_third_wrapper; ?> class="ds-below-third<?php print $below_third_classes; ?>">
      <?php print $below_third; ?>
    </<?php print $below_third_wrapper; ?>>

    <<?php print $below_fourth_wrapper; ?> class="ds-below-fourth<?php print $below_fourth_classes; ?>">
      <?php print $below_fourth; ?>
    </<?php print $below_fourth_wrapper; ?>>

    <<?php print $footer_wrapper; ?> class="ds-footer<?php print $footer_classes; ?>">
      <?php print $footer; ?>
    </<?php print $footer_wrapper; ?>>

</<?php print $layout_wrapper ?>>

<!-- Needed to activate display suite support on forms. -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
