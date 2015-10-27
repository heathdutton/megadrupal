<?php

/**
 * @file
 * Custom tpl that displays the entity in the review phase.
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php
    print render($entity->rendered);
  ?>
</div>
