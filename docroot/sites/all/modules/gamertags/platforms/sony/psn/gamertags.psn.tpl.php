<?php
/**
 * @file
 * Theme implementation for displaying Playstation Network Gamertags.
 *
 * Available variables:
 *  - $alt: The alternative image text to display
 *  - $classes: String of classes that can be used to style contextually through
 *      CSS. It can be manipulated through the variable $classes_array from
 *      preprocess functions
 *  - $gamertag: The users Playstation Network gamertag
 *  - $region: The region the user is located in (only valid for Playstation)
 *  - $url: The parsed gamertag URL
 *
 * @see gamertags_preprocess_gamertags_psn()
 */
?>
<img src="<?php print $url; ?>" class="<?php print $classes; ?>" alt="<?php print $alt; ?>" title="<?php print $gamertag; ?>" name="gamertags_psn" />