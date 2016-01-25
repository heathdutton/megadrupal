<?php
/**
 * @file media_issuu/includes/themes/media-issuu-document.tpl.php
 *
 * Template file for theme('media_issuu_document').
 *
 * Variables available:
 *  $uri - The media uri for the Issuu document (e.g., issuu:chirojeugd-vlaanderen/docs/oudersbrochure).
 *  $config_id - The unique identifier of the Issuu document (e.g., 5552872/4992625).
 *  $width - The width value set in Media: Issuu file display options.
 *  $height - The height value set in Media: Issuu file display options.
 *  $fullscreen - fullscreen boolean variable
 */
?>
<div data-url=<?php print $url; ?>
     style="width: <?php print $width; ?>px; height: <?php print $height; ?>px;"
     class="issuuembed">
</div>
