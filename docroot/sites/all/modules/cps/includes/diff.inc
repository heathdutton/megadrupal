<?php

/**
 * @file
 * CPS support for diff.
 */

/**
 * Render a diff between two entities of the same type.
 *
 * @param string $entity_type
 *   The entity type to compare.
 * @param object $old
 *   Node for comparison which will be displayed on the left side.
 * @param object $new
 *   Node for comparison which will be displayed on the right side.
 * @param string $state
 *   The state to render for the diff.
 * @param array $context
 *   Default context properties to send to the diff.
 *
 * @return array
 *   An array suitable for drupal_render().
 */
function cps_render_diff($entity_type, $old, $new, $state = 'raw', $context = array()) {
  if (!module_exists('diff')) {
    return array();
  }

  // cps_diff can be long if there's a LOT of content, so let's add some time.
  drupal_set_time_limit(30);
  $rows = cps_diff($entity_type, $old, $new, $state, $context);
  $header = array();

  $build = array(
    '#theme' => 'table__diff__standard',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('diff')),
    '#colgroups' => _diff_default_cols(),
    '#sticky' => FALSE,
    '#changes' => 0,
  );

  if ($theme = variable_get('diff_theme', 'default')) {
    $build['#attached']['css'][] = ctools_attach_css("diff.{$theme}.css", 'diff');
  }

  foreach ($rows as $key => $row) {
    if (strpos($key, 'diff-row-') === 0) {
      // The Diff module sometimes adds empty rows which are keyed like a
      // change, but are not. Discount these.
      // Rows representing an actual visible change should always have a '+'
      // (which precedes the new content), a '-' (which precedes the removed
      // content) or both. See DrupalDiffFormatter::addedLine() and
      // DrupalDiffFormatter::deletedLine().
      $empty_row = TRUE;
      if (is_array($row)) {
        foreach ($row as $element) {
          if (is_array($element) && isset($element['data']) && in_array($element['data'], array('+', '-'))) {
            $empty_row = FALSE;
            break;
          }
        }
      }
      if (!$empty_row) {
        $build['#changes']++;
      }
    }
  }

  return $build;
}

/**
 * Creates an array of rows which represent the difference between nodes.
 *
 * This function relies on the diff module. It is largely copied from
 * _diff_body_rows() which is unnecessarily node specific.
 *
 * @param string $entity_type
 *   The entity type to compare.
 * @param object $old
 *   Node for comparison which will be displayed on the left side.
 * @param object $new
 *   Node for comparison which will be displayed on the right side.
 * @param string $state
 *   The state to render for the diff.
 * @param array $context
 *   Default context properties to send to the diff.
 *
 * @return array
 *   The rows of the diff.
 */
