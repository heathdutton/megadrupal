<?php
/**
 * @file
 * This template is the theme khipu_field_redirect_form_field.
 */
?>
<div class="khipu-field">
  <h3><?php print drupal_render($form['subject_markup']);?></h3>
  <div class="khipu-col-1">
    <?php if ($form['amount']['#type'] == 'textfield'):?>
      <?php print drupal_render($form['amount']);?>
    <?php else:?>
      <?php print drupal_render($form['amount_markup']);?>
    <?php endif;?>
    <?php if ($form['payer_email']['#type'] == 'textfield'):?>
      <?php print drupal_render($form['payer_email']);?>
    <?php endif;?>

  </div>
  <div class="khipu-col-2">
    <?php print drupal_render_children($form);?>
  </div>
  <div class="clearfix"></div>
</div>
