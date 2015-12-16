<?php
/**
 * @file
 *
 * Default theme implementation to display a shelter record from Petfinder.com.
 * For more information on the fields returned, visit:
 *
 * http://www.petfinder.com/developers/api-docs
 * http://api.petfinder.com/schemas/0.9/petfinder.xsd
 *
 * Available variables:
 * - $shelter: The complete shelter record from Petfinder.com
 * - $page: Flag for the page state
 *
 * Most other standard page-level variables are also available at this level
 */

//_petfinder_dpm($variables);
?>
<div class="<?php print $classes; ?> petfinder-shelter-<?php print $shelter['id']; ?>">
  
  <?php if (!$page) : ?>
  <div class="petfinder-shelter-name">
    <?php print $shelter['name']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['address1'])) : ?>
  <div class="petfinder-shelter-address1">
    <?php print $shelter['address1']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['address2'])) : ?>
  <div class="petfinder-shelter-address2">
    <?php print $shelter['address2']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['city']) || !empty($shelter['state']) || !empty($shelter['zip'])) : ?>
  <div class="petfinder-shelter-citystatezip">
    <?php if (!empty($shelter['city'])) : ?><span class="petfinder-shelter-city"><?php print $shelter['city']; ?>, </span><?php endif; ?>
    <?php if (!empty($shelter['state'])) : ?><span class="petfinder-shelter-state"><?php print $shelter['state']; ?> </span><?php endif; ?>
    <?php if (!empty($shelter['zip'])) : ?><span class="petfinder-shelter-zip"><?php print $shelter['zip']; ?></span><?php endif; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['country'])) : ?>
  <div class="petfinder-shelter-country">
    <?php print $shelter['country']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['phone'])) : ?>
  <div class="petfinder-shelter-phone">
    Phone: <?php print $shelter['phone']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['fax'])) : ?>
  <div class="petfinder-shelter-fax">
    FAX: <?php print $shelter['fax']; ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($shelter['email'])) : ?>
  <div class="petfinder-shelter-email">
    <?php print l($shelter['email'], 'mailto:' . $shelter['email']); ?>
  </div>
  <?php endif; ?>

</div>
