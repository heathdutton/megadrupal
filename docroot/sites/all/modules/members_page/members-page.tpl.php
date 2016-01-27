<?php
/**
 * @file
 * Drupal template for the Members Page module.
 *
 * There are 3 variables passed to this template:
 *
 *   $greeting -- in view or edit mode, authenticated or anon, render array.
 *   $sidebar -- in view or edit mode, authenticated or anonymous. html format.
 *   $panel_width -- left and right panel size as %
 *   $register -- a link to the special user registration page.
 */
?>
<div id="members-greeting" style="<?php print $panel_width['left']; ?>">
  <?php print render($greeting); ?>
</div>

<div id="members-sidebar" style="<?php print $panel_width['right']; ?>">
  <div class="sidebar">
    <?php print $sidebar; ?>
  </div>
</div>

<br style="clear:both" />

<?php print $register; ?>

