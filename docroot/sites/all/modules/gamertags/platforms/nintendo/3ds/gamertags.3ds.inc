<?php
/**
 * @file
 * Gamertag Plugin: Provides Nintendo 3DS Gamertags.
 */

class gamertags_3ds extends gamertags {
  /**
   * Return information about this platform.
   *
   * @return
   *   name: Machine-name of this platform, should match the class name; "xbox" for "gamertags_xbox"
   *   title: Human-readable title for administrative displays
   *   description: (optional) Human-readable description for administrative displays
   */
  static public function getInfo() {
    return array(
      'name' => '3ds',
      'title' => t('3DS Gamertags'),
      'description' => t('Nintendo 3DS Friend Code'),
    );
  }

  /**
   * Return filter information about this platform.
   *
   * @return
   *   short: Human-readable short description for filter tips
   *   long: Human-readable long description for filter tips
   */
  static public function filterInfo() {
    return array(
      'short' => '[3ds:id]',
      'long' => t('Nintendo 3DS Gamertag by typing [3ds:id]'),
    );
  }

  /**
   * Return token information about this platform.
   *
   * @return
   *   array containing token information where the key is a machine-name and the value
   *   a description of the token output
   */
  static public function tokenList() {
    return array(
      'user-gamertag-3ds' => array(
        'name' => t('Gamertags: 3DS'),
        'description' => t('Corresponding Nintendo 3DS Gamertag for this user.'),
      ),
      'user-gamertag-3ds-text' => array(
        'name' => t('Gamertags: 3DS (text)'),
        'description' => t('Corresponding Nintendo 3DS Gamertag for this user (as plain text).'),
      ),
    );
  }

  /**
   * Return token values for this platform.
   *
   * @return
   *   array containing token values where the key is a machine-name and the value
   *   contains themed gamertag output
   */
  static public function tokenValues(stdClass $account, $sanitize = FALSE) {
    return array(
      'user-gamertag-3ds' => self::render($account),
      'user-gamertag-3ds-text' => $sanitize ? check_plain($account->data['gamertags_3ds']) : $account->data['gamertags_3ds'],
    );
  }

  /**
   * Extends hook_user_view().
   */
  public function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_3ds_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('3DS Friend Code'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_3ds_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  public function userForm(stdClass $account) {
    return array(
      'gamertags_3ds' => array(
        '#type' => 'textfield',
        '#title' => t('Nintendo 3DS'),
        '#default_value' => isset($account->data['gamertags_3ds']) ? $account->data['gamertags_3ds'] : '',
        '#description' => t('If you have one, please enter your 3DS Friend Code.'),
        '#weight' => (int) variable_get('gamertags_3ds_weight'),
      ),
    );
  }

  /**
   * Validation for userForm().
   */
  function userFormValidate(&$values) {
    if (($three_ds_gamertag = preg_replace(array("/\s+/", "/-+/"), "", $values['gamertags_3ds'])) && (!preg_match("/^[0-9]+$/", $three_ds_gamertag) || drupal_strlen($three_ds_gamertag) != 12)) {
      form_set_error('gamertags_3ds', t('3DS Friend Codes can be 12 digit numbers only.'));
    }
    // For fun, let's format all 3ds friendcodes so they look like this 1234-1234-1234.
    if (!empty($three_ds_gamertag)) {
      $values['gamertags_3ds'] = preg_replace("/([0-9]{4})([0-9]{4})([0-9]{4})/", "$1-$2-$3", $three_ds_gamertag);
    }
  }

  public function adminUrlSettings() { return array(); }

  /**
   * Extends hook_theme().
   */
  public function theme() {
    return array(
      'variables' => array(
        'gamertag' => NULL,
      ),
      'template' => 'platforms/nintendo/3ds/gamertags.3ds',
      'file' => 'platforms/nintendo/3ds/gamertags.3ds.inc',
    );
  }

  /**
   * Provides values for SimpleTest.
   */
  public function _testContent(&$edit) {
    $edit['gamertags_3ds'] = '111111111111';
  }
}

/**
 * Preprocess function for gamertags_3ds theme function.
 */
function gamertags_preprocess_gamertags_3ds(&$variables) {
  $variables['gamertag'] = check_plain($variables['gamertag']);
}
