<?php
/**
 * @file
 * Contains Licensing information for miniOrange SAML Login Module.
 */

 /**
 * Showing Licensing form info.
 */
 function miniorange_saml_licensing() {

  $dyi_plan = "'drupal_miniorange_saml_basic_plan'";
  $premium_plan = "'drupal_miniorange_saml_premium_plan'";
  $admin_username = "'" . variable_get('miniorange_saml_customer_admin_email', '') . "'";

  $form['markup_1'] = array(
    '#markup' => '<div class="mo_saml_table_layout">'
    . '<table class="mo_saml_local_pricing_table">'
    . '<h2>Licensing Plans (On-Prem)</h2><hr>'
    . '<tr style="vertical-align:top;">'
    . '<td><div class="mo_saml_local_thumbnail mo_saml_local_pricing_free_tab">'
    . '<h3 class="mo_saml_local_pricing_header">Free</h3>'
    . '<h4 class="mo_saml_local_pricing_sub_header">(You are automatically on this plan)</h4>'
    . '<br/><hr><p class="mo_saml_local_pricing_text" style="padding:10px;">$0</p><hr>'
    . '<p class="mo_saml_local_pricing_text">Unlimited Authentications via IdP<br>'
    . 'Basic Role Mapping<br>Basic Attribute Mapping<br/><br><br><br><br><br><br/><br><br></p>',
  );

  $form['markup_2'] = array(
    '#markup' => '<hr><p class="mo_saml_local_pricing_text">Basic Support by Email</p></td>'
    . '<td><div class="mo_saml_local_thumbnail mo_saml_local_pricing_paid_tab" >'
    . '<h3 class="mo_saml_local_pricing_header">Do it yourself</h3><p><?p>'
    . '<h4 class="mo_saml_local_pricing_sub_header" style="padding-bottom:8px !important;">'
    . '<a class="btn btn-primary btn-large" style="padding:5px;" onclick="payment(' . $dyi_plan .', ' 
    . $admin_username .');">Click to Upgrade</a>*</h4><hr>'
    . '<p class="mo_saml_local_pricing_text" style="padding:10px;">$149 - One Time Payment</p><hr>'
    . '<p class="mo_saml_local_pricing_text">'
    . 'Unlimited Authentications via IdP<br>Customized Role Mapping<br>Customized Attribute Mapping<br>'
    . 'Auto-Redirect to IdP<br>Step-By-Step Guide to Setup IdP<br>Single Logout<br>Protect your whole site<br>'
    . 'Options to select SAML Request Binding Type<br>Integrated Windows Authentication<br><br><br/></p><hr>'
    . '<p class="mo_saml_local_pricing_text" >Basic Support by Email</p></div></td>',
  );

  $form['markup_3'] = array(
    '#markup' => '<td><div class="mo_saml_local_thumbnail mo_saml_local_pricing_free_tab" >'
    . '<h3 class="mo_saml_local_pricing_header">Premium</h3><p></p>'
    . '<h4 class="mo_saml_local_pricing_sub_header" style="padding-bottom:8px !important;">'
    . '<a class="btn btn-primary btn-large" style="padding:5px;" onclick="payment(' . $premium_plan .', ' 
    . $admin_username .');">Click to upgrade</a>*</h4><hr>'
    . '<p class="mo_saml_local_pricing_text">$149 + One Time Setup Fees <br>( $45 per hour )</p><hr>'
    . '<p class="mo_saml_local_pricing_text">Unlimited Authentications via IdP<br>Customized Role Mapping<br>'
    . 'Customized Attribute Mapping<br>Auto-Redirect to IdP<br>Step-By-Step Guide to Setup IdP<br>Single Logout<br>Protect your whole site<br>'
    . 'Options to select SAML Request Binding Type<br>Integrated Windows Authentication<br>Multiple IdP Support for Cloud Service Providers<br>'
    . 'End to End Identity Provider Configuration **<br></p><hr><p class="mo_saml_local_pricing_text">Premium Support Plans Available</p>'
    . '</div></td></tr></table>'
  );

  
  $form['markup_4'] = array(
    '#markup' => '<h3>Identity Providers Supported</h3>'
    . 'Google Apps, ADFS, Okta, Salesforce, Shibboleth, SimpleSAMLphp, OpenAM, Centrify, Ping, RSA'
    . ', IBM, Oracle, OneLogin, Bitium, WSO2, NetIQ, miniOrange Identity Provider'
  );

  $form['markup_5'] = array(
    '#markup' => '<h3>Steps to Upgrade to Premium Plugin</h3>'
    . '<ol><li>You will be redirected to miniOrange Login Console. Enter your password with which you created an'
    . ' account with us. After that you will be redirected to payment page.</li>'
    . '<li>Enter you card details and complete the payment. On successful payment completion, you will see the '
    . 'link to download the premium plugin.</li>'
    . 'Once you download the premium plugin, just unzip it and replace the folder with existing plugin. Clear Drupal Cache.</li></ol>'
  );

  $form['markup_6'] = array(
    '#markup' => '<h3>** End to End Identity Provider Integration</h3>'
    . 'We will setup a Conference Call / Gotomeeting and do end to end configuration for you for IDP '
    . 'as well as plugin. We provide services to do the configuration on your behalf.'
  );

  $form['markup_7'] = array(
    '#markup' => '<h3>*Integrated Windows Authentication</h3>'
    . 'With Integrated windows authentication, if the user comes to your Drupal Site from a domain joined machine'
    . ' then he will not even have to re-enter his credentials because he already did that when he unlocked his computer.</div>'
  );

  return $form;

 }