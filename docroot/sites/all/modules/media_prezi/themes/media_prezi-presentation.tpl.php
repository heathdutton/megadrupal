<?php

/**
 * @file
 * Media_prezi/themes/media-prezi-presentation.tpl.php.
 *
 * Template file for theme('media_prezi-presentation').
 *
 * Variables available:
 *  $uri - The media uri for the Prezi presentation (e.g., prezi://v/xsy7x8c9).
 *  $presentation_id - Unique identifier of Prezi presentation (e.g., xsy7x8c9).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the Prezi iframe.
 *  $options - An array containing the Media Prezi formatter options.
 *  $api_id_attribute - An id attribute if the Javascript API is enabled;
 *  otherwise NULL.
 *  $width - The width value set in Media: Prezi file display options.
 *  $height - The height value set in Media: Prezi file display options.
 *  $title - The Media: YouTube file's title.
 *  $alternative_content - Text to display for browsers that don't support
 *  iframes.
 */

?>
<div class="<?php print $classes; ?> media-prezi-<?php print $id; ?>">
  <iframe class="media-prezi-player"   src="<?php print $url; ?>"  width="<?php print $width; ?>" height="<?php print $height; ?>" title="<?php print $title; ?>"  frameborder="0" webkitAllowFullScreen mozAllowFullscreen allowfullscreen></iframe>
</div>
