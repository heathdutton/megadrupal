<?php

/**
 * @file
 * Quiz module direction component.
 * @author IP-TECH
 */

/**
 * Create a default direction component.
 */
function _dquarks_defaults_direction() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'default_jump' => 0,
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form.
 * @return array
 *   An array of form items to be displayed on the edit component page.
 */
function _dquarks_edit_direction($currfield, $node, &$form_state) {
  $form = array();

  $list_available_jump = _dquarks_direction_available_jump($currfield, $node);
  $list_available_field = _dquarks_direction_available_fields($currfield, $node);


  $form['extra']['default_jump'] = array(
    '#type' => 'select',
    '#title' => t('Default page to jump'),
    '#default_value' => $currfield['extra']['default_jump'],
    '#options' => $list_available_jump,
    '#description' => t('Define the next page to jump by default.'),
    '#weight' => 0,
  );
  $form['extra']['description'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );
  $form['advanced']['pid'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );
  $form['advanced']['mandatory'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );

  if (!empty($currfield['cid'])) {
    $form['add'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rules'),
      '#weight' => -2,
      '#theme' => '_dquarks_edit_direction',
      '#prefix' => "<div id='all_roles'>",
      '#suffix' => "</div>",
    );
    $tests = array();
    $add_weight = _dquarks_direction_load_tests($tests, $currfield);
    $ahah_path = "node/" . $node->nid . "/dquarks/components/";
    foreach ($tests as $testid => $direction_test) {
      $form['add']['tests'][$testid] = array();
      $form['add']['tests'][$testid]['testid'] = array(
        '#type' => 'hidden',
        '#default_value' => $direction_test['testid'],
      );
      $form['add']['tests'][$testid]['test_name'] = array(
        '#type' => 'textfield',
        '#size' => 15,
        '#maxlength' => 255,
        '#default_value' => $direction_test['test_name'],
      );
      $form['add']['tests'][$testid]['tested_field'] = array(
        '#type' => 'select',
        '#default_value' => $direction_test['tested_field'],
        '#options' => $list_available_field,
        '#weight' => 0,
      );
      $form['add']['tests'][$testid]['test_type'] = array(
        '#type' => 'select',
        '#default_value' => $direction_test['test_type'],
        '#options' => array(
          '0' => t('Equal'),
          '1' => t('Not equal'),
          '2' => t('Contain'),
          '3' => t('Do not contain'),
          '4' => t('More then'),
          '5' => t('Less then'),
          '6' => t('After date'),
          '7' => t('Before date')),
      );
      $form['add']['tests'][$testid]['testvalue'] = array(
        '#type' => 'textfield',
        '#size' => 15,
        '#maxlength' => 255,
        '#default_value' => $direction_test['testvalue'],
      );
      $form['add']['tests'][$testid]['next_jump'] = array(
        '#type' => 'select',
        '#default_value' => $direction_test['next_jump'],
        '#options' => $list_available_jump,
      );
      $form['add']['tests'][$testid]['weight'] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#default_value' => $direction_test['weight'],
      );
    }

    $form['add']['nid'] = array(
      '#type' => 'hidden',
      '#default_value' => $node->nid,
    );
    $form['add']['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $currfield['cid'],
    );
    $form['add']['testid'] = array(
      '#type' => 'hidden',
      '#default_value' => (count($tests) > 0) ? max(array_keys($tests)) + 1 : 1,
    );
    $form['add']['test_name'] = array(
      '#type' => 'textfield',
      '#size' => 15,
      '#maxlength' => 255,
      '#default_value' => '',
    );
    $form['add']['tested_field'] = array(
      '#type' => 'select',
      '#default_value' => 0,
      '#options' => $list_available_field,
      '#weight' => 0,
    );
    $form['add']['test_type'] = array(
      '#type' => 'select',
      '#default_value' => 0,
      '#options' => array(
        '0' => t('Equal'),
        '1' => t('Not equal'),
        '2' => t('Contain'),
        '3' => t('Do not contain'),
        '4' => t('More then'),
        '5' => t('Less then'),
        '6' => t('After date'),
        '7' => t('Before date')),
    );
    $form['add']['testvalue'] = array(
      '#type' => 'textfield',
      '#size' => 15,
      '#maxlength' => 255,
      '#default_value' => '',
    );
    $form['add']['next_jump'] = array(
      '#type' => 'select',
      '#default_value' => 0,
      '#options' => $list_available_jump,
    );
    $form['add']['weight'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $add_weight,
    );
    $form['add']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Rule'),
      '#weight' => 45,
    );
  }

  return $form;
}

