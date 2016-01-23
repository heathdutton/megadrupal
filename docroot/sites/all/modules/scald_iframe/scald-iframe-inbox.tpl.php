<?php
/**
 * @file
 *   Default theme implementation for the Scald Iframe inbox player.
 *
 * Available variables:
 * - $vars['iframe_url'] : The url of the iframe
 * - $vars['iframe_width']
 * - $vars['iframe_height']
 * - $vars['iframe_ratio']
 * - $vars['thumbnail'] : Uri of the image.
 * - $atom : The scald atom.
 *
 */
?>
<a class="inbox-link" href="<?php print $vars['iframe_url']; ?>">
  <img src="<?php print file_create_url($vars['thumbnail']); ?>" alt="" />
</a>