<?php

$plugin = array(
  'label' => 'SQL',
  'handler' => array(
    'class' => 'BasePerfSql',
  ),
);

class BasePerfSql extends BasePerf {
  private $query = 'SELECT 1 FROM {variable}';

  function getDescription() {
    return t('This test performs query «!query» the specified number of times.', array('!query' => $this->query));
  }

  function configForm() {
    $form['count'] = array(
      '#type' => 'textfield',
      '#title' => t('How much times?'),
      '#description' => t('Enter the number of times you want this query to be performed.'),
      '#default_value' => 10000,
      '#required' => TRUE,
    );
    return $form;
  }

  function configFormValidate($form, &$values) {
    $count = (int)$values['count'];
    if ($count < 1) {
      form_error($form['count'], t('Must be a positive integer.'));
    }
  }

  function execute($config) {
    for ($i = 0; $i < $config['count']; $i++) {
      db_query($this->query);
    }
  }
}
