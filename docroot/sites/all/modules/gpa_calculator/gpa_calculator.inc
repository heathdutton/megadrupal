<?php

/**
 * @file
 * The actual gpa calculator called in hook_block_view.
 */

/**
 * GPA Calculator form.
 */
function gpa_calculator_form($form, &$form_state) {
  $form = array();

  // Get GPA admin settings.
  // Format the instructions based on the format it was saved with.
  // If no format was saved in the variable_get then run check_plain().
  $gpa_calculator_instructions_format = variable_get('gpa_calculator_instructions', '');
  if (isset($gpa_calculator_instructions_format['format'])) {
    $gpa_calculator_instructions_value = variable_get('gpa_calculator_instructions', '');
    $gpa_calculator_instructions = check_markup($gpa_calculator_instructions_value['value'], $gpa_calculator_instructions_format['format']);
  }
  else {
    $gpa_calculator_instructions = check_plain(variable_get('gpa_calculator_instructions', ''));
  }
  $gpa_calculator_grades = check_plain(variable_get('gpa_calculator_grades', ''));
  $gpa_calculator_grades_array = array();
  // Turn grades string into array.
  foreach (explode("\n", $gpa_calculator_grades) as $grade) {
    if ($gpa_calculator_grades != '') {
      list ($key, $value) = explode('|', $grade, 2);
      $gpa_calculator_grades_array[$key] = $value;
    }
  }

  $gpa_instructions = $gpa_calculator_instructions != '' ? $gpa_calculator_instructions : '';

  $form['gpa_instructions'] = array(
    '#markup' => '<div class="gpa-calculator-instructions-wrapper">' . $gpa_instructions . '</div>',
  );

  $form['add_row'] = array(
    '#markup' => l(t('Add Row'), 'javascript:void(0);',
      array(
        'fragment' => '',
        'external' => TRUE,
        'attributes' => array(
          'id' => 'gpa-calculator-add-row',
        ),
      )
    ),
  );

  $gpa_table_head = '<div id="gpa-calculator-grades-table">';
  $gpa_table_head .= '<div class="gpa-table-thead">';
  $gpa_table_head .= '<div class="gpa-table-cell gpa-th">#</div>';
  $gpa_table_head .= '<div class="gpa-table-cell gpa-th">' . t('Class/Course Name') . '</div>';
  $gpa_table_head .= '<div class="gpa-table-cell gpa-th">' . t('Grade') . '</div>';
  $gpa_table_head .= '<div class="gpa-table-cell gpa-th">' . t('Credits Earned') . '</div>';
  $gpa_table_head .= '</div>';
  $gpa_table_head .= '<div class="gpa-table-body"></div>';

  $form['gpa_table_head'] = array(
    '#markup' => $gpa_table_head,
  );

  $form['gpa_table_end'] = array(
    '#markup' => '</div>',
  );

  $form['prev_gpa'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Cumulative GPA'),
    '#description' => variable_get('gpa_calculator_prev_gpa_instructions', ''),
    '#attributes' => array('id' => 'gpa-calculator-prev-gpa'),
    '#size' => 3,
  );

  $form['prev_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Cumulative Credits Earned'),
    '#description' => variable_get('gpa_calculator_prev_credits_instructions', ''),
    '#attributes' => array('id' => 'gpa-calculator-prev-hours'),
    '#size' => 3,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Calculate'),
  );

  $form['gpa_results_wrapper'] = array(
    '#markup' => '<div class="gpa-results-wrapper">',
  );

  $form['gpa_current_output'] = array(
    '#markup' => '<div id="gpa-calculator-gpa-current-output"></div>',
  );

  $form['gpa_cumulative_output'] = array(
    '#markup' => '<div id="gpa-calculator-gpa-cumulative-output"></div>',
  );

  $form['gpa_results_wrapper_end'] = array(
    '#markup' => '</div>',
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'gpa_calculator') . '/css/gpa_calculator.css', 'module', 'all', FALSE,
  );

  $form['#attached']['js'] = array(
    array(
      'data' => drupal_get_path('module', 'gpa_calculator') . '/js/gpa_calculator.js',
      'scope' => 'footer',
    ),
    // Pass grades array to javascript.
    array(
      'data' => array(
        'gpa_calculator' => array(
          'gpa_calculator_grades' => $gpa_calculator_grades_array,
        ),
      ),
      'type' => 'setting',
    ),
  );

  return $form;
}