function cps_diff($entity_type, $old, $new, $state = 'raw', $context) {
  if (!module_exists('diff')) {
    return FALSE;
  }

  // This is an unique index only, so no need for drupal_static().
  static $table_row_counter = 0;

  if ($theme = variable_get('diff_theme', 'default')) {
    drupal_add_css(drupal_get_path('module', 'diff') . "/css/diff.{$theme}.css");
  }
  module_load_include('inc', 'diff', 'diff.pages');

  $rows = array();
  $any_visible_change = FALSE;

  // Some special handling if the old version is the special unpublished
  // live revision.

  $is_new = !cps_entity_in_published_changeset($entity_type, $old);

  if (!$is_new) {
    $context += array(
      'entity_type' => $entity_type,
      'states' => array($state),
      'view_mode' => 'diff_standard',
    );

    $diffs = diff_compare_entities($old, $new, $context);

    // Track line numbers between multiple diffs.
    $line_stats = array(
      'counter' => array('x' => 0, 'y' => 0),
      'offset' => array('x' => 0, 'y' => 0),
    );

    // Render diffs for each.
    foreach ($diffs as $diff) {
      $show_header = !empty($diff['#name']);
      // These are field level settings.
      if ($show_header && isset($diff['#settings']['show_header'])) {
        $show_header = $show_header && $diff['#settings']['show_header'];
      }

      // Line counting and line header options.
      if (empty($diff['#settings']['line_counter'])) {
        $line_counter = FALSE;
      }
      else {
        $line_counter = $diff['#settings']['line_counter'];
      }
      // Every call to 'line' resets the counters.
      if ($line_counter) {
        $line_stats['counter']['x'] = 0;
        $line_stats['counter']['y'] = 0;
        if ($line_counter == 'line' && 0) {
          $line_stats['offset']['x'] = 0;
          $line_stats['offset']['y'] = 0;
        }
        $line_stats_ref = $line_stats;
      }
      else {
        $line_stats_ref = NULL;
      }

      list($old_content, $new_content) = diff_extract_state($diff, $state);
      if ($diff_rows = diff_get_rows($old_content, $new_content, $line_counter && $line_counter != 'hidden', $line_stats_ref)) {
        if ($line_counter && $line_counter != 'line') {
          $line_stats['offset']['x'] += $line_stats_ref['counter']['x'];
          $line_stats['offset']['y'] += $line_stats_ref['counter']['y'];
        }
        if ($show_header) {
          $rows['diff-header-' . $table_row_counter++] = array(
            array(
              'data' => t('Changes to %name', array('%name' => $diff['#name'])),
              'class' => 'diff-section-title',
              'colspan' => 4,
            ),
          );
        }
        // To avoid passing counter to the Diff engine, index rows manually here
        // to allow modules to interact with the table. i.e. no array_merge().
        foreach ($diff_rows as $row) {
          $rows['diff-row-' . $table_row_counter++] = $row;
        }
        $any_visible_change = TRUE;
      }
    }
  }

  if (!$any_visible_change) {
    $rows['diff-empty-' . $table_row_counter++] = array(
      array(
        'data' => !$is_new ? t('No visible changes') : t('Newly created'),
        'class' => 'diff-section-title',
        'colspan' => 4,
      ),
    );
    // @todo: revise this.
    // Needed to keep safari happy.
    $rows['diff-empty-' . $table_row_counter++] = array(
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
    );
  }

  return $rows;
}

/**
 * Render a change into a changeset's entity view.
 *
 * @param string $entity_type
 *   The type of entity being rendered.
 * @param object $change
 *   The changed entity being rendered.
 * @param CPSChangeset $entity
 *   The changeset this is for.
 * @return array
 *   An array suitable for drupal_render().
 */
