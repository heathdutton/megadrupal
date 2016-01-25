<?php

/**
 * @file
 * Profiling report item default template.
 * 
 * Please don't override unless you really need it because of a custom theme
 * incompatiblity.
 */
?>
<div<?php print $attributes; ?>>
  <h3><span<?php print $title_attributes; ?>><?php print $title; ?></span></h3>
  <div class="profiling-collapsible">
    <?php print render($content); ?>
  </div>
</div>
