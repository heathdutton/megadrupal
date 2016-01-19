<?php
/**
 * @file
 * Define the webform submission source plugin.
 */

$plugin = array(
  'label' => t('Webform Submissions'),
  'handler' => array('class' => 'PgbarSourceWebformSubmissions'),
);

class PgbarSourceWebformSubmissions {
  /**
   * Constructor: save entity, field and field_instance.
   */
  public function __construct($entity, $field, $instance) {
    $this->entity = $entity;
    $this->field = $field;
    $this->instance = $instance;
  }
  /**
   * Get the value for the given item.
   *
   * @return int
   *   The number of webform submissions in $this-entity
   *   and all it's translations.
   */
  public function getValue($item) {
    $entity = $this->entity;
    $q = db_select('node', 'n');
    $q->addExpression('COUNT(ws.nid)');
    $q->innerJoin('webform_submissions', 'ws', 'n.nid=ws.nid');
    $q->where(
      'n.nid=:nid OR ((n.nid=:tnid OR n.tnid=:tnid) AND :tnid>0)',
      array(':nid' => $entity->nid, ':tnid' => $entity->tnid)
    );
    return $q->execute()->fetchField();
  }
  /**
   * No extra configuration for the widget needed.
   */
  public function widgetForm($item) {
    return NULL;
  }
}
