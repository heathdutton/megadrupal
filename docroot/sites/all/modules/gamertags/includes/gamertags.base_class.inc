<?php
/**
 * @file
 */

abstract class gamertags {
  /**
   * Return information about this platform.
   *
   * @return
   *   name: Machine-name of this platform, should match the class name; "xbox" for "gamertags_xbox"
   *   title: Human-readable title for administrative displays
   *   description: (optional) Human-readable description for administrative displays
   */
  abstract static function getInfo();

  /**
   * Return filter information about this platform.
   *
   * @return
   *   short: Human-readable short description for filter tips
   *   long: Human-readable long description for filter tips
   */
  abstract static function filterInfo();

  /**
   * Return token information about this platform.
   *
   * @return
   *   array containing token information where the key is a machine-name and the value
   *   a description of the token output
   */
  abstract static function tokenList();

  /**
   * Return token values for this platform.
   *
   * @return
   *   array containing token values where the key is a machine-name and the value
   *   contains themed gamertag output
   */
  abstract static function tokenValues(stdClass $account, $sanitize = FALSE);

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
//  abstract function returnPath($gamertag, $options = array());

  /**
   * Returns the rendered HTML for the platform Gamertag.
   */
  public function render(stdClass $account) {
    return theme(get_called_class(),
      array(
        'gamertag' => $account->data[get_called_class()],
      )
    );
  }

  /**
   * Extends hook_user_view().
   */
  abstract public function userView(stdClass &$account, $view_mode, $langcode);

  /**
   * Provides the administration option for whether to enable this platform or not.
   */
  public function adminCollectionSettings() {
    $platform_info = static::getInfo();
    $form['checkbox']["gamertags_{$platform_info['name']}_collect"] = array(
      '#type' => 'checkbox',
      '#title' => $platform_info['title'],
      '#description' => !empty($platform_info['description']) ? $platform_info['description'] : '',
      '#default_value' => variable_get("gamertags_{$platform_info['name']}_collect"),
      '#weight' => variable_get("gamertags_{$platform_info['name']}_weight"),
    );
    $form['weight']["gamertags_{$platform_info['name']}_weight"] = array(
      '#type' => 'weight',
      '#delta' => 50,
      '#default_value' => variable_get("gamertags_{$platform_info['name']}_weight"),
    );
    return $form;
  }

  /**
   * Provide any extra settings for this platform.
   */
  public function adminExtraSettings() { return array(); }

  /**
   * Provide the URL settings for this platform.
   */
  abstract function adminUrlSettings();

  /**
   * Extends hook_user_form().
   *
   * @return
   *   array containing Drupal form information for the hook_user_form() area of
   *   the site. Should be keyed by the class name.
   */
  abstract function userForm(stdClass $account);

  /**
   * @return
   *   for the platform.
   */
  public function userSave(&$edit, stdClass $account, $category) {
    $edit['data'][get_called_class()] = isset($edit[get_called_class()]) ? $edit[get_called_class()] : NULL;
  }

  /**
   * Validation callback for userForm().
   */
  public function userFormValidate(&$values) {}

  /**
   * Extends hook_theme().
   *
   * @return
   *   array containing theme information for this platform.
   */
  abstract public function theme();

  /**
   *
   */
  public function viewsField() {
    $platform_info = static::getInfo();
    return array(
      'real field' => 'data',
      'field' => array(
        'title' => $platform_info['title'],
        'help' => $platform_info['description'],
        'handler' => 'views_handler_field_user_gamertags',
        'click sortable' => FALSE,
      ),
    );
  }

  /**
   * Provides values for SimpleTest.
   */
  public function _testContent(&$edit) {
    $edit[get_called_class()] = 'test';
  }
}
