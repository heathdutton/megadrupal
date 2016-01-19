<?php
/**
 * @file
 * Code for login block.
 */

?>

<p><?php print render($intro_text); ?></p>

<div class="row">
  <div class="col-md-6">
    <?php print drupal_render_children($form) ?>
    <div class="md-top-margin">
      <?php print l(t('Forgot your password?'), 'user/password') ?>
    </div>
  </div>
</div>
