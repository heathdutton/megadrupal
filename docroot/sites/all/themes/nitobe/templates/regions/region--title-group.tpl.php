<?php
// $Id: region--title-group.tpl.php,v 1.3 2010/11/16 15:36:36 shannonlucas Exp $
/**
 * @file
 * Handles rendering the site title for Nitobe.
 *
 * The following variables are added to those normally available to
 * region.tpl.php:
 * - $front_page The path of the site's front page.
 * - $logo The URI for the site logo, if present.
 * - $nitobe_title The site title with Nitobe's title effect applied.
 * - $title The page title, if present
 * - $site_name The site's name.
 * - $site_slogan The site slogan, if present.
 *
 * @see _nitobe_build_title_group()
 * @see nitobe_preprocess_region()
 */
?>
<div class="<?php echo $classes; ?>">
  <?php if ($logo): ?>
    <a href="<?php echo url("<front>"); ?>" title="<?php echo $site_name; ?>"><img src="<?php echo $logo; ?>" alt="<?php echo $site_name; ?>" id="logo" /></a>
  <?php endif; ?>
  <?php if ($nitobe_title && isset($title)): ?>
    <span class="site-title"><a href="<?php echo url("<front>"); ?>" title="<?php echo $site_name; ?>"><?php echo $nitobe_title; ?></a></span>
  <?php elseif ($nitobe_title && !isset($title)): ?>
    <h1 class="site-title"><a href="<?php echo url("<front>"); ?>" title="<?php echo $site_name; ?>"><?php echo $nitobe_title; ?></a></h1>
  <?php endif; ?>
  <?php if ($site_slogan): ?>
    <span class="site-slogan"><?php echo $site_slogan; ?></span>
  <?php endif; ?>
</div><!-- /title-group -->
