<?php

/**
 * @file
 * Squid Pro's theme implementation to customize the contact page.
 */
?>

<?php if (theme_get_setting('show_map', 'squid_pro')): ?>
<div id="contact_map">
 <?php $iframe_link = theme_get_setting('iframe_link', 'squid_pro'); 
 print $iframe_link;
 ?>
</div>
<?php endif; ?>

<div class="my-form-wrapper grid_6">
<h3>Send us a message..</h3>
  <?php print $contact; ?>
</div>
<?php if (theme_get_setting('show_office_one', 'squid_pro')): ?>
<div id="office_one" class="grid_3">
<h3>Office One</h3>
 <?php $address = theme_get_setting('address', 'squid_pro'); 
 print $address;
 ?>
</div>
<?php endif; ?>
<?php if (theme_get_setting('show_office_two', 'squid_pro')): ?>
<div id="office_two" class="grid_3">
<h3>Office Two</h3>
 <?php $address2 = theme_get_setting('address2', 'squid_pro'); 
 print $address2;
 ?>
</div>
<?php endif; ?>
