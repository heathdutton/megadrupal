<?php
/**
 * @file
 * This is a base class that makes it easier to test content providers features,
 * which matches the description of any feature that defines a content type.
 */

/**
 * Test class that can be used to make it easier to test features that provides
 * content types.
 */
class NSContentProviderTestCase extends NodeStreamWebTestCase {

  /**
   * Check that a a particular references dialog view is attached to an instance.
   * @param $view_name The name of the view that should be attached.
   * @param $display_name The name of the display that should be attached.
   * @param $field_instance The instance the view is attached to.
   */
  public function assertReferencesDialogView($view_name, $display_name, array $field_instance, $message = NULL) {
    if (!isset($message)) {
      $message = t('The references dialog search view is attached to %field_name in %bundle on %entity_type',
        array('%field_name' => $field_instance['field_name'], '%bundle' => $field_instance['bundle'], '%entity_type' => $field_instance['entity_type']));
    }
    $view = views_get_view($view_name);
    $this->assertTrue(isset($view), t('The view %view_name exists', array('%view_name', $view_name)));
    $this->assertTrue(isset($view->display[$display_name]), t('The display %display_name exists', array('%display_name' => $display_name)));
    $display = $view->display[$display_name];
    $attachable_name = $field_instance['entity_type'] . ':' . $field_instance['field_name'] . ':' . $field_instance['bundle'];
    $this->assertTrue(in_array($attachable_name, $display->display_options['attach']), $message);
  }

  /**
   * Asserts that a set of fields exists.
   * @param array $fields an array with the names of the fields that should be checked.
   */
  function assertFieldsExisting(array $fields) {
    $existing_fields = field_info_fields();
    foreach ($fields as $field) {
      $this->assertTrue(isset($existing_fields[$field]), t('The field %field exists.', array('%field' => $field)));
    }
  }

  /**
   * Assert that a a particular content type exists.
   * @param $name
   *   The name of the content type to check.
   */
  function assertContentType($name, $message = NULL) {
    if (!isset($message)) {
      $message = t('The content type %type exists', array('%type' => $name));
    }
    $types = node_type_get_types();
    $this->assertTrue(isset($types[$name]), t('The article content type exists'), $message);
  }
  /**
   * Assert that a content type doesn't exist.
   * @param $name
   *   The name of the content type to check.
   */
  function assertNoContentType($name, $message = NULL) {
    if (!isset($message)) {
      $message = t('The content type %type exists', array('%type' => $name));
    }
    $types = node_type_get_types();
    $this->assertFalse(isset($types[$name]), t('The article content type exists'), $message);
  }

  /**
   * Create a content editor for a set of content types.
   * @param array $content_types
   *   An array of content types to add permissions for.
   * @return A created user.
   */
  protected function createContentEditor($content_types) {
    if (!is_array($content_types)) {
      $content_types = array($content_types);
    }
    $permissions = array(
      'access content',
      'view own unpublished content',
    );
    foreach ($content_types as $content_type) {
      $permissions = array_merge($permissions, array(
        "create $content_type content",
        "edit own $content_type content",
        "delete own $content_type content",
      ));
    }
    return $this->drupalCreateUser($permissions);
  }

  public function assertDeleteForm($editor, $nid) {
    if (is_object($nid)) {
      $nid = $nid->nid;
    }
    $this->drupalLogin($editor);
    $this->drupalGet('node/' . $nid . '/delete');
    $this->drupalPost(NULL, array(), t('Delete'));
    $deleted_node = node_load($nid, NULL, TRUE);
    $this->assertFalse($deleted_node, t('The node does not exist, it has been deleted.'));
  }
}

class NSContentTypeAccessHelper extends TestHelper {
  var $contentDefaults = array();
  protected function getNodeAddPath($content_type) {
    return 'node/add/' . str_replace('_', '-', $content_type);
  }

  function __construct($test_case, $role) {
    parent::__construct($test_case);
    if (strtolower($role) != 'anonymous') {
      $this->user = $this->drupalCreateUser();
      if (strtolower($role) != 'authenticated') {
        $this->grantRoles($this->user, array($role));
      }
      $this->drupalLogin($this->user);
    }
    else {
      $this->drupalLogout();
      $this->user = new stdClass;
      $this->user->uid = 0;
    }
    $this->role = $role;
  }

  function can() {
    $this->responseCode = 200;
    $this->verb = "can";
    return $this;
  }

  function cant() {
    $this->responseCode = 403;
    $this->verb = "can't";
    return $this;
  }

  function viewContent($content_type = NULL, $message = NULL) {
    $defaults = array();
    if (isset($content_type)) {
      $defaults['content_type'] = $content_type;
    }
    $node = $this->drupalCreateNode($defaults);
    $this->drupalGet('node/' . $node->nid);
    $this->result('view', $content_type, $message);
    return $this;
  }

  function accessPath($path, $message = NULL) {
    $this->drupalGet($path);
    if (empty($message)) {
      $message = "{$this->role} {$this->verb} access $path";
    }
    $this->assertResponse($this->responseCode, $message);
    return $this;
  }

  function editOwn($content_type, $message = NULL) {
    $node = $this->drupalCreateNode(array('type' => $content_type, 'uid' => $this->user->uid) + $this->contentDefaults);
    $this->drupalGet('node/' . $node->nid . '/edit');
    $this->result('edit own', $content_type, $message);
    return $this;
  }

  function create($content_type, $message = NULL) {
    $this->drupalGet($this->getNodeAddPath($content_type));
    $this->result('create', $content_type, $message);
    return $this;
  }

  function editOther($content_type, $message = NULL) {
    $node = $this->drupalCreateNode(array('type' => $content_type, 'uid' => 0) + $this->contentDefaults);
    $this->drupalGet('node/' . $node->nid . '/edit');
    $this->result('edit others', $content_type, $message);
    return $this;
  }

  function deleteOwn($content_type, $message = NULL) {
    $node = $this->drupalCreateNode(array('type' => $content_type, 'uid' => $this->user->uid) + $this->contentDefaults);
    $this->drupalGet('node/' . $node->nid . '/delete');
    $this->result('delete own', $content_type, $message);
    return $this;
  }

  function deleteOther($content_type, $message = NULL) {
    $node = $this->drupalCreateNode(array('type' => $content_type, 'uid' => $this->user->uid) + $this->contentDefaults);
    $this->drupalGet('node/' . $node->nid . '/delete');
    $this->result('delete own', $content_type, $message);
    return $this;
  }

  function result($op, $content_type, $message = NULL) {
    if (empty($message)) {
      $message = "{$this->role} {$this->verb} $op $content_type";
    }
    $this->assertResponse($this->responseCode, $message);
  }
}
