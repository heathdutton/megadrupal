<?php

/**
 * @file views_grouping_limit_style_plugin.inc
 * Our handler.
 */

class views_limit_grouping_style_plugin extends views_plugin_style {

  /**
   * Overrides parent::options_form().
   *
   * Add our options to the form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    foreach ($form['grouping'] as $index => $info) {
      $defaults = $this->grouping_limit_settings($index);

      $form['grouping'][$index]['grouping-limit'] = array(
        '#type' => 'fieldset',
        '#title' => t('Limit for grouping field Nr.!num', array('!num' => $index + 1)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#states' => array('invisible' => array(
          "select[name=\"style_options[grouping][{$index}][field]\"]" => array('value' => ''),
        )),
        'grouping-limit' => array(
          '#type' => 'textfield',
          '#title' => t('Items to display:'),
          '#default_value' => isset($defaults['grouping-limit']) ? $defaults['grouping-limit'] : 0,
          '#size' => 6,
          '#element_validate' => array('grouping_validate'),
          '#description' => t('The number of rows to show under each grouping header (only works when "Items to display" in the main view is set to unlimited).'),
        ),
        'grouping-offset' => array(
          '#type' => 'textfield',
          '#title' => t('Offset:'),
          '#default_value' => isset($defaults['grouping-offset']) ? $defaults['grouping-offset'] : 0,
          '#size' => 6,
          '#element_validate' => array('grouping_validate'),
          '#description' => t('The row to start on (<em>0</em> means it will start with first row, <em>1</em> means an offset of 1, and so on).'),
        ),
      );
    }
  }

  /**
   * Overrides parent::render_grouping().
   */
  function render_grouping($records, $groupings = array(), $group_rendered = NULL) {
    $sets = parent::render_grouping($records, $groupings, $group_rendered);
    // Apply the offset and limit.
    array_walk($sets, array($this, 'group_limit_recursive'));
    return $sets;
  }

  /**
   * Recursively limits the number of rows in nested groups.
   *
   * @param array $group_data
   *   A single level of grouped records.
   *
   * @param mixed $key
   *   The key of the array being passed in. Used for when this function is
   *   called from array_walk() and the like. Do not set directly.
   *
   * @params int $level
   *   The current level we are gathering results for. Used for recursive
   *   operations; do not set directly.
   *
   * @return array
   *   An array with a "rows" property that is recursively grouped by the
   *   grouping fields.
   */
  function group_limit_recursive(&$group_data, $key = NULL, $level = 0) {
    $settings = $this->grouping_limit_settings($level);

    // Slice up the rows according to the offset and limit.
    $group_data['rows'] = array_slice($group_data['rows'], $settings['grouping-offset'], $settings['grouping-limit'], TRUE);
   
    // For each row, if it appears to be another grouping, recurse again.
    foreach ($group_data['rows'] as &$data) {
      if (is_array($data) && isset($data['group']) && isset($data['rows'])) {
        $this->group_limit_recursive($data, NULL, $level + 1);
      }
    }
  }

  /**
   * Helper function to retrieve settings for grouping limit.
   *
   * @param int $index
   *   The grouping level to fetch settings for.
   *
   * @return array
   *   Settings for this grouping level.
   */
  function grouping_limit_settings($index) {
    return $this->options['grouping'][$index]['grouping-limit'];
  }
}

/**
 * Validate the added form elements.
 */
function grouping_validate($element, &$form_state) {
  // Checking to see if numeric.
  if(!is_numeric($element['#value'])) {
    form_error($element, t('%element must be numeric.', array('%element' => $element['#title'])));
  }

  // Checking for negative values.
  if ($element['#value'] < 0) {
    form_error($element, t('%element cannot be negative.', array('%element' => $element['#title'])));
  }
}
