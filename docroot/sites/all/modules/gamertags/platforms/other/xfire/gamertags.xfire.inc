<?php
/**
 * @file
 * Gamertag Plugin: Provides Xfire Gamertags.
 */

class gamertags_xfire extends gamertags {
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
      'name' => 'xfire',
      'title' => t('XFire Gamertags'),
      'description' => t('XFire Network Gamertags'),
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
      'short' => '[xfire:id:skin:size]',
      'long' => t('XFire Network Gamertag by typing [xfire:id:skin:size] (skin and size are optional)'),
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
      'user-gamertag-xfire' => array(
        'name' => t('Gamertags: XFire'),
        'description' => t('Corresponding XFire Network Gamertag for this user.'),
      ),
      'user-gamertag-xfire-text' => array(
        'name' => t('Gamertags: XFire (text)'),
        'description' => t('Corresponding XFire Network Gamertag for this user (as plain text).'),
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
      'user-gamertag-xfire' => self::render($account),
      'user-gamertag-xfire-text' => $sanitize ? check_plain($account->gamertags_xfire) : $account->data['gamertags_xfire'],
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
    $args = array(
      '@gamertag' => $gamertag,
      '@skin' => $options['skin'],
      '@size' => (int) $options['size'],
      '@extension' => $options['size'] == 4 ? 'gif' : 'png',
    );
    return format_string(variable_get('gamertags_xfire_url', 'http://miniprofile.xfire.com/bg/@skin/type/@size/@gamertag.@extension'), $args);
  }

  /**
   * Helper function to return the external link for Xfire profiles.
   */
  public function _returnPath($gamertag) {
    return format_string(variable_get('gamertags_xfire_url_profiles', 'http://profile.xfire.com/@gamertag'), array('@gamertag' => $gamertag));
  }

  /**
   * Returns the rendered HTML for the platform Gamertag.
   */
  public function render(stdClass $account) {
    return theme('gamertags_xfire',
      array(
        'gamertag' => $account->data['gamertags_xfire'],
        'skin' => isset($account->data['gamertags_xfire_skin']) ? $account->data['gamertags_xfire_skin'] : variable_get('gamertags_xfire_default_skin', 'bg'),
        'size' => isset($account->data['gamertags_xfire_size']) ? $account->data['gamertags_xfire_size'] : variable_get('gamertags_xfire_default_size', 0),
      )
    );
  }

