<?php
/**
 * @file
 * Definition of "views_handler_filter_numeric_ioo" Views plugin.
 */

/**
 * Class views_handler_filter_numeric_ioo.
 *
 * @property \views_plugin_query_default $query
 */
// @codingStandardsIgnoreStart
class views_handler_filter_numeric_ioo extends \views_handler_filter_numeric {
  // @codingStandardsIgnoreEnd

  /**
   * {@inheritdoc}
   */
  public function operators() {
    $operators = parent::operators();

    $operators['in'] = [
      'title' => t('Is one of'),
      'method' => 'oneOf',
      'short' => t('one of'),
      'values' => 1,
    ];

    $operators['not in'] = [
      'title' => t('Is not one of'),
      'method' => 'oneOf',
      'short' => t('not one of'),
      'values' => 1,
    ];

    return $operators;
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function value_submit($form, &$form_state) {
    // @codingStandardsIgnoreEnd
    $value =& $form_state['values']['options']['value']['value'];
    $value = implode(', ', self::processValue($value));
  }

  /**
   * Handle the "IN" and "NOT IN" conditions to numeric filter.
   *
   * @param string $field
   *   Field identifier (like "node.nid").
   */
  public function oneOf($field) {
    if (!empty($this->value['value'])) {
      $this->query->add_where($this->options['group'], $field, self::processValue($this->value['value']), $this->operator);
    }
  }

  /**
   * Process user input.
   *
   * @param string $value
   *   Input value.
   *
   * @return string[]
   *   Parsed value.
   */
  private static function processValue($value) {
    // Handle something like "41 ,   42,3  ,14 ,,, 21".
    return array_unique(array_map('trim', array_filter(preg_split('/(?:\s+)?,+(?:\s+)?/', $value))));
  }

}
