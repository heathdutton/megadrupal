<?php

/**
 * @file
 * TODO.
 */

/**
 * Form builder.
 */
function gcc_context_admin($form, $form_state) {

  $form = array();

  $form['explainations'] = array(

    '#markup' => t(
      '<p>The context detection is done in two steps.<br />
      This first step is to determine the current entity based on the current path.<br />
      The second step is to determine the current group context based on the current entity</p>'
    ),
  );

  $form['options'] = array(

    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $form['options']['history'] = array(

    '#type' => 'checkbox',
    '#title' => t('Use historical data to determine the current context.'),
    '#description' => t('When a page corresponds to multiple contexts, we can use the previously visited pages to choose the correct one in the list.'),
    '#default_value' => variable_get('gcc_context_history', FALSE),
  );

  $form['entity'] = array(

    '#type' => 'fieldset',
    '#title' => t('Entity Detectors'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $form['context'] = array(

    '#type' => 'fieldset',
    '#title' => t('Context Detectors'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $header = array(

    t('Name'),
    t('Description'),
    t('Enabled'),
    t('Weight'),
  );

  $entity_detectors = gcc_context_get_entity_detectors();
  $context_detectors = gcc_context_get_context_detectors();

  foreach (array(

    'entity' => $entity_detectors,
    'context' => $context_detectors,

  ) as $type => $detectors) {

    $form[$type]['detectors'] = array(

      '#theme' => 'table',
      '#header' => $header,
      '#empty' => t('No @type detectors is currently available', array('@type' => $type)),
      '#attributes' => array(
        'id' => 'gcc-context-' . $type . '-detectors-table',
      ),
    );

    $rows = array();

    foreach ($detectors as $key => $detector) {

      $form[$type]['detectors'][$key]['enabled'] = array(

        '#type' => 'checkbox',
        '#title' => t('Enable'),
        '#title_display' => 'invisible',
        '#default_value' => $detector['enabled'],
      );

      $form[$type]['detectors'][$key]['weight'] = array(

        '#type' => 'weight',
        '#title' => t('Weight'),
        '#title_display' => 'invisible',
        '#default_value' => $detector['weight'],
        '#delta' => 50,
        '#attributes' => array(
          'class' => array('detector-weight-' . $type),
        ),
      );

      $row = array();
      $row[] = $detector['title'];
      $row[] = $detector['description'];
      $row[] = array(

        'data' => &$form[$type]['detectors'][$key]['enabled'],
      );
      $row[] = array(

        'data' => &$form[$type]['detectors'][$key]['weight'],
      );

      $rows[] = array(

        'data' => $row,
        'class' => array('draggable'),
      );
    }

    $form[$type]['detectors']['#rows'] = $rows;

    drupal_add_tabledrag('gcc-context-' . $type . '-detectors-table', 'order', 'sibling', 'detector-weight-' . $type);
  }

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler.
 */
function gcc_context_admin_submit($form, &$form_state) {

  variable_set('gcc_context_entity_detectors', $form_state['values']['entity']['detectors']);
  variable_set('gcc_context_context_detectors', $form_state['values']['context']['detectors']);
  variable_set('gcc_context_history', $form_state['values']['options']['history']);
}
