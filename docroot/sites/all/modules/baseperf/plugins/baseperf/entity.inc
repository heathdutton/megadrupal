<?php

$plugin = array(
  'label' => 'Entity',
  'handler' => array(
    'class' => 'BaseperfEntity',
  ),
);

class BaseperfEntity extends Baseperf {

  function requiredModules() {
    return array('entity');
  }

  function configForm() {
    $options = array();
    $info = entity_get_info();
    foreach ($info as $entity_type => $entity_info) {
      foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
        $count = $this->countEntities($entity_type, $bundle);
        $options[$entity_type . ':' . $bundle] = $entity_info['label'] . ': ' . $bundle_info['label'] . ' (' . $count . ')';
      }
    }
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Load entities of type'),
      '#description' => t('The number in parenthesis indicates the number of entities in database.'),
      '#options' => $options,
      '#required' => TRUE,
    );
    $form['order'] = array(
      '#type' => 'radios',
      '#title' => t('Which entities to pick?'),
      '#options' => array(
        'ASC' => t('First ones'),
        'DESC' => t('Last ones'),
        'both' => t('Half and half'),
      ),
      '#default_value' => 'both',
      '#required' => TRUE,
    );
    $form['count'] = array(
      '#type' => 'textfield',
      '#title' => t('How much entities?'),
      '#default_value' => 5,
      '#required' => TRUE,
    );
    $form['render'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render the entities'),
      '#default_value' => FALSE,
    );

    return $form;
  }

  function configFormValidate($form, &$values) {
    if (form_get_errors()) return;
    list($entity_type, $bundle) = explode(':', $values['type']);
    $count = $this->countEntities($entity_type, $bundle);
    if (((int) $values['count']) > $count) {
      form_error($form['count'], t('Number of entities selected is greater than existing entities.'));
    }
  }

  function execute($config) {
    list($entity_type, $bundle) = explode(':', $config['type']);
    if ($config['order'] != 'both') {
      $ids = $this->getIds($entity_type, $bundle, $config['count'], $config['order']);
    }
    else {
      $ids1 = $this->getIds($entity_type, floor($config['count'] / 2), 'ASC');
      $ids2 = $this->getIds($entity_type, ceil($config['count'] / 2), 'DESC');
      $ids = array_merge($ids1, $ids2);
    }
    $entities = entity_load($entity_type, $ids);
    if ($config['render']) {
      entity_view($entity_type, $entities);
    }
  }

  function countEntities($entity_type, $bundle) {
    // Comment entity type doesn't support bundle condition.
    // @see https://api.drupal.org/api/drupal/includes!entity.inc/function/EntityFieldQuery%3A%3AentityCondition/7
    if ($entity_type == 'comment') {
      // Bundle has the form comment_node_NODE_TYPE.
      $node_type = substr($bundle, 13);
      $result = db_query("SELECT count(cid) FROM {comment} c LEFT JOIN {node} n ON c.nid=n.nid WHERE n.type = :node_type", array(':node_type' => $node_type));
      return current($result->fetchObject());
    }

    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $entity_type)
      ->entityCondition('bundle', $bundle)
      ->count();
    return $query->execute();
  }

  function getIds($entity_type, $count, $order) {
    $info = entity_get_info($entity_type);
    $id = $info['entity keys']['id'];

    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $entity_type)
      ->propertyOrderBy($id, $order)
      ->range(0, $count);
    $results = $query->execute();
    foreach ($results[$entity_type] as $result) {
      $ids[] = $result->{$id};
    }
    return $ids;
  }
}
