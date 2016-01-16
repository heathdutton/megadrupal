<?php

/**
 * @file
 * Menu page callbacks of Synonyms module.
 */

/**
 * Page callback: Outputs JSON for taxonomy autocomplete suggestions.
 *
 * This callback outputs term name suggestions in response to Ajax requests
 * made by the synonyms autocomplete widget for taxonomy term reference
 * fields. The output is a JSON object of plain-text term suggestions,
 * keyed by the user-entered value with the completed term name appended.
 * Term names containing commas are wrapped in quotes. The search is made
 * with consideration of synonyms.
 *
 * @param string $field_name
 *   The name of the term reference field.
 * @param string $entity_type
 *   Entity type to which the supplied $field_name is attached to
 * @param string $bundle
 *   Bundle name to which the supplied $field_name is attached to
 * @param string $tags_typed
 *   (optional) A comma-separated list of term names entered in the
 *   autocomplete form element. Only the last term is used for autocompletion.
 *   Defaults to '' (an empty string).
 */
function synonyms_autocomplete($field_name, $entity_type, $bundle, $tags_typed = '') {
  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  // Shift off the $entity_type argument.
  array_shift($args);
  // Shift off the $bundle argument.
  array_shift($args);
  $tags_typed = implode('/', $args);

  // Make sure the field exists and is a taxonomy field.
  if (!($field = field_info_field($field_name)) || $field['type'] != 'taxonomy_term_reference') {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('Taxonomy field @field_name not found.', array('@field_name' => $field_name));
    exit;
  }

  if (!($instance = field_info_instance($entity_type, $field['field_name'], $bundle))) {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('There was not found an instance of @field_name in @entity_type.', array(
      '@field_name' => $field_name,
      '@entity_type' => $entity_type,
    ));
    exit;
  }

  $widget = $instance['widget']['type'] == 'synonyms_autocomplete' ? $instance['widget']['settings'] : field_info_widget_settings('synonyms_autocomplete');

  // How many suggestions maximum we are able to output.
  $max_suggestions = $widget['suggestion_size'];

  // Whether we are allowed to suggest more than one entry per term, shall that
  // entry be either term name itself or one of its synonyms.
  $suggest_only_unique = $widget['suggest_only_unique'];

  // The user enters a comma-separated list of tags. We only autocomplete the
  // last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $tags_typed_tids = array();
  if (!empty($tags_typed)) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'taxonomy_term');
    $efq->propertyCondition('name', $tags_typed);
    $tags_typed_tids = $efq->execute();
    if (isset($tags_typed_tids['taxonomy_term'])) {
      $tags_typed_tids = array_keys($tags_typed_tids['taxonomy_term']);
    }
  }

  $term_matches = array();
  if ($tag_last != '') {
    // Part of the criteria for the query come from the field's own settings.
    $vocabularies = array();
    $tmp = taxonomy_vocabulary_get_names();
    foreach ($field['settings']['allowed_values'] as $tree) {
      $vocabularies[$tmp[$tree['vocabulary']]->vid] = $tree['vocabulary'];
    }
    $vocabularies = taxonomy_vocabulary_load_multiple(array_keys($vocabularies));

    // Array of found suggestions. Each subarray of this array will represent
    // a single suggestion entry. The sub array must contain the following keys:
    // - tid: (int) tid of the suggested term
    // - name: (string) name of the suggested term
    // - wording: (string) human friendly XSS escaped text of the suggestion
    //   entry
    $tags_return = array();

    // Firstly getting a list of tids that match by $term->name.
    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');

    // Do not select already entered terms.
    if (!empty($tags_typed_tids)) {
      $query->condition('t.tid', $tags_typed_tids, 'NOT IN');
    }
    // Select rows that match by term name.
    $result = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', array_keys($vocabularies))
      ->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE')
      ->range(0, $max_suggestions)
      ->execute();
    foreach ($result as $v) {
      $v = (array) $v;
      $v['wording'] = check_plain($v['name']);
      $tags_return[] = $v;
    }

    // Now we go vocabulary by vocabulary looking through synonym fields.
    foreach ($vocabularies as $vocabulary) {
      // Now we go a synonym field by synonym field gathering suggestions.
      $bundle = field_extract_bundle('taxonomy_term', $vocabulary);

      $behavior_implementations = synonyms_behavior_get('autocomplete', 'taxonomy_term', $bundle, TRUE);
      foreach ($behavior_implementations as $implementation) {
        $condition = db_and();
        $condition->condition(AbstractSynonymsSynonymsBehavior::COLUMN_PLACEHOLDER, '%' . db_like($tag_last) . '%', 'LIKE');

        if (!empty($tags_typed_tids)) {
          $condition->condition('entity_id', $tags_typed_tids, 'NOT IN');
        }
        if ($suggest_only_unique && !empty($tags_return)) {
          $tmp = array();
          foreach ($tags_return as $tag_return) {
            $tmp[] = $tag_return['tid'];
          }
          $condition->condition('entity_id', $tmp, 'NOT IN');
        }

        $new_tids = array();
        foreach (synonyms_synonyms_find_behavior($condition, $implementation) as $synonym) {
          if (!$suggest_only_unique || !in_array($synonym->entity_id, $new_tids)) {
            $tags_return[] = array(
              'tid' => $synonym->entity_id,
              'name' => '',
              'synonym' => $synonym->synonym,
              'implementation' => $implementation,
            );
            $new_tids[] = $synonym->entity_id;
          }
        }
      }
    }

    $synonym_terms = array();
    foreach ($tags_return as $v) {
      if (isset($v['synonym'])) {
        $synonym_terms[] = $v['tid'];
      }
    }

    if (!empty($synonym_terms)) {
      $synonym_terms = taxonomy_term_load_multiple($synonym_terms);
      foreach ($tags_return as &$v) {
        if (isset($v['synonym'])) {
          $instance = field_info_instance($v['implementation']['entity_type'], $v['implementation']['field_name'], $v['implementation']['bundle']);
          $v['name'] = $synonym_terms[$v['tid']]->name;
          $v['wording'] = format_string(filter_xss($v['implementation']['settings']['wording']), array(
            '@synonym' => $v['synonym'],
            '@term' => $v['name'],
            '@field_name' => drupal_strtolower($instance['label']),
          ));
        }
      }
    }
    $prefix = empty($tags_typed) ? '' : drupal_implode_tags($tags_typed) . ', ';

    if (count($tags_return) > $max_suggestions) {
      $tags_return = array_slice($tags_return, 0, $max_suggestions);
    }

    // Now formatting the results.
    foreach ($tags_return as $info) {
      $n = $info['name'];
      // Term names containing commas or quotes must be wrapped in quotes.
      if (strpos($info['name'], ',') !== FALSE || strpos($info['name'], '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $info['name']) . '"';
      }
      while (isset($term_matches[$prefix . $n])) {
        $n .= ' ';
      }
      $term_matches[$prefix . $n] = $info['wording'];
    }
  }
  drupal_json_output($term_matches);
}