/**
 * Format the text output for this component.
 *
 * @param array $variables
 *   Contain the form to theme it
 *
 * @return string
 *   Textual output to be included in direction component page.
 */
function theme__dquarks_edit_direction($variables) {
  $form = $variables['form'];

  // Add CSS to display submission info. Don't preprocess because this CSS file
  // is used rarely.
  drupal_add_css(drupal_get_path('module', 'dquarks') . '/css/dquarks.css', 'module', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'dquarks') . '/js/dquarks.js', 'module', 'header', FALSE, TRUE, FALSE);

  $output = '';
  $cid = $form['cid']['#value'];
  if (!empty($cid)) {
    // You must save direction to get access to rules.
    drupal_add_tabledrag('dquarks-direction-tests', 'order', 'sibling', 'dquarks-weight');
    $headers = array(
      '',
      t('Name'),
      t('Field to test values'),
      t('Type'),
      t('Test value'),
      t('Next jump'),
      t('Weight'),
      t('Suppression'),
    );
    $rows = array();
    $count_tests = 0;
    if (!empty($form['tests'])) {
      foreach ($form['tests'] as $testid => $test) {
        if (!is_int($testid)) {
          continue;
        }
        $count_tests++;
        $test['cid']['#attributes']['class'] = array('dquarks-cid');
        $test['testid']['#attributes']['class'] = array('dquarks-testid');
        $test['weight']['#attributes']['class'] = array('dquarks-weight');
        $rows_data = array(
          filter_xss($testid),
          drupal_render($test['test_name']),
          drupal_render($test['tested_field']),
          drupal_render($test['test_type']),
          drupal_render($test['testvalue']),
          drupal_render($test['next_jump']),
          drupal_render($test['weight']),
          l(t('Delete'), 'node/' . $form['nid']['#value'] . '/dquarks/components/' . $cid . '/delrule/' . $testid),
        );
        $rows[] = array(
          'data' => $rows_data,
          'class' => array(
            'draggable',
            'tabledrag-leaf'),
        );
      }
      unset($form['tests']);
    }
    else {
      $rows[] = array(array('data' => t('No rules, add a rules below.'), 'colspan' => 8));
    }

    $form['add']['cid']['#attributes']['class'] = array('dquarks-cid');
    $form['add']['testid']['#attributes']['class'] = array('dquarks-pid');
    $form['add']['weight']['#attributes']['class'] = array('dquarks-weight');

    $rows_data = array(
      filter_xss($form['testid']['#default_value']),
      drupal_render($form['test_name']),
      drupal_render($form['tested_field']),
      drupal_render($form['test_type']),
      drupal_render($form['testvalue']),
      drupal_render($form['next_jump']),
      drupal_render($form['weight']),
      drupal_render($form['add']),
    );
    $rows[] = array(
      'data' => $rows_data,
      'class' => array(
        'draggable',
        'dquarks-add-form'),
    );
  }

  if (!empty($cid)) {
    $output .= theme('table', array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array('id' => 'dquarks-direction-tests'))
    );
  }
  return $output;
}

/**
 * Module specific instance of hook_help().
 *
 * @param string $section
 *   The path of the page.
 *
 * @return string
 *   Description of section .
 */
function _dquarks_help_direction($section) {
  switch ($section) {
    case 'admin/settings/dquarks#direction_description':
      return t('Break up a multi-page quiz with test value for jump.');
  }
}

/**
 * Render direction as hidden elements.
 *
 * @param array $component
 *   An array of information describing the component.
 *
 * @return array
 *   An array of a form item rendered as hidden type to be displayed in emails
 * as separators.
 */
