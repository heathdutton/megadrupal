<?php

/**
 * @file
 * Administration pages and forms for configuring components for a Publish to Apple News article type.
 */

/**
 * Admin page for configuring the components for an article type.
 */
function publish_to_apple_news_type_configure_page($article_type) {
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  drupal_add_css($module_directory . '/css/components-ui.css');
  drupal_add_library('system', 'ui.sortable');
  drupal_add_js($module_directory . '/js/sortable-components.js');
  // jQuery Modal - https://github.com/kylefox/jquery-modal
  drupal_add_js($module_directory . '/js/modal/jquery.modal.js');
  drupal_add_css($module_directory . '/js/modal/jquery.modal.css');
  
  //close modal when replaceent pattern has been selected:
  $close_modal_js = "jQuery('#ui-replacement-patterns .token-key a').click(function() {jQuery('.close-modal').click();}); ";
  //focus on the input when opening the replacement patternt modal:
  $replacement_focus_js = "jQuery('a[href=\"#ui-replacement-patterns\"]').click(function() {jQuery(this).parent().parent().find('input[type=\"text\"]').focus();}); ";
  
  drupal_add_js('jQuery(document).ready(function () {' . $close_modal_js . $replacement_focus_js . '});', 'inline');

  $page = array(
    'token_tree' => array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
      '#value' => theme('token_tree', array('token_types' => array('node'), 'recursion_limit' => 2)),
      '#prefix' => '<div id="ui-replacement-patterns" style="display: none">',
      '#suffix' => '</div>',
    ),
    'components' => array(
      '#prefix' => '<div id="components-none-0" class="components-container" data-type="none">',
      '#suffix' => '</div>',
    ),
    'add_component_form' => drupal_get_form('publish_to_apple_news_component_add_form', $article_type),
  );

  // Get top level components
  $components = publish_to_apple_news_get_components($article_type->tid);

  foreach ($components as $component) {
    $page['components'][$component->weight] = publish_to_apple_news_component_section($article_type, $component->entity_type, $component, 0, '', $component->weight, TRUE);
  }

  return $page;
}

/**
 * Get all child components of all entity types and sort them by weight.
 *
 * @param $tid
 *  The article type tid to filter components by.
 * @param $component_type
 *  The entity machine name of the parent component.
 * @param $parent_cid
 *  The parent cid to get children for.
 */
function publish_to_apple_news_get_components($tid, $component_type = '', $parent_cid = 0) {
  $component_types = publish_to_apple_news_component_types();
  $entity_types = array_keys($component_types);
  $components = array();

  foreach ($entity_types as $entity_type) {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', $entity_type)
      ->propertyCondition('tid', $tid)
      ->propertyCondition('parent_cid', $parent_cid)
      ->propertyCondition('parent_type', $component_type)
      ->propertyOrderBy('weight', 'ASC')
      ->execute();

    if (isset($result[$entity_type])) {
      $cids = array_keys($result[$entity_type]);
      $type_components = entity_load($entity_type, $cids);

      foreach ($type_components as $component) {
        $component->entity_type = $entity_type;
        $components[$component->weight] = $component;
      }
    }
  }

  ksort($components);

  return $components;
}


/**
 * Form that allows you to add a component to a given parent.
 *
 * @param $article_type
 *  The fully-loaded article type entity.
 * @param $parent_cid
 *  The cid of the component that child components will be added to.
 * @param $parent_type
 *  The entity machine name of the parent component.
 */
function publish_to_apple_news_component_add_form($form, &$form_state, $article_type, $parent_cid = 0, $parent_type = 'none') {
  $component_types = publish_to_apple_news_component_types();

  // $article_type is needed to instantiate the component's class (to generate form)
  $form_state['article_type'] = $article_type;

  $form_state['parent_cid'] = $parent_cid;
  $form_state['parent_type'] = $parent_type == 'none' ? '' : $parent_type;

  // Generate a list of components, keyed by entity type
  $component_options = array(
    '' => 'Select...',
  );
  foreach ($component_types as $entity_machine_name => $entity_class_name) {
    // Some components can only exist at the top level
    $top_level_only = array(
      'publish_to_apple_news_ad_component',
      'publish_to_apple_news_section_component'
    );
    
    if ($parent_cid && in_array($entity_machine_name, $top_level_only))
      continue;
    
    $class = new $entity_class_name($article_type);
    $component_options[$entity_machine_name] = $class->name;
  }

  $form['add_component'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Add new component'),
    'component_type' => array(
      '#type' => 'select',
      '#options' => $component_options,
      '#title' => t('Component type'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#ajax' => array(
        'callback' => 'publish_to_apple_news_component_add_callback',
        'wrapper' => 'components-' . $parent_type . '-' . $parent_cid,
        'method' => 'append',
      ),
    ),
  );

  return $form;
}

