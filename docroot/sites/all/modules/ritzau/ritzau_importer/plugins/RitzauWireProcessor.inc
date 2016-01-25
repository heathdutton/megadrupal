<?php

/**
 * @file
 * Class definition of RitzauWireProcessor.
 */

/**
 * Creates nodes from feed items.
 */
class RitzauWireProcessor extends FeedsNodeProcessor {

  /**
   * Creates a new node in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $node           = new stdClass();
    $node->type     = $this->config['content_type'];
    $node->changed  = REQUEST_TIME;
    $node->created  = REQUEST_TIME;
    $node->language = LANGUAGE_NONE;
    node_object_prepare($node);
    // Populate properties that are set by node_object_prepare().
    $node->log = 'Created by RitzauWireProcessor';
    $node->uid = $this->config['author'];
    return $node;
  }

  /**
   * Loads an existing node.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the node
   * table will be loaded, foregoing the node_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $nid) {
    $node = node_load($nid, NULL, TRUE);
    node_object_prepare($node);
    return $node;
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'log' => array(
        'name' => t('Node Log'),
        'description' => t('The node log'),
      ),
    );
    return $targets;
  }

  public function configForm(&$form_state) {
    $types = node_type_get_names();
    array_walk($types, 'check_plain');

    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#description' => t('Select the content type for the nodes to be created. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> nodes of the content type selected here regardless of the node level permissions. Further, users with "clear !feed_id permissions" will be able to <strong>delete</strong> imported nodes regardless of their node level permissions.', array('!feed_id' => $this->id)),
      '#options' => $types,
      '#default_value' => $this->config['content_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the nodes to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );
    return $form;
  }

  public function configFormValidate(&$values) {
    parent::configFormValidate($values);
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    $values['expire'] = FEEDS_EXPIRE_NEVER;
    $values['update_existing'] = FEEDS_UPDATE_EXISTING;
    parent::configFormSubmit($values);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $config = array(
      'content_type' => 'ritzau',
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
      'ritzau_id_field' => 'field_ritzau_id',
      'ritzau_reference_id_field' => 'field_refering_ritzau_id',
      'update_existing' => FEEDS_UPDATE_EXISTING,
    ) + parent::configDefaults();
    return $config;
  }

  /**
   * Override setTargetElement to operate on a target item that is a node.
   */
  public function setTargetElement(FeedsSource $source, $target_node, $target_element, $value) {
    switch ($target_element) {
      case 'log':
        $target_node->log = $value;
        break;

      default:
        parent::setTargetElement($source, $target_node, $target_element, $value);
        break;
    }
  }

  /**
   * Get nid of an existing feed item node if available.
   */
  protected function existingEntityId(FeedsSource$source, FeedsParserResult$result) {

    $ref_id = $result->current_item['refering_ritzau_id'];
    if (!isset($ref_id) || empty($ref_id)) {
      return 0;
    }
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')->entityCondition('bundle', $this->config['content_type'])->fieldCondition($this->config['ritzau_id_field'], 'value', $ref_id, '=');

    $query_result = $query->execute();
    if (!isset($query_result['node']) || empty($query_result['node'])) {
      return 0;
    }
    if (count($query_result['node']) > 1) {
      watchdog('Ritzau Import', 'Several nodes matching Reference Id "%ref_uno" - Taking the first', array('%ref_uno' => $ref_id), WATCHDOG_ERROR);
    }
    $nid = key($query_result['node']);
    if ($nid) {
      // Return with the first nid found.
      return $nid;
    }
    return 0;
  }
}

