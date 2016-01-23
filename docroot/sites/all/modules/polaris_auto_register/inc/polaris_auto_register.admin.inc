<?php

/**
 * @file
 * Provides administrator related functions for the Polaris Auto Register module.
 */

/**
 * Admin settings form
 */
function polaris_auto_register_settings_form() {
  $form = array();

  $form['polaris_auto_register_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris Credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_access_id'] = array(
    '#type' => 'textfield',
    '#title' => t('PAPI Access ID'),
    '#description' => t('Enter the PAPI <b>Access ID</b> Assigned when you registered to use the Polaris PAPI'),
    '#default_value' => variable_get('polaris_auto_register_access_id'),
    '#size' => 50,
    '#maxlength' => 40,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('PAPI Access Key'),
    '#description' => t('Enter the PAPI <b>Access Key</b> Assigned when you registered to use the Polaris PAPI'),
    '#default_value' => variable_get('polaris_auto_register_access_key'),
    '#size' => 50,
    '#maxlength' => 36,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_polaris_api_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris API Host'),
    '#description' => t('Enter the Polaris <b>API Host</b> for the Polaris PAPI (e.g., catalog.yourlibrary.com)'),
    '#default_value' => variable_get('polaris_auto_register_polaris_api_host'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_polaris_api_staff_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris Staff ID'),
    '#description' => t('Enter the staff id for the staff user that will be making the protected calls'),
    '#default_value' => variable_get('polaris_auto_register_polaris_api_staff_id'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_polaris_api_staff_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris Staff Domain'),
    '#description' => t('Enter the Windows Active Directory domain for the staff user that will be making the protected calls'),
    '#default_value' => variable_get('polaris_auto_register_polaris_api_staff_domain'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_polaris_api_staff_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris Staff Username'),
    '#description' => t('Enter the Windows Active Directory username for the staff user that will be making the protected calls'),
    '#default_value' => variable_get('polaris_auto_register_polaris_api_staff_username'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => TRUE,
  );
  $form['polaris_auto_register_fs']['polaris_auto_register_polaris_api_staff_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris Staff Password'),
    '#description' => t('Enter the Windows Active Directory password for the staff user that will be making the protected calls'),
    '#default_value' => variable_get('polaris_auto_register_polaris_api_staff_password'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_library'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library Information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_barcode_format'] = array(
    '#type' => 'select',
    '#title' => t('Barcode Format'),
    '#description' => t('Which barcode format is used by your library system? This will be used to create a printable card.'),
    '#default_value' => variable_get('polaris_auto_register_barcode_format', 'codabar'),
    '#required' => TRUE,
    '#options' => array(
      'codabar' => 'Codabar',
      'code11' => 'Code 11',
      'code39' => 'Code 39',
      'code39extended' => 'Code 39 Extended',
      'code93' => 'Code 93',
      'code128' => 'Code 128',
      'ean8' => 'EAN-8',
      'ean13' => 'EAN-13',
      'gs1128' => 'GS1-128',
      'i25' => 'Interleaved 2 of 5',
      'intelligentmail' => 'Intelligent Mail',
      'isbn' => 'ISBN-10 and ISBN-13',
      'msi' => 'MSI Plessey',
      'othercode' => 'othercode',
      'postnet' => 'PostNet',
      's25' => 'Standard 2 of 5',
      'upca' => 'UPC-A',
      'upce' => 'UPC-E',
      'upcext2' => 'UPC Supplemental Barcode 2 digits',
      'upcext5' => 'UPC Supplemental Barcode 5 digits',
    ),
  );
  $form['polaris_auto_register_library']['polaris_auto_register_card_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Library card logo path'),
    '#description' => t('If you want your library cards to contain an image at the top (logo, etc.), enter that path/URL here. <b>Ideal size is 265px x 70px or smaller & it must be a .png type of image</b>. Note: if you don\'t enter a path here, the name of your Drupal site will be displayed on the card instead.'),
    '#default_value' => variable_get('polaris_auto_register_card_image_path'),
    '#required' => FALSE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Staff email address'),
    '#description' => t('Emails to customers containing the library card as an attachment will come from this email address'),
    '#default_value' => variable_get('polaris_auto_register_email'),
    '#required' => TRUE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_help_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number to call for help'),
    '#description' => t('Enter a phone number to call for help if there are errors during the registration process. This will be displayed in certain error messages to users.'),
    '#default_value' => variable_get('polaris_auto_register_help_phone'),
    '#required' => TRUE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_county'] = array(
    '#type' => 'textfield',
    '#title' => t('County'),
    '#description' => t('Which county or counties are residents allowed to use to get a card? (Separate multiple counties with a comma.)'),
    '#default_value' => variable_get('polaris_auto_register_county'),
    '#required' => TRUE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#description' => t('e.g, SC, NY, MO, etc. Which state or states are residents allowed to use to get a card? (Separate multiple states with a comma.)'),
    '#default_value' => variable_get('polaris_auto_register_state'),
    '#required' => TRUE,
  );
  $form['polaris_auto_register_library']['polaris_auto_register_other_countries'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow other users in other countries to register for a card'),
    '#description' => t('Normally anyone outside of the county specified above will not be able to register for a card. Check this box to allow users from other countries such as Mexico and Canada to register for cards as well (thereby bypassing the county check).'),
    '#default_value' => variable_get('polaris_auto_register_other_countries'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Use Last 4 Phone Digits as PIN'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['polaris_auto_register_fs2']['polaris_auto_register_user_pin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use last four digits of primary phone number as default PIN'),
    '#description' => t('Check to use the last four digits of the primary phone number as the PIN for the barcode'),
    '#default_value' => variable_get('polaris_auto_register_user_pin'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris API Default Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['polaris_auto_register_fs3']['polaris_auto_register_logon_branch_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Logon Branch ID (LogonBranchID)'),
    '#description' => t('Enter a default value for the Polaris LogonBranchID field'),
    '#default_value' => variable_get('polaris_auto_register_logon_branch_id'),
    '#size' => 10,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_fs3']['polaris_auto_register_logon_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Logon User ID (LogonUserID)'),
    '#description' => t('Enter a default value for the Polaris LogonUserID field'),
    '#default_value' => variable_get('polaris_auto_register_logon_user_id'),
    '#size' => 10,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_fs3']['polaris_auto_register_logon_workstation_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Logon Workstation ID (LogonWorkID)'),
    '#description' => t('Enter a default value for the Polaris LogonBranchID field'),
    '#default_value' => variable_get('polaris_auto_register_logon_branch_id'),
    '#size' => 10,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_fs3']['polaris_auto_register_patron_branch_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Patron Branch ID (PatronBranchID)'),
    '#description' => t('Enter a default value for the Polaris PatronBranchID field'),
    '#default_value' => variable_get('polaris_auto_register_patron_branch_id'),
    '#size' => 10,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  $form['polaris_auto_register_fs4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Markup Placed Before the Registration Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs4']['polaris_auto_register_subtitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Subtitle'),
    '#description' => t('Full HTML: Subtitle of page'),
    '#default_value' => variable_get('polaris_auto_register_subtitle'),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4']['polaris_auto_register_markup1'] = array(
    '#type' => 'textarea',
    '#title' => t('Page Mark Up 1'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_markup1'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4']['polaris_auto_register_markup2'] = array(
    '#type' => 'textarea',
    '#title' => t('Page Mark Up 2'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_markup2'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4']['polaris_auto_register_markup3'] = array(
    '#type' => 'textarea',
    '#title' => t('Page Mark Up 3'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_markup3'),
    '#required' => FALSE,
  );

  // below section is for adding custom subtitle and markup for the Thank You page after successful registration
  $form['polaris_auto_register_fs4b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Markup Placed for the Thank You page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs4b']['polaris_auto_register_thank_you_subtitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Thank You Page Subtitle'),
    '#description' => t('Full HTML: Subtitle of Thank You page'),
    '#default_value' => variable_get('polaris_auto_register_thank_you_subtitle'),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4b']['polaris_auto_register_thank_you_markup1'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank You Page Mark Up 1'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_thank_you_markup1'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4b']['polaris_auto_register_thank_you_markup2'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank You Page Mark Up 2'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_thank_you_markup2'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs4b']['polaris_auto_register_thank_you_markup3'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank You Page Mark Up 3 (this section will display below the card image)'),
    '#description' => t('Full HTML: add a custom paragraph or two, or three!'),
    '#default_value' => variable_get('polaris_auto_register_thank_you_markup3'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Markup for the Email Message'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs_email']['polaris_auto_register_email_markup1'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Message Mark Up 1'),
    '#description' => t('Full HTML: add a custom message to appear <b>before</b> the barcode and pin'),
    '#default_value' => variable_get('polaris_auto_register_email_markup1'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs_email']['polaris_auto_register_email_markup2'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Message Mark Up 2'),
    '#description' => t('Full HTML: add a custom message to appear <b>after</b> the barcode and pin'),
    '#default_value' => variable_get('polaris_auto_register_email_markup2'),
    '#required' => FALSE,
  );

  // $form['polaris_auto_register_fs_reprint_barcode'] = array(
  //   '#type' => 'fieldset',
  //   '#title' => t('Custom Markup for the Reprint Barcode Page'),
  //   '#collapsible' => TRUE,
  //   '#collapsed' => TRUE,
  // );

  // $form['polaris_auto_register_fs_reprint_barcode']['polaris_auto_register_reprint_barcode_subtitle'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Page Subtitle'),
  //   '#description' => t('Full HTML: Subtitle of page'),
  //   '#default_value' => variable_get('polaris_auto_register_reprint_barcode_subtitle'),
  //   '#size' => 60,
  //   '#maxlength' => 60,
  //   '#required' => FALSE,
  // );

  // $form['polaris_auto_register_fs_reprint_barcode']['polaris_auto_register_reprint_barcode_markup1'] = array(
  //   '#type' => 'textarea',
  //   '#title' => t('Reprint Barcode Mark Up 1'),
  //   '#description' => t('Full HTML: add a custom message to appear <b>above</b> the form'),
  //   '#default_value' => variable_get('polaris_auto_register_reprint_barcode_markup1'),
  //   '#required' => FALSE,
  // );

  // $form['polaris_auto_register_fs_reprint_barcode']['polaris_auto_register_reprint_barcode_markup2'] = array(
  //   '#type' => 'textarea',
  //   '#title' => t('Reprint Barcode Mark Up 2'),
  //   '#description' => t('Full HTML: add a custom message to appear <b>below</b> the form'),
  //   '#default_value' => variable_get('polaris_auto_register_reprint_barcode_markup2'),
  //   '#required' => FALSE,
  // );

  $form['polaris_auto_register_fs_reprint_barcode_success'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Markup for the Reprint Barcode Success Page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs_reprint_barcode_success']['polaris_auto_register_' .
      'reprint_barcode_success_subtitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Subtitle'),
    '#description' => t('Full HTML: Subtitle of page'),
    '#default_value' => variable_get('polaris_auto_register_reprint_barcode_success_subtitle'),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs_reprint_barcode_success']['polaris_auto_' .
      'register_reprint_barcode_success_markup1'] = array(
    '#type' => 'textarea',
    '#title' => t('Reprint Barcode Success Mark Up 1'),
    '#description' => t('Full HTML: add a custom message to appear <b>above</b> the reprinted barcode'),
    '#default_value' => variable_get('polaris_auto_register_reprint_barcode_success_markup1'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs_reprint_barcode_success']['polaris_auto_register' .
      '_reprint_barcode_success_markup2'] = array(
    '#type' => 'textarea',
    '#title' => t('Reprint Barcode Success Mark Up 2'),
    '#description' => t('Full HTML: add a custom message to appear <b>below</b> the reprinted barcode'),
    '#default_value' => variable_get('polaris_auto_register_reprint_barcode_success_markup2'),
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris System Defined Field: User1'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs5']['polaris_auto_register_user1_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Question or Label for User1 Field'),
    '#description' => t('Enter a descriptive Question or Label for the User1 field'),
    '#default_value' => variable_get('polaris_auto_register_user1_label'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5']['polaris_auto_register_user1_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for User1 Field'),
    '#description' => t('Enter the description for the User1 field'),
    '#default_value' => variable_get('polaris_auto_register_user1_info'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris System Defined Field: User2'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs5b']['polaris_auto_register_user2_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Question or Label for User2 Field'),
    '#description' => t('Enter a descriptive Question or Label for the User2 field'),
    '#default_value' => variable_get('polaris_auto_register_user2_label'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5b']['polaris_auto_register_user2_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for User2 Field'),
    '#description' => t('Enter the description for the User2 field'),
    '#default_value' => variable_get('polaris_auto_register_user2_info'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5c'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris System Defined Field: User3'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs5c']['polaris_auto_register_user3_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Question or Label for User3 Field'),
    '#description' => t('Enter a descriptive Question or Label for the User3 field'),
    '#default_value' => variable_get('polaris_auto_register_user3_label'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5c']['polaris_auto_register_user3_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for User3 Field'),
    '#description' => t('Enter the description for the User3 field'),
    '#default_value' => variable_get('polaris_auto_register_user3_info'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5d'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris System Defined Field: User4'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs5d']['polaris_auto_register_user4_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Question or Label for User4 Field'),
    '#description' => t('Enter a descriptive Question or Label for the User4 field'),
    '#default_value' => variable_get('polaris_auto_register_user4_label'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5d']['polaris_auto_register_user4_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for User4 Field'),
    '#description' => t('Enter the description for the User4 field'),
    '#default_value' => variable_get('polaris_auto_register_user4_info'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5e'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris System Defined Field: User5'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['polaris_auto_register_fs5e']['polaris_auto_register_user5_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Question or Label for User5 Field'),
    '#description' => t('Enter a descriptive Question or Label for the User5 field'),
    '#default_value' => variable_get('polaris_auto_register_user5_label'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => FALSE,
  );

  $form['polaris_auto_register_fs5e']['polaris_auto_register_user5_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for User5 Field'),
    '#description' => t('Enter the description for the User5 field'),
    '#default_value' => variable_get('polaris_auto_register_user5_info'),
    '#size' => 80,
    '#maxlength' => 80,
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
