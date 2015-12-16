<?php
  /*
    field array
    $$variables


    vCard available values
      $prefix
      $first_name
      $last_name
      $suffix
      $full_name
      $photo
      $title
      $organization
      $address_type
      $address
      $city
      $region
      $country
      $phone_default
      $phone_cell
      $phone_fax
      $phone_home
      $email
      $link // contact URL


    Vcard link info
      $label - Link label user data
      $vcard_url  - URL for custom formatting
      $vcard_link  - full link to get vcard
  */
?>
<div class="vcardfield-wrapper <?php echo $classes.' '.$zebra; ?>">

    <?php echo $full_name; ?><br />
    <?php echo $vcard_link; ?>

</div>
