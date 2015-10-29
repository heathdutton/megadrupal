<?php
/**
 * @file
 * Gamertag Plugin: Provides Microsoft Xbox Live Gamertags.
 */

class gamertags_xbox extends gamertags {
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
      'name' => 'xbox',
      'title' => t('Xbox Gamertags'),
      'description' => t('Microsoft Xbox Live Gamertags'),
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
      'short' => '[xbox:id]',
      'long' => t('Xbox Live Gamertag by typing [xbox:id]'),
    );
  }

  /**
   * Return token information about this platform.
   *
   * @return
   *   array containing token information where the key is a machine-name and the value
   *   an array containing the name and description of the token output
   */
  static public function tokenList() {
    return array(
      'user-gamertag-xbox' => array(
        'name' => t('Gamertags: Xbox'),
        'description' => t('Corresponding Xbox Gamertag for this user.'),
      ),
      'user-gamertag-xbox-text' => array(
        'name' => t('Gamertags: Xbox (text)'),
        'description' => t('Corresponding Xbox Gamertag for this user (as plain text).'),
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
      'user-gamertag-xbox' => self::render($account),
      'user-gamertag-xbox-text' => $sanitize ? check_plain($account->data['gamertags_xbox']) : $account->data['gamertags_xbox'],
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
    return format_string(variable_get('gamertags_xbox_url', 'http://gamercard.xbox.com/@gamertag.card'), array('@gamertag' => $gamertag));
  }

  /**
   * Extends hook_user_view().
   */
  public function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_xbox_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Xbox Live Gamertag'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_xbox_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  public function userForm(stdClass $account) {
    return array(
      'gamertags_xbox' => array(
        '#type' => 'textfield',
        '#title' => t('Xbox Live'),
        '#default_value' => isset($account->data['gamertags_xbox']) ? $account->data['gamertags_xbox'] : '',
        '#description' => t('If you have one, please enter your Xbox Live GamerTag.'),
        '#weight' => (int) variable_get('gamertags_xbox_weight'),
      ),
    );
  }

  /**
   * Custom validation for userForm().
   */
  public function userFormValidate(&$values) {
    if (($xbox_gamertag = $values['gamertags_xbox']) && (!preg_match("/^[a-zA-Z]{1}[a-zA-Z0-9 ]+$/", $xbox_gamertag) || drupal_strlen($xbox_gamertag) > 15)) {
      form_set_error('gamertags_xbox', t('Xbox Live gamertag field should contain numbers, letters and spaces only and be up to 15 characters in length.'));
    }
  }

  /**
   * Provide the URL settings for this platform.
   */
  function adminUrlSettings() {
    return array(
      'gamertags_xbox_url' => array(
        '#type' => 'textfield',
        '#title' => t('Xbox (Microsoft) URL'),
        '#default_value' => variable_get('gamertags_xbox_url', 'http://gamercard.xbox.com/@gamertag.card'),
        '#required' => TRUE,
        '#size' => 70,
      ),
    );
  }

/**
 * Extends hook_theme().
 */
  public function theme() {
    return array(
      'variables' => array(
        'gamertag' => NULL,
      ),
      'template' => 'platforms/microsoft/gamertags.xbox',
      'file' => 'platforms/microsoft/gamertags.xbox.inc',
    );
  }
}

/**
 * Preprocess function for gamertags_xbox theme function.
 */
function gamertags_preprocess_gamertags_xbox(&$variables) {
  $variables['gamertag'] = check_plain($variables['gamertag']);
  $variables['no_iframe_message'] = t('If you can see this, your browser does not support iframes!');
  $variables['url'] = gamertags_xbox::returnPath($variables['gamertag']);
}
