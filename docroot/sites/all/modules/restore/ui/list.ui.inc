<?php
/**
 * @file
 * List restore scripts.
 */

/**
 * Form callback for the script list.
 */
function restore_ui_list($form, &$form_state) {
  $form = array();

  $scripts = restore_scripts();
  $form['scripts'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'details',
    '#tree' => TRUE,
  );

  if (count($scripts)) {
    $groups = array();

    foreach ($scripts as $id => $script) {
      $group = array_search($script->group(), $groups);
      if ($group === FALSE) {
        $group = count($groups);
        $groups[] = $script->group();

        $form['scripts'][$group] = array(
          '#type' => 'fieldset',
          '#title' => $groups[$group],
          '#tree' => TRUE,
        );

        $form['scripts'][$group]['select_all'] = array(
          '#type' => 'checkbox',
          '#attributes' => array(
            'class' => array('select-all'),
          ),
        );
      }

      $form['scripts'][$group][$id] = array('#tree' => TRUE);
      $form['scripts'][$group][$id]['select'] = array(
        '#type' => 'checkbox',
        '#attributes' => array(
          'class' => array('select-script'),
        ),
      );

      $form['scripts'][$group][$id]['title'] = array(
        '#type' => 'markup',
        '#markup' => l($script->title(), RESTORE_MENU_PATH . "/script/{$id}"),
      );

      $form['scripts'][$group][$id]['description'] = array(
        '#type' => 'markup',
        '#markup' => $script->description(),
      );

      $form['scripts'][$group][$id]['storage'] = array(
        '#type' => 'markup',
        '#markup' => $script->storage(),
      );

      $form['scripts'][$group][$id]['status'] = _restore_status($script);
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['revert'] = array(
      '#type' => 'submit',
      '#value' => t('Revert'),
      '#submit' => array('restore_ui_list_revert'),
    );

    _restore_form_legend($form['actions']);
  }
  else {
    $form['no_scripts'] = array(
      '#type' => 'markup',
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('Failed to find any restore scripts. Add one by either using the API or the UI module.'),
    );
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'restore') . '/assets/css/restore.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'restore') . '/assets/js/restore.js';

  return $form;
}

/**
 * Revert button callback.
 */
function restore_ui_list_revert($form, &$form_state) {
  if (count($form_state['values']['scripts'])) {
    foreach ($form_state['values']['scripts'] as $values) {
      if (!is_array($values)) {
        continue;
      }

      foreach ($values as $name => $script_values) {
        if (!is_array($script_values) || empty($script_values['select'])) {
          continue;
        }

        $script = restore_load($name);
        if ($script) {
          $script->restore();
          drupal_set_message(t('Restored the script @name.', array(
            '@name' => $script->title(),
          )));
        }
        else {
          drupal_set_message(t('Failed to load the restore script @name.', array(
            '@name' => $name,
          )), 'error');
        }
      }
    }
  }
}

/**
 * Theme callback for the scripts list form.
 */
function theme_restore_ui_list($variables) {
  $form = $variables['form'];
  $output = '';

  $elements = element_children($form['scripts']);
  if (count($elements)) {
    foreach ($elements as $group) {
      $rows = array();

      $header = array(
        array(
          'data' => drupal_render($form['scripts'][$group]['select_all']),
          'width' => 25,
        ),
        array(
          'data' => t('Title'),
          'width' => 250,
        ),
        array(
          'data' => t('Description'),
        ),
        array(
          'data' => t('Storage'),
          'width' => 100,
        ),
        array(
          'data' => t('Status'),
          'width' => 150,
        ),
      );

      foreach (element_children($form['scripts'][$group]) as $id) {
        if ($id == 'select_all') {
          continue;
        }

        $rows[] = array(
          drupal_render($form['scripts'][$group][$id]['select']),
          drupal_render($form['scripts'][$group][$id]['title']),
          drupal_render($form['scripts'][$group][$id]['description']),
          drupal_render($form['scripts'][$group][$id]['storage']),
          drupal_render($form['scripts'][$group][$id]['status']),
        );
      }

      $form['scripts'][$group]['#value'] = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'empty' => t('There are no restore scripts available at the moment.'),
      ));
    }
  }

  $output .= drupal_render_children($form);

  return $output;
}
