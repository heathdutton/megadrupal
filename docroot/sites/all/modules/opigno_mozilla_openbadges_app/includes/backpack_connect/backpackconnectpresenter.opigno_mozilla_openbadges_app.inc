<?php

/**
 * Class BackpackApiImplementation
 * This class allow the developer to integrate the Backpack API to an Opigno LMS instance.
 */
class BackpackConnectPresenter
{

  /**
   * The url to the issuer javascript of the backpack
   */
  const ISSUER_JS_URL = 'https://backpack.openbadges.org/issuer.js';

  /**
   * Add the JS files to the form
   *
   * @param $form Array The form as expected for drupal_render().
   */
  public static function _add_jS(&$form)
  {
    // Attach JS for backpack
    if (empty($form['#attached']['js'])) {
      $form['#attached']['js'] = array();
    }
    $form['#attached']['js'] = array_merge($form['#attached']['js'], array(
      self::ISSUER_JS_URL => array('type' => 'external'),
      drupal_get_path('module', 'opigno_mozilla_openbadges_app') . '/opigno_mozilla_openbadges_app.js'
    ));
  }


  /**
   * Remove the GET params that came from the Mozilla Backpack Connect API and refresh the page in order to
   *   take the modification in account.
   */
  public static function remove_gets_queries_from_url()
  {
    $toRemove = array('api_root', 'access_token', 'refresh_token', 'expires');
    $redirectUrl = strtok($_SERVER['SCRIPT_NAME'], '?') .'?';
    foreach ($_GET as $key => $value)
    {
      if (!in_array($key, $toRemove))
        $redirectUrl .= $key .'='. $value .'&';
    }
    $redirectUrl = substr($redirectUrl, 0, -1);

    header('Location: '. $redirectUrl, true);
    die;
  }


  /**
   * Add the synchronize buttons and the remove buttons for each backpack entries of the DB
   *
   * @param $dbConnectEntries Array An associative array who contains the entries of the DB
   * @param $form Array The form array as expected for drupal_render().
   */
  public static function _add_sync_buttons($dbConnectEntries, &$form, &$form_state)
  {
    $i = 0;
    foreach ($dbConnectEntries as $apiRoot => $accessValues)
    {
      $form['backpack_connect_settings']['button_backpack_connect_' . $i] = array(
        '#type' => 'button',
        '#value' => t('Sync with my @url', array('@url' => parse_url($apiRoot, PHP_URL_HOST))),
        '#name' => 'backpack_sync_button',
        '#button-data' => $apiRoot,
        '#ajax' => array(
          'callback' => 'ajax_opigno_mozilla_openbadges_app_backpack_connect_settings_callback',
          'wrapper' => 'backpack_connect_settings',
          'method' => 'replace',
          'effect' => 'fade'
        )
      );

      $form['backpack_connect_settings']['button_backpack_remove_connect_' . $i] = array(
        '#type' => 'button',
        '#value' => t('Disconnect this backpack'),
        '#name' => 'backpack_remove_connection_button',
        '#button-data' => $apiRoot,
        '#ajax' => array(
          'callback' => 'ajax_opigno_mozilla_openbadges_app_backpack_connect_settings_callback',
          'wrapper' => 'backpack_connect_settings',
          'method' => 'replace',
          'effect' => 'fade'
        )
      );

      $i++;
    }
  }

  /**
   * Will issue the badge to all the backpacks registered by the user.
   *
   * @param $uid Integer The user ID (uid)
   * @param $assertBadgeFileUri String The URI of the badge to issue
   */
  public static function _issue_badge_to_all_backpacks($uid, $assertBadgeFileUri)
  {
    // Get apiRoot, connectValues
    $accessEntries = BackpackConnectDb::getEntries($uid);
    foreach ($accessEntries as $apiRoot => $connectValues)
    {
      $backpackApi = new BackpackConnectApi(
        $apiRoot,
        $connectValues[BackpackConnectDb::FIELD_ACCESS_TOKEN],
        $connectValues[BackpackConnectDb::FIELD_REFRESH_TOKEN]
      );

      // Renew access token if necessary
      if ($connectValues[ BackpackConnectDb::FIELD_EXPIRES_TIMESTAMP ] <= time())
      {
        self::refresh_token($backpackApi, $apiRoot, $connectValues, $uid);
      }

      // Update the file to match the good recipient for the backpack
      OpignoOpenbadgeFm::updateAssertFile(
        $assertBadgeFileUri,
        array('recipient' => $connectValues[BackpackConnectDb::FIELD_RECIPIENT])
      );

      $issueIsOk = $backpackApi->issueBadge($assertBadgeFileUri);

      if (!$issueIsOk)
      {
        $errors[] = t('Error while issuing badge: @uri', array('@uri' => $assertBadgeFileUri));
      }
    }
  }

