<?php
/**
 * @file fortissimmo-form-landingpage.tpl.php
 *
 * Theme the landing page shown after a Fortissimmo form has been filled in
 *
 *  Available variables:
 *  $property_id: the ID of the property for which the inquiry was made
 *                (not available for the general request form)
 *  $property_name: the name of the property for which the inquiry was made
 *                  (not available for the general request form)
 */
?>

<div class="fortissimmo-form-landingpage">
  <?php if (empty($property_id)): ?>
    <?php print t('Thank you for your request. We will keep you informed on new properties from now on.'); ?>
  <?php else: ?>
    <?php print t('Thank you for your request. We will keep you informed on the property "@property_name".', array('@property_name' => $property_name)); ?>
  <?php endif; ?>
</div>