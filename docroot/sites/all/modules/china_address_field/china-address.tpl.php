<?php

/**
 * @file
 * Default template implementation to display the value of China address field.
 *
 * Available variables:
 * - $address: An array of field values.
 * - $label: Determine whether to display label for each region type.
 */

?>

<?php if ($address['province']) : ?>
  <?php if ($label) : ?> <b> <?php print t('Province'); ?> : </b> <?php endif; ?>
  <?php print $address['province']; ?> 
<?php endif; ?>

<?php if ($address['city']) : ?>
  <?php if ($label) : ?> &nbsp; &nbsp; <b> <?php print t('City'); ?> : </b> <?php endif; ?>
  <?php print $address['city']; ?>
<?php endif; ?>

<?php if ($address['county']) : ?>  
  <?php if ($label) : ?> &nbsp; &nbsp;<b> <?php print t('County'); ?> : </b> <?php endif; ?> 
  <?php print $address['county']; ?>
<?php endif; ?>

<?php if ($address['detail']) : ?>  
  <?php if ($label) : ?> &nbsp; &nbsp;<b> <?php print t('Detail'); ?> : </b> <?php endif; ?> 
  <?php print $address['detail']; ?>
<?php endif; ?>
