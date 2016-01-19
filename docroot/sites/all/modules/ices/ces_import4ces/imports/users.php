<?php
/**
 * @file
 * Functions from parse users
 */

/**
 * @defgroup ces_import4ces_user Parse users from CES
 * @ingroup ces_import4ces
 * @{
 * Functions from parse users
 */

/**
 * Parse users.
 */
function ces_import4ces_parse_users($import_id, $data, $row, &$context, $width_ajax = TRUE) {
  if (isset($context['results']['error'])) {
    return;
  }
  $tx = db_transaction();
  try {
    ob_start();
    $context['results']['import_id'] = $import_id;
    $import = ces_import4ces_import_load($import_id);

    if (CES_IMPORT4CES_RESET_PASSWORD) {
      $password = user_password(8);
    }
    else {
      $password = $data['Password'];
    }

    /*
    UserType: El tipus de compte. adm per administrador, org per organització
    sense ànim de lucre, ind per individual, fam per compartit, com per
    empreses, pub per a comptes públics, vir per virtual. L'administrador té
    més permisos i només n'hi ha un (que jo sàpiga), org, ind, fam i com són
    iguals a la pràctica (crec). pub és més accessible en el sentit que les
    transaccions d'aquest compte les pot veure tothom. vir són comptes per
    comptabilitzar els intercanvis  amb altres xarxes i no pertanyen a ningú.

    kind One of INDIVIDUAL (0), SHARED (1), ORGANIZATION (2), COMPANY (3),
    PUBLIC (4)

    'kind' => CesBankLocalAccount::TYPE_INDIVIDUAL,

    const TYPE_INDIVIDUAL = 0;
    const TYPE_SHARED = 1;
    const TYPE_ORGANIZATION = 2;
    const TYPE_COMPANY = 3;
    const TYPE_PUBLIC = 4;
    const TYPE_VIRTUAL = 5;
    */

    $type_user = array(
      'adm' => CesBankLocalAccount::TYPE_INDIVIDUAL,
      'org' => CesBankLocalAccount::TYPE_ORGANIZATION,
      'ind' => CesBankLocalAccount::TYPE_INDIVIDUAL,
      'fam' => CesBankLocalAccount::TYPE_SHARED,
      'com' => CesBankLocalAccount::TYPE_COMPANY,
      'pub' => CesBankLocalAccount::TYPE_PUBLIC,
      'vir' => CesBankLocalAccount::TYPE_VIRTUAL,
    );
    $language = _ces_import4ces_get_language($data['Lang']);

    // Set up the user fields.
    $fields = array(
      'name' => $data['UID'],
      'mail' => ($import->anonymous) ? 'test-' . $data['UID'] . '@test.com' : $data['Email'],
      'pass' => $password,
      'status' => ($data['Locked'] == 0) ? 1 : 0,
      // 'init' => ( $GLOBALS['anonymous'] ) ? 'test-'.$data['UID'].'@test.com'
      // : $data['Email'],
      'language' => $language,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      ),
      // User custom fields.
      'ces_firstname' => array(LANGUAGE_NONE => array(array('value' => $data['Firstname']))),
      'ces_surname' => array(LANGUAGE_NONE => array(array('value' => $data['Surname']))),
      'ces_address' => array(LANGUAGE_NONE => array(array('value' => $data['Address1'] . "\n" . $data['Address2']))),
      'ces_town' => array(LANGUAGE_NONE => array(array('value' => $data['Address3']))),
      'ces_postcode' => array(LANGUAGE_NONE => array(array('value' => $data['Postcode']))),
      'ces_phonemobile' => array(LANGUAGE_NONE => array(array('value' => $data['PhoneM']))),
      'ces_phonework' => array(LANGUAGE_NONE => array(array('value' => $data['PhoneW']))),
      'ces_phonehome' => array(LANGUAGE_NONE => array(array('value' => $data['PhoneH']))),
      'ces_website' => array(LANGUAGE_NONE => array(array('value' => $data['WebSite']))),
      'created' => strtotime($data['Created']),
    );

    $extra_data = array(
      'OrgNameShort' => $data['OrgNameShort'],
      'OrgNameLong' => $data['OrgNameLong'],
      'SubArea' => $data['SubArea'],
      'DefaultSub' => $data['DefaultSub'],
      'PhoneF' => $data['PhoneF'],
      'IM' => $data['IM'],
      'DOB' => $data['DOB'],
      'NoEmail1' => $data['NoEmail1'],
      'NoEmail2' => $data['NoEmail2'],
      'NoEmail3' => $data['NoEmail3'],
      'NoEmail4' => $data['NoEmail4'],
      'Hidden' => $data['Hidden'],
      'Created' => $data['Created'],
      'LastAccess' => $data['LastAccess'],
      'LastEdited' => $data['LastEdited'],
      'EditedBy' => $data['EditedBy'],
      'InvNo' => $data['InvNo'],
      'OrdNo' => $data['OrdNo'],
      'Coord' => $data['Coord'],
      'LocalOnly' => $data['LocalOnly'],
      'Notes' => $data['Notes'],
      'Photo' => $data['Photo'],
      'HideAddr1' => $data['HideAddr1'],
      'HideAddr2' => $data['HideAddr2'],
      'HideAddr3' => $data['HideAddr3'],
      'HideArea' => $data['HideArea'],
      'HideCode' => $data['HideCode'],
      'HidePhoneH' => $data['HidePhoneH'],
      'HidePhoneW' => $data['HidePhoneW'],
      'HidePhoneF' => $data['HidePhoneF'],
      'HidePhoneM' => $data['HidePhoneM'],
      'HideEmail' => $data['HideEmail'],
      'IdNo' => $data['IdNo'],
      'LoginCount' => $data['LoginCount'],
      'SubsDue' => $data['SubsDue'],
      'Closed' => $data['Closed'],
      'DateClosed' => $data['DateClosed'],
      'Translate' => $data['Translate'],
      'Buddy' => $data['Buddy'],
    );

    // Admin user has already been created in the first step, but we now
    // are completing the record with the user info.
    if (substr($data['UID'], -4) == '0000') {
      $user_drupal = user_load_by_name($data['UID']);
    }
    else {
      $user_drupal = FALSE;
    }
    $user_drupal = user_save($user_drupal, $fields);
    if ($user_drupal === FALSE) {
      throw new Exception(t('Error creating Drupal user.'));
    }

    // If you want to send the welcome email, use the following code
    // Manually set the password so it appears in the e-mail.
    $user_drupal->password = $fields['pass'];

    // Account in CES. The administrative account has already been created in
    // exchange import.
    if (substr($user_drupal->name, -4) != '0000') {
      $bank = new CesBank();
      $limit = _ces_import4ces_get_limitchain($import->exchange_id,
        $data['DebLimit'], $data['CredLimit']);
      $account = array(
        'exchange' => $import->exchange_id,
        'name' => $data['UID'],
        'limitchain' => $limit['id'],
        'kind' => $type_user[$data['UserType']],
        'state' => CesBankLocalAccount::STATE_HIDDEN,
        'users' => array(
          array(
            'user' => $user_drupal->uid,
            'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
          ),
        ),
        'created' => strtotime($data['Created']),
        'modified' => strtotime($data['Created']),
      );
      $bank->createAccount($account, FALSE);
      $bank->activateAccount($account);
    }
    db_insert('ces_import4ces_objects')
      ->fields(array(
        'import_id' => $import_id,
        'object' => 'user',
        'object_id' => $user_drupal->uid,
        'row' => $row,
        'data' => serialize($extra_data),
      ))->execute();
    ces_import4ces_update_row($import_id, $row);
    ob_end_clean();
  }
  catch (Exception $e) {
    ob_end_clean();
    $tx->rollback();
    $context['results']['error'] = check_plain($e->getMessage());
    $_SESSION['ces_import4ces_row_error']['row']  = $row;
    $_SESSION['ces_import4ces_row_error']['m']    = $e->getMessage();
    $_SESSION['ces_import4ces_row_error']['data'] = $data;
    if ($width_ajax) {
      $result = array('status' => FALSE, 'data' => check_plain($e->getMessage()));
      die(json_encode($result));
    }
    else {
      ces_import4ces_batch_fail_row($import_id, array_keys($data), array_values($data), $row, $context);
    }
  }
}