/**
 * Default theme implementation for behavior settings form element.
 */
function theme_synonyms_behaviors_settings($variables) {
  drupal_add_css(drupal_get_path('module', 'synonyms') . '/synonyms.css');

  $element = &$variables['element'];

  $table = array(
    'header' => array(t('Field')),
    'rows' => array(),
    'empty' => t('Seems like there are no fields for which synonyms functionality is available. Try adding a text field to get started.'),
  );

  $instance_ids = array();
  foreach (element_children($element) as $behavior) {
    $table['header'][] = check_plain($element[$behavior]['#title']);
    $instance_ids = array_unique(array_merge($instance_ids, element_children($element[$behavior])));
  }

  foreach ($instance_ids as $instance_id) {
    $row = array();
    $row_title = '';
    foreach (element_children($element) as $behavior) {
      if (isset($element[$behavior][$instance_id]['#title']) && !$row_title) {
        $row_title = check_plain($element[$behavior][$instance_id]['#title']);
      }
      $row[] = array(
        'data' => isset($element[$behavior][$instance_id]) ? drupal_render($element[$behavior][$instance_id]) : t('Not implemented'),
        'class' => array('synonyms-behavior-settings', 'synonyms-behavior-settings-' . $behavior),
      );
    }
    array_unshift($row, $row_title);
    $table['rows'][] = $row;
  }

  return '<div id="' . $element['#id'] . '">' . theme('table', $table) . drupal_render_children($element) . '</div>';
}
