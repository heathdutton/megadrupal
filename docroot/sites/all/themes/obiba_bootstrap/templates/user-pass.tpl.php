<?php
/**
 * @file
 * Code for login form.
 */

?>

<p><?php print render($intro_text); ?></p>

<div class="row">
  <div class="col-md-6">
      <?php print drupal_render_children($form) ?>
  </div>
</div>
