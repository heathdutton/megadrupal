<?php

/**
 * @file
 * Template file to customize gallery markup in prettyphoto.
 */
?>
<div class="pp_gallery">
  <a href="#" class="pp_arrow_previous"><?php print $prev; ?></a>
  <div>
    <ul>
      {gallery}
    </ul>
  </div>
  <a href="#" class="pp_arrow_next"><?php print $next; ?></a>
</div>
