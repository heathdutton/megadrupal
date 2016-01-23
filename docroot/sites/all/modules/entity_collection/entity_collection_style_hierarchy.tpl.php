<?php
/**
 * @file
 * This is the template file for the hierarchy style plugin.
 */

?>
<?php if( empty($parent) ): ?>
<div class="<?php print $classes; ?>">
  <?php print render($children) ?>
</div>

<?php elseif ( empty($children) ): ?>
<div class="<?php print $classes; ?>">
  <?php print render($parent) ?>
</div>

<?php else: ?>
<div class="<?php print $classes; ?>">
    <div class="parent"><?php print render($parent) ?></div>
    <div class="children"><?php print render($children) ?></div>
</div>
<?php endif ?>
