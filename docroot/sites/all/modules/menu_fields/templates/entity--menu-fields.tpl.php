<?php
/**
 * @file
 * Used to override the default entity template.
 * @see entity.tpl.php
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content) ?>
  </div>
</div>
