<?php
/**
 * @file
 * Gamertag Plugin: Provides Steam Gamertags.
 */

class gamertags_steam extends gamertags {
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
      'name' => 'steam',
      'title' => t('Steam Gamertags'),
      'description' => t('Steam Network Gamertags'),
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
      'short' => '[steam:id]',
      'long' => t('Steam Network Gamertag by typing [steam:id]'),
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
      'user-gamertag-steam' => array(
        'name' => t('Gamertags: Steam'),
        'description' => t('Corresponding Steam Network Gamertag for this user.'),
      ),
      'user-gamertag-steam-text' => array(
        'name' => t('Gamertags: Steam (text)'),
        'description' => t('Corresponding Steam Network Gamertag for this user (as plain text).'),
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
      'user-gamertag-steam' => self::render($account),
      'user-gamertag-steam-text' => $sanitize ? check_plain($account->data['gamertags_steam']) : $account->data['gamertags_steam'],
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
    return format_string(variable_get('gamertags_steam_url', 'http://steamcard.com/do/modern/@gamertag.png'), array('@gamertag' => $gamertag));
  }

  /**
   * Extends hook_user_view().
   */
  function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_steam_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Steam Network Gamertag'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_steam_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  function userForm(stdClass $account) {
    return array(
      'gamertags_steam' => array(
        '#type' => 'textfield',
        '#title' => t('Steam'),
        '#default_value' => isset($account->data['gamertags_steam']) ? $account->data['gamertags_steam'] : '',
        '#description' => t('If you have one, please enter your Steam GamerTag.'),
        '#weight' => (int) variable_get('gamertags_steam_weight'),
      ),
    );
  }

  /**
   * Validation for userForm().
   */
  function userFormValidate(&$values) {
    if (($steam_gamertag = $values['gamertags_steam']) && (!preg_match("/^[a-zA-Z0-9_]+$/", $steam_gamertag))) {
      form_set_error('gamertags_steam', t('Steam gamertag field should contain numbers, letters and underscores only.'));
    }
  }

  /**
   * Provide the URL settings for this platform.
   */
  function adminUrlSettings() {
    $form['gamertags_steam_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Steam Card URL'),
      '#default_value' => variable_get('gamertags_steam_url', 'http://steamcard.com/do/modern/@gamertag.png'),
      '#required' => TRUE,
      '#size' => 70,
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
      'template' => 'platforms/other/steam/gamertags.steam',
      'file' => 'platforms/other/steam/gamertags.steam.inc',
    );
  }
}

/**
 * Preprocess function for gamertags_steam theme function.
 */
function gamertags_preprocess_gamertags_steam(&$variables) {
  $variables['alt'] = t('Steam Gamertag');
  $variables['gamertag'] = check_plain($variables['gamertag']);
  $variables['url'] = gamertags_steam::returnPath($variables['gamertag']);
}