/**
 * Get limitchain.
 *
 * Return a limit chain for this exchange with these properties. It creates the
 * object if necessary. 0 means no limit. $debit and $credit are nonnegatives.
 */
function _ces_import4ces_get_limitchain($exchange_id, $debit, $credit) {
  $bank = new CesBank();
  $limitchains = &drupal_static(__FUNCTION__);
  if (empty($limitchains)) {
    $limitchains = $bank->getAllLimitChains($exchange_id);
  }
  foreach ($limitchains as $limitchain) {
    $limit_credit = 0;
    $limit_debit = 0;
    foreach ($limitchain['limits'] as $limit) {
      if ($limit['classname'] == 'CesBankAbsoluteCreditLimit') {
        $limit_credit = $limit['value'];
      }
      elseif ($limit['classname'] == 'CesBankAbsoluteDebitLimit') {
        $limit_debit = $limit['value'];
      }
    }
    if ($limit_credit == $credit && $limit_debit == -$debit) {
      return $limitchain;
    }
  }
  // We have not found any suitable limit chain, so create a new one.
  $limitchain = array(
    'exchange' => $exchange_id,
    'limits' => array(),
    'name' => 'x',
  );
  if ($debit != 0) {
    $limitchain['name'] = '-' . $debit . ' < ' . $limitchain['name'];
    $limitchain['limits'][] = array(
      'classname' => 'CesBankAbsoluteDebitLimit',
      'value' => -$debit,
      'block' => FALSE,
    );
  }
  if ($credit != 0) {
    $limitchain['name'] .= ' < ' . $credit;
    $limitchain['limits'][] = array(
      'classname' => 'CesBankAbsoluteCreditLimit',
      'value' => $credit,
      'block' => FALSE,
    );
  }
  $bank->createLimitChain($limitchain);
  drupal_static_reset(__FUNCTION__);
  return $limitchain;
}
/** @} */
