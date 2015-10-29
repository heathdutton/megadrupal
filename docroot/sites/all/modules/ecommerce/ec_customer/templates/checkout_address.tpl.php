<?php

/**
 * @file
 */

if (isset($form['use_for_shipping'])) {
  hide($form['use_for_shipping']);
}
hide($form['select_address']);
hide($form['address_submit']);
?>
<div id="<?php echo $form['#address_type']; ?>-address-form" class="address-form">
  <?php echo drupal_render_children($form); ?>
</div>
<?php
if (isset($form['use_for_shipping'])) {
  echo render($form['use_for_shipping']);
}
?>
<?php echo render($form['select_address']); ?>
<?php echo render($form['address_submit']); ?>