  /**
   * Issue all the badges of a user to one of his backpack.
   *
   * @param $apiRoot
   * @param $connectValues
   * @param $backpackApi
   * @param $uid
   *
   * @return array|bool
   */
  public static function _issue_all_badges_to_backpack($apiRoot, &$connectValues, $backpackApi, $uid)
  {
    // Renew access token if necessary
    if ($connectValues[ BackpackConnectDb::FIELD_EXPIRES_TIMESTAMP ] <= time())
    {
      self::refresh_token($backpackApi, $apiRoot, $connectValues, $uid);
    }


    // Then, issue the badges to the backpack
    $errors = array();
    $badgesIds = array();
    $badgesFilesDb = OpignoOpenbadgeFm::getBadgesFiles($uid);
    foreach ($badgesFilesDb as $badgeFile)
    {
      // Get only the last revision of each badges
      $ids = OpignoOpenbadgesPresenter::_get_ids_from_uri($badgeFile->uri);
      if (empty($badgesIds[ $ids['nid'] ]) || $badgesIds[ $ids['nid'] ]['vid'] < $ids['vid'])
      {
        $badgesIds[ $ids['nid'] ] = array(
          'vid' => $ids['vid'],
          'assertUri' => $ids['folderUri'] . OpignoOpenbadgeFm::getAssertFileName($uid)
        );
      }
    }

    foreach ($badgesIds as $badgeNid => $badgeValues)
    {
      // Update the files to match the good recipient for the backpack
      OpignoOpenbadgeFm::updateAssertFile(
        $badgeValues['assertUri'],
        array('recipient' => $connectValues[BackpackConnectDb::FIELD_RECIPIENT])
      );

      $issueIsOk = $backpackApi->issueBadge($badgeValues['assertUri']);

      if (!$issueIsOk)
      {
        $errors[] = t('Error while issuing badge: @uri', array('@uri' => $badgeValues['assertUri']));
      }
    }

    return (empty($errors) ? true : $errors);
  }

  /**
   * Refresh the tokens using the backpack api
   *
   * @param $backpackApi
   * @param $apiRoot
   * @param $connectValues
   * @param $uid
   *
   * @return bool
   */
  public static function refresh_token($backpackApi, $apiRoot, &$connectValues, $uid)
  {
    $newToken = $backpackApi->getNewToken();
    if ($newToken != false)
    {
      BackpackConnectDb::save(
        $uid,
        $newToken->access_token,
        $newToken->refresh_token,
        $newToken->expires,
        $apiRoot,
        $connectValues[BackpackConnectDb::FIELD_RECIPIENT]
      );
      $connectValues[ BackpackConnectDb::FIELD_ACCESS_TOKEN ] = $newToken->access_token;

      return true;
    }
    else
    {
      return false;
    }
  }

  /**
   * Add a connect my backpack button to the form
   *
   * @param $form Array An array of the form as expected by drupal_render()
   */
  public static function add_connect_button(&$form)
  {
    $form['backpack_connect_settings']['button_backpack_connect'] = array(
      '#markup' => '<input'.
        ' type="button"'.
        ' name="opigno_mozilla_openbadges_app_backpack_connect_button"'.
        ' value="'. t('Connect a Backpack') .'"'.
        ' />'
    );
  }


