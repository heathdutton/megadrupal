<?php

/**
 * @file media_8tracks/includes/themes/media-8tracks-audio.tpl.php
 *
 * Template file for theme('media_8tracks_audio').
 *
 * Variables available:
 *  $uri - The media uri for the 8Tracks audio (e.g., 8tracks://id/1234).
 *  $audio_id - The unique identifier of the 8Tracks audio (e.g., 1234).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the 8Tracks iframe.
 *  $options - An array containing the Media 8Tracks formatter options.
 *  $api_id_attribute - An id attribute if the Javascript API is enabled;
 *  otherwise NULL.
 *  $width - The width value set in Media: 8Tracks file display options.
 *  $height - The height value set in Media: 8Tracks file display options.
 *  $title - The Media: 8Tracks file's title.
 *  $alternative_content - Text to display for browsers that don't support
 *  iframes.
 *
 */

?>

<div class="<?php print $classes; ?> media-8tracks-<?php print $id; ?>">
  <iframe class="media-8tracks-player" <?php print $api_id_attribute; ?>width="<?php print $width; ?>" height="<?php print $height; ?>" title="<?php print $title; ?>" src="<?php print $url; ?>" frameborder="0" allowfullscreen><?php print $alternative_content; ?></iframe>
</div>
