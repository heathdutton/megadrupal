<?php
/**
 * @file
 * Restore script UI view.
 */

/**
 * Form callback.
 */
function restore_ui_script_view($form, &$form_state, $script) {
  $form = array();

  $form['#script'] = $script;
  $conflicts = $script->conflicts();
  if ($conflicts) {
    drupal_set_message(t('Warning: This script has identified some conflicts.'), 'warning');
  }

  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'details',
  );

  $form['tabs']['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Script details'),
    '#weight' => -10,
  );

  $form['tabs']['details']['title'] = array(
    '#type' => 'item',
    '#title' => t('Title'),
    '#markup' => $script->title(),
  );

  $form['tabs']['details']['description'] = array(
    '#type' => 'item',
    '#title' => t('Descripton'),
    '#markup' => $script->description(),
  );

  $form['tabs']['details']['group'] = array(
    '#type' => 'item',
    '#title' => t('Group'),
    '#markup' => $script->group(),
  );

  $status = _restore_status($script);
  $form['tabs']['details']['status'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#markup' => drupal_render($status),
  );

  $form['tabs']['operations'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Operations'),
  );

  $form['tabs']['operations']['__all'] = array(
    '#type' => 'checkbox',
    '#attributes' => array(
      'class' => array('select-all'),
    ),
  );

  foreach ($script as $idx => $operation) {
    $form['tabs']['operations'][$idx + 1] = array('#tree' => TRUE);
    $form['tabs']['operations'][$idx + 1]['restore'] = array('#type' => 'checkbox');
    $form['tabs']['operations'][$idx + 1]['__title'] = array(
      '#type' => 'markup',
      '#markup' => t('Operation: @title - @description', array(
        '@title' => $operation ? $operation->title() : t('Unknown'),
        '@description' => $operation ? $operation->description() : t('This is an unknown operation.'),
      )),
    );

    $form['tabs']['operations'][$idx + 1]['status'] = _restore_status($operation);

    $form['tabs']['operations'][$idx + 1]['details'] = array(
      '#type' => 'markup',
      '#markup' => implode(' ', $operation ? $operation->summary() : array()),
    );
  }

  $form['tabs']['conflicts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Conflicts'),
  );

  if (empty($conflicts)) {
    $form['tabs']['conflicts']['no_conflicts'] = array(
      '#type' => 'markup',
      '#markup' => t('No conflicts have been found.'),
    );
  }
  else {
    foreach ($conflicts as $conflict_script_name => $conflict_script) {
      $form['tabs']['conflicts'][$conflict_script_name] = array('#tree' => TRUE);
      $form['tabs']['conflicts'][$conflict_script_name]['title'] = array(
        '#type' => 'markup',
        '#markup' => t('Script: <b>!title</b> - @description', array(
          '!title' => l($conflict_script['script']->title(), RESTORE_MENU_PATH . "/script/{$conflict_script_name}"),
          '@description' => $conflict_script['script']->description(),
        )),
      );

      foreach ($conflict_script['operations'] as $idx => $conflict_operation) {
        $form['tabs']['conflicts'][$conflict_script_name][$idx + 1] = array('#tree' => TRUE);
        $form['tabs']['conflicts'][$conflict_script_name][$idx + 1]['title'] = array(
          '#type' => 'markup',
          '#markup' => t('Operation: <b>@title</b> - @description', array(
            '@title' => $conflict_operation['operation'] ? $conflict_operation['operation']->title() : t('Unknown'),
            '@description' => $conflict_operation['operation'] ? $conflict_operation['operation']->description() : t('This is an unknown operation.'),
          )),
        );

        $form['tabs']['conflicts'][$conflict_script_name][$idx + 1]['total'] = array(
          '#type' => 'markup',
          '#markup' => count($conflict_operation['conflict']),
        );

        $form['tabs']['conflicts'][$conflict_script_name][$idx + 1]['conflicts'] = array(
          '#type' => 'markup',
          '#markup' => t('This operation has conflicts.'),
        );

        if (count($conflict_operation['conflict'])) {
          $ops = array();
          foreach ($conflict_operation['conflict'] as $op) {
            $ops[] = '<span class="restore-status restore-status-conflict" title="' .
                t("The operation '@op' is conflicting.", array('@op' => $op)) .
                '">' . $op . '</span>';
          }

          $form['tabs']['conflicts'][$conflict_script_name][$idx + 1]['conflicts']['#markup'] = implode(' ', $ops);
        }
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['restore'] = array(
    '#type' => 'submit',
    '#value' => t('Restore'),
    '#submit' => array('restore_ui_script_view_restore'),
  );

  _restore_form_legend($form['actions']);

  $form['#attached']['css'][] = drupal_get_path('module', 'restore') . '/assets/css/restore.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'restore') . '/assets/js/restore.js';

  return $form;
}

/**
 * Restore submit callback.
 */
function restore_ui_script_view_restore($form, &$form_state) {
  $script = $form_state['build_info']['args'][0];

  $operations = array();
  foreach ($form_state['values']['operations'] as $op => $values) {
    if ($op == '__all') {
      continue;
    }

    if ($values['restore'] || $form_state['values']['operations']['__all']) {
      $operations[] = $op - 1;
    }
  }

  $script->restore($operations);
  drupal_set_message(t('The restore script @name has been restored.', array(
    '@name' => $script->title(),
  )));
}

/**
 * Form theme callback.
 */
function theme_restore_ui_script_view($variables) {
  $form = $variables['form'];
  $output = '';

  $rows = array();

  foreach (element_children($form['tabs']['operations']) as $op_group) {
    if ($op_group == '__all') {
      continue;
    }

    $rows[] = array(
      'data' => array(
        array(
          'data' => drupal_render($form['tabs']['operations'][$op_group]['restore']),
          'header' => TRUE,
        ),
        array(
          'data' => drupal_render($form['tabs']['operations'][$op_group]['__title']),
          'header' => TRUE,
          'colspan' => 2,
        ),
      ),
      'no_striping' => TRUE,
    );

    $rows[] = array(
      array(
        'data' => drupal_render($form['tabs']['operations'][$op_group]['status']),
        'colspan' => 2,
      ),
      drupal_render($form['tabs']['operations'][$op_group]['details']),
    );
  }

  $form['tabs']['operations']['#value'] = theme('table', array(
    'empty' => t('There are no operations.'),
    'header' => array(
      array(
        'data' => drupal_render($form['tabs']['operations']['__all']),
        'width' => 25,
      ),
      array(
        'data' => t('Status'),
        'width' => 100,
      ),
      t('Operation details'),
    ),
    'rows' => $rows,
  ));

  $form['tabs']['conflicts']['#value'] = '';
  if (!isset($form['tabs']['conflicts']['no_conflicts'])) {
    foreach (element_children($form['tabs']['conflicts']) as $op_script) {
      $rows = array();
      $rows[] = array(
        'data' => array(
          array(
            'data' => drupal_render($form['tabs']['conflicts'][$op_script]['title']),
            'header' => TRUE,
            'colspan' => 2,
          ),
        ),
        'no_striping' => TRUE,
      );

      foreach (element_children($form['tabs']['conflicts'][$op_script]) as $op_operation) {
        if ($op_operation == 'title') {
          continue;
        }

        $rows[] = array(
          array(
            'data' => drupal_render($form['tabs']['conflicts'][$op_script][$op_operation]['title']),
            'header' => TRUE,
          ),
          array(
            'data' => drupal_render($form['tabs']['conflicts'][$op_script][$op_operation]['total']),
            'header' => TRUE,
          ),
        );

        $rows[] = array(
          array(
            'data' => drupal_render($form['tabs']['conflicts'][$op_script][$op_operation]['conflicts']),
            'colspan' => 2,
          ),
        );
      }

      $form['tabs']['conflicts']['#value'] .= theme('table', array(
        'empty' => t('There are no conflicts.'),
        'header' => array(),
        'rows' => $rows,
      )) . '<br />';
    }
  }

  $output .= drupal_render_children($form);
  return $output;
}
