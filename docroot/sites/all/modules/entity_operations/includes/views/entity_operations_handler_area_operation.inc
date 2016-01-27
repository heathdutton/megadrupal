<?php

/**
 * @file
 * Definition of entity_operations_handler_area_operation.
 */

/**
 * Views area handler for outputting an entity operation.
 *
 * @ingroup views_area_handlers
 */
class entity_operations_handler_area_operation extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();

    $options['entity_type'] = array('default' => NULL);
    $options['operation_name'] = array('default' => NULL);
    $options['entity_id'] = array('default' => NULL);
    $options['tokenize'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $entity_type_options = array();
    $entity_type_options[''] = t('- Select a value -');
    foreach (entity_get_info() as $entity_type => $entity_info) {
      $entity_type_options[$entity_type] = $entity_info['label'];
    }
    asort($entity_type_options);

    $entity_type = $this->options['entity_type'];

    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#options' => $entity_type_options,
      '#description' => t('Choose the entity type you want to display in the area.'),
      '#default_value' => $entity_type,
      '#ajax' => array(
        'path' => views_ui_build_form_url($form_state),
      ),
      '#submit' => array('views_ui_config_item_form_submit_temporary'),
      '#executes_submit_callback' => TRUE,
    );

    if ($entity_type) {
      $entity_operations_info = entity_operations_get_operation_info($entity_type);
      $options = array();
      $options[''] = t('- Select a value -');
      foreach ($entity_operations_info as $operation_name => $operation) {
        // Get the handler and the operation info.
        $handler_class = $operation['handler'];
        $operation_handler = new $handler_class($entity_type, $operation_name);
        $operation_info = $operation_handler->operationInfo();

        $options[$operation_name] = $operation_info['label'] . ' (' . $operation_name . ')';
      }

      $form['operation_name'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Entity operation'),
        '#default_value' => $this->options['operation_name'],
      );
    }

    $form['entity_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Entity id'),
      '#description' => t('Choose the entity you want to display in the area.'),
      '#default_value' => $this->options['entity_id'],
    );

    // Token UI copied from views_handler_area_text.
    // Note the TODO there: @TODO: Refactor token handling into a base class.
    $form['tokenize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use replacement tokens from the first row'),
      '#default_value' => $this->options['tokenize'],
    );

    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }

    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    if (!empty($options)) {
      $output = '<p>' . t('The following tokens are available. If you would like to have the characters \'[\' and \']\' please use the html entity codes \'%5B\' or  \'%5D\' or they will get replaced with empty space.' . '</p>');
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . check_plain($value);
          }
          $output .= theme('item_list',
            array(
              'items' => $items,
              'type' => $type
            ));
        }
      }

      $form['token_help'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#value' => $output,
        '#id' => 'edit-options-token-help',
        '#dependency' => array(
          'edit-options-tokenize' => array(1),
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
    }
  }

  function render($empty = FALSE) {
    $entity_id = $this->options['entity_id'];

    if ($this->options['tokenize']) {
      // Replace tokens from the first row.
      $entity_id = $this->view->style_plugin->tokenize_value($entity_id, 0);
    }

    if (!is_numeric($entity_id)) {
      return t("ERROR: no valid entity ID given or obtained from tokens.");
    }

    $entity = entity_load_single($this->options['entity_type'], $entity_id);

    $build = entity_operations_get_operation($this->options['entity_type'], $entity, $this->options['operation_name']);

    // Views expects a string of output, not a render array.
    return drupal_render($build);
  }

}
