<?php

/**
 * Search API data alteration callback that filters out items based on the
 * fields that refer to them.
 *
 * Only availability calendar fields and availability calendar entities are
 * supported.
 */
class AvailabilityCalendarAlterFieldFilter extends SearchApiAbstractAlterCallback {

  public function supportsIndex(SearchApiIndex $index) {
    return $index->item_type === 'availability_calendar_calendar';
  }

  public function alterItems(array &$items) {
    if ($this->index->item_type === 'availability_calendar_calendar') {
      $fields = $this->options['fields'];
      foreach ($items as $id => $item) {
        $referring_field = availability_calendar_get_referring_field($id, $fields);
        if ( ($this->options['default'] === 'exclude' && !empty($referring_field))
          || ($this->options['default'] === 'include' && !$referring_field)) {
          unset($items[$id]);
        }
      }
    }
  }

  public function configurationForm() {
    if ($this->index->item_type === 'availability_calendar_calendar') {
      $options = drupal_map_assoc($this->get_fields_by_type('availability_calendar'));
      $form = array(
        'default' => array(
          '#type' => 'radios',
          '#title' => t('Which items should be indexed?'),
          '#default_value' => isset($this->options['default']) ? $this->options['default'] : 'exclude',
          '#options' => array(
            'exclude' => t('All but those from one of the selected fields'),
            'include' => t('Only those from the selected fields'),
          ),
        ),
        'fields' => array(
          '#type' => 'select',
          '#title' => t('Fields'),
          '#default_value' => isset($this->options['fields']) ? $this->options['fields'] : array(),
          '#options' => $options,
          '#size' => min(3, count($options)),
          '#multiple' => TRUE,
        ),
      );
    }
    else {
      $form = array(
        'forbidden' => array(
          '#markup' => '<p>' . t("This index doesn't index availability calendars and therefore cannot be filtered on referring availability calendar fields here.") . '</p>',
        ),
      );
    }
    return $form;
  }

  /**
   * Returns all fields of the given type.
   *
   * @param string $type
   *
   * @return array
   *   An array with field (machine) names.
   */
  protected function get_fields_by_type($type) {
    // @todo: As of Drupal 7.22?: $instances = field_info_field_map();
    $result = array();
    $fields = field_info_fields();
    foreach ($fields as $field_name => $field) {
      if ($field['type'] === $type && $field['deleted'] == 0) {
        $result[] = $field_name;
      }
    }
    return $result;
  }
}