  /**
   * Extends hook_user_view().
   */
  public function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_xfire_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Xfire Gamertag'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_xfire_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  public function userForm(stdClass $account) {
    $form['xfire']['#weight'] = (int) variable_get('gamertags_xfire_weight');
    $form['xfire']['gamertags_xfire'] = array(
      '#type' => 'textfield',
      '#title' => t('Xfire'),
      '#default_value' => isset($account->data['gamertags_xfire']) ? $account->data['gamertags_xfire'] : '',
      '#description' => t('If you have one, please enter your xFire GamerTag.'),
    );
    if (variable_get('gamertags_xfire_ask_skin')) {
      $form['xfire']['extra']['gamertags_xfire_skin'] = array(
        '#type' => 'select',
        '#title' => t('Xfire skin'),
        '#description' => t('If you have one, please enter your xFire GamerTag.'),
        '#default_value' => !empty($account->data['gamertags_xfire_skin']) ? $account->data['gamertags_xfire_skin'] : variable_get('gamertags_xfire_default_skin', 'bg'),
        '#options' => self::_skins(),
      );
    }
    if (variable_get('gamertags_xfire_ask_size')) {
      $form['xfire']['extra']['gamertags_xfire_size'] = array(
        '#type' => 'select',
        '#title' => t('Xfire size'),
        '#description' => t('If you have one, please enter your xFire GamerTag.'),
        '#default_value' => !empty($account->data['gamertags_xfire_size']) ? $account->data['gamertags_xfire_size'] : variable_get('gamertags_xfire_default_size', 0),
        '#options' => self::_sizes(),
      );
    }
    if (!empty($form['xfire']['extra'])) {
      $form['xfire']['#attached']['js'][] = drupal_get_path('module', 'gamertags') . '/platforms/other/xfire/gamertags.xfire.js';
      $form['xfire']['extra']['#prefix'] = '<div id="xfire-extras-wrapper">';
      $form['xfire']['extra']['#suffix'] = '</div>';
    }
    return $form;
  }

  /**
   * @return
   *   for the platform.
   */
  public function userSave(&$edit, stdClass $account, $category) {
    $edit['data']['gamertags_xfire'] = isset($edit['gamertags_xfire']) ? $edit['gamertags_xfire'] : NULL;
    $edit['data']['gamertags_xfire_skin'] = isset($edit['gamertags_xfire_skin']) ? $edit['gamertags_xfire_skin'] : variable_get('gamertags_xfire_default_skin', 'bg');
    $edit['data']['gamertags_xfire_size'] = isset($edit['gamertags_xfire_size']) ? $edit['gamertags_xfire_size'] : variable_get('gamertags_xfire_default_size', 0);
  }

  /**
   * Validation for userForm().
   */
  public function userFormValidate(&$values) {}

  /**
   * Provide the URL settings for this platform.
   */
  function adminUrlSettings() {
    $form['gamertags_xfire_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Xfire URL'),
      '#default_value' => variable_get('gamertags_xfire_url', 'http://miniprofile.xfire.com/bg/@skin/type/@size/@gamertag.@extension'),
      '#required' => TRUE,
      '#size' => 70,
    );
    $form['gamertags_xfire_url_profiles'] = array(
      '#type' => 'textfield',
      '#title' => t('Xfire Profiles URL'),
      '#default_value' => variable_get('gamertags_xfire_url_profiles', 'http://profile.xfire.com/@gamertag'),
      '#required' => TRUE,
      '#size' => 70,
    );
    return $form;
  }

  /**
   * Provide any extra settings for this platform.
   */
  function adminExtraSettings() {
    $form['xfire'] = array(
      '#type' => 'fieldset',
      '#title' => t('Xfire'),
      '#description' => t("Select the default skin and card size to use for Xfire gamertags."),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );
    $form['xfire']['gamertags_xfire_ask_skin'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ask for skin?'),
      '#description' => t('Check to ask a user to select the skin they wish to use.'),
      '#default_value' => variable_get('gamertags_xfire_ask_skin'),
    );
    $form['xfire']['gamertags_xfire_default_skin'] = array(
      '#type' => 'select',
      '#title' => t('Default skin'),
      '#description' => t('Select the default Xfire skin.
        If "Ask skin" is enabled this will be the selected option but the user
        will be able to change this themselves.'
      ),
      '#options' => self::_skins(),
      '#default_value' => variable_get('gamertags_xfire_default_skin', 'bg'),
    );
    $form['xfire']['gamertags_xfire_ask_size'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ask for size?'),
      '#description' => t('Check to ask a user to select the gamercard size they wish to use.'),
      '#default_value' => variable_get('gamertags_xfire_ask_size'),
    );
    $form['xfire']['gamertags_xfire_default_size'] = array(
      '#type' => 'select',
      '#title' => t('Default size'),
      '#description' => t('Select the default Xfire gamercard size.
        If "Ask size" is enabled this will be the selected option but the user
        will be able to change this themselves.'),
      '#options' => self::_sizes(),
      '#default_value' => variable_get('gamertags_xfire_default_size', 0),
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
        'skin' => NULL,
        'size' => NULL,
      ),
      'template' => 'platforms/other/xfire/gamertags.xfire',
      'file' => 'platforms/other/xfire/gamertags.xfire.inc',
    );
  }

  /**
   * Helper function to map the available Xfire gamertag skins.
   */
  private function _skins() {
    return array(
      'bg' => t('Xfire default'),
      'sh' => t('Shadow'),
      'co' => t('Combat'),
      'sf' => t('Sci-fi'),
      'os' => t('Fantasy'),
      'wow' => t('World of Warcraft'),
    );
  }

  /**
   * Helper function to map the available Xfire gamertag sizes.
   */
  private function _sizes() {
    return array(
      0 => t('Classic'),
      1 => t('Compact'),
      2 => t('Short and Wide'),
      3 => t('Tiny'),
      4 => t('Micro'),
    );
  }
}

/**
 * Preprocess function for gamertags_xfire theme function.
 */
function gamertags_preprocess_gamertags_xfire(&$variables) {
  $variables['alt'] = t('Xfire Gamertag');
  $variables['gamertag'] = check_plain($variables['gamertag']);
  $variables['skin'] = check_plain($variables['skin']);
  $variables['size'] = check_plain($variables['size']);
  $variables['url'] = gamertags_xfire::returnPath($variables['gamertag'], array('skin' => $variables['skin'], 'size' => $variables['size']));
  $variables['link'] = gamertags_xfire::_returnPath($variables['gamertag']);
}
