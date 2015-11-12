<?php

/**
 * Creates webforms in code.
 *
 * @code
 * $webform = CINC::init('Webform')->set('title', 'Contact Form');
 * @endcode
 */
class CINC_Webform extends CINC implements CINC_CRUD {

  public $config_machine_name = 'title';
  public $cinc_yaml_prefix = 'webform';

  function __construct() {

    $this->config = array(
      'type' => 'webform',
      'title' => '',
      'nid' => '',
      'language' => 'en',
      'comment' => 0,
      'promote' => 0,
      'uid' => 1,
      'webform' => array(
        'confirmation' => '',
        'confirmation_format' => NULL,
        'redirect_url' => '<confirmation>',
        'status' => '1',
        'block' => '0',
        'teaser' => '0',
        'allow_draft' => '0',
        'auto_save' => '0',
        'submit_notice' => '1',
        'submit_text' => '',
        'submit_limit' => '-1',
        'submit_interval' => '-1',
        'total_submit_limit' => '-1',
        'total_submit_interval' => '-1',
        'record_exists' => TRUE,
        'roles' => array(
          0 => '1',
        ),
        'emails' => array(),
        'components' => array(),
      ),
    );

  }

  /**
   * Saves webform to the database.
   */
  public function create() {

    $node = (object) $this->config;

    foreach ($node->webform['components'] as $index => $component) {
      $node->webform['components'][$index] = $component->config;
    }

    node_save($node);

    return $this;
  }

  /**
   * Tests if webform exists.
   */
  public function exists() {
    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'webform')
      ->propertyCondition('title', $this->config['title'])
      ->addMetaData('account', user_load(1));

    $result = $query->execute();

    if (isset($result['node'])) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads webform from the database.
   */
  public function read() {

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'webform')
      ->propertyCondition('title', $this->config['title'])
      ->addMetaData('account', user_load(1));

    $result = $query->execute();

    if (isset($result['node'])) {

      list($this->config['nid']) = array_keys($result['node']);
      $node = node_load($this->config['nid']);

      $this->config = (array) $node;

      foreach ($this->config['webform']['components'] as $index => $component) {
        $component_config = $this->config['webform']['components'][$index];
        $this->config['webform']['components'][$index] = CINC::init('WebformComponent');
        $this->config['webform']['components'][$index]->config = $component_config;
      }

    }

    return $this;
  }

  /**
   * Adds a webform component.
   */
  public function add_component($component) {
    $component->clear('nid');
    if (!is_null($this->get('nid'))) {
      $component->set('nid', $this->get('nid'));
    }
    $component->clear('cid');
    $this->config['webform']['components'][] = $component;

    return $this;
  }

  /**
   * Adds email.
   */
  public function add_email($email) {
    $this->config['webform']['emails'][] = array(
      'email' => $email,
      'subject' => 'default',
      'from_name' => 'default',
      'from_address' => 'default',
      'template' => 'default',
      'excluded_components' => array(),
    );
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves webform to the database and migrates previous data if machine name changed.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes webform from the database.
   */
  public function delete() {
    node_delete($this->config['nid']);
    return $this;
  }

}

/**
 * Creates webform components in code.
 *
 * @code
 * $component = CINC::init('WebformComponent')->machine_name('1-name');
 * @endcode
 */
class CINC_WebformComponent extends CINC implements CINC_CRUD {

  public $config_machine_name = 'nid-form_key';
  public $cinc_yaml_prefix = 'webform.component';

  function __construct() {

    $this->config_filters['nid-form_key'] = 'set_nid_form_key';
    $this->config_filters['form_key'] = 'set_form_key';

    $this->config_filters['type'] = 'set_type';

    $this->config = array(
      'nid' => '',
      'pid' => 0,
      'form_key' => '',
      'name' => '',
      'type' => 'textfield',
      'value' => '',
      'extra' => array(
        'title_display' => 'before',
        'private' => 0,
      ),
      'required' => 0,
      'weight' => 0,
      'page_num' => 1,
    );

  }

  /**
   * Sets both NID and form_key.
   */
  public function set_nid_form_key(&$nid_form_key) {
    list($nid, $form_key) = explode('-', $nid_form_key);
    $this->set('nid', $nid);
    $this->set('form_key', $form_key);
    return FALSE;
  }

  /**
   * Auto-adds name when form_key is set, and cleans type.
   */
  public function set_form_key(&$form_key) {
    $form_key = CINC::clean_machine_name($form_key);
    if (empty($this->config['name'])) {
      $this->set('name', CINC::display_name_from_machine_name($form_key));
    }
    return TRUE;
  }

  /**
   * Sets defaults based on type.
   */
  public function set_type(&$type) {

    switch ($type) {
    case 'checkbox':
      $type = 'select';
      $this->set('extra.multiple', 1);
      break;

    }

    return TRUE;

  }

  /**
   * Saves webform component to the database.
   */
  public function create() {
    return $this;
  }

  /**
   * Tests if webform component exists.
   */
  public function exists() {
    $config = db_select('webform_component')
      ->fields('webform_component')
      ->condition('nid', $this->config['nid'])
      ->condition('form_key', $this->config['form_key'])
      ->execute()
      ->fetchAssoc();

    if ($config) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Loads webform component from the database.
   */
  public function read() {

    $config = db_select('webform_component')
      ->fields('webform_component')
      ->condition('nid', $this->config['nid'])
      ->condition('form_key', $this->config['form_key'])
      ->execute()
      ->fetchAssoc();

    if ($config) {
      $this->config = $config;
      $this->config['extra'] = unserialize($this->config['extra']);
    }

    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves webform component to the database and migrates previous data if machine name changed.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes webform component from the database.
   */
  public function delete() {
    return $this;
  }

}
