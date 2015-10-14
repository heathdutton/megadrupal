<?php
/**
 * @file
 * bean_entity_view.inc
 *
 * Entity View Bean functionality.
 */

class EntityViewBean extends BeanPlugin {
  /**
   * Type of entity this bean will display.
   */
  public $entity_type = '';

  /**
   * View mode to use during display.
   */
  public $entity_view_mode = '';

  /**
   * Entity ID to display. NULL = entity being viewed.
   */
  public $entity_id = '';

  /**
   * Entity ID position in the path. Defaults to 1.
   */
  public $entity_id_position = 1;

  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'entity_type' => '',
      'entity_view_mode' => '',
      'entity_id' => '',
      'entity_id_position' => 1,
    );
    $values += parent::values();

    return $values;
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {

    $entities = entity_get_info();
    $entity_options = array();
    $view_modes = array();

    if (!isset($form_state['entity_view_modes'])) {
      foreach ($entities as $entity_type => $entity) {
        $entity_options[$entity_type] = $entity['label'];
        foreach ($entity['view modes'] as $name => $view_mode) {
          $view_modes[$entity_type][$name] = $view_mode['label'];
        }
      }
      $form_state['entity_view_modes'] = $view_modes;
      $form_state['entity_options'] = $entity_options;
    }
    else {
      $view_modes = $form_state['entity_view_modes'];
      $entity_options = $form_state['entity_options'];
    }

    $entity_type_selected = isset($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : $bean->entity_type;

    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#empty_option' => t('- Select an entity type-'),
      '#default_value' => $entity_type_selected,
      '#options' => $entity_options,
      '#required' => TRUE,
      '#description' => t('The type of entity to display.'),
      '#ajax' => array(
        'callback' => 'bean_entity_view_view_modes_callback',
        'wrapper' => 'entity-view-mode-select',
      ),
    );

    $form['entity_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('Entity view mode'),
      '#default_value' => isset($bean->entity_view_mode) ?
        $bean->entity_view_mode : NULL,
      '#options' => isset($view_modes[$entity_type_selected]) ? $view_modes[$entity_type_selected] : array(),
      '#required' => TRUE,
      '#prefix' => '<div id="entity-view-mode-select">',
      '#suffix' => '</div>',
      '#description' => t('The view mode to use on the entity.'),
    );

    if (empty($form_state['values']['entity_type'])) {
      $form['entity_view_mode']['#disabled'] = TRUE;
      $form['entity_view_mode']['#description'] = t('You must select and entity type before selecting a view mode.');
    }

    $form['entity_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Entity ID to display'),
      '#default_value' => isset($bean->entity_id) ?
        $bean->entity_id : NULL,
      '#description' => t('Leave empty to use the entity currently being viewed.'),
    );

    $form['entity_id_position'] = array(
      '#type' => 'textfield',
      '#title' => t('Position of entity ID in the path'),
      '#default_value' => isset($bean->entity_id) ?
          $bean->entity_id_position : 1,
      '#description' => t('The position in the path of the entity ID that is being loaded.'),
      '#states' => array(
        'visible' => array(
          ':input[name="entity_id"]' => array('value' => ''),
        ),
        'required' => array(
          ':input[name="entity_id"]' => array('value' => ''),
        ),
      ),
    );


    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $content = array();

    if ($bean->entity_id) {
      // ID provided, load this entity.
      $entity = entity_load_single($bean->entity_type, $bean->entity_id);
    }
    else {
      // Get the current entity of $bean->entity_type in URL position $bean->entity_id_position.
      $entity = menu_get_object($bean->entity_type, $bean->entity_id_position);
    }

    if ($entity) {
      $entity_id = entity_id($bean->entity_type, $entity);
      // Get the render array for this entity.
      $entities_view = entity_view($bean->entity_type, array($entity), $bean->entity_view_mode, $langcode, FALSE);
      // Reduce returned array to just the entity.
      $entity_view = $entities_view[$bean->entity_type][$entity_id];
      if ($entity_view) {
        // Filter out returned values beginning with '#' and ignore links
        // (returned even if no field data. This is done so we can hide output
        // when view mode has no data. May not take into account extra fields.
        $fields = array_filter(array_keys($entity_view), function($a) { return (substr($a, 0, 1) !== '#' && $a !== 'links'); });
        // We have at least one field populated, so return the render array.
        if ($fields) {
          $content[] = $entity_view;
        }
      }
    }
    return $content;
  }

}

/*
 * Entity view modes callback.
 */
function bean_entity_view_view_modes_callback($form, $form_state) {
//  $form_state['rebuild'] = TRUE;
  return $form['entity_view_mode'];
 }
