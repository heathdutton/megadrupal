<?php
/**
 * @file
 * Implement the webform_field_sum source plugin.
 *
 * Sums up all values submitted for a certain webform component.
 */

$plugin = array(
  'label' => t('Webform - Sum of a field'),
  'handler' => array('class' => 'PgbarSourceWebformSum'),
);

class PgbarSourceWebformSum {
  /**
   * Constructor: Save entity and field instance.
   */
  public function __construct($entity, $instance) {
    $this->entity = $entity;
    $this->instance = $instance;
  }
  /**
   * Get the value for the given item.
   *
   * @return int
   *   Sum of all values for the webform_component with
   *   form_key == $item['options']['source']['form_key'] in all
   *   webform submissions in $this-entity and all it's translations.
   */
  public function getValue($item) {
    $entity = $this->entity;
    $sql = <<<EOSQL
SELECT SUM(wsd.data)
FROM {node} n
  INNER JOIN {webform_component} wc ON n.nid=wc.nid
  INNER JOIN {webform_submitted_data} wsd ON wsd.nid=wc.nid AND wc.cid=wsd.cid
WHERE
  (n.nid=:nid OR ((n.nid=:tnid OR n.tnid=:tnid) AND :tnid>0)) AND wc.form_key=:fkey
EOSQL;
    $args = array(
      ':nid' => $entity->nid,
      ':tnid' => $entity->tnid,
      ':fkey' => $item['options']['source']['form_key'],
    );
    return db_query($sql, $args)->fetchField();
  }
  /**
   * Build the configuration form for the field widget.
   *
   * Add a field to configure the form_key.
   * @todo make this a select box instead.
   */
  public function widgetForm($item) {
    $source_options = isset($item['options']['source']) ? $item['options']['source'] : array();
    $source_options += array('form_key' => '');
    $form['form_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Form key'),
      '#desription' => t('All values with this form key are summed up to get the current value for the progress bar.'),
      '#default_value' => $source_options['form_key'],
    );
    return $form;
  }
}
