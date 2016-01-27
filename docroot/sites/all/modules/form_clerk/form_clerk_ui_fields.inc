<?php

/**
 * @file
 * form_clerk UI form field constructors.
 *
 */

/**
 * Build the form field based on the $field parameter.
 * @param $form
 *  The form argument.
 * @param $form_state
 *  The form state argument.
 * @param $field
 *  String field name.
 */
function form_clerk_ui_build_form_field(&$form, &$form_state, $field='') {

  switch ($field) {
    // //////////////////////////////////////////////////////////////////////////////////
    case 'result_fieldset':
      $form['result'] = array(
        '#type' => 'fieldset',
        '#title' => t('Results'),
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'form_list':
      // Display the form info.
      $field_order = array('tag', 'type', 'label', 'name', 'value');
      $highlighted = array(1 => 'name', 2 => 'value');
      $markup = '<h3>URL: ' . $form_state['storage']['url'] . '</h3>';
      $form_list = $form_state['storage']['form_list'];
      if (count($form_list) == 0) {
        return 'No forms were found at that URL.';
      }
      for ($f = 0; $f < count($form_list); $f++) {
        $this_form = $form_list[$f];
        $table = array();
        $table['caption'] = 'Form ID: ' . $this_form['id'];
        $table['attributes'] = array('class' => 'form_clerk_table');
        $table['sticky'] = FALSE;
        $table['header'] = array();
        $table['rows'] = array();
        foreach ($field_order as $key => $field) {
            if (array_search($field, $highlighted)) {
              $table['header'][] = array('data' => $field,
                                         'class' => 'form_clerk_highlighted',
                                          );
            }
            else {
              $table['header'][] = $field;
            }
        }
        $elements = $this_form['elements'];
        foreach ($elements as $element) {
          $this_row = array();
          $this_row['data'] = array();
          $this_row['no_striping'] = TRUE;
          if ($element['type'] == 'hidden') {
            $this_row['class'] = array('form_clerk_hidden');
          }
          foreach ($field_order as $field) {
            if (array_search($field, $highlighted)) {
              $this_row['data'][] = array('data' => $element[$field],
                                          'class' => 'form_clerk_highlighted',
                                          );
            }
            else {
              $this_row['data'][] = $element[$field];
            }
          }
          $table['rows'][] = $this_row;
        }
        $markup .= theme('table', $table);
      }
      drupal_add_css(FORM_CLERK_PATH . '/form_clerk.css');
      $form['result']['form_list'] = array(
        '#type' => 'item',
        '#markup' => $markup,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    case 'config_fieldset':
      $form['config'] = array(
        '#type' => 'fieldset',
        '#title' => t('Form Clerk Configuration'),
        '#tree' => TRUE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'config_directory':
      $dir = variable_get("form_clerk_directory", NULL);
      $default = isset($dir) ? $dir : '';
      $form['config']['directory'] = array(
        '#type'  => 'textfield',
        '#title' => 'Form Clerk directory',
        '#default_value' => $default,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'config_help':
      $form['config']['help'] = array(
        '#type' => 'markup',
        '#markup' => t('Set the directory where Form Clerk input files and reports are stored. The directory must have subdirectories "inputs" and "reports". Directories must exist and be readable and writeable.'),
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'config_submit':
      $form['info']['submit-config'] = array(
        '#type'  => 'submit',
        '#value' => 'Save Configuration',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'config_watchdog':
      $watchdog = variable_get("form_clerk_watchdog", NULL);
      $default = isset($watchdog) ? $watchdog : TRUE;
      $form['config']['watchdog'] = array(
        '#type'  => 'checkbox',
        '#title' => 'Write a Watchdog message on each run of Form Clerk.',
        '#default_value' => $default,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_fieldset':
      $form['info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Get Form Information'),
        '#tree' => TRUE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_help':
      $form['info']['help'] = array(
        '#type' => 'markup',
        '#markup' => t('This shows details of the form structure. This is useful for creating input files.'),
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_login_fieldset':
      $form['info']['login'] = array(
        '#type' => 'fieldset',
        '#title' => t('Login Information'),
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_login_help':
      $form['info']['login']['help'] = array(
        '#type' => 'markup',
        '#markup' => t('If you need to be logged in to view the form, provide login information here.'),
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_form_url':
      $form['info']['form_url'] = array(
        '#type'     => 'textfield',
        '#size'     => 80,
        '#title'    => 'Form URL',
        '#default'    => isset($form_state['storage']['url']) ? $form_state['storage']['url'] : '',
//        '#required' => TRUE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_login_form_id':
      $form['info']['login']['login_form_id'] = array(
        '#type'     => 'textfield',
        '#size'     => 80,
        '#title'    => 'Login Form ID',
        '#default_value' => 'user-login',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_login_url':
      $form['info']['login']['login_url'] = array(
        '#type'     => 'textfield',
        '#size'     => 80,
        '#title'    => 'Login URL',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_password':
      $form['info']['login']['password'] = array(
        '#type'  => 'password',
        '#size'  => 20,
        '#title' => 'Password',
//        '#default'    => isset($form_state['storage']['password']) ? $form_state['storage']['password'] : '',
//        '#required' => TRUE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_submit':
      $form['info']['submit-info'] = array(
        '#type'  => 'submit',
        '#value' => 'Get Form Info',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'info_user':
      $form['info']['login']['user'] = array(
        '#type'  => 'textfield',
        '#size'  => 20,
        '#title' => 'User name',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    // //////////////////////////////////////////////////////////////////////////////////
    case 'input_fieldset':
      $form['input'] = array(
        '#type' => 'fieldset',
        '#title' => t('Input Data'),
        '#tree' => TRUE,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'input_help':
      $top_dir  = variable_get('form_clerk_directory', 'tmp') . '/inputs';
      $form['input']['help'] = array(
        '#type' => 'markup',
        '#markup' => t('Provide one or more input files, separated by commas.<br> Input file directory is: <code>@dir</code>', array('@dir' => $top_dir)),
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'input_file':
      $form['input']['input_file'] = array(
        '#type'  => 'textfield',
        '#title' => 'Form data input file(s)',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'input_password':
      $form['input']['password'] = array(
        '#type'  => 'password',
        '#size'  => 20,
        '#title' => 'Password on the target system',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'input_submit':
      $form['input']['submit-input'] = array(
        '#type'  => 'submit',
        '#value' => 'Input Form Data',
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    case 'report':
      // Write brief report.
      $markup = '<pre>' . $form_state['storage']['report'] . '</pre>';
      $form['result']['report'] = array(
        '#type' => 'item',
        '#markup' => $markup,
      );
      break;

    // //////////////////////////////////////////////////////////////////////////////////
    default:
      // We shouldn't get here.
  }
}
