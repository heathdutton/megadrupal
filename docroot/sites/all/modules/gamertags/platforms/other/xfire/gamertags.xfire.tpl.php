<?php
/**
 * @file
 * Theme implementation for displaying Xfire Gamertags.
 *
 * Available variables:
 *  - $alt: The alternative image text to display
 *  - $classes: String of classes that can be used to style contextually through
 *      CSS. It can be manipulated through the variable $classes_array from
 *      preprocess functions
 *  - $gamertag: The users Playstation Network gamertag
 *  - $url: The parsed gamertag URL
 *  - $link: The external URL for the Xfire user profile.
 *
 * @see gamertags_preprocess_gamertags_xfire()
 */
?>
<a href="<?php print $link; ?>">
  <img src="<?php print $url; ?>" class="<?php print $classes; ?>" alt="<?php print $alt; ?>" title="<?php print $gamertag; ?>" name="gamertags_xfire" />
</a>