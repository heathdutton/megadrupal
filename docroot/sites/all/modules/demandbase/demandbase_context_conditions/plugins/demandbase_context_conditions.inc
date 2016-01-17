<?php

/**
 * @file
 * Serve Company Name data from demandbase.
 */

/**
 * Expose company as a context condition.
 */
class demandbase_context_conditions extends context_condition {
  /**
   * Omit condition values. We will provide a custom input form for our
   * conditions.
   */
  function condition_values() {
    return array('demandbase' => t('Enable demandbase conditions (Uncheck and save to remove all demandbase conditions.)'));
  }

  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    $form = array();
    $condition_name = $this->plugin;
    $all_conditions = demandbase_get_specified_conditions();
    $form['demandbase_conditions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Select demandbase conditions'),
      '#description' => '',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      );
    $form['demandbase_conditions']['meet_criteria'] = array(
      '#type' => 'radios',
      '#title' => t('Conditions meet criteria'),
      '#default_value' => isset($defaults['demandbase_conditions']['meet_criteria']) ? $defaults['demandbase_conditions']['meet_criteria'] : 'any',
      '#options' => array('all' =>  t('Meet All conditions'), 'any' => t('Meet any conditions')),
      );
    if (empty($all_conditions)) {
      $path = l(t('Demandbase condition'), 'admin/config/services/demandbase/conditions');
      $form['demandbase_conditions']['markup'] = array(
        '#type' => 'markup',
        '#markup' => t('Note: No demandbase fields are set for context condtions. Please set the demandbase fields as context condition. These demandbase conditions are set at') . $path,
        );
    }
    if (!empty($all_conditions)) {
      foreach ($all_conditions as  $key => $value) {
        $form['demandbase_conditions'][$key] = array(
          '#type' => 'checkbox',
          '#title' => check_plain(ucfirst(t($value))),
          '#default_value' => isset($defaults['demandbase_conditions'][$key]) ? $defaults['demandbase_conditions'][$key] : '',
          );

        $label = "{$this->title}: ";
        $label = isset($options[$value]) ? trim($options[$value], ' -') : $value;
        $form['demandbase_conditions'][$key . 'textarea'] = array(
          '#type' => 'textarea',
          '#rows' => 3,
          '#title' => check_plain(ucfirst(t($label))),
          '#default_value' => isset($defaults['demandbase_conditions'][$key . 'textarea']) ? $defaults['demandbase_conditions'][$key . 'textarea'] : '',
          '#states' => array(
        'visible' => array(   // action to take.
          ":input[name='conditions[plugins][demandbase_context_conditions][options][demandbase_conditions][$key]']"  // element to evaluate condition on
            => array('checked' => TRUE),  // condition
            ),
        ),
          );
      }
    }
    return $form;
  }


  /**
   * Meet all the demandbase conditions
   */
  function meet_demandbase_conditions($context) {
    $my_context = $this->fetch_from_context($context, 'options');
    $demandbase_client_obj = new demandbase_client();
    $ip = drupal_strtolower($demandbase_client_obj->fetch_data());
    $match_condition = $my_context['demandbase_conditions']['meet_criteria'];
    foreach ($my_context['demandbase_conditions'] as $key => $condition_set) {
      if ($key == 'meet_criteria') {
        continue; // skip as this is condition field for meeting all or any conditions
      }
      if (str_ireplace("textarea", '', $key) != $key) {
        continue;
      }
      if (!$condition_set) {
        continue;
      }
      $condition_value = (isset($my_context['demandbase_conditions'][$key . "textarea"])) 
      ? $my_context['demandbase_conditions'][$key . "textarea"] : '';
      if (empty($condition_value)) {
        continue;
      }
      $demandbase_data = (isset($demandbase_client_obj->processed_data[$key])) ? drupal_strtolower($demandbase_client_obj->processed_data[$key]) : '';
      $condition_arr = explode("\n", $condition_value);
      for ($i=0; $i<count($condition_arr); $i++) {
        $supplied_item = $condition_arr[$i];
        $condition_values[$supplied_item] = $supplied_item;
      }
      $matched = FALSE;
      if (!empty($demandbase_data)) {
        $matched = $demandbase_client_obj->match($demandbase_data, $condition_values, TRUE);
      }
      switch ($match_condition) {
        case 'all':
        if (!$matched) {
      return FALSE; // get back when any condition fails
    }
    break;
    case 'any':
    if ($matched) {
      return TRUE; // get back when any condition meets
    }
    break;
        }
        continue;
      }
    // Finally now set that condition is met or not based on the type of meet condition criteria
      switch ($match_condition) {
        case 'all':
        return TRUE; // get back when no condition fails; flag condition meets
        case 'any':
        return FALSE; // get back when no condition meets; flag no condition meets
      }
    }


  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        $matched = $this->meet_demandbase_conditions($context);
        if ($matched) {
    $this->condition_met($context);
        }
      }
    }
  }
}//end class
