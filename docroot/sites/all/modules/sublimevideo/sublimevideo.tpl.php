<?php
/**
 * @file
 * SublimeVideo Player
 *
 * Variables available:
 * - $file_url: The url of the file to be played
 *
 */
?>


<div class="sublimevideo">
<video class="sublime" width="640" height="360" poster="video-poster.jpg" preload="none">
  <source src="<?php print $file_url ?>" />
</video>
</div>
