<?php
/**
 * @file
 * Code for profile form registration.
 */

?>

<p><?php print render($intro_text); ?></p>

<div class="row">
      <div class="col-md-6">
        <?php print drupal_render_children($form) ?>
      </div>
    </div>

    <?php if (!empty($form['obiba_agate']) && $form['obiba_agate']['#value'] == 'obiba_agate_user_register_form'): ?>
      <div class="text-center">
        <p><?php print t('Please use this following link to register') ?></p>
        <?php print l(t('Sign Up'), 'agate/register') ?>
      </div>
   <?php endif; ?>