/**
 * Ajax callback for adding a new component.
 */
function publish_to_apple_news_component_add_callback($form, $form_state) {
  if (!empty($form_state['values']['component_type'])) {
    $component_type = $form_state['values']['component_type'];

    // Increment weight
    $weight = 1;
    $components = publish_to_apple_news_get_components($form_state['article_type']->tid, $form_state['parent_type'], $form_state['parent_cid']);

    if (!empty($components)) {
      $last_component = array_pop($components);
      $weight = $last_component->weight + 1;
    }

    return publish_to_apple_news_component_section($form_state['article_type'], $component_type, NULL, $form_state['parent_cid'], $form_state['parent_type'], $weight);
  }
}

/**
 * Generates a ui render array for an entire component.
 *
 * @param $article_type
 *  The article type of the component.
 * @param $component_type
 *  The entity type of the component.
 * @param $component
 *  The fully-loaded component for editing.
 * @param $parent_cid
 *  The cid of the parent component.
 * @param $parent_type
 *  The entity machine name of the parent component.
 * @param $weight
 *  For a new entity - the weight that the entity will be created with.
 * @param $hide_add_button
 *  Whether to hide the "Add components" button in the entity form (if applicable - header, section, chapter).
 *
 * @return array
 *  A render array representing the entire component and its children.
 */
function publish_to_apple_news_component_section($article_type, $component_type, $component = NULL, $parent_cid = 0, $parent_type = '', $weight = 1, $hide_add_button = FALSE) {
  $component_types = publish_to_apple_news_component_types();
  $class = new $component_types[$component_type]($article_type, $component);

  $component_form = drupal_get_form('publish_to_apple_news_component_entity_form', $article_type, $component_type, $parent_cid, $parent_type, $component, $weight, $hide_add_button);
  $render = array(
    'form' => $component_form,
  );

  if (!empty($component) && $class->acceptsComponents) {
    $render['components'] = array(
      '#prefix' => '<h3>' . t('@name Components', array('@name' => $component->name)) .
        '</h3><div id="components-' . $component_type . '-' . $component->cid .
          '" class="components-container" data-type="' . $component_type . '">',
      '#suffix' => '</div>',
    );

    // Get child components
    $child_components = publish_to_apple_news_get_components($article_type->tid, $component_type, $component->cid);
    foreach ($child_components as $child_component) {
      $render['components'][$child_component->weight] = publish_to_apple_news_component_section($article_type, $child_component->entity_type, $child_component, $component->cid, $component_type, $child_component->weight, TRUE);
    }

    $render['component_add_form'] = drupal_get_form('publish_to_apple_news_component_add_form', $article_type, $component->cid, $component_type);
  }

  $render = array(
    'wrapper' => array(
      '#theme' => 'fieldset',
      '#title' => !empty($component) ? $component->name . ' (' . $class->name . ')' : $class->name,
      '#prefix' => '<div id="components-' . $component_type . '-' . $parent_cid . '-' . $weight . '">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('collapsible')),
      '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
      '#children' => drupal_render($render),
    ),
  );
  
  // Render component fieldset collapsed if this is not a new (not yet saved) component
  if (!empty($component) && $hide_add_button) {
    $render['wrapper']['#attributes']['class'][] = 'collapsed';
  }

  return $render;
}

/**
 * Form handler that instantiates a component's class and returns its form.
 *
 * @param $article_type
 *  Needed to instantiate the component's class.
 * @param $component_type
 *  The entity type of the component.
 * @param $parent_cid
 *  The cid of this component's parent.
 * @param $parent_type
 *  The entity machine name of this component's parent.
 * @param $entity
 *  The component entity to edit, if applicable. Otherwhise, one will be created via entity_create.
 * @param $weight
 *  For a new entity - the weight that the entity will be created with.
 * @param $hide_add_button
 *  Whether to hide the "Add components" button in the entity form (if applicable - header, section, chapter).
 *
 * @return array
 *  An array consisting of a form and form_state with important variables loaded - see generateForm() in component_base.inc.
 */
