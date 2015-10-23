<?php

/**
 * @file
 * Class SearchApiHashtagAlterCallback
 */

/**
 * Implements a data alteration to extract hashtags from existing fields an
 * index them in a specific hashtag field.
 */
class SearchApiHashtagAlterCallback extends SearchApiAbstractAlterCallback {

  /**
   * {@inheritdoc}
   */
  public function alterItems(array &$items) {
    $fields = array_keys($this->options['fields']);
    foreach ($items as &$item) {
      $hashtag = array();
      foreach ($fields as $field) {
        preg_match_all('/#(\w+)/', $item->{$field}, $matches);
        $field_hashtags = $this->options['lowercase'] ? array_map('strtolower', $matches[1]) : $matches[1];
        $hashtag = array_merge($hashtag, $field_hashtags);
      }
      $item->hashtag = array_values(array_unique($hashtag));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function propertyInfo() {
    return array(
      'hashtag' => array(
        'label' => t('Hashtag'),
        'description' => t('Hashtag items extracted from text'),
        'type' => 'list<text>',
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form['#attached']['css'][] = drupal_get_path('module', 'search_api') . '/search_api.admin.css';

    $fields = $this->index->getFields();
    $field_options = array();
    $default_fields = array();
    if (isset($this->options['fields'])) {
      $default_fields = drupal_map_assoc(array_keys($this->options['fields']));
    }
    foreach ($fields as $name => $field) {
      $field_options[$name] = $field['name'];
      if (!empty($default_fields[$name]) || (!isset($this->options['fields']) && $this->testField($name, $field))) {
        $default_fields[$name] = $name;
      }
    }

    // Avoid to run in the own hashtag field.
    unset($field_options['hashtag']);

    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields to run on'),
      '#options' => $field_options,
      '#default_value' => $default_fields,
      '#attributes' => array('class' => array('search-api-checkboxes-list')),
    );

    $form['lowercase'] = array(
      '#type' => 'checkbox',
      '#title' => t('Convert all indexed hashtags values to lowercase'),
      '#default_value' => $this->options['lowercase'],
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    $fields = array_filter($values['fields']);
    if ($fields) {
      $fields = array_fill_keys($fields, TRUE);
    }
    $values['fields'] = $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    $this->options = $values;
    return $values;
  }

  /**
   * Checks if a field is available to be altered by hashtag alteration.
   *
   * @param string $name
   *   The field's machine name.
   * @param array $field
   *   The field's information.
   *
   * @return bool
   *   TRUE, if the field should be processed.
   */
  protected function testField($name, array $field) {
    if (empty($this->options['fields'])) {
      return $this->testType($field['type']);
    }
    return !empty($this->options['fields'][$name]);
  }

  /**
   * Checks the type of the field to be selected.
   *
   * @return bool
   *   TRUE, if the type should be processed.
   */
  protected function testType($type) {
    return search_api_is_text_type($type, array('text', 'tokens'));
  }

}
