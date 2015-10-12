<?php

/**
 * @file
 * Plugin to provide access control based on user's OpenID relying party realm.
 */

/**
 * Describing the plugin.
 */
$plugin = array(
  'title' => t("OpenID relying parties"),
  'description' => t('Control access by checking which realm is in use.'),
  'callback' => 'openid_sso_provider_selection_realm_ctools_access_check',
  'default' => array('openid_sso_provider_selection_ctools_access_realm' => ''),
  'settings form' => 'openid_sso_provider_selection_realm_ctools_access_settings',
  'summary' => 'openid_sso_provider_selection_realm_ctools_access_summary',
);


/**
 * Settings form for the 'by relying party' access plugin
 */
function openid_sso_provider_selection_realm_ctools_access_settings($form, &$form_state, $conf) {

  $relying_parties = array();
  foreach (openid_sso_provider_get_relying_parties() as $relying_party) {
    $relying_parties[$relying_party->realm] = check_plain($relying_party->label . ' - ' . $relying_party->realm);
  }

  $form['settings']['openid_sso_provider_selection_ctools_access_realm_all'] = array(
    '#type' => 'radios',
    '#options' => array(
      'any' => t('Any valid Relying Party'),
      'specific' => t('Specific settings depending on certain Relying Parties selected below')),
    '#title' => t('Your selection settings'),
    '#default_value' => $conf['openid_sso_provider_selection_ctools_access_realm_all'],
  );

  $form['settings']['openid_sso_provider_selection_ctools_access_realm_rps'] = array(
    '#type' => 'checkboxes',
    '#options' => $relying_parties,
    '#title' => t('OpenID relying parties'),
    '#default_value' => $conf['openid_sso_provider_selection_ctools_access_realm_rps'],
    '#description' => t('This will only be accessed if the current realm is the selected relying party.'),
    '#states' => array(
      'visible' => array(
        ':input[name="openid_sso_provider_selection_ctools_access_realm_all"]' => array(
          'value' => 'specific',
        ),
      ),
    ),
  );
  return $form;
}

/**
 * Check for access.
 */
function openid_sso_provider_selection_realm_ctools_access_check($conf, $context) {

  $realm = openid_sso_provider_get_realm();
  if ($realm) {
    if ($conf['openid_sso_provider_selection_ctools_access_realm_all'] == 'any') {
      if (openid_sso_provider_rps_load_by_realm($realm)) {
        return TRUE;
      }
    }
    else {
      if (is_array($conf['openid_sso_provider_selection_ctools_access_realm_rps'])) {
        foreach ($conf['openid_sso_provider_selection_ctools_access_realm_rps'] as $relying_party) {
          if ($realm === $relying_party) {
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function openid_sso_provider_selection_realm_ctools_access_summary($conf, $context) {

  // No relying party has been defined at all.
  if ($conf['openid_sso_provider_selection_ctools_access_realm_all'] == 'specific') {

    if (!isset($conf['openid_sso_provider_selection_ctools_access_realm_rps'])) {
      return t('Error, no relying parties defined.');
    }

  else {
      // Prepare relying parties selection information.
      $relying_parties = array();
      foreach($conf['openid_sso_provider_selection_ctools_access_realm_rps'] as $relying_party) {
        if ($relying_party) {
          $relying_parties[] = $relying_party;
        }
      }
      switch (count($relying_parties)) {

        // No relying party has been selected.
        case 0:
          return t('No relying parties selected.');

        // Only one relying party has been selected
        case 1:
          return t('User comes from the folllowing relying party: "@relying_party"', array('@relying_party' => array_pop($relying_parties)));

        // More than one relying party has been selected.
        default:
          $relying_parties_string = '';
          while (count($relying_parties) > 1) {
            $relying_parties_string .= array_shift($relying_parties) . ', ';
          }
          $relying_parties_string .= array_shift($relying_parties);
          return t('User comes from the folllowing relying parties: "@relying_parties"', array('@relying_parties' => $relying_parties_string));
      }
    }
  }
}
