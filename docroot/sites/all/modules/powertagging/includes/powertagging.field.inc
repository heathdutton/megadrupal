<?php

/**
 * @file
 * All hooks required for the powertagging field.
 */

/**
 * Implements hook_field_info().
 */
function powertagging_field_info() {
  return array(
    'powertagging' => array(
      'label' => t('PowerTagging Tags'),
      'settings' => array(
        'powertagging_id' => '',
      ),
      'instance_settings' => array(
        'fields' => array(),
        'project_id' => '',
      ),
      'description' => t('This field stores concepts in the database.'),
      'default_widget' => 'powertagging_ui',
      'default_formatter' => 'powertagging_list',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function powertagging_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $powertagging_configs = powertagging_config_load();
  $powertagging_config_options = array();
  foreach ($powertagging_configs as $powertagging_config) {
    $powertagging_config_options[$powertagging_config->powertagging_id] = $powertagging_config->title;
  }

  $form = array();
  $form['powertagging_id'] = array(
    '#type' => 'select',
    '#title' => t('Select the PowerTagging Configuration'),
    '#options' => $powertagging_config_options,
    '#default_value' => $settings['powertagging_id'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function powertagging_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form = array();

  if (!empty($field['settings']['powertagging_id'])) {
    $powertagging_config = powertagging_config_load($field['settings']['powertagging_id']);
    $entity_fields = field_info_instances($instance['entity_type'], $instance['bundle']);

    $project_options = array();
    if (!empty($powertagging_config->config['projects'])) {
      foreach ($powertagging_config->config['projects'] as $project_id => $project) {
        if (!empty($project['taxonomy_name'])) {
          $project_options[$project_id] = $project['label'];
        }
      }
    }

    $form['powertagging_id'] = array(
      '#type' => 'hidden',
      '#value' => $powertagging_config->powertagging_id,
    );

    $form['project_id'] = array(
      '#type' => 'select',
      '#options' => $project_options,
      '#title' => t('PoolParty Project (tagging taxonomy)'),
      '#default_value' => isset($settings['project_id']) ? $settings['project_id'] : '',
      '#required' => TRUE,
      '#description' => (!empty($project_options) ? t('The project ID from the corresponding taxonomy') : t('Projects have to be configured in the PowerTagging configuration first.')),
    );

    // Check if this content type already has fields with tags.
    $existing_tag_fields = array();
    foreach (field_info_fields() as $field_id => $field) {
      /*
      if ($field['type'] == 'taxonomy_term_reference' && isset($entity_fields[$field_id]) && isset($fields['settings']['allowed_values']) && !empty($fields['settings']['allowed_values']) && $fields['settings']['allowed_values'][0]['vocabulary'] == ) {
      */
      if ($field['type'] == 'taxonomy_term_reference' && isset($entity_fields[$field_id])) {
        $existing_tag_fields[$field_id] = $entity_fields[$field_id]['label'];
      }
    }
    if (!empty($existing_tag_fields)) {
      $form['default_tags_field'] = array(
        '#type' => 'select',
        '#options' => $existing_tag_fields,
        '#title' => t('Use tags of an existing tags-field as default values'),
        '#default_value' => isset($settings['default_tags_field']) ? $settings['default_tags_field'] : '',
        '#empty_value' => '',
      );
    }

    $concepts_per_extraction = isset($settings['concepts_per_extraction']) ? $settings['concepts_per_extraction'] : $powertagging_config->config['concepts_per_extraction'];
    $concepts_threshold = isset($settings['concepts_threshold']) ? $settings['concepts_threshold'] : $powertagging_config->config['concepts_threshold'];
    $freeterms_per_extraction = isset($settings['freeterms_per_extraction']) ? $settings['freeterms_per_extraction'] : $powertagging_config->config['freeterms_per_extraction'];
    $freeterms_threshold = isset($settings['freeterms_threshold']) ? $settings['freeterms_threshold'] : $powertagging_config->config['freeterms_threshold'];
    powertagging_global_limits_form($form, $concepts_per_extraction, $concepts_threshold, $freeterms_per_extraction, $freeterms_threshold, array('instance', 'settings'));

    $options = powertagging_field_list($instance, 'instance_settings_form');
    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields to extract tags from'),
      '#options' => $options,
      '#default_value' => $settings['fields'],
      '#required' => TRUE,
      '#description' => t('Select the fields from which the concepts and free terms are extracted'),
    );
  }
  // Global field settings need to be saved first.
  else {
    $form['project_id'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages error">The global field settings have to be saved before editing the field instance is possible.</div>',
    );
  }

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function powertagging_field_is_empty($item, $field) {
  if (is_string($item) && $item !== '') {
    return FALSE;
  }

  return TRUE;
}


/**
 * Implements hook_field_widget_info().
 */
function powertagging_field_widget_info() {
  return array(
    'powertagging_ui' => array(
      'label' => t('User Interface'),
      'field types' => array('powertagging'),
      'settings' => array('size' => 60),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function powertagging_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'powertagging_ui':
      // Show the legend
      $legend_markup = '<div class="powertagging-legend">';
      $legend_types = array(
        'concept' => t('Concepts from the thesaurus'),
        'freeterm' => t('Free terms'),
        'disabled' => t('Already selected tags'),
      );
      foreach ($legend_types as $type => $label) {
        $legend_markup .= '<div class="powertagging-legend-item"><span id="powertagging-legend-item-colorbox-' . $type . '" class="powertagging-legend-item-colorbox">&nbsp;</span>' . $label . '</div>';
      }
      $legend_markup .= '</div>';
      $element['legend'] = array(
        '#type' => 'item',
        '#markup' => $legend_markup,
      );

      // Show the PowerTagging form field
      $element['powertagging'] = array(
        '#title' => $instance['label'],
        '#type' => 'fieldset',
      );

      $entity = NULL;
      switch ($instance['entity_type']) {
        case 'node':
          if (isset($form_state['node'])) {
            $entity = $form_state['node'];
          }
          break;

        case 'user':
          if (isset($form_state['user'])) {
            $entity = $form_state['user'];
          }
          break;

        case 'taxonomy_term':
          if (isset($form_state['term'])) {
            $entity = $form_state['term'];
          }
          break;
      }

      // Check if the instance was correctly configured.
      if (powertagging_check_field_instance($instance)) {
        $tag_ids = array();
        foreach ($items as $item) {
          if (isset($item['tid'])) {
            $tag_ids[] = $item['tid'];
          }
        }
        $tag_string = implode(',', $tag_ids);

        // Get the default tags if required.
        if (empty($tag_string) && !empty($instance['settings']['default_tags_field']) && !is_null($entity)) {
          $default_tags_id = $instance['settings']['default_tags_field'];
          if (isset($entity->{$default_tags_id}) && !empty($entity->{$default_tags_id})) {
            $language_keys = array_keys($entity->{$default_tags_id});
            $field_values = $entity->{$default_tags_id}[$language_keys[0]];
            if (!empty($field_values)) {
              $default_terms_ids = array();
              foreach ($field_values as $field_value) {
                if (isset($field_value['tid'])) {
                  $default_terms_ids[] = $field_value['tid'];
                }
              }

              $terms = taxonomy_term_load_multiple($default_terms_ids);
              $default_terms = array();
              foreach ($terms as $term) {
                $default_terms[] = $term->name . '|';
              }

              $tag_string = implode(',', $default_terms);
            }
          }
        }

        $element['#after_build'] = array('powertagging_add_field_js');
        $element['tag_string'] = array(
          '#type' => 'hidden',
          '#maxlength' => 32000,
          '#default_value' => !empty($tag_string) ? $tag_string : NULL,
          '#element_validate' => array('powertagging_ui_validate'),
          '#attributes' => array(
            'class' => array('powertagging_tag_string'),
          ),
        );

        // Add a field to display an error if the selected language is not
        // supported.
        $error_markup = t('Tagging is not possible for the currently selected language.');
        if (user_access('administer powertagging')) {
          $error_markup .= '<br />' . t('Select a PoolParty language in your !moduleconfig.', array('!moduleconfig' => l(t('powertagging configuration'), 'admin/config/services/powertagging/' . $instance['settings']['powertagging_id'] . '/edit')));
        }
        $element['powertagging']['language_error'] = array(
          '#type' => 'item',
          '#markup' => '<div class="messages warning">' . $error_markup . '</div>',
        );

        $element['powertagging']['manual'] = array(
          '#title' => t('Add tags manually'),
          '#type' => 'textfield',
          '#description' => t('The autocomplete-mechanism will suggest concepts from the thesaurus.'),
          '#attributes' => array(
            'class' => array('powertagging_autocomplete_tags'),
          ),
        );

        $element['powertagging']['tags_result'] = array(
          '#type' => 'item',
          '#title' => t('Your selected tags'),
          '#markup' => '<div id="powertagging-tag-result"></div>',
        );

        $element['powertagging']['tags'] = array(
          '#type' => 'item',
          '#title' => t('Tags extracted from'),
          '#markup' => '<div class="ajax-progress" style="display:none;"><div class="throbber">Loading...</div></div><div id="powertagging-extracted-tags" style="display:none;"></div>',
        );

        $element['powertagging']['get_tags'] = array(
          '#value' => t('Get tags'),
          '#type' => 'button',
        );

        $element['field_instance'] = array(
          '#value' => $instance,
          '#type' => 'value',
        );

        // Get the language of the current entity.
        $language = LANGUAGE_NONE;
        if (!is_null($entity) && property_exists($entity, 'language')) {
          $language = $entity->language;
        }

        $element['language'] = array(
          '#value' => $language,
          '#type' => 'value',
        );
      }
      else {
        $element['powertagging']['error_info'] = array(
          '#type' => 'item',
          '#markup' => t('The "PowerTagging Tags"-field needs to be properly configured before content can be tagged.') . '<br />' . t('Make sure all the required fields have a selection and "Number of values" was changed to "Unlimited".'),
        );
      }

      break;
  }

  return $element;
}

/**
 * Add all the required JavaScript to the powertagging field widget form.
 *
 * @param array $element
 *   The form element that includes the field-instance-object.
 * @return array
 *   The form element that includes the field-instance-object.
 */
function powertagging_add_field_js(array $element) {
  // AJAX request do not need to add JavaScripts again.
  if (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    return $element;
  }

  $instance = $element['field_instance']['#value'];
  $language = $element['language']['#value'];
  $taxonomy_id = powertagging_get_taxonomy_id($instance['settings']['powertagging_id'], $instance['settings']['project_id']);
  $selected_tags = array();

  if (!$taxonomy_id) {
    drupal_set_message(t('The project selected in field "%name" is not connected with a vocabulary. Please select one in the connected <a href="!link">PowerTagging configuration area</a>.', array(
      '!link' => url('admin/config/services/powertagging/' . $instance['settings']['powertagging_id']),
      '%name' => $element['#title']
    )), 'warning');
  }

  // Check if all tids are integer values or new free terms.
  if (!empty($element['tag_string']['#value'])) {
    $new_tags = array();
    $tids = array();

    foreach (explode(',', $element['tag_string']['#value']) as $tag_id) {
      if (is_numeric($tag_id) && (intval($tag_id) == floatval($tag_id))) {
        $tids[] = $tag_id;
      }
      elseif (is_string($tag_id) && strpos($tag_id, '|')) {
        $new_tags[] = $tag_id;
      }
    }

    if (!empty($tids)) {
      // Get URI from each term to check if term is a concept or a free term.
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'taxonomy_term')
        ->propertyCondition('vid', $taxonomy_id)
        ->propertyCondition('tid', $tids, 'IN')
        ->execute();
      if (!empty($result)) {
        // Check if the term needs to be saved as a translation.
        $is_translation = ($language != LANGUAGE_NONE && $language != language_default('language') && powertagging_translation_possible());

        foreach ($result['taxonomy_term'] as $term) {
          $term = taxonomy_term_load($term->tid);
          $selected_tags[] = array(
            'tid' => $term->tid,
            'uri' => empty($term->field_uri) ? '' : $term->field_uri[LANGUAGE_NONE][0]['value'],
            'label' => (!$is_translation) ? $term->name : i18n_taxonomy_term_name($term, $language),
            'type' => empty($term->field_uri) ? 'freeterm' : 'concept',
          );
        }
      }
    }

    foreach ($new_tags as $new_tag) {
      list($label, $uri) = explode('|', $new_tag);
      $selected_tags[] = array(
        'tid' => 0,
        'uri' => empty($uri) ? '' : $uri,
        'label' => $label,
        'type' => empty($uri) ? 'freeterm' : 'concept',
      );
    }
  }

  // Sort the selected tags: concepts on top and free terms to the bottom.
  usort($selected_tags, 'powertagging_sort_selected_tags');

  // Find out what Drupal languages are available for tagging.
  $powertagging_config = powertagging_config_load($instance['settings']['powertagging_id']);
  $project_languages = $powertagging_config->config['projects'][$instance['settings']['project_id']]['languages'];
  $allowed_drupal_languages = array();
  foreach ($project_languages as $drupal_lang => $pp_lang) {
    if (!empty($pp_lang) && ($drupal_lang == LANGUAGE_NONE || $drupal_lang == language_default('language') || powertagging_translation_possible())) {
      $allowed_drupal_languages[] = $drupal_lang;
    }
  }

  $settings[$instance['field_name']][time()] = array(
    'fields' => powertagging_field_list($instance, 'widget_form'),
    'settings' => array(
      'field_name' => $instance['field_name'],
      'powertagging_id' => $instance['settings']['powertagging_id'],
      'project_id' => $instance['settings']['project_id'],
      'taxonomy_id' => $taxonomy_id,
      'concepts_per_extraction' => $instance['settings']['concepts_per_extraction']['value'],
      'concepts_threshold' => $instance['settings']['concepts_threshold']['value'],
      'freeterms_per_extraction' => $instance['settings']['freeterms_per_extraction']['value'],
      'freeterms_threshold' => $instance['settings']['freeterms_threshold']['value'],
      'entity_language' => $language,
      'allowed_languages' => $allowed_drupal_languages,
    ),
    'selected_tags' => $selected_tags,
  );

  drupal_add_js(array('powertagging' => $settings), 'setting');
  drupal_add_library('system', 'ui.autocomplete');
  drupal_add_library('system', 'ui.slide');
  drupal_add_js(drupal_get_path('module', 'powertagging') . '/js/powertagging.field.js');

  return ($element);
}

/**
 * Get the taggable fields for a field instance.
 *
 * Searches for possible fields for the term extraction and return it as a list
 * of select options.
 *
 * @param array $instance
 *   Instance of the PowerTagging field.
 * @param string $type
 *   Defines the return value ('instance_settings_form' | 'widget_form').
 *
 * @return array
 *   List of supported fields depending on $type.
 */
function powertagging_field_list(array $instance, $type) {
  $allowed_widgets = array(
    'text' => array(
      'text_textfield',
      'text_textarea',
      'text_textarea_with_summary',
    ),
    'file' => array('file_generic'),
  );
  $allowed_modules = array_keys($allowed_widgets);

  $entity_info = field_info_instances($instance['entity_type'], $instance['bundle']);
  uasort($entity_info, 'powertagging_sort_by_weight');
  $result = array();

  switch ($type) {
    case 'instance_settings_form':
      // Add fields specific to entity types.
      switch ($instance['entity_type']) {
        case 'node':
          $node_type = node_type_load($instance['bundle']);
          $result['title'] = $node_type->title_label . ' <span class="description">[Text field]</span>';
          break;

        case 'taxonomy_term':
          $result['name'] = 'Name of the term <span class="description">[Text field]</span>';
          $result['description'] = 'Description <span class="description">[Text area]</span>';
          break;
      }

      foreach ($entity_info as $field_name => $field_data) {
        if (in_array($field_data['widget']['module'], $allowed_modules) &&
          in_array($field_data['widget']['type'], $allowed_widgets[$field_data['widget']['module']])
        ) {
          $widget_info = field_info_widget_types($field_data['widget']['type']);
          $result[$field_name] = $field_data['label'] . ' <span class="description">[' . $widget_info['label'] . ']</span>';
        }
      }
      break;

    // JavaScript does not maintain the order during the conversion from
    // associative array to object, so we have to make a normal array out of the
    // field list.
    case 'widget_form':
      $selected_fields = $instance['settings']['fields'];

      // Add fields specific to entity types.
      switch ($instance['entity_type']) {
        case 'node':
          if ($selected_fields['title']) {
            $result[] = array(
              'field_name' => 'title',
              'module' => 'text',
              'widget' => 'text_textfield_title',
            );
          }
          break;

        case 'taxonomy_term':
          if ($selected_fields['name']) {
            $result[] = array(
              'field_name' => 'name',
              'module' => 'text',
              'widget' => 'text_textfield_title',
            );
          }
          if ($selected_fields['description']) {
            $result[] = array(
              'field_name' => 'description',
              'module' => 'text',
              'widget' => 'text_textarea_with_summary',
            );
          }
          break;
      }

      foreach ($entity_info as $field_name => $field_data) {
        if (in_array($field_data['widget']['module'], $allowed_modules) &&
          in_array($field_data['widget']['type'], $allowed_widgets[$field_data['widget']['module']]) &&
          isset($selected_fields[$field_name]) &&
          $selected_fields[$field_name]
        ) {

          $result[] = array(
            'field_name' => $field_name,
            'module' => $field_data['widget']['module'],
            'widget' => $field_data['widget']['type'],
          );
        }
      }
      break;
  }

  return $result;
}

/**
 * Validation handler for the PowerTagging Tags field.
 */
function powertagging_ui_validate($element, &$form_state) {
  $error = FALSE;

  // Only check if value is not empty.
  if (!empty($element['#value'])) {
    $tag_ids = explode(',', $element['#value']);
    $tids = array();
    $new_tags = array();

    // Check if all tids are integer-values or new freeterms.
    foreach ($tag_ids as $tag_id) {
      if (is_numeric($tag_id) && (intval($tag_id) == floatval($tag_id))) {
        $tids[] = $tag_id;
      }
      elseif (is_string($tag_id)) {
        if (strpos($tag_id, '|')) {
          $new_tags[] = $tag_id;
        }
      }
      else {
        $error = TRUE;
      }
    }

    // Check if all the terms are still existent if there was no error till now.
    if (!$error) {
      $terms = taxonomy_term_load_multiple($tids);
      // All of the terms are existent.
      if (count($terms) != count($tids)) {
        $error = TRUE;
      }
    }

    // If there is no error at all, add taxonomy terms for the new freeterms.
    if (!$error) {
      $project_id = $form_state['values'][$element['#parents'][0]][$element['#parents'][1]]['field_instance']['settings']['project_id'];
      $powertagging_id = $form_state['values'][$element['#parents'][0]][$element['#parents'][1]]['field_instance']['settings']['powertagging_id'];
      $taxonomy_id = powertagging_get_taxonomy_id($powertagging_id, $project_id);
      if ($taxonomy_id !== FALSE) {
        $new_term_ids = powertagging_add_new_tag($new_tags, $taxonomy_id, (isset($form_state['values']['language']) ? $form_state['values']['language'] : LANGUAGE_NONE), (module_exists('skos_importer') ? $project_id : ''));
      }

      form_set_value($element, implode(',', array_merge($tids, $new_term_ids)), $form_state);
    }
  }

  if ($error) {
    form_set_error($element['#name'], t('Invalid tag selection.'));
  }
}

/**
 * Add new concepts or freeterms to the taxonomy of a PoolParty project.
 *
 * @param array $new_tags
 *   Array of strings of new tag to add.
 * @param string $taxonomy_id
 *   The taxonomy id in which the new tag must be stored.
 * @param string $language
 *   The Drupal language of the terms to add.
 *
 * @return array
 *   Array of created term-ids.
 */
function powertagging_add_new_tag(array $new_tags, $taxonomy_id, $language, $skos_importer_id = '') {
  $tag_ids = array();
  $skos_importer_tags = array();
  $parent = powertagging_get_term_list_id($taxonomy_id);

  // Check if the term needs to be saved as a translation.
  $is_translation = ($language != LANGUAGE_NONE && $language != language_default('language') && powertagging_translation_possible());

  foreach ($new_tags as $new_tag) {
    list($label, $uri) = explode('|', $new_tag);

    // Check if the term already exists.
    $query = new EntityFieldQuery();
    $old_term = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $label)
      ->propertyCondition('vid', $taxonomy_id)
      ->execute();

    // If the term already exists and the entered term has no URI then do
    // nothing --> This case should never appear.
    if (!empty($old_term) && empty($uri)) {
      $tid = array_shift($old_term['taxonomy_term']);
      $tag_ids[] = $tid->tid;
      continue;
    }

    // Instantiate a new term.
    $term = new stdClass();
    $term->name = $label;
    $term->vid = $taxonomy_id;

    // If the term exists, set the tid so that the term will be updated.
    if (!empty($old_term)) {
      $tid = array_shift($old_term['taxonomy_term']);
      $term->tid = $tid->tid;
    }

    // If no URI is given, save the term as a free term, otherwise as a concept.
    if (empty($uri)) {
      $term->parent = $parent['freeterms'];
    }
    else {
      $term->field_uri[LANGUAGE_NONE][0]['value'] = $uri;
      $term->parent = $parent['concepts'];
    }

    // Save free terms and concepts in the default language normally.
    if (empty($uri) || !$is_translation) {
      taxonomy_term_save($term);
    }
    // Translated concepts need to be saved as a translated term.
    else {
      $term->name = '[[undefined]]';
      taxonomy_term_save($term);
      i18n_string_translation_update(array(
        'taxonomy',
        'term',
        $term->tid,
        'name',
      ), $label, $language, $term->name);
    }

    // If skos_importer is installed, find a way to update the skos_importer
    // tables with the imported concepts.
    if (!empty($uri) && !empty($skos_importer_id)) {
      $skos_importer_tags[] = array(
        'tid' => $term->tid,
        'importer_id' => $skos_importer_id,
        'imported' => REQUEST_TIME,
        'uri' => $uri,
        'vid' => $taxonomy_id,
        // Creating the object required for building the has would cost too
        // much performance.
        'hash' => '',
      );
    }

    $tag_ids[] = $term->tid;
  }

  // Add SKOS Importer Terms if required.
  if (!empty($skos_importer_tags)) {
    $tag_add_query = db_insert('skos_importer_terms')
      ->fields(array('tid','importer_id', 'imported', 'uri', 'vid', 'hash'));
    foreach ($skos_importer_tags as $skos_importer_tag) {
      $tag_add_query->values($skos_importer_tag);
    }
    $tag_add_query->execute();
  }

  return $tag_ids;
}

/**
 * Implements hook_field_presave().
 */
function powertagging_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($instance['widget']['type'] == 'powertagging_ui') {
    $values_array = array();
    if (isset($items[0]) && is_string($items[0]) && !empty($items[0])) {
      $items_string = $items[0];
      // Remove the language if one gets added automatically.
      if (!empty($items_string) && count(explode(',', $items_string)) == 1 && !is_numeric($items_string) && isset($items[1]) && is_string($items[1]) && !empty($items[1])) {
        $items_string = $items[1];
      }

      // Make savable values out of the value-string.
      foreach (explode(',', $items_string) as $value) {
        if (is_numeric($value)) {
          $values_array[] = array('tid' => $value);
        }
      }

      $items = $values_array;
    }
  }
}

/**
 * Callback function for extracting concepts and free terms from the content.
 *
 * @param string $content
 *   The text content to extract tags from.
 * @param array $files
 *   Array of Drupal file-ids of files to extract tags from.
 * @param array $settings
 *   Array of settings to use for the extraction process.
 * @param string $return_type
 *   The format to use to return the tags. Supported values are "json" or
 *   "array".
 *
 * @return object|array
 *   The extracted tags --> The return type depends on the given
 *   $return_type parameter.
 */
function powertagging_extract($content = NULL, array $files = NULL, array $settings = NULL, $return_type = 'json') {
  if (is_null($content)) {
    $content = $_POST['content'];
  }
  if (is_null($files)) {
    $files = $_POST['files'];
  }
  if (is_null($settings)) {
    if (isset($_POST['settings'])) {
      $settings = $_POST['settings'];
    }
    else {
      $settings = array();
    }
  }

  // Remove line breaks and HTML tags from the content and convert HTML
  // characters to normal ones.
  $content = html_entity_decode(str_replace(array("\r", "\n", "\t"), "", strip_tags($content)), ENT_COMPAT, 'UTF-8');

  $param = array(
    'projectId' => $settings['project_id'],
    'numberOfConcepts' => (int) $settings['concepts_per_extraction'],
    'numberOfTerms' => (int) $settings['freeterms_per_extraction'],
  );
  $tags = array(
    'content' => array(
      'concepts' => array(),
      'freeterms' => array(),
    ),
    'suggestion' => array(
      'concepts' => array(),
      'freeterms' => array(),
    ),
    'messages' => array(),
  );
  $suggestion = array(
    'concepts' => array(),
    'freeterms' => array(),
  );

  // Find out what language to extract.
  $powertagging_config = powertagging_config_load($settings['powertagging_id']);
  $project_languages = $powertagging_config->config['projects'][$settings['project_id']]['languages'];

  // Language mapping for the content languages exists.
  if (isset($project_languages[$settings['entity_language']]) && !empty($project_languages[$settings['entity_language']])) {
    $pp_api = powertagging_getPoolPartyApi((int) $settings['powertagging_id']);

    // Extract the concepts and free terms.
    if (isset($settings['taxonomy_id']) && $settings['taxonomy_id'] > 0) {
      if (!empty($content)) {
        $extraction = $pp_api->extractConcepts($content, $project_languages[$settings['entity_language']], $param);
        $extracted_tags = powertagging_extract_tags($extraction, $settings);
        $tags['content'] = $extracted_tags;
        $suggestion['concepts'] = array_merge($suggestion['concepts'], $extracted_tags['concepts']);
        $suggestion['freeterms'] = array_merge($suggestion['freeterms'], $extracted_tags['freeterms']);
      }

      if (!empty($files)) {
        $tags['files'] = array();
        foreach ($files as $file_id) {
          $file = file_load($file_id);
          // Use only existing files for tagging.
          if (file_exists($file->uri)) {
            $extraction = $pp_api->extractConcepts($file, $project_languages[$settings['entity_language']], $param);
            $extracted_tags = powertagging_extract_tags($extraction, $settings);
            $tags['files'][$file->filename] = $extracted_tags;
            $suggestion['concepts'] = array_merge($suggestion['concepts'], $extracted_tags['concepts']);
            $suggestion['freeterms'] = array_merge($suggestion['freeterms'], $extracted_tags['freeterms']);
          }
        }
      }
    }

    // Merge all extracted concepts and free terms for the suggestion.
    if (!empty($suggestion['concepts'])) {
      usort($suggestion['concepts'], 'powertagging_sort_by_score');
      $uris = array();
      $count = 1;
      foreach ($suggestion['concepts'] as $tag) {
        if (in_array($tag['uri'], $uris)) {
          continue;
        }
        $tags['suggestion']['concepts'][] = $tag;
        $uris[] = $tag['uri'];
        if ($settings['concepts_per_extraction'] <= $count++) {
          break;
        }
      }
    }
    if (!empty($suggestion['freeterms'])) {
      usort($suggestion['freeterms'], 'powertagging_sort_by_score');
      $labels = array();
      $count = 1;
      foreach ($suggestion['freeterms'] as $tag) {
        if (in_array($tag['label'], $labels)) {
          continue;
        }
        $tags['suggestion']['freeterms'][] = $tag;
        $labels[] = $tag['label'];
        if ($settings['freeterms_per_extraction'] <= $count++) {
          break;
        }
      }
    }
  }

  if (empty($tags['messages']) && empty($tags['suggestion']['concepts']) && empty($tags['suggestion']['freeterms'])) {
    $tags['messages'][] = array(
      'type' => 'info',
      'message' => t('No concepts or freeterms could be extracted from the entity\'s content.'),
    );
  }

  switch ($return_type) {
    case 'json':
      echo drupal_json_encode($tags);
      exit();

    case 'array':
      return $tags;
  }
}

/**
 * Extracts the tags from the extraction result set.
 *
 * @param object $extraction
 *   The extraction result set.
 * @param array $settings
 *   The settings for threshold, concepts_per_extraction, ...
 *
 * @return array
 *   A list of found concepts and free terms with the corresponding taxonomy ids
 *   if available.
 */
function powertagging_extract_tags($extraction, array $settings) {
  $concepts = array();
  $free_terms = array();
  $tags = array(
    'concepts' => array(),
    'freeterms' => array(),
  );

  // Go through the concepts.
  if (isset($extraction->concepts) && !empty($extraction->concepts)) {
    // Ignore all concepts with the score less than the threshold.
    $treshold = (int) $settings['concepts_threshold'];

    foreach ($extraction->concepts as $concept) {
      if ($concept->score >= $treshold) {
        $concepts[] = $concept;
      }
    }

    // Get the corresponding taxonomy term id.
    powertagging_add_tid_to_concept($concepts, $settings['taxonomy_id'], 'concepts', $settings['entity_language']);

    // Ignore all not found taxonomy terms.
    if (!empty($concepts)) {
      foreach ($concepts as $concept) {
        $tags['concepts'][] = array(
          'tid' => $concept->tid,
          'uri' => $concept->uri,
          'label' => $concept->prefLabel,
          'score' => $concept->score,
          'type' => 'concept',
        );
      }
    }
  }

  // Go through the free terms.
  if (isset($extraction->freeTerms) && !empty($extraction->freeTerms)) {
    // Ignore all free terms with the score less than the threshold.
    $treshold = (int) $settings['freeterms_threshold'];
    foreach ($extraction->freeTerms as $free_term) {
      if ($free_term->score >= $treshold) {
        $free_terms[] = $free_term;
      }
    }

    // Get the corresponding taxonomy term id.
    powertagging_add_tid_to_concept($free_terms, $settings['taxonomy_id'], 'free_terms', $settings['entity_language']);

    if (!empty($free_terms)) {
      foreach ($free_terms as $free_term) {
        $tags['freeterms'][] = array(
          'tid' => isset($free_term->tid) ? $free_term->tid : 0,
          'uri' => '',
          'label' => $free_term->textValue,
          'score' => $free_term->score,
          'type' => 'freeterm',
        );
      }
    }
  }

  return $tags;
}

/**
 * Add the corresponding taxonomy term id to the concepts or free terms.
 *
 * @param array $concepts
 *   The concepts or free terms found from PP Extractor.
 * @param int $taxonomy_id
 *   The taxonomy id in which the taxonomy is imported.
 * @param string $type
 *   The type of the concepts (concepts or free terms).
 * @param string $language
 *   The language of the concept label.
 */
function powertagging_add_tid_to_concept(array &$concepts, $taxonomy_id, $type, $language = LANGUAGE_NONE) {
  if (empty($concepts)) {
    return;
  }

  switch ($type) {
    case 'concepts':
      // Get all concept uris.
      $uris = array();
      foreach ($concepts as $concept) {
        $uris[] = $concept->uri;
      }

      // Search for the corresponding tid.
      $query = db_select('taxonomy_term_data', 't')
        ->fields('f', array('entity_id', 'field_uri_value'))
        ->fields('t', array('name'))
        ->condition('t.vid', $taxonomy_id)
        ->condition('f.field_uri_value', $uris, 'IN');
      $query->join('field_data_field_uri', 'f', 'f.entity_id = t.tid AND f.entity_type = \'taxonomy_term\'');
      $result = $query->execute()
        ->fetchAll();

      $terms = array();
      foreach ($result as $term) {
        $terms[$term->field_uri_value] = array(
          'tid' => $term->entity_id,
          'name' => $term->name,
        );
      }

      $default_language = language_default('language');

      // Add the tid to each concept if exists otherwise set tid = 0.
      foreach ($concepts as &$concept) {
        if (isset($terms[$concept->uri])) {
          $concept->tid = $terms[$concept->uri]['tid'];
          // Default language --> Save the term label.
          if ($language == LANGUAGE_NONE || $language == $default_language || !powertagging_translation_possible()) {
            if ($concept->prefLabel != $terms[$concept->uri]['name']) {
              $taxonomy_term = taxonomy_term_load($concept->tid);
              $taxonomy_term->name = $concept->prefLabel;
              taxonomy_term_save($taxonomy_term);
            }
          }
          // Not the default language --> Update the translation.
          else {
            i18n_string_translation_update(array(
              'taxonomy',
              'term',
              $concept->tid,
              'name'
            ), $concept->prefLabel, $language, $terms[$concept->uri]['name']);
          }
        }
        else {
          $concept->tid = 0;
        }
      }
      break;

    case 'free_terms':
      $taxonomy = taxonomy_vocabulary_load($taxonomy_id);
      // Add the tid to each free term if exists otherwise set tid to 0.
      foreach ($concepts as &$concept) {
        $terms = taxonomy_get_term_by_name($concept->textValue, $taxonomy->machine_name);
        if (!empty($terms)) {
          $term = array_shift($terms);
          $concept->tid = $term->tid;
        }
        else {
          $concept->tid = 0;
        }
      }
      break;
  }
}

/**
 * Get the taxonomy id (vid) from the project id.
 *
 * @param string $powertagging_id
 *   The ID of the powertagging configuration.
 * @param string $project_id
 *   The ID of the PoolParty project.
 *
 * @return int
 *   The vid of the taxonomy.
 */
function powertagging_get_taxonomy_id($powertagging_id, $project_id) {
  /*if (module_exists('skos_importer')) {
    $vid = db_select('skos_importer_terms', 's')
      ->fields('s', array('vid'))
      ->condition('s.importer_id', $project_id)
      ->range(0, 1)
      ->execute()
      ->fetchField();
  }
  else {*/
    $powertagging_config = powertagging_config_load($powertagging_id);
    if (isset($powertagging_config->config['projects'][$project_id]['taxonomy_id'])) {
      $vid = $powertagging_config->config['projects'][$project_id]['taxonomy_id'];
    }
    else {
      $vid = 0;
    }
  //}

  return $vid;
}

/**
 * Get the term IDs of the fixed values of the powertagging taxonomy.
 *
 * Get the taxonomy term id from the "Free Terms" and "Concepts" list term;
 * If it does not exists, it will be created.
 *
 * @param int $taxonomy_id
 *   The taxonomy id in which the list is.
 *
 * @return array
 *   The taxonomy term ids.
 */
function powertagging_get_term_list_id($taxonomy_id) {
  $list_ids = array(
    'concepts' => NULL,
    'freeterms' => NULL,
  );
  $list_names = array(
    'concepts' => 'Concepts',
    'freeterms' => 'Free Terms',
  );

  foreach ($list_names as $key => $list_name) {
    // Check if free term list exists.
    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('vid', (int) $taxonomy_id)
      ->propertyCondition('name', $list_name)
      ->execute();

    if (!empty($result)) {
      foreach ($result['taxonomy_term'] as $term) {
        $parents = taxonomy_get_parents($term->tid);
        $term = taxonomy_term_load($term->tid);
        if (empty($parents) && empty($term->field_uri)) {
          $list_ids[$key] = $term->tid;
          break;
        }
      }
    }

    // If free term list not exists then create it.
    if (is_null($list_ids[$key])) {
      $term = new stdClass();
      $term->name = $list_name;
      $term->vid = $taxonomy_id;
      taxonomy_term_save($term);
      $list_ids[$key] = $term->tid;
    }
  }

  return $list_ids;
}

/**
 * Implements hook_field_formatter_info().
 */
function powertagging_field_formatter_info() {
  return array(
    'powertagging_list' => array(
      'label' => t('Tag list'),
      'field types' => array('powertagging'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function powertagging_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $markup = NULL;

  switch ($display['type']) {
    case 'powertagging_list':
      if (!empty($items)) {
        $context = array(
          'items' => $items,
          'entity' => $entity,
          'langcode' => $langcode,
          'instance' => $instance,
        );
        drupal_alter('powertagging_tag_list', $markup, $context);

        if (is_null($markup)) {
          $tids = array();
          foreach ($items as $tid) {
            $tids[] = $tid['tid'];
          }
          $terms = taxonomy_term_load_multiple($tids);

          /*$pp_config = powertagging_config_load($instance['settings']['powertagging_id'])->connection->getConfig();
          foreach ($pp_config['projects'] as $project) {
            if ($project->id == $instance['settings']['project_id']) {
              $project_sparql_url = $project->sparql_endpoint_url;
            }
          }

          $glossary_path = '';
          if (isset($project_sparql_url)) {
            $entity_language = ($entity->language == LANGUAGE_NONE ? language_default('language') : $entity->language);
            foreach (smart_glossary_config_load() as $glossary) {
              if ($glossary->connection->getURL() == $project_sparql_url) {
                foreach ($glossary->language_mapping as $lang_key => $language_data) {
                  if (empty($language_data['page_language']) || $language_data['page_language'] == $entity_language) {
                    $glossary_path = $glossary->base_path . '/' . $lang_key;
                    break;
                  }
                }
                if (!empty($glossary_path)) {
                  break;
                }
              }
            }
          }*/

          foreach ($terms as $term) {
            $elements[] = l(field_filter_xss($term->name), 'taxonomy/term/' . $term->tid);
            //$elements[] = (!empty($glossary_path) && !empty($term->field_uri) ? l(field_filter_xss($term->name), $glossary_path . '/' . $term->name, array('query' => array('uri' => $term->field_uri[array_shift(array_keys($term->field_uri))][0]['value']))) : $term->name);
          }

          $markup = implode(', ', $elements);
        }
      }
      break;
  }

  if (!is_null($markup)) {
    return array(array('#markup' => $markup));
  }
  else {
    return FALSE;
  }
}

/**
 * Callback function for sorting the fields in the entity info.
 */
function powertagging_sort_by_weight($a, $b) {
  if ($a['widget']['weight'] == $b['widget']['weight']) {
    return 0;
  }
  return ($a['widget']['weight'] < $b['widget']['weight']) ? -1 : 1;
}

/**
 * Callback function for sorting tags by score.
 */
function powertagging_sort_by_score($a, $b) {
  if ($a['score'] == $b['score']) {
    return 0;
  }
  return ($a['score'] < $b['score']) ? 1 : -1;
}

/**
 * Callback function to sort the selected tags.
 */
function powertagging_sort_selected_tags($a, $b) {
  if ((empty($a['uri']) && empty($b['uri'])) || (!empty($a['uri']) && !empty($b['uri']))) {
    return strcasecmp($a['label'], $b['label']);
  }

  return empty($a['uri']) ? 1 : -1;
}
