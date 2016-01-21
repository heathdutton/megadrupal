<?php

/**
 * @file
 * Themeable function callbacks for user_diff.module.
 */

/**
 * Theme function to display the revisions formular with means to select
 * two revisions.
 */
function theme_user_diff_user_revisions($vars) {
  $form = $vars['form'];
  $output = '';
  $rows = array();
  // Overview table:
  $header = array(
    t('Revision'),
    array('data' => drupal_render($form['submit']), 'colspan' => 2),
    array('data' => t('Operations'), 'colspan' => 2)
  );
  if (isset($form['info']) && is_array($form['info'])) {
    foreach (element_children($form['info']) as $key) {
      $row = array();
      if (isset($form['operations'][$key][0])) {
        // Note: even if the commands for revert and delete are not permitted,
        // the array is not empty since we set a dummy in this case.
        $row[] = drupal_render($form['info'][$key]);
        $row[] = drupal_render($form['diff']['old'][$key]);
        $row[] = drupal_render($form['diff']['new'][$key]);
        $row[] = drupal_render($form['operations'][$key][0]);
        $row[] = drupal_render($form['operations'][$key][1]);
        $rows[] = $row;
      }
      else {
        // its the current revision (no commands to revert or delete)
        $row[] = array('data' => drupal_render($form['info'][$key]), 'class' => array('revision-current'));
        $row[] = array('data' => drupal_render($form['diff']['old'][$key]), 'class' => array('revision-current'));
        $row[] = array('data' => drupal_render($form['diff']['new'][$key]), 'class' => array('revision-current'));
        $row[] = array('data' => t('current revision'), 'class' => array('revision-current'), 'colspan' => '2');
        $rows[] = array(
          'data' => $row,
          'class' => array('error'),
        );
      }
    }
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Display inline diff metadata.
 */
function theme_user_diff_inline_metadata($vars) {
  drupal_add_css(drupal_get_path('module', 'diff') .'/diff.css');
  $user = $vars['user'];

  $output = "<div class='diff-inline-metadata clear-block'>";
  $output .= "<div class='diff-inline-byline'>";
  $output .= t('Updated by !name on @date', array(
    '!name' => theme('username', array('account' => $user)),
    '@date' => format_date($user->revision_timestamp, 'small'),
  ));
  $output .= "</div>";
  $output .= "<div class='diff-inline-legend clear-block'>";
  $output .= "<label>". t('Legend') ."</label>";
  $output .= theme('diff_inline_chunk', array('text' => t('Added'), 'type' => 'add'));
  $output .= theme('diff_inline_chunk', array('text' => t('Changed'), 'type' => 'change'));
  $output .= theme('diff_inline_chunk', array('text' => t('Deleted'), 'type' => 'relete'));
  $output .= "</div>";
  $output .= "</div>";
  return $output;
}
