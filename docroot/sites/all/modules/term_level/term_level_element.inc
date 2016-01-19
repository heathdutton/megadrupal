<?php

/**
 * @file term_level_element form type
 */
define('TERM_LEVEL_TAG_CLOUD_LINKS', 50);


/**
 * Implements hook_element_info().
 */
function term_level_element_info() {
  $type['term_level'] = array(
    '#input' => TRUE,
    '#process' => array('term_level_element_process'),
    '#terms' => array(),
    '#levels' => array(),
    '#parent' => 0,
    '#vid' => 0,
    '#tag_cloud_terms' => array(),
    '#theme' => 'term_level_element',
  );
  return $type;
}

/**
 * Implements hook_theme().
 */
function term_level_theme() {
  return array(
    'term_level_element' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Processes the term_level element type.
 */
function term_level_element_process($element, $form_state) {
  $module_path = drupal_get_path('module', 'term_level');
  $element['#attached']['css'][] = $module_path .'/term_level.css';
  $element['#attached']['js'][] = $module_path .'/term_level.js';
  $element['#attached']['js'][] = array('data' => array('termLevel' => array('term-level-element-table-' . $element['#vid'] . '-' . $element['#parent'])), 'type' => 'setting');

  $element['#levels']['none'] = t('None');
  $element['terms'] = array();
  foreach ($element['#terms'] as $term) {
    $element['terms'][$term->tid] = array(
      '#type' => 'radios',
      '#weight' => isset($term->row_weight) ? $term->row_weight : 0,
      '#options' => $element['#levels'],
      '#default_value' => isset($term->level) ? $term->level : NULL,
      '#tid' => $term->tid,
    );
    $element['terms'][$term->tid]['term-name'] = array('#markup' => check_plain($term->name));
  }

  $tag_cloud_term_options = array();
  $tag_cloud_markup = term_level_element_get_tag_cloud($element['#tag_cloud_terms'], $tag_cloud_term_options);
  $element['tag_cloud_term_links'] = array('#markup' => $tag_cloud_markup);

  $element['tag_cloud_term_options']['select'] = array(
    '#type' => 'select',
    '#options' => $tag_cloud_term_options,
    '#default_value' => 0,
  );
  $element['tag_cloud_term_options']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add to @parent', array('@parent' => $element['#title'])) ,
    '#name' => 'term-level-add-' . $element['#vid'] . '-' . $element['#parent'],
    '#submit' => array('term_level_element_add_terms_submit'),
    '#limit_validation_errors' => array(),
    '#term_level_parent' => $element['#parent'],
    '#vid' => $element['#vid'],
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'term_level_element_ajax_callback',
      //'effect' => 'fade',
      //'speed' => 'fast',
    ),
  );
  return $element;
}

/**
 * Submit handler for additional terms.
 *
 * Extracts existing and added terms and saves them under 'default_terms' and
 * 'added_term' in $form_state.
 */
function term_level_element_add_terms_submit($form, &$form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  $field_values = $form_state['input'];
  for ($i = 0; $i < count($parents); $i++) {
    if ($parents[$i+1] === 'tag_cloud_term_options') {
      break;
    }
    $field_values = $field_values[$parents[$i]];
  }

  $default_terms = array();
  $added_term = FALSE;
  foreach ($field_values as $group_id => $group) {
    // Table
    if (!empty($group['terms'])) {
      foreach ($group['terms'] as $tid => $level) {
        if ($tid != $group['tag_cloud_term_options']['select']) {
          $default_terms[$tid] = $level;
        }
      }
    }
    // Select List
    if ($group['tag_cloud_term_options']['select']) {
      $added_term = $group['tag_cloud_term_options']['select'];
    }
  }

  form_set_value($form, array('default_terms' => $default_terms, 'added_term' => $added_term), $form_state);
  $form_state['rebuild'] = TRUE;
}

/**
 * Returns the current field with updated terms.
 */
