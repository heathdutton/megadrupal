<?php
/**
 * @file
 * Gamertag Plugin: Provides Nintendo Wii Gamertags.
 */

class gamertags_wii extends gamertags {
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
      'name' => 'wii',
      'title' => t('Wii Friend Codes'),
      'description' => t('Nintendo Wii Friend Codes'),
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
      'short' => '[wii:id]',
      'long' => t('Nintendo Wii Gamertag by typing [wii:id]'),
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
      'user-gamertag-wii' => array(
        'name' => t('Gamertags: Wii'),
        'description' => t('Corresponding Nintendo Wii Gamertag for this user.'),
      ),
      'user-gamertag-wii-text' => array(
        'name' => t('Gamertags: Wii (text)'),
        'description' => t('Corresponding Nintendo Wii Gamertag for this user (as plain text).'),
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
      'user-gamertag-wii' => self::render($account),
      'user-gamertag-wii-text' => $sanitize ? check_plain($account->data['gamertags_wii']) : $account->data['gamertags_wii'],
    );
  }

  /**
   * Return the gamertag path for this platform.
   *
   * @param $gamertag
   *   The users Gamertag.
   * @param $options
   *  (optional) dependant on platform:
   *  * $region: A users region; ignore unless this platform uses it.
   *  * $skin: If the platform uses it, the skin for this gamercard.
   *  * $size: If the platform uses it, the size for this gamercard.
   *
   * @return
   *   A fully formed and parsed Gamertag URL.
   */
  public function returnPath($gamertag, $options = array()) {
    return format_string(variable_get('gamertags_wii_url', 'http://mapwii.com/wii-number/member.cfm?wii_number=@gamertag'), array('@gamertag' => preg_replace("/-+/", '', $gamertag)));
  }

  /**
   * Extends hook_user_view().
   */
  function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_wii_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Wii Friend Code'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_wii_weight'),
      '#attached' => array('css' => array(drupal_get_path('module', 'gamertags') . '/platforms/nintendo/wii/gamertags.wii.css')),
    );
  }

  /**
   * Extends hook_user_form().
   */
  function userForm(stdClass $account) {
    return array(
      'gamertags_wii' => array(
        '#type' => 'textfield',
        '#title' => t('Nintendo Wii'),
        '#default_value' => isset($account->data['gamertags_wii']) ? $account->data['gamertags_wii'] : '',
        '#description' => t('If you have one, please enter your Wii Friend Code.'),
        '#weight' => (int) variable_get('gamertags_wii_weight'),
      ),
    );
  }

  /**
   * Validation for userForm().
   */
  function userFormValidate(&$values) {
    if (($wii_gamertag = preg_replace(array("/\s+/", "/-+/"), "", $values['gamertags_wii'])) && (!preg_match("/^[0-9]+$/", $wii_gamertag) || drupal_strlen($wii_gamertag) != 16)) {
      form_set_error('gamertags_wii', t('Wii Friend Codes can be 16 digit numbers only.'));
    }
    // For fun, let's format all Wii friendcodes so they look like this 1234-1234-1234-1234.
    if (!empty($wii_gamertag)) {
      $values['gamertags_wii'] = preg_replace("/([0-9]{4})([0-9]{4})([0-9]{4})([0-9]{4})/", "$1-$2-$3-$4", $wii_gamertag);
    }
  }

  /**
   * Provide the URL settings for this platform.
   */
  function adminUrlSettings() {
    $form['gamertags_wii_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Wii URL'),
      '#default_value' => variable_get('gamertags_wii_url', 'http://mapwii.com/wii-number/member.cfm?wii_number=@gamertag'),
      '#required' => TRUE,
      '#size' => 70,
    );
    return $form;
  }

  /**
   * Provide any extra settings for this platform.
   */
  function adminExtraSettings() {
    $form['gamertags_use_mapwii'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use mapwii.com for Wii Friend Codes'),
      '#description' => t('Check this box to allow users wii friend codes to
        link through to the mapwii.com friend code repository.'
      ),
      '#default_value' => variable_get('gamertags_use_mapwii'),
    );
    return $form;
  }

  /**
   * Extends hook_theme().
   */
  public function theme() {
    return array(
      'variables' => array(
        'gamertag' => NULL,
      ),
      'template' => 'platforms/nintendo/wii/gamertags.wii',
      'file' => 'platforms/nintendo/wii/gamertags.wii.inc',
    );
  }

  /**
   * Provides values for SimpleTest.
   */
  public function _testContent(&$edit) {
    $edit['gamertags_wii'] = '1111111111111111';
  }
}

/**
 * Preprocess function for gamertags_wii theme function.
 */
function gamertags_preprocess_gamertags_wii(&$variables) {
  $variables['alt'] = t('Wii Gamertag');
  $variables['mapwii'] = variable_get('gamertags_use_mapwii');
  $variables['gamertag'] = check_plain($variables['gamertag']);
  $variables['url'] = gamertags_wii::returnPath($variables['gamertag']);
}
