<?php

/**
 * @file media_soundcloud/themes/media-soundcloud-audio.tpl.php
 *
 * Template file for theme('media_soundcloud_audio').
 *
 * Variables available:
 *  $uri - The media uri for the SoundCloud audio (e.g., soundcloud://u/[user-name]/a/[audio-code]).
 *  $audio_id - The unique identifier of the SoundCloud audio (e.g., u/[user-name]/a/[audio-code]).
 *  $id - The file entity ID (fid).
 *  $url - The full url including query options for the SoundCloud iframe.
 *  $options - An array containing the Media SoundCloud formatter options.
 *  $api_id_attribute - An id attribute if the Javascript API is enabled;
 *  otherwise NULL.
 *  $width - The width value set in Media: SoundCloud file display options.
 *  $height - The height value set in Media: SoundCloud file display options.
 *  $title - The Media: SoundCloud file's title.
 *  $alternative_content - Text to display for browsers that don't support
 *  iframes.
 *
 */

?>
<div class="<?php print $classes; ?> media-soundcloud-<?php print $id; ?>">
  <iframe class="media-soundcloud-player" <?php print $api_id_attribute; ?>width="<?php print $width; ?>" height="<?php print $height; ?>" title="<?php print $title; ?>" src="<?php print $url; ?>" frameborder="0" allowfullscreen><?php print $alternative_content; ?></iframe>
</div>