function term_level_element_ajax_callback($form, &$form_state) {
  $commands = array();
  if (isset($form_state['values']['default_terms']) || isset($form_state['values']['added_term'])) {
    $parents = $form_state['triggering_element']['#parents'];
    $field = $form;
    foreach ($parents as $parent) {
      if ($parent === 'tag_cloud_term_options') {
        break;
      }
      $field = $field[$parent];
    }

    $table = drupal_render($field);
    $rows = explode('</tr>', $table);
    array_pop($rows); //Remove last entry with </tbody></table>.
    if (count($rows) > 1) {
      // mMore than the header.
      $last_row = array_pop($rows);
      if (strpos($last_row, "term-level-element-tag-cloud-row") !== FALSE) {
        $tag_cloud_row = trim($last_row) . '</tr>';
        if (count($rows) > 1) {
          $last_row = array_pop($rows);
        }
      }
      $term_added = $form_state['values']['added_term'];
      if ($term_added && strpos($last_row, "term-level-element-table-row-" . $term_added) !== FALSE) {
        // Remove any unnecessary suffix content.
        $first_tr = strpos($last_row, '<tr');
        $added_term_row = substr($last_row, $first_tr) .'</tr>';
      }
    }

    $table_selector = '#term-level-element-table-' . $form_state['triggering_element']['#vid'] . '-' . $form_state['triggering_element']['#term_level_parent'];
    $commands[] = ajax_command_css($table_selector . ' tr.term-level-element-table-row-none', array('display' => 'none'));
    $tag_cloud_selector = $table_selector . ' tr.term-level-element-tag-cloud-row';


    if (isset($added_term_row)) {
      // Add table header, if not available yet.
      if (count($rows) == 1 && preg_match('/<thead>.*<\/thead>/', $table, $matches)) {
        $commands[] = ajax_command_before($table_selector . ' tbody', $matches[0]);
      }

      // Remove possible existing row with this term
      $commands[] = ajax_command_remove('#term-level-element-table-row-' . $term_added);
      $commands[] = ajax_command_before($tag_cloud_selector, $added_term_row);
    }
    if (isset($tag_cloud_row)) {
      $commands[] = ajax_command_replace($tag_cloud_selector, $tag_cloud_row);
    }
  }
  $commands[] = ajax_command_prepend("#content", theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Themes the term level element into a table.
 */
function theme_term_level_element($variables) {
  $element = $variables['element'];

  // Theme element in a table.
  $table['#theme'] = 'table';
  $table['#attributes']['class'] = array('term-level-element-table');
  $table['#attributes']['id'] = 'term-level-element-table-' . $element['#vid'] . '-' . $element['#parent'];

  // Set table caption and header, if the element has a title.
  if (!empty($element['#title'])) {
    $table['#caption'] = check_plain($element['#title']);
  }
  if (count($element['#terms'])) {
    $table['#header'][] = array('data' => '', 'class' => array('term-level-element-first-col'));
  }
  foreach ($element['#levels'] as $level) {
    if ($level === t('None')) {
      // Hide label, if JS enabled (see 'Remove' link).
      $level =  '<span class="term-level-hide-js">' . $level . '</span>';
    }
    else {
      $level = check_plain($level);
    }
    if (count($element['#terms'])) {
      $table['#header'][] = array(
        'data' => $level,
        'class' => 'term-level-element-table-header-col',
      );
    }
  }

  // Render the listed terms.
  foreach (element_children($element['terms'], TRUE) as $key) {
    $row = array();
    $row['class'][] = "term-level-element-table-row";
    if ($element['terms'][$key]['#default_value'] == 'none') {
      $row['class'][] = "term-level-hide-js";
    }
    $row['id'] = "term-level-element-table-row-" . $element['terms'][$key]['#tid'];
    foreach (element_children($element['terms'][$key]) as $child) {
      if ($child === "term-name") {
        $class = "term-level-element-table-col-name";
      }
      else if ($child === "none") {
        $class = "term-level-element-table-col-radio";
        unset($element['terms'][$key][$child]['#title']);

        // Display 'Remove' link, if JS enabled, else the 'None' radio button.
        $remove_link = '<span class="term-level-show-js">' . l(t('Remove'), '', array('attributes' => array('class' => array('term-level-remove-links')))) . '<span>';
        $element['terms'][$key][$child]['#prefix'] = $remove_link . '<span class="term-level-hide-js">';
        $element['terms'][$key][$child]['#suffix'] = '</span>';
      }
      else {
        $class = "term-level-element-table-col-radio";
        unset($element['terms'][$key][$child]['#title']);
      }
      $row['data'][] = array(
        'data' => $element['terms'][$key][$child],
        'class' => $class,
      );
    }
    $table['#rows'][] = $row;
  }

  // Render Tag Cloud.
  $tag_cloud_links = "";
  if (!empty($element['tag_cloud_term_links']['#markup'])) {
    $tag_cloud_links .= '<div class="term-level-element-tag-cloud-title">' . t('Additional terms:') . '</div>';
    $tag_cloud_links .= '<div class="term-level-show-js">' . drupal_render($element['tag_cloud_term_links']) . '</div>';
  }
  else {
    $tag_cloud_row['class'][] = 'term-level-hide-js';
  }
  $tag_cloud_options = '<div class="term-level-hide-js">' . drupal_render($element['tag_cloud_term_options']) . '</div>';
  $tag_cloud_row['class'][] = 'term-level-element-tag-cloud-row';
  $tag_cloud_row['data'][] = array(
    'data' => $tag_cloud_links . $tag_cloud_options,
    'colspan' => count($element['#levels'])+1,
  );
  $table['#rows'][] = $tag_cloud_row;
  return drupal_render($table);
}

/**
 * Helper function for generating the tag cloud.
 *
 * In case two hierarchical levels are passed, a list of tag clouds is returned.
 */
function term_level_element_get_tag_cloud($tag_clould_terms, &$tag_cloud_term_options = array()) {
  $tag_cloud_term_options[0] = '-';

  if (isset($tag_clould_terms['all'])) {
    $tag_cloud_term_links = array();
    foreach ($tag_clould_terms['all'] as $term) {
      $tag_cloud_term_options[$term->tid] = $term->name;
      // The term description is furthermore escaped by drupal_attributes().
      $tag_cloud_term_links[] = l($term->name, '', array('attributes' => array('title' => strip_tags($term->description), 'class' => array('term-level-tag-cloud-links'), 'id' => 'term-level-tag-cloud-links-id-' . $term->tid)));
    }
    return term_level_element_get_tag_cloud_links_markup($tag_cloud_term_links);
  }
  elseif (isset($tag_clould_terms['groups'])) {
    $items = array();
    foreach ($tag_clould_terms['groups'] as $tid => $group) {
      $tag_cloud_term_links = array();
      $group_term = $tag_clould_terms['group_terms'][$tid];
      foreach ($group as $term) {
        $tag_cloud_term_options[$term->tid] = $term->name;
        $tag_cloud_term_links[] = l($term->name, '', array('attributes' => array('title' => strip_tags($term->description), 'class' => array('term-level-tag-cloud-links'), 'id' => 'term-level-tag-cloud-links-id-' . $term->tid)));
      }
      $links_markup = term_level_element_get_tag_cloud_links_markup($tag_cloud_term_links);
      $items[] = check_plain($group_term->name) . ': ' . $links_markup;
    }
    return theme('item_list', array('items' => $items));
  }
}

/**
 * Returns the tag cloud links markup and in case too many links are passed
 * (> TERM_LEVEL_TAG_CLOUD_LINKS), a 'more' link is added.
 */
function term_level_element_get_tag_cloud_links_markup($tag_cloud_term_links) {
  if (count($tag_cloud_term_links) > TERM_LEVEL_TAG_CLOUD_LINKS) {
    $tag_cloud_term_links_show = array_slice($tag_cloud_term_links, 0, TERM_LEVEL_TAG_CLOUD_LINKS);
    $tag_clould_term_links_hide = array_slice($tag_cloud_term_links, TERM_LEVEL_TAG_CLOUD_LINKS);
    $tag_cloud_term_links_markup = implode(', ', $tag_cloud_term_links_show);
    $tag_cloud_term_links_markup .= ', ' . l(t('more ...'), '', array('attributes' => array('class' => array('term-level-tag-cloud-more'))));
    $tag_cloud_term_links_markup .= ' <span class="term-level-tag-cloud-hide">' . implode(', ', $tag_clould_term_links_hide) .'</span>';
  }
  else {
    $tag_cloud_term_links_markup = implode(', ', $tag_cloud_term_links);
  }
  return $tag_cloud_term_links_markup;
}
