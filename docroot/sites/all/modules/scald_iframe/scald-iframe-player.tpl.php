<?php
/**
 * @file
 *   Default theme implementation for the Scald Iframe Player
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
<div class="iframe-wrapper" style="padding-bottom: <?php print $vars['iframe_ratio'] ?>;">
  <iframe seamless="seamless" <?php print (!empty($vars['iframe_width']) ? 'width="'.$vars['iframe_width'].'"' : '' ); ?> <?php print (!empty($vars['iframe_height']) ? 'height="'.$vars['iframe_height'].'"' : '' ); ?> src="<?php print $vars['iframe_url']; ?>" frameborder="0"></iframe>
</div>
