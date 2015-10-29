<?php
/**
 * @file
 * Gamertag Plugin: Provides Sony Playstation Network Gamertags.
 */

class gamertags_psn extends gamertags {
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
      'name' => 'psn',
      'title' => t('Playstation Gamertags'),
      'description' => t('Playstation Network Gamertags'),
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
      'short' => '[psn:id:region]',
      'long' => t('Playstation Network Gamertag by typing [psn:id:region]'),
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
      'user-gamertag-psn' => array(
        'name' => t('Gamertags: Playstation'),
        'description' => t('Corresponding Playstation Network Gamertag for this user.'),
      ),
      'user-gamertag-psn-text' => array(
        'name' => t('Gamertags: Playstation (text)'),
        'description' => t('Corresponding Playstation Network Gamertag for this user (as plain text).'),
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
      'user-gamertag-psn' => self::render($account),
      'user-gamertag-psn-text' => $sanitize ? check_plain($account->data['gamertags_psn']) : $account->data['gamertags_psn'],
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
    switch (drupal_strtolower($options['region'])) {
      case 'us':
        return format_string(variable_get('gamertags_psn_url_us', 'http://fp.profiles.us.playstation.com/playstation/psn/pid/@gamertag.png'), array('@gamertag' => $gamertag));
      default:
        return format_string(variable_get('gamertags_psn_url', 'http://mypsn.eu.playstation.com/psn/profile/@gamertag.jpg'), array('@gamertag' => $gamertag));
    }
  }

  /**
   * Returns the rendered HTML for the platform Gamertag.
   */
  public function render(stdClass $account) {
    return theme('gamertags_psn',
      array(
        'gamertag' => $account->data['gamertags_psn'],
        'region' => $account->data['gamertags_psn_region'],
      )
    );
  }

  /**
   * Extends hook_user_view().
   */
  public function userView(stdClass &$account, $view_mode, $langcode) {
    $account->content['summary']['gamertags_psn_view'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Playstation Network Gamertag'),
      '#markup' => self::render($account),
      '#weight' => (int) variable_get('gamertags_psn_weight'),
    );
  }

  /**
   * Extends hook_user_form().
   */
  public function userForm(stdClass $account) {
    $form['psn']['#attached']['js'][] = drupal_get_path('module', 'gamertags') . '/platforms/sony/psn/gamertags.psn.js';
    $form['psn']['#weight'] = (int) variable_get('gamertags_psn_weight');
    $form['psn']['gamertags_psn'] = array(
      '#type' => 'textfield',
      '#title' => t('Playstation Network'),
      '#default_value' => isset($account->data['gamertags_psn']) ? $account->data['gamertags_psn'] : '',
      '#description' => t('If you have one, please enter your Playstation Network GamerTag.'),
    );
    $form['psn']['gamertags_psn_region'] = array(
      '#type' => 'select',
      '#title' => t('Playstation Region'),
      '#options' => array(
        'na' => t('N/A'),
        'eu' => t('Europe'),
        'us' => t('America'),
      ),
      '#default_value' => isset($account->data['gamertags_psn_region']) && !empty($account->data['gamertags_psn']) ? $account->data['gamertags_psn_region'] : 'na',
      '#required' => TRUE, // Set to TRUE although it's pointless as there's no NULL value - this is just for emphasis.
      '#description' => t("As Sony don't seem to have thought this through properly, please enter the region for your PSN gamertag."),
    );
    return $form;
  }

  /**
   * @return
   *   for the platform.
   */
  public function userSave(&$edit, stdClass $account, $category) {
    $edit['data']['gamertags_psn'] = isset($edit['gamertags_psn']) ? $edit['gamertags_psn'] : NULL;
    $edit['data']['gamertags_psn_region'] = isset($edit['gamertags_psn_region']) ? $edit['gamertags_psn_region'] : NULL;
  }

  /**
   * Validation for userForm().
   */
  public function userFormValidate(&$values) {
    if (($psn_gamertag = $values['gamertags_psn']) && (!preg_match("/^[a-zA-Z]{1}[a-zA-Z0-9-_]+$/", $psn_gamertag) || drupal_strlen($psn_gamertag) < 3 || drupal_strlen($psn_gamertag) > 16)) {
      form_set_error('gamertags_psn', t('Playstation Network gamertag field should begin with a letter, only contain numbers, letters, hyphens and underscores and be between 3 to 16 characters in length.'));
    }
    // Not all users will have js enabled so pick up the possibility that N/A may have been chosen when the field is required.
    if (!empty($psn_gamertag) && $values['gamertags_psn_region'] == 'na') {
      form_set_error('gamertags_psn_region', t('The Playstation Network region field is required when you enter a PSN gamertag.'));
    }
  }

  /**
   * Provide the URL settings for this platform.
   */
  public function adminUrlSettings() {
    $form['gamertags_psn_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Playstation Network (European) URL'),
      '#default_value' => variable_get('gamertags_psn_url', 'http://mypsn.eu.playstation.com/psn/profile/@gamertag.jpg'),
      '#required' => TRUE,
      '#size' => 70,
    );
    $form['gamertags_psn_url_us'] = array(
      '#type' => 'textfield',
      '#title' => t('Playstation Network (American) URL'),
      '#default_value' => variable_get('gamertags_psn_url_us', 'http://fp.profiles.us.playstation.com/playstation/psn/pid/@gamertag.png'),
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
        'region' => NULL,
      ),
      'template' => 'platforms/sony/psn/gamertags.psn',
      'file' => 'platforms/sony/psn/gamertags.psn.inc',
    );
  }

  /**
   * Provides values for SimpleTest.
   */
  public function _testContent(&$edit) {
    parent::_testContent($edit);
    $edit['gamertags_psn_region'] = 'eu';
  }
}

/**
 * Preprocess function for gamertags_psn theme function.
 */
function gamertags_preprocess_gamertags_psn(&$variables) {
  $variables['alt'] = t('Playstation Network Gamertag');
  $variables['gamertag'] = check_plain($variables['gamertag']);
  $variables['region'] = $variables['region'];
  $variables['url'] = gamertags_psn::returnPath($variables['gamertag'], array('region' => $variables['region']));
}