function _dquarks_render_direction($component) {
  $form = array();
  // Render direction as hidden elements so that they can be displayed in
  // emails as separators.
  $form = array(
    '#type' => 'hidden',
    '#value' => $component['name'],
    '#weight' => $component['weight'],
  );
  return $form;
}

/**
 * Module specific instance of hook_theme().
 */
function _dquarks_theme_direction() {

  return array(
    'dquarks_mail_direction' => array(
      'variables' => array('data' => NULL, 'component' => NULL),
      'file' => 'components/direction.inc',
    ),
    '_dquarks_edit_direction' => array(
      'render element' => 'form',
      'file' => 'components/direction.inc',
    ),
  );
}

/**
 * Format the output of e-mailed data for this component.
 *
 * @param array $variables
 *   Contain a string or array of the submitted data,an array of
 *   information describing the component, directly correlating to the
 *   dquarks_component database schema.
 *
 * @return string
 *   Textual output to be included in the email.
 */
function theme_dquarks_mail_direction($variables) {
  $component = $variables['component'];
  return "\n-- " . $component['name'] . " --\n";
}

/**
 * Return the list of the available jump.
 *
 * @param array $currentfild
 *   The current displayed component.
 * @param object $node
 *   The dquarks node object.
 *
 * @return array
 *   The available jump.
 */
function _dquarks_direction_available_jump($currentfild, $node) {
  $return = array(0 => t('Next page'));
  $counter = 1;
  foreach ($node->dquarks['components'] as $component) {
    if (($component['type'] == 'direction')
            && ($component['weight'] > $currentfild['weight'])) {
      $return[$component['cid']] = 'Page ' . ++$counter;
    }
    elseif ($component['type'] == 'direction') {
      $counter++;
    }
  }
  return $return;
}

/**
 * Return the list of the available fields.
 *
 * @param array $currentfild
 *   The current displayed component.
 * @param object $node
 *   The dquarks node object.
 *
 * @return array
 *   The available component name.
 */
function _dquarks_direction_available_fields($currentfild, $node) {
  $return = array();
  foreach ($node->dquarks['components'] as $component) {
    if (isset($currentfild['cid']) && $component['cid'] == $currentfild['cid']) {
      break;
    }
    elseif ($component['type'] != 'direction') {
      $return[$component['cid']] = $component['name'];
    }
    elseif ($component['type'] == 'direction') {
      $return = array();
    }
  }
  return $return;
}

/**
 * Display the confirmation message to delete a rule of component direction.
 *
 * @param array $form_state
 *   An array of form items of direction component.
 * @param object $node
 *   The dquarks node object.
 * @param array $component
 *   The direction component.
 * @param object $rule
 *   The rule applyed for a direction component.
 *
 * @return array
 *   Generate a form array for a confirmation form.
 */
function _dquarks_delrule_direction(&$form_state, $node, $component, $rule) {

  $cid = $component['cid'];

  $form = array();
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['component'] = array(
    '#type' => 'value',
    '#value' => $component,
  );

  $form['rule'] = array(
    '#type' => 'value',
    '#value' => $rule,
  );

  $question = t('Delete the %name rule?', array('%name' => $rule->test_name));
  $description = t('This will immediately delete the %name rule from the %direction direction. This cannot be undone.', array(
    '%name' => $rule->test_name,
    '%direction' => $node->dquarks['components'][$cid]['name'])
  );

  return confirm_form($form, $question, 'node/' . $node->nid . '/dquarks/components/' . $cid, $description, t('Delete'));
}

/**
 * Delete the rule of the direction component from the table.
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form items.
 */
function _dquarks_delrule_submit_direction($form, $form_state) {
  db_delete('dquarks_direction_tests')
          ->condition('nid', $form_state['values']['node']->nid)
          ->condition('cid', $form_state['values']['component']['cid'])
          ->condition('testid', $form_state['values']['rule']->testid)
          ->execute();
}

/**
 * Load tests.
 *
 * @param array $tests
 *   Loaded tests.
 * @param array $direction
 *   The current direction component.
 *
 * @return int
 *   The weight of tests
 */