function publish_to_apple_news_component_entity_form($form, &$form_state, $article_type, $component_type, $parent_cid = 0, $parent_type = '', $entity = NULL, $weight = 1, $hide_add_button = FALSE) {
  $component_types = publish_to_apple_news_component_types();

  if (!isset($entity)) {
    $entity = entity_create($component_type, array(
      'tid' => $article_type->tid,
      'parent_cid' => $parent_cid,
      'parent_type' => $parent_type,
      'weight' => $weight,
    ));
  }

  if (!empty($form_state['rebuild'])) {
    $entity = entity_load_single($component_type, $form_state['storage']['cid']);
  }

  if ($hide_add_button) {
    $form_state['storage']['hide_add_button'] = TRUE;
  }

  $class = new $component_types[$component_type]($article_type, $entity);
  
  $form_state['component_parent_cid'] = $parent_cid;
  $form_state['component_weight'] = $weight;
  
  // Auto-generate identifier
  if (!isset($entity->cid)) {
    $entity->identifier = $class->role . '-' . uniqid();
    
    // Set default properties
    module_load_include('inc', 'publish_to_apple_news', 'includes/defaults');
    $defaults = publish_to_apple_news_get_component_defaults();
    if (!empty($defaults['component_properties'][$class->role])) {
      // Attempt to find an existing component with the same role and use those properties
      // This only applies to component properties specified in includes/defaults.inc
      $componentQuery = new EntityFieldQuery();
      $componentResult = $componentQuery->entityCondition('entity_type', $class->entityMachineName)
        ->propertyCondition('tid', $article_type->tid)
        ->propertyOrderBy('weight', 'ASC')
        ->range(0, 1)
        ->execute();
      if (isset($componentResult[$class->entityMachineName])) {
        $ids = array_keys($componentResult[$class->entityMachineName]);
        $existing_component = entity_load_single($class->entityMachineName, $ids[0]);
      }
      
      foreach ($defaults['component_properties'][$class->role] as $property => $value) {

        if (!empty($existing_component->{$property})) {
          // Use property value from existing component
          $entity->{$property} = $existing_component->{$property};
        } else {
          if ($class->role == 'body') {
            // Body is a special case. Image column start & span properties aren't found in the JSON for the body component.
            // Instead, inline images are taken out of the body, added as photo components, and anchored appropriately.
            // So, let's use the layout from "default_photo_layout"
            $photoLayoutQuery = new EntityFieldQuery();
            $photoLayoutResult = $photoLayoutQuery->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
              ->propertyCondition('tid', $article_type->tid)
              ->propertyCondition('identifier', 'default_photo_layout')
              ->execute();
            if (isset($photoLayoutResult['publish_to_apple_news_component_layout'])) {
              $ids = array_keys($photoLayoutResult['publish_to_apple_news_component_layout']);
              $photo_layout = entity_load_single('publish_to_apple_news_component_layout', $ids[0]);
              if ($property == 'image_column_start') {
                $entity->image_column_start = $photo_layout->column_start;
              } elseif ($property == 'image_column_span') {
                $entity->image_column_span = $photo_layout->column_span;
              }
            } else {
              $entity->{$property} = $value;
            }
          } else {
            // Use defaults specified in includes/defaults.inc
            $entity->{$property} = $value;
          }
        }
      }
    }
    
    // Select a default layout
    $default_layout_identifier = 'default_' . $class->role . '_layout';
    $layoutQuery = new EntityFieldQuery();
    $layoutResult = $layoutQuery->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
      ->propertyCondition('tid', $article_type->tid)
      ->propertyCondition('identifier', $default_layout_identifier)
      ->execute();
    if (isset($layoutResult['publish_to_apple_news_component_layout'])) {
      $ids = array_keys($layoutResult['publish_to_apple_news_component_layout']);
      $entity->lid = $ids[0];
    }
    
    // Select a default style
    $default_style_identifier = 'default_' . $class->role . '_style';
    $styleQuery = new EntityFieldQuery();
    $styleResult = $styleQuery->entityCondition('entity_type', 'publish_to_apple_news_component_style')
      ->propertyCondition('tid', $article_type->tid)
      ->propertyCondition('identifier', $default_style_identifier)
      ->execute();
    if (isset($styleResult['publish_to_apple_news_component_style'])) {
      $ids = array_keys($styleResult['publish_to_apple_news_component_style']);
      $entity->sid = $ids[0];
    }
    
    // Select a default textstyle
    $default_text_style_identifier = 'default_' . $class->role . '_text_style';
    $textStyleQuery = new EntityFieldQuery();
    $textStyleResult = $textStyleQuery->entityCondition('entity_type', 'publish_to_apple_news_component_text_style')
      ->propertyCondition('tid', $article_type->tid)
      ->propertyCondition('identifier', $default_text_style_identifier)
      ->execute();
    if (isset($textStyleResult['publish_to_apple_news_component_text_style'])) {
      $ids = array_keys($textStyleResult['publish_to_apple_news_component_text_style']);
      $entity->tsid = $ids[0];
    }
  }
  
  $form_and_state = $class->generateForm($form, $form_state);
  $form_state = $form_and_state['form_state'];

  return $form_and_state['form'];
}

