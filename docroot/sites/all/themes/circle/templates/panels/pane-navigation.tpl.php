<?php
/**
 * @file
 * Theme implementation to display the messages area, which is normally
 * included roughly in the content area of a page.
 *
 * This utilizes the following variables thata re normally found in
 * page.tpl.php:
 * - $main_menu
 * - $secondary_menu
 * - $breadcrumb
 * - $mission
 *
 * Additional items can be added via theme_preprocess_pane_messages(). See
 * template_preprocess_pane_messages() for examples.
 */
 ?>
<div class="pe-navigation" class="menu <?php if (!empty($main_menu)): print "withprimary"; elseif (!empty($secondary_menu)): print " withsecondary"; endif; ?> ">
  <?php if (!empty($main_menu)): ?>
    <div class="pe-primary-nav clearfix">
      <?php print $main_menu; ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($secondary_menu)): ?>
    <div class="pe-secondary-nav clearfix">
      <?php print $secondary_menu; ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($breadcrumb)): ?>
    <div class="breadcrumb-wrapper"><?php print $breadcrumb; ?></div>
  <?php endif; ?>
  <?php if (!empty($mission)): ?>
    <div class="mission"><?php print $mission; ?></div>
  <?php endif; ?>

</div> <!-- /navigation -->