function _dquarks_direction_load_tests(&$tests, $direction) {
  $result = db_query('SELECT * FROM {dquarks_direction_tests} WHERE nid= :nid AND cid= :cid ORDER BY weight', array(
    ':nid' => $direction['nid'],
    ':cid' => $direction['cid'])
  );
  $nv_weight = 0;
  foreach ($result as $test) {
    $test = (array) $test;
    $tests[$test['testid']] = $test;
    $nv_weight = max($nv_weight, $test['weight']);
  }
  return $nv_weight + 1;
}

/**
 * Load rule of specific direcion component.
 *
 * @param object $node
 *   The object node
 * @param array $direction
 *   The current direction
 * @param int $testid
 *   The idenifier of the test
 *
 * @return array
 *   Rule of Direction component
 */
function _dquarks_loadrule_direction($node, $direction, $testid) {

  $result = array();
  $tests = db_query('SELECT * FROM {dquarks_direction_tests} WHERE nid = :nid AND cid = :cid AND testid = :testid', array(
    ':nid' => $direction['nid'],
    ':cid' => $direction['cid'],
    ':testid' => $testid)
  );
  foreach ($tests as $test) {
    $result[] = $test;
  }
  return $result[0];
}

/**
 * Delete information from dquarks_direction_tests table after update.
 *
 * @param array $component
 *   An array of information describing the component
 */
function _dquarks_after_update_direction($component) {
  if (isset($component['add']['tests']) && is_array($component['add']['tests'])) {
    foreach ($component['add']['tests'] as $test) {
      db_update('dquarks_direction_tests')
              ->fields(array(
                'test_name' => $test['test_name'],
                'tested_field' => $test['tested_field'],
                'test_type' => $test['test_type'],
                'weight' => $test['weight'],
                'testvalue' => $test['testvalue'],
                'next_jump' => $test['next_jump'],
              ))
              ->condition('nid', $component['nid'])
              ->condition('cid', $component['cid'])
              ->condition('testid', $test['testid'])
              ->execute();
    }
  }
}

/**
 * Delete information from dquarks_direction_tests table after delete.
 *
 * @param array $component
 *   An array of information describing the component
 */
function _dquarks_after_delete_direction($component) {
  db_lock_table('dquarks_direction_tests');
  db_delete('dquarks_direction_tests')
          ->condition('nid', $component['nid'])
          ->condition('cid', $component['cid'])
          ->execute();
  db_unlock_tables('dquarks_direction_tests');
}

/**
 * Add rule to the direction component.
 *
 * @param array $form_state
 *   An array of form items
 * @param object $node
 *   An object node
 * @param array $component
 *   An array of information describing the component
 */
function _dquarks_addrule_direction(&$form_state, $node, $component) {

  $result = db_select('dquarks_direction_tests')
          ->condition('nid', $component['nid'])
          ->condition('cid', $component['cid']);
  $result->addExpression('MAX(testid)', 'testid');
  $testid = $result->execute()->fetchField();
  $testid++;


  db_insert('dquarks_direction_tests')
          ->fields(array(
            'nid' => $node->nid,
            'cid' => $component['cid'],
            'testid' => (int) $testid,
            'test_name' => (string) urldecode($form_state['values']['add']['test_name']),
            'tested_field' => (int) $form_state['values']['add']['tested_field'],
            'test_type' => (int) $form_state['values']['add']['test_type'],
            'weight' => (int) $form_state['values']['add']['weight'],
            'testvalue' => (string) urldecode($form_state['values']['add']['testvalue']),
            'next_jump' => (int) $form_state['values']['add']['next_jump'],
          ))
          ->execute();


  drupal_set_message(t("Added rule"));
  drupal_goto(array('node/' . $node->nid . '/dquarks/components/' . $component['cid'], array(
      'query' => array(drupal_get_destination())))
  );
}

/**
 * Display a warning message if the added rule is empty.
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form items.
 */
function _dquarks_edit_validate_direction($form, &$form_state) {
  if ($form_state['values']['op'] == t('Add Rule')) {
    if (empty($form_state['values']['add']['test_name'])) {
      form_set_error('choices', t('You do not have permission to add an empty rule'));
    }
  }
}
