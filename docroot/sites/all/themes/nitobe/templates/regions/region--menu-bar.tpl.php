<?php
// $Id: region--menu-bar.tpl.php,v 1.3 2010/11/26 06:17:01 shannonlucas Exp $
/**
 * @file
 * The menu bar for Nitobe.
 *
 * The following variables are added to those normally available to
 * region.tpl.php:
 * - $main_menu The array of primary links.
 * - $nitobe_main_menu The HTML for the rendered primary links.
 * - $nitobe_secondary_menu The HTML for the rendered secondary links.
 * - $secondary_menu The array of secondary links.
 *
 * @see _nitobe_build_menu_bar()
 * @see nitobe_preprocess_region()
 */
?>
<?php if ($content || isset($nitobe_main_menu)): ?>
  <div id="header-links" class="<?php echo $classes; ?>">
    <?php if (isset($nitobe_main_menu)) echo $nitobe_main_menu; ?>
    <?php if (isset($nitobe_secondary_menu)) : ?>
      <hr class="break"/>
    <?php echo $nitobe_secondary_menu; ?>
    <?php endif; ?>
    <?php if ($content): ?>
      <?php if (isset($nitobe_main_menu)): ?>
        <hr class="break"/>
      <?php endif; ?>
      <?php echo $content; ?>
    <?php endif; ?>
  </div><!-- #header-links -->
<?php else: ?>
  <hr class="rule-bottom no-menu-rule grid-16"/>
<?php endif; ?>
