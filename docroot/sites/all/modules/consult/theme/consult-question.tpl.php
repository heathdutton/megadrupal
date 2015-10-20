<?php
/**
 * @file
 * Consult question template.
 *
 * Do not change js- prefixed classes as they are required for the Backbone
 * views to attach correctly.
 */
?>
<div <?php print $attributes; ?>>
  <?php print render($content); ?>

  <div class="js-answers consult--question--answers">

  </div>
</div>
