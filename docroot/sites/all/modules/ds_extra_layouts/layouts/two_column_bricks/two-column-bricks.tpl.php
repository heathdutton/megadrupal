<?php
/**
 * @file
 * Display Suite Two Column Bricks template.
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
 * - $above_left: Rendered content for the "Above Left" region.
 * - $above_left_classes: String of classes that can be used to style the "Above
 * -  Left" region.
 *
 * - $above_right: Rendered content for the "Above Right" region.
 * - $above_right_classes: String of classes that can be used to style the 
 * - "Above Right" region.
 *
 * - $middle: Rendered content for the "Middle" region.
 * - $middle_classes: String of classes that can be used to style the "Middle" 
 * - region.
 *
 * - $below_left: Rendered content for the "Below Left" region.
 * - $below_left_classes: String of classes that can be used to style the "Below
 * -  Left" region.
 *
 * - $below_right: Rendered content for the "Below Right" region.
 * - $below_right_classes: String of classes that can be used to style the 
 * - "Below Right" region.
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

    <<?php print $above_left_wrapper; ?> class="ds-above-left<?php print $above_left_classes; ?>">
      <?php print $above_left; ?>
    </<?php print $above_left_wrapper; ?>>

    <<?php print $above_right_wrapper; ?> class="ds-above-right<?php print $above_right_classes; ?>">
      <?php print $above_right; ?>
    </<?php print $above_right_wrapper; ?>>

    <<?php print $middle_wrapper; ?> class="ds-middle<?php print $middle_classes; ?>">
      <?php print $middle; ?>
    </<?php print $middle_wrapper; ?>>

    <<?php print $below_left_wrapper; ?> class="ds-below-left<?php print $below_left_classes; ?>">
      <?php print $below_left; ?>
    </<?php print $below_left_wrapper; ?>>

    <<?php print $below_right_wrapper; ?> class="ds-below-right<?php print $below_right_classes; ?>">
      <?php print $below_right; ?>
    </<?php print $below_right_wrapper; ?>>

    <<?php print $footer_wrapper; ?> class="ds-footer<?php print $footer_classes; ?>">
      <?php print $footer; ?>
    </<?php print $footer_wrapper; ?>>

</<?php print $layout_wrapper ?>>

<!-- Needed to activate display suite support on forms. -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
