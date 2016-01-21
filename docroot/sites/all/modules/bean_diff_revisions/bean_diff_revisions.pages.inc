<?php
/**
 * @file bean_diff_revisions additional file.
 */



/**
 * Menu callback - an overview table of older revisions.
 *
 * Generate an overview table of older revisions of a node and provide
 * an input form to select two revisions for a comparison.
 */
function bean_diff_revisions_diffs_overview($bean) {
  drupal_set_title(t('Revisions for %title', array('%title' => $bean->title)), PASS_THROUGH);
  return drupal_get_form('bean_diff_revisions_bean_revisions', $bean);
}

/**
 * Input form to select two revisions.
 */
function bean_diff_revisions_bean_revisions($form, $form_state, $bean) {
  $bean->loadRevisions();

  $form['bean_delta'] = array(
    '#type' => 'hidden',
    '#value' => $bean->delta,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Compare'));

  // Build the sortable table header.
  $header = array(
    'compare' => array('data' => drupal_render($form['submit'])),
    'vid' => array('data' => t('Revision ID')),
    'title' => array('data' => t('Title')),
    'author' => array('data' => t('Author')),
    'changed' => array('data' => t('Date Changed')),
    'operations' => array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );

  $rows = array();

  // Dont' show link for default revision
  $can_delete = bean_access('delete', $bean);
  $can_edit = bean_access('edit', $bean);

  $revisions = array();
  $options = array();
  foreach ($bean->revisions as $rev_value) {
    $revisions[] = $rev_value;
    $options["bean_{$rev_value->vid}"] = $rev_value->vid;
  }
  $form['checkboxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#printed' => TRUE,
  );

  foreach ($revisions as $rev) {
    $active = $rev->vid == $bean->vid ? TRUE : FALSE;
    $user = user_load($rev->uid);
    $rows[$rev->vid]  = array(
      'compare' => "<input name='checkboxes[bean_" . $rev->vid . "]' type='checkbox' value='bean_" . $rev->vid . "'/>",
      'vid' => $active ? t('@label (Current Revision)', array('@label' => $rev->vid)) : $rev->vid,
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $rev->label,
          '#href' => "block/{$rev->delta}/revisions/{$rev->vid}",
        ),
      ),
      'author' => format_username($user),
      'changed' => format_date($rev->changed),
    );

    $operations = array();

    if ($can_edit) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => "block/{$bean->delta}/revisions/{$rev->vid}/edit",
        'query' => drupal_get_destination(),
      );
    }

    if ($can_delete && !$active) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => "block/{$bean->delta}/revisions/{$rev->vid}/delete",
        'query' => drupal_get_destination(),
      );
    }

    if ($can_edit && !$active) {
      $operations['set-active'] = array(
        'title' => t('set active'),
        'href' => "block/{$bean->delta}/revisions/{$rev->vid}/set-active",
        'query' => drupal_get_destination(),
      );
    }

    $rows[$rev->vid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__bean_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no revisions'),
  );

  return $form;
}

/**
 * Submit code for input form to select two revisions.
 */
function bean_diff_revisions_bean_revisions_submit($form, &$form_state) {
  $form_state['redirect'] = 'block/' . $form_state['values']['bean_delta'] . '/revisions/view/' . $form_state['values']['first_vid'] . '/' . $form_state['values']['second_vid'];
}

/**
 * Validation for input form to select two revisions.
 */
function bean_diff_revisions_bean_revisions_validate($form, &$form_state) {
  $vids = array();
  foreach ($form_state['values']['checkboxes'] as $key => $value) {
    if ($value === $key) {
      list(, $vids[]) = explode('_', $value);
    }
  }
  if (count($vids) != 2) {
    form_set_error('checkbox' , t('Please select 2 revisions only to compare'));
  }
  else {
    $first_vid = array_shift($vids);
    $second_vid = array_shift($vids);
    if ($first_vid == $second_vid)  {
      form_set_error('', t('Select different revisions to compare.'));
    }
    $form_state['values']['first_vid'] = $first_vid;
    $form_state['values']['second_vid'] = $second_vid;
  }
}