function cps_render_changed_entity($entity_type, $change, $entity) {
  list($entity_id, , $bundle) = entity_extract_ids($entity_type, $change);
  $uri = entity_uri($entity_type, $change);
  $changeset_uri = $entity->uri();

  // Cache this on the changeset entity so that we don't run the query
  // over and over again on the changeset view page.
  if (!isset($entity->cps_changeset_to_diff_against)) {
    if (!$entity->published) {
      $entity->cps_changeset_to_diff_against = CPS_PUBLISHED_CHANGESET;
    }
    else {
      $entity->cps_changeset_to_diff_against = db_query("SELECT changeset_id FROM {cps_changeset} WHERE published < :published ORDER by published DESC LIMIT 1", array(':published' => $entity->published))->fetchField();
    }
  }

  // We can't actually trust $change->published_revision because it might have
  // loaded incompletely, so let's load a fresh version.

  // Make sure that CPS doesn't swap out the entity it loads!
  cps_override_changeset($entity->cps_changeset_to_diff_against);
  $original = entity_load_single($entity_type, $entity_id);
  cps_override_changeset(NULL);

  if ($entity->cps_changeset_to_diff_against != CPS_PUBLISHED_CHANGESET) {
    $new = !empty($original->default_revision);
  }
  else {
    $new = !cps_entity_in_published_changeset($entity_type, $original);
  }

  if (!$new) {
    $diff = cps_render_diff($entity_type, $original, $change);
    if (!empty($diff['#changes'])) {
      $title = format_plural($diff['#changes'], '1 change', '@count changes');
    }
    else {
      // If there are no changes, we should get either new entity or no visible changes.
      if ($diff) {
        $title = reset($diff['#rows']);
        $title = $title[0]['data'];
      }
      else {
        $title = t('No visible changes');
      }
    }
  }
  else {
    $diff = array();
    $title = t('Newly created');
  }

  $can_edit_entities = cps_can_edit_entities('update', $entity_type, $change, $entity->changeset_id);
  $can_edit_changeset = entity_access('update', 'cps_changeset', $entity);

  $entity_info = entity_get_info($entity_type);
  $build = array(
    '#entity' => $change,
    'top' => array(
      '#prefix' => '<tr class="even">',
      'title' => array(
        '#prefix' => '<td class="cps-title">',
        '#markup' => "[$entity_id] " . check_plain(entity_label($entity_type, $change)),
        '#suffix' => '</td>',
      ),
      'bundle' => array(
        '#prefix' => '<td class="cps-bundle">',
        '#markup' => $entity_info['bundles'][$bundle]['label'],
        '#suffix' => '</td>',
      ),
      'operations' => array(
        '#prefix' => '<td class="cps-operations">',
        'view' => array(
          '#theme' => 'link',
          '#text' => t('view'),
          '#path' => $uri['path'],
          '#options' => array(
            'query' => array('changeset_id' => $entity->changeset_id),
            'html' => TRUE,
            'attributes' => array('class' => array('cps-changeset-operation')),
          ),
        ),
        'remove' => array(
          '#theme' => 'link',
          '#text' => t('remove from site version'),
          '#path' => $changeset_uri['path'] . '/remove/' . $entity_type . '/' . $entity_id,
          '#options' => array(
            'query' => drupal_get_destination(),
            'html' => TRUE,
            'attributes' => array('class' => array('cps-changeset-operation')),
          ),
          '#access' => $can_edit_changeset,
        ),
        'move' => array(
          '#theme' => 'link',
          '#text' => $can_edit_entities ? t('move/copy to current site version') : t('copy to current site version'),
          '#path' => $changeset_uri['path'] . '/move/' . $entity_type . '/' . $entity_id,
          '#options' => array(
            'query' => drupal_get_destination(),
            'html' => TRUE,
            'attributes' => array('class' => array('cps-changeset-operation')),
          ),
          '#access' => cps_can_edit_entities('move to', $entity_type, $change) && $entity->changeset_id != cps_get_current_changeset(),
        ),
        '#suffix' => '</td>',
      ),
      '#suffix' => '</tr>',
    ),
    'bottom' => array(
      '#prefix' => '<tr class="odd"><td colspan="3" class="cps-changes"><div class="ctools-collapsible-container ctools-collapsed">',
      'title' => array(
        '#prefix' => '<div class="ctools-collapsible-handle">',
        '#markup' => $title,
        '#suffix' => '</div>',
      ),
      'changes' => array(
        '#prefix' => '<div class="ctools-collapsible-content">',
        'diff' => $diff,
        '#suffix' => '</div>',
      ),
      '#suffix' => '</div></td></tr>',
    ),
    '#suffix' => '</div>',
  );

  return $build;
}

/**
 * Get the diff header for a changeset.
 * @param CPSChangeset $changeset
 *   The changeset.
 *
 * @return string
 *   The rendered header for that portion of the diff.
 */
function _cps_diff_header($changeset) {
  $rows = array();

  $uri = $changeset->uri();
  $labels = cps_changeset_get_state_labels();

  // This isn't a real label.
  $labels['live'] = t('Live');

  $rows[] = array(
    t('Site version'),
    l($changeset->name, $uri['path'])
  );

  $rows[] = array(
    t('Created by'),
    theme('username', array('account' => user_load($changeset->uid))),
  );

  $rows[] = array(
    t('On'),
    format_date($changeset->changed),
  );

  $rows[] = array(
    t('State'),
    $labels[$changeset->status],
  );

  return theme('table', array('rows' => $rows));
}