/**
 * Submit handler for adding/editing a component.
 */
function publish_to_apple_news_component_entity_form_submit($form, &$form_state) {
  $errors = form_get_errors();

  if (empty($errors)) {
    $component_types = publish_to_apple_news_component_types();
    $component = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);
    
   $class = new $component_types[$form_state['entity_type']]($form_state['article_type'], $component);
   $class->saveExtraProperties($component, $form_state);
    
    entity_save($form_state['entity_type'], $component);

    drupal_set_message(t('Your component has been saved.'));

    $form_state['storage']['cid'] = $component->cid;
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Validation when adding/editing a component.
 */
function publish_to_apple_news_component_entity_form_validate($form, &$form_state) {
  // Validate no other component has the same identifier
  if (!empty($form_state['values']['identifier'])) {
    $component_types = publish_to_apple_news_component_types();
    $entity_types = array_keys($component_types);
  
    foreach ($entity_types as $entity_type) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', $entity_type)
        ->propertyCondition('identifier', $form_state['values']['identifier'])
        ->propertyCondition('tid', $form_state[$form_state['entity_type']]->tid);
  
      if (!empty($form_state[$form_state['entity_type']]->cid) && $form_state['entity_type'] == $entity_type) {
        $query->propertyCondition('cid', $form_state[$form_state['entity_type']]->cid, '!=');
      }
  
      $result = $query->execute();
  
      if (!empty($result[$entity_type])) {
        form_set_error('identifier', t('A component with this identifier already exists.'));
      }
    }
  }
}

/**
 * Ajax submit handler for adding/editing a component.
 */
function publish_to_apple_news_component_entity_form_callback($form, $form_state) {
  return $form;
}

/**
 * Ajax submit handler that refreshes a components form, passing the fully-loaded component entity so that children components are shown.
 */
function publish_to_apple_news_component_form_add_callback($form, $form_state) {
  $component = $form_state[$form_state['entity_type']];
  
  return publish_to_apple_news_component_section($form_state['article_type'], $form_state['entity_type'], $component, $component->parent_cid, $component->parent_type, $component->weight);
}

/**
 * Confirm form for deleting a component.
 *
 * @param $entity_type
 *  The component's entity type.
 * @param $cid
 *  The unique identifier of the component to delete
 */
function publish_to_apple_news_component_delete_form($form, &$form_state, $entity_type, $cid) {
  $component = entity_load_single($entity_type, $cid);
  
  if (empty($component)) {
    return drupal_not_found();
  }
  
  $form_state['component'] = $component;
  
  return confirm_form(
    $form,
    t('Are you sure you want to delete "@name"?', array('@name' => $component->name)),
    'admin/structure/publish-to-apple-news/types/' . $component->tid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting a component.
 */
function publish_to_apple_news_component_delete_form_submit(&$form, &$form_state) {
  $component = $form_state['component'];
  $cid = $component->cid;
  $tid = $component->tid;
  
  $component->delete();
  
  drupal_set_message(t('Your component has been deleted.'));
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $tid;
}

/**
 * Ajax callback that saves component weights.
 *
 * @param $tid
 *  The tid of the article type.
 * @param $component_type
 *  The entity machine name of the component type whose child components are being reordered.
 * @param $parent_cid
 *  The cid of the parent component to get child components.
 * @param $serialized
 *  Serialized string of positions returned by jQuery Sortable.
 */
function publish_to_apple_news_save_weights($tid, $component_type, $parent_cid, $serialized) {
  $weights = explode('-sort=', $serialized);
  $weights[0] = str_replace('sort=', '', $weights[0]);
  $component_type = $component_type == 'none' ? '' : $component_type;
  
  // Get components, keyed by weight
  $components = publish_to_apple_news_get_components($tid, $component_type, $parent_cid);
  
  // Save new weights
  $i = 1;
  foreach ($weights as $weight) {
    if (!empty($components[$weight])) {
      $components[$weight]->weight = $i;
      $components[$weight]->save();
      
      $i++;
    }
  }
}
