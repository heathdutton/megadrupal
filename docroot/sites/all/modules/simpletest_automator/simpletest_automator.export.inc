<?php

/**
 * Menu callback: export a SimpleTest automator instance to the code form.
 */
function simpletest_automator_export_test($simpletest_automator) {
  // Send out the proper headers.
  drupal_set_header('Content-Type: application/x-httpd-php');
  drupal_set_header('Content-Disposition: attachment; filename="'. $simpletest_automator->file .'"');
  $code[] = '<?php';
  $code[] = '// $I' . 'd$';
  $code[] = '';
  $code[] = '/**';
  $code[] = ' * ' . $simpletest_automator->description;
  $code[] = ' */';
  $code[] = 'class ' . $simpletest_automator->class . ' extends DrupalWebTestCase {';
  $code = array_merge($code, _simpletest_automator_export_initial($simpletest_automator));
  $code = array_merge($code, _simpletest_automator_export_test($simpletest_automator));
  $code[] = '}';
  drupal_alter('simpletest_automator', $code);
  $code = _simpletest_automator_to_code($code);
  exit(implode("\n", $code));
}

/**
 * Generate the initial SimpleTest automator code: the setUp() and getInfo()
 * implementations. Helper function for simpletest_automator_export_test().
 *
 * @param $simpletest_automator
 *   The SimpleTest automator to export.
 * @return
 *   An array of lines of code corresponding to the SimpleTest's getInfo() and
 *   setUp() functions.
 */
function _simpletest_automator_export_initial($simpletest_automator) {
  $code[] = '  public static function getInfo() {';
  $code[] = '    return array(';
  $code[] = array(
    'value' => "      'name' => %s,",
    'arguments' => array($simpletest_automator->name),
  );
  $code[] = array(
    'value' => "      'description' => %s,",
    'arguments' => array($simpletest_automator->description),
  );
  $code[] = array(
    'value' => "      'group' => %s,",
    'arguments' => array($simpletest_automator->test_group),
  );
  $code[] = '    );';
  $code[] = '  }';
  $code[] = '';
  $code[] = '  function setUp() {';
  $code[] = array(
    'value' => '    parent::setUp(' . implode(', ', array_fill(0, count($simpletest_automator->modules), '%s')) . ');',
    'arguments' => $simpletest_automator->modules,
  );
  $code[] = '  }';
  $code[] = '';
  return $code;
}

/**
 * Export the test-specific actions into the SimpleTest test method.
 *
 * @param $simpletest_automator
 *   The SimpleTest automator to export.
 * @return
 *   An array of lines of code corresponding to the SimpleTest's test*()
 *   method.
 * @todo - More than one test method?
 */
function _simpletest_automator_export_test($simpletest_automator) {
  $code[] = '  function '. $simpletest_automator->method .'() {';
  $code[] = array(
    'value' => '    $this->test_user = $this->drupalCreateUser(array(' . implode(', ', array_fill(0, count($simpletest_automator->permissions), '%s')) .'));',
    'arguments' => $simpletest_automator->permissions,
  );
  $code[] = '    $this->drupalLogin($this->test_user);';
  foreach ($simpletest_automator->actions as $action) {
    $action_code = module_invoke_all('simpletest_automator_actions', $action, 'export');
    foreach ($action_code as $key => $line) {
      if (is_array($line)) {
        $action_code[$key]['value'] = '    ' . $line['value'];
      }
      else {
        $action_code[$key] = '    ' . $line;
      }
    }
    $code = array_merge($code, $action_code);
  }
  $code[] = '  }';
  return $code;
}

/**
 * Convert an export to it's simple code array format.
 *
 * @param $export
 *   The complex export array, to be translated into raw code.
 * @return
 *   A linear array of raw lines of code.
 */
function _simpletest_automator_to_code($export) {
  foreach ($export as $line) {
    if (is_string($line)) {
      $line = array('value' => $line);
    }
    if (isset($line['arguments'])) {
      $args = array($line['value']);
      foreach ($line['arguments'] as $arg) {
        $args[] = var_export($arg, TRUE);
      }
      $code[] = call_user_func_array('sprintf', $args);
    }
    else {
      $code[] = $line['value'];
    }
  }
  return $code;
}

function simpletest_automator_export_php(&$form_state, $simpletest_automator) {
  unset($simpletest_automator->said);
  unset($simpletest_automator->db_prefix);
  foreach ($simpletest_automator->actions as $index => $action) {
    unset($simpletest_automator->actions[$index]->aid);
    unset($simpletest_automator->actions[$index]->said);
  }
  $actions = $simpletest_automator->actions;
  unset($simpletest_automator->actions);
  $output = array();
  _simpletest_automator_export_php($simpletest_automator, '$simpletest_automator', $output);
  $output[] = "";
  foreach ($actions as $action) {
    _simpletest_automator_export_php($action, '$action', $output);
    $output[] = "\$simpletest_automator->actions[] = \$action;";
    $output[] = "";
  }
  $form = array();
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code'),
    '#description' => t('Copy this code and paste it where you want.'),
    '#cols' => 60,
    '#rows' => 20,
    '#default_value' => implode("\n", $output),
    '#attributes' => array('readonly' => 'readonly'),
  );
  
  return $form;
}

function _simpletest_automator_export_php($element, $variable, &$output) {
  $output[] = "$variable = new stdClass;";
  foreach ($element as $key => $value) {
    if (is_array($value)) {
      $elements = array();
      foreach ($value as $value_element) {
        $elements[] = (count($value) > 5 ? "  " : "") . var_export($value_element, TRUE) . (count($value) > 5 ? "," : "");
      }
      $temp = "$variable"."->$key = array(";
      if (count($elements) < 6) {
        $output[] = $temp . implode(', ', $elements) .");";
      }
      else {
        $output[] = $temp;
        $output = array_merge($output, $elements);
        $output[] = ");";
      }
    }
    else {
      $output[] = "$variable"."->$key = ". var_export($value, TRUE) .";";
    }
  }
}

