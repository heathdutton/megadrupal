<?php
/**
 * @file
 * User address list template
 */
?>

<div id="ec-address-list-wrapper">
  <div class="ec-address-list">
    <?php print $address_list; ?>
  </div>
  <hr />
  <div class="ec-address-form">
    <h3><?php print t('Add address'); ?></h3>
    <?php print $address_form; ?>
  </div>
</div>
