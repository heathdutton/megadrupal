<?php
/**
 * @file
 * Gamertag Plugin: Provides Nintendo Mario Kart Gamertags.
 */

class gamertags_mariokart extends gamertags {
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
      'name' => 'mariokart',
      'title' => t('Mario Kart Gamertags'),
      'description' => t('Nintendo Mario Kart Friend Code'),
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
      'short' => '[mariokart:id]',
      'long' => t('Nintendo Mario Kart Gamertag by typing [mariokart:id]'),
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
      'user-gamertag-mariokart' => array(
        'name' => t('Gamertags: Mario Kart'),
        'description' => t('Corresponding Nintendo Mario Kart Gamertag for this user.'),
      ),
      'user-gamertag-mariokart-text' => array(
        'name' => t('Gamertags: Mario Kart (text)'),
        'description' => t('Corresponding Nintendo Mario Kart Gamertag for this user (as plain text).'),
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
      'user-gamertag-mariokart' => self::render($account),
      'user-gamertag-mariokart-text' => $sanitize ? check_plain($account->data['gamertags_mariokart']) : $account->data['gamertags_mariokart'],
    );
  }

  /**
   * Extends hook_user_view().
   */
  public function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_mariokart_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Mario Kart Friend Code'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_mariokart_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  public function userForm(stdClass $account) {
    return array(
      'gamertags_mariokart' => array(
        '#type' => 'textfield',
        '#title' => t('Nintendo Mario Kart'),
        '#default_value' => isset($account->data['gamertags_mariokart']) ? $account->data['gamertags_mariokart'] : '',
        '#description' => t('If you have one, please enter your Mario Kart Friend Code.'),
        '#weight' => (int) variable_get('gamertags_mariokart_weight'),
      ),
    );
  }

  /**
   * Validation for userForm().
   */
  function userFormValidate(&$values) {
    if (($mariokart_gamertag = preg_replace(array("/\s+/", "/-+/"), "", $values['gamertags_mariokart'])) && (!preg_match("/^[0-9]+$/", $mariokart_gamertag) || drupal_strlen($mariokart_gamertag) != 12)) {
      form_set_error('gamertags_mariokart', t('Mario Kart Friend Codes can be 12 digit numbers only.'));
    }
    // For fun, let's format all Mario Kart friendcodes so they look like this 1234-1234-1234.
    if (!empty($mariokart_gamertag)) {
      $values['gamertags_mariokart'] = preg_replace("/([0-9]{4})([0-9]{4})([0-9]{4})/", "$1-$2-$3", $mariokart_gamertag);
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
      'template' => 'platforms/nintendo/mariokart/gamertags.mariokart',
      'file' => 'platforms/nintendo/mariokart/gamertags.mariokart.inc',
    );
  }

  /**
   * Provides values for SimpleTest.
   */
  public function _testContent(&$edit) {
    $edit['gamertags_mariokart'] = '111111111111';
  }
}

/**
 * Preprocess function for gamertags_mariokart theme function.
 */
function gamertags_preprocess_gamertags_mariokart(&$variables) {
  $variables['gamertag'] = check_plain($variables['gamertag']);
}
