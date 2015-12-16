<?php
/**
 * @file
 * Token support for the Contact Info module.
 */

/**
 * Implements hook_token_info().
 */
function contactinfo_token_info() {
  $type = array(
    'name' =>  t('Contact information'),
    'description' => t('Tokens for this site\'s contact information.'),
  );

  $tokens['type'] = array(
    'name' => t('Contact information type'),
    'description' => t('Either "Personal" or "Organization/Business".'),
  );
  $tokens['given-name'] = array(
    'name' => t('First Name'),
    'description' => t('Site contact person\'s first name.'),
  );
  $tokens['family-name'] = array(
    'name' => t('Last Name'),
    'description' => t('Site contact person\'s last name.'),
  );
  $tokens['org'] = array(
    'name' => t('Organization/Business Name'),
    'description' => t('The name of the organization or business.'),
  );
  $tokens['tagline'] = array(
    'name' => t('Tagline'),
    'description' => t('A short tagline.'),
  );
  $tokens['adr-street-address'] = array(
    'name' => t('Street Address'),
    'description' => t('The street address portion of the contact address.'),
  );
  $tokens['adr-extended-address'] = array(
    'name' => t('Extended Address'),
    'description' => t('The extended address portion of the contact address.'),
  );
  $tokens['adr-locality'] = array(
    'name' => t('City'),
    'description' => t('The city portion of the contact address.'),
  );
  $tokens['adr-region'] = array(
    'name' => t('State/Province'),
    'description' => t('The state or province portion of the contact address.'),
  );
  $tokens['adr-postal-code'] = array(
    'name' => t('Postal Code'),
    'description' => t('The postal code portion of the contact address.'),
  );
  $tokens['adr-country-name'] = array(
    'name' => t('Country'),
    'description' => t('The country name portion of the contact address.'),
  );
  $tokens['location-longitude'] = array(
    'name' => t('Longitude'),
    'description' => t('Longitude, in full decimal format (like -121.629562).'),
  );
  $tokens['location-latitude'] = array(
    'name' => t('Latitude'),
    'description' => t('Latitude, in full decimal format (like 38.827382).'),
  );
  $tokens['phone-voice'] = array(
    'name' => t('Voice Phone Number(s)'),
    'description' => t('Voice phone numbers, separated by commas.'),
  );
  $tokens['phone-fax'] = array(
    'name' => t('Fax Number(s)'),
    'description' => t('Fax numbers, separated by commas.'),
  );
  $tokens['email'] = array(
    'name' => t('Email'),
    'description' => t('This site\'s contact email address.'),
  );

  return array(
    'types' => array(
      'contactinfo' => $type,
    ),
    'tokens' => array(
      'contactinfo' => $tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function contactinfo_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'contactinfo') {
    $contactinfo = contactinfo_get_contactinfo();
    foreach ($tokens as $name => $original) {
      $value = FALSE;
      switch ($name) {
        case 'type':
          $value = $contactinfo['type'];
          break;

        case 'given-name':
          $value = $contactinfo['fn_n']['given-name'];
          break;

        case 'family-name':
          $value = $contactinfo['fn_n']['family-name'];
          break;

        case 'org':
          $value = $contactinfo['use_site_name'] ? variable_get('site_name', '') : $contactinfo['org'];
          break;

        case 'tagline':
          $value = $contactinfo['use_site_slogan'] ? variable_get('site_slogan', '') : $contactinfo['tagline'];
          break;

        case 'adr-street-address':
          $value = $contactinfo['adr']['street-address'];
          break;

        case 'adr-extended-address':
          $value = $contactinfo['adr']['extended-address'];
          break;

        case 'adr-locality':
          $value = $contactinfo['adr']['locality'];
          break;

        case 'adr-region':
          $value = $contactinfo['adr']['region'];
          break;

        case 'adr-postal-code':
          $value = $contactinfo['adr']['postal-code'];
          break;

        case 'adr-country-name':
          $value = $contactinfo['adr']['country-name'];
          break;

        case 'location-latitude':
          $value = $contactinfo['location']['latitude'];
          break;

        case 'location-longitude':
          $value = $contactinfo['location']['longitude'];
          break;

        case 'phone-voice':
          $value = $contactinfo['phone']['voice'];
          break;

        case 'phone-fax':
          $value = $contactinfo['phone']['fax'];
          break;

        case 'email':
          $value = $contactinfo['email'];
          break;

      }
      if ($value) {
        $replacements[$original] = $sanitize ? check_plain($value) : $value;
      }
    }
  }

  return $replacements;
}
