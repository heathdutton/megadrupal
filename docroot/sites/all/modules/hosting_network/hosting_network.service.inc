<?php

/**
 * The base service type class , registered with hook_hosting_service_type.
 */
class hostingService_network_node extends hostingService {
  public $service = 'network_node';
}

class hostingService_network_node_hostmaster extends hostingService_network_node {
  public $type = 'hostmaster';
  public $has_port = FALSE;

  function form(&$form) {
    parent::form($form);
    $form['oauth_key'] = array(
      '#type' => 'textfield',
      // TODO: #required doesn't work. Bug with hosting_server?
      '#required' => !empty($this->available),
      '#title' => t('OAuth Key'),
      '#description' => t('The OAuth consumer key. See the network_node user.'),
      '#default_value' => isset($this->oauth_key) ? $this->oauth_key : NULL,
      '#maxlength' => 64,
      '#size' => 30,
      '#weight' => 5,
    );
    $form['oauth_secret'] = array(
      '#type' => 'password',
      '#required' => FALSE,
      '#title' => t('OAuth Secret'),
      '#default_value' => isset($this->oauth_secret) ? $this->oauth_secret : NULL,
      '#description' => t('The OAuth consumer secret.'),
      '#size' => 30,
      '#weight' => 10,
    );
  }

  function insert() {
    parent::insert();
    $id = db_insert('hosting_network_server')
      ->fields(array(
        'vid' => $this->server->vid,
        'nid' => $this->server->nid,
        'oauth_key' => $this->oauth_key,
        'oauth_secret' => $this->oauth_secret,
      ))
      ->execute();
  }

  function update() {
    if (!empty($this->oauth_secret)) {
      parent::update();
    }
    else {
      // only do the parent's update routine.
      parent::delete_revision();
      parent::insert();
    }
  }

  function delete_revision() {
    parent::delete_revision();
    db_delete('hosting_network_server')
      ->condition('vid', $this->server->vid)
      ->execute();
  }

  function delete() {
    parent::delete();
    db_delete('hosting_network_server')
      ->condition('nid', $this->server->nid)
      ->execute();
  }


  function load() {
    parent::load();
    $this->mergeData('SELECT oauth_key, oauth_secret FROM {hosting_network_server} WHERE vid = :vid', array(':vid' => $this->server->vid));
  }

  function view(&$render) {
    parent::view($render);

    $render['oauth_key'] = array(
      '#type' => 'item',
      '#title' => t('Database user'),
      '#markup' => filter_xss($this->oauth_key),
    );

  }

  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);
  }

  public function context_import($context) {
    parent::context_import($context);
  }

}

