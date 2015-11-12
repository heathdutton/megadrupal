<?php

/**
 * @file
 * Custom tpl that displays the entity in the checkout form phase.
 */
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php
    hide($elements['links']['comment']);
    print render($elements);
  ?>
</div>

