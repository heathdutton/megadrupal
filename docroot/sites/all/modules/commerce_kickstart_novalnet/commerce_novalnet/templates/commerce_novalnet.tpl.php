<!-- Mandate overlay START-->
<link rel='stylesheet' type='text/css' media='all' href='<?php print $form_style; ?>'>
<div class="bgCover">&nbsp;</div>
<div id='sepa_mandate_overlay_block' style='display:none;' class='overlay_window_block' onkeydown = 'return key_lock(event)'>
  <div class='nn_header'>
    <h1><?php print $confirm_title; ?></h1>
  </div>
  <div class='body_div' id='overlay_window_block_body'>
  <p>
    <table>
      <tr>
        <td><?php print $payee; ?></td><td>:</td><td><span id='sepa_overlay_payee_span'>&nbsp;</span></td>
      </tr>
      <tr>
        <td><?php print $creditor_identification_number;?></td><td>:</td><td><span id='sepa_overlay_creditoridentificationnumber_span'>&nbsp;</span></td>
      </tr>
      <tr>
        <td><?php print $mandate_reference;?></td><td>:</td><td><span id='sepa_overlay_mandatereference_span'>&nbsp;</span></td>
      </tr>
      <tr><td colspan=3><br/><?php print $mandate_confirm_paragraph;?><br/><br/></td></tr>
    </table>
    <table>
      <tr>
        <td><?php print $enduser_fullname;?></td><td>:</td><td><span id='overlay_card_holder_name'></span></td>
      </tr>
      <tr>
        <td><?php print $address;?></td><td>:</td><td><?php print $address_value;?></td>
      </tr>
      <tr>
        <td><?php print $zipcode_and_city;?></td><td>:</td><td><?php print $zipcode_and_city_value;?></td>
      </tr>
      <tr>
        <td><?php print $country;?></td><td>:</td><td><span id='mandate_overlay_country'></span></td>
      </tr>
      <tr>
        <td><?php print $email;?></td><td>:</td><td><?php print $email_value;?></td>
      </tr>
      <tr id='nn_sepa_overlay_iban_tr'>
        <td><?php print $iban; ?></td><td>:</td><td><span id='sepa_overlay_iban_span'>&nbsp;</span></td>
      </tr>
      <tr id='nn_sepa_overlay_bic_tr'>
        <td><?php print $bic; ?></td><td>:</td><td><span id='sepa_overlay_bic_span'>&nbsp;</span></td>
      </tr>
    </table>
    <br/>
    <?php print $city_value; ?>, <span id='sepa_overlay_mandatedate_span'>&nbsp;</span>, <span id='overlay_card_holder_sign'>&nbsp;</span>
  </p>
  </div>
  <div class='nn_footer'>
  <img src='<?php print $plugin_img_dir; ?>novalnet.png' alt='Novalnet AG' style='float:right;width:20%'/>
    <span class='mandate_confirm_btn' onclick='confirm_mandate_overlay();' id='mandate_confirm_btn'> <?php print $confirm_btn; ?></span>
    <span class='mandate_confirm_btn' onclick='close_mandate_overlay_on_cancel();' id='mandate_cancel_btn' > <?php print $cancel_btn; ?></span>

  </div>
</div>
<!-- Mandate overlay END-->

