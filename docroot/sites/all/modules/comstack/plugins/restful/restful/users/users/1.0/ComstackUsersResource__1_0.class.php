<?php

/**
 * @file
 * Contains ComstackUsersResource__1_0.
 */

class ComstackUsersResource__1_0 extends \ComstackRestfulEntityBase {
  /**
   * Overrides parent::getEntityInfo().
   */
  public function getEntityInfo($type = NULL) {
    $info = parent::getEntityInfo($type);
    $info['entity keys']['label'] = 'name';
    return $info;
  }

  /**
   * Overrides \RestfulEntityBase::getList().
   *
   * Make sure only privileged users may see a list of users.
   */
  public function getList() {
    $account = $this->getAccount();
    if (!user_access('administer users', $account) && !user_access('access user profiles', $account)) {
      throw new \RestfulForbiddenException('You do not have access to listing of users.');
    }
    return parent::getList();
  }
  /**
   * Overrides \RestfulEntityBase::getQueryForList().
   *
   * Skip the anonymous user in listing.
   */
  public function getQueryForList() {
    $query = parent::getQueryForList();
    $query->entityCondition('entity_id', 0, '>');
    return $query;
  }

  /**
   * Overrides \RestfulDataProviderEFQ::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '^(\d+,)*\d+$' => array(
        // Only allow API users to GET a single user entity.
        \RestfulInterface::GET => 'viewEntities',
      ),
    );
  }

  /**
   * Overrides \RestfulEntityBase::defaultSortInfo().
   */
  public function defaultSortInfo() {
    // Sort by 'id' in descending order.
    return array('name' => 'ASC');
  }

  /**
   * Overrides \RestfulEntityBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    // Reorder things.
    $id_field = $public_fields['id'];
    unset($public_fields['id']);

    /**
     * We want to simply output a string, if we want to do this then we could
     * do it as following with a static callback method within this class.
     */
    /*$public_fields['type'] = array(
      'callback' => 'static::getResourceType',
    );*/
    /**
     * Simplest way is to use a method in an ancestor class.
     */
    $public_fields['type'] = array(
      'callback' => array('\RestfulManager::echoMessage', array('user')),
    );

    $public_fields['id'] = $id_field;
    // Force correct data type on output.
    $public_fields['id']['process_callbacks'][] = 'intval';

    $public_fields['name'] = array(
      'property' => 'name',
    );

    $public_fields['avatars'] = array(
      'property' => 'picture',
      'process_callbacks' => array(
        // First array is for the callback, second for parameter to pass to the
        // callback. The field value is automatically passed as the first
        // parameter.
        // If this were a regular image field then we could use the following
        // style of code...
        // https://github.com/RESTful-Drupal/restful/blob/7.x-1.x/modules/restful_example/plugins/restful/node/articles/1.5/RestfulExampleArticlesResource__1_5.class.php#L28
        array(
          array($this, 'userPictureProcess'),
          array(array('comstack-100-100', 'comstack-200-200')),
        ),
      ),
    );

    // @todo this should be a count of the number of time the current user has
    // contacted the user in question.
    $public_fields['contact_frequency'] = array(
      'callback' => array('\RestfulManager::echoMessage', array(0)),
    );

    // Add in a link to the users profile.
    $public_fields['profile'] = array(
      'callback' => array(
        array($this, 'profileURL'),
        array(array()),
      ),
    );

    // Remove default properties we don't want, yeah self. Discoverable
    // dischmoverable. This will be revisited and done properly at a later
    // date. @todo expand API documentation with HAL self stuff.
    unset($public_fields['label']);
    unset($public_fields['mail']);
    unset($public_fields['self']);

    return $public_fields;
  }

  /**
   * Process callback for user image.
   *
   * @param object $file
   *   The image object.
   * @param array $image_styles
   *   An array of image styles to apply the to image.
   *
   * @return array
   *   A cleaned image array.
   */
  public function userPictureProcess($file, array $image_styles) {
    // If we don't any image styles we're not going to have much luck.
    if (empty($image_styles)) {
      return;
    }

    // If we have a valid image URI use it, else try to use the default.
    $default_avatar = variable_get('user_picture_default', '');
    if ((!is_object($file) || !$file) && $default_avatar) {
      $uri = $default_avatar;
    }
    // Valid file.
    else if ($file && isset($file->uri)) {
      $uri = $file->uri;
    }
    // Special consideration for letter_default_avatar module.
    //else if (module_exists('letter_default_avatar')) {

    //}
    // Nothing to do here.
    else {
      return;
    }

    // Loop through the image styles.
    $output = array();
    foreach ($image_styles as $style) {
      $url = image_style_url($style, $uri);

      if ($url) {
        // Replace "comstack-" with nothing from the image style name.
        $key_name = str_replace('comstack-', '', $style);
        $output[$key_name] = $url;
      }
    }

    return $output;
  }

  /**
   * Generate the URL of a users profile.
   */
  public function profileURL($wrapper) {
    $id = $wrapper->getIdentifier();
    return url("user/$id", array('absolute' => TRUE));
  }
}
