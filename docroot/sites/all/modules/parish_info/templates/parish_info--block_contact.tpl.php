<?php
/**
 * @file
 * Contact info block for parishes.
 *
 * Available variables:
 * - $template: The template being used (default is 'block_contact').
 * - $address_street: Street address.
 * - $address_street_2: Street address (line 2).
 * - $address_city: City.
 * - $address_state: State.
 * - $address_zip: Zip code.
 * - $map_link: Map link.
 * - $phone: Phone number.
 * - $fax: Fax number.
 * - $email: Email address.
 */
?>

<p>
  <?php print $address_street; ?><?php if ($map_link) : ?> (<?php print $map_link ?>)<?php endif; ?><br />
  <?php if ($address_street_2) : ?>
    <?php print $address_street_2; ?><br />
  <?php endif; ?>
  <?php print $address_city; ?>, <?php print $address_state; ?> <?php print $address_zip; ?>
</p>

<?php if ($phone || $fax || $email) : ?>
  <?php
    $contact_data = array(
      empty($phone) ? NULL : t('Phone: ') . $phone,
      empty($fax) ? NULL : t('Fax: ') . $fax,
      empty($email) ? NULL : t('Email: ') . '<a href="mailto:' . $email . '">' . $email . '</a>',
    );
    $contact_printable = implode('<br />', array_filter($contact_data));
  ?>
  <p><?php print $contact_printable; ?></p>
<?php endif; ?>
