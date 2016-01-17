<?php
/**
 * @file
 * Renders an imagereference with an optional link.
 * Returned by hook_odir_show_file_list_item()
 */
?>
<?php if (isset($url_link)): ?>
  <a href="<?php print $url_link ?>" rel="<?php print $rel_attribute ?>" class="<?php print $rel_attribute ?>">
<?php endif ?>
  <img alt="<?php print $alt ?>" src="<?php print $url_image ?>" />
<?php if ($htmllink_image_cache_preset): ?>
  </a>
<?php endif ?>
