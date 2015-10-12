<?php
/**
 * @file
 * Template for Uptolike share buttons.
 *
 * Do not remove "uptolike-buttons" class. It is used by Uptolike.
 *
 * Available Variables:
 *  $url: entity url.
 *  $language: share buttons language.
 *  $data: buttons settings.
 */
?>

<div<?php if (isset($url)): ?> data-url="<?php print $url; ?>" <?php endif; ?> data-lang="<?php print $language; ?>" <?php print $data; ?> class="uptolike-buttons"></div>