  /**
   * Add a settings container for the Mozilla Backpack Connect.
   *
   * @param $form array The form as expected for drupal_render
   * @param $form_state array The state of the form
   */
  public static function form_alter_add_settings(&$form, &$form_state)
  {
    BackpackConnectPresenter::_add_jS($form);

    $form['backpack_connect_settings'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="backpack_connect_settings">',
      '#suffix' => '</div>',
      '#weight' => 100
    );
    $form['#group_children']['backpack_connect_settings'] = 'group_badge_settings';

    // If a backpack is registered, add the button to sync the badges with this backpack
    $backpackConnectEntries = BackpackConnectDb::getEntries($form['#user']->uid);
    if ($backpackConnectEntries != false)
    {
      // Add the sync buttons for each connect entries that are in DB
      BackpackConnectPresenter::_add_sync_buttons($backpackConnectEntries, $form, $form_state);
    }
    else
    {
      // Add button to connect to backpack
      BackpackConnectPresenter::add_connect_button($form);
    }

    // Need to have a backpack in order to automatically sync the badges
    $form['#validate'][] = 'opigno_mozilla_openbadges_app_user_form_validate_backpack_connect';
  }

  /**
   * Do the ajax behaviours for the backpack Connect Presenter.
   *
   * @param $form array An array of the form as expected by drupal_render()
   * @param $form_state array The state of the form.
   */
  public static function form_alter_do_ajax_behaviours($form, $form_state)
  {
    // If the user clicked on the sync button, push the badges in the backpack
    if (isset($form_state['triggering_element']['#name'])&&$form_state['triggering_element']['#name'] == 'backpack_sync_button')
    {
      $backpackConnectEntries = BackpackConnectDb::getEntries($form['#user']->uid);

      $apiRoot = $form_state['triggering_element']['#button-data'];
      $connectValues = $backpackConnectEntries[$apiRoot];

      $backpackApi = new BackpackConnectApi(
        $apiRoot,
        $connectValues[BackpackConnectDb::FIELD_ACCESS_TOKEN],
        $connectValues[BackpackConnectDb::FIELD_REFRESH_TOKEN]);

      BackpackConnectPresenter::_issue_all_badges_to_backpack($apiRoot, $connectValues, $backpackApi, $form['#user']->uid);

      drupal_set_message(t('Your badges are now in your backpack'));
    }
    else if (isset($form_state['triggering_element']['#name'])&&$form_state['triggering_element']['#name'] == 'backpack_remove_connection_button')
    {
      BackpackConnectDb::clear($form['#user']->uid, $form_state['triggering_element']['#button-data']);

      // Remove the sync auto here
      $fieldsToUpdate = new stdClass();
      $fieldsToUpdate->uid = $form['#user']->uid;
      $fieldsToUpdate->field_badge_sync_auto['und'][0]['value'] = 0;
      field_attach_update('user', $fieldsToUpdate);
      // TODO: Refresh the "sync auto" checkbox in order to see the change

      drupal_set_message(t('Your backpack has been disconnected'));
    }
  }


  /**
   * Will check if the params are good and will save the data in database.
   *
   * @param $apiRoot string The root API URL
   * @param $accessToken string The access token for the Backpack Connect API
   * @param $refreshToken string The refresh token to use in order to get a new access token for the Backpack Connect API
   * @param $uid integer The internal user ID
   * @param $expiresSec integer The number of seconds before needing to get a new access token
   */
  public static function check_and_save_db($apiRoot, $accessToken, $refreshToken, $uid, $expiresSec)
  {
    // Get the good recipient to add in DB
    $backpackApi = new BackpackConnectApi($apiRoot, $accessToken, $refreshToken);
    $recipient = $backpackApi->getRecipient();

    if ($recipient != false)
    {
      $goodRecipient = array(
        'type' => $recipient->type,
        'hashed' => true,
        'salt' => $recipient->salt,
        'identity' => $recipient->recipient
      );

      BackpackConnectDb::save(
        $uid,
        $accessToken,
        $refreshToken,
        $expiresSec,
        $apiRoot,
        json_encode($goodRecipient)
      );
    }
    else {
      watchdog(OPENBADGES_APP_NAME, 'Error while getting the recipient from the Backpack API. These informations where used :'.
        '$apiRoot => '. $apiRoot .
        ' ; $accessToken => '. $accessToken .
        ' ; $refreshToken => '. $refreshToken .
        ' ; $uid => '. $uid .
        ' ; $expiresSec => '. $expiresSec
      );
    }
  }

}
