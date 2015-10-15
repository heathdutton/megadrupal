<?php

/**
 * @file
 * Creates the previous next node block.
 */

/**
 * Implements hook_block_info().
 */
function prev_next_block_info() {
  $num_blocks = variable_get('prev_next_num_blocks', PREV_NEXT_NUM_BLOCKS_DEFAULT);
  for ($b = 0; $b < $num_blocks; $b++) {
    $blocks[$b] = array(
      'info' => t('Prev/Next links !blocknum', array('!blocknum' => 1 + $b)),
      'status' => 0,
      'cache' => DRUPAL_CACHE_PER_PAGE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function prev_next_block_configure($delta) {
  $description = 'Use the available tokens ' . (module_exists('token') ? '(see below)' : '') . ' to customize the link text.';

  $form['previous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Previous Node'),
    '#collapsible' => TRUE,
  );
  $form['previous']['prev_next_display_prev' . $delta] = array(
    '#type' => 'checkbox',
    '#title' => t('Display'),
    '#default_value' => variable_get('prev_next_display_prev' . $delta, PREV_NEXT_DISPLAY_DEFAULT),
  );
  $form['previous']['prev_next_display_text_prev' . $delta] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#description' => $description,
    '#default_value' => variable_get('prev_next_display_text_prev' . $delta, PREV_NEXT_DISPLAY_TEXT_PREV_DEFAULT),
  );
  if (module_exists('token')) {
    $form['previous']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['previous']['tokens']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  $form['next'] = array(
    '#type' => 'fieldset',
    '#title' => t('Next Node'),
    '#collapsible' => TRUE,
  );
  $form['next']['prev_next_display_next' . $delta] = array(
    '#type' => 'checkbox',
    '#title' => t('Display'),
    '#default_value' => variable_get('prev_next_display_next' . $delta, PREV_NEXT_DISPLAY_DEFAULT),
  );
  $form['next']['prev_next_display_text_next' . $delta] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#description' => $description,
    '#default_value' => variable_get('prev_next_display_text_next' . $delta, PREV_NEXT_DISPLAY_TEXT_NEXT_DEFAULT),
  );
  if (module_exists('token')) {
    $form['next']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['next']['tokens']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }
  return $form;
}

/**
 * Implements hook_block_save().
 */
function prev_next_block_save($delta, $edit) {
  variable_set('prev_next_display_prev' . $delta, $edit['prev_next_display_prev' . $delta]);
  variable_set('prev_next_display_next' . $delta, $edit['prev_next_display_next' . $delta]);
  variable_set('prev_next_display_text_prev' . $delta, $edit['prev_next_display_text_prev' . $delta]);
  variable_set('prev_next_display_text_next' . $delta, $edit['prev_next_display_text_next' . $delta]);
}

/**
 * Implements hook_block_view().
 */
function prev_next_block_view($delta) {
  $content = '';
  $block = array();
  $next_display = variable_get('prev_next_display_next' . $delta, PREV_NEXT_DISPLAY_DEFAULT);
  $next_text = variable_get('prev_next_display_text_next' . $delta, PREV_NEXT_DISPLAY_TEXT_NEXT_DEFAULT);
  $prev_display = variable_get('prev_next_display_prev' . $delta, PREV_NEXT_DISPLAY_DEFAULT);
  $prev_text = variable_get('prev_next_display_text_prev' . $delta, PREV_NEXT_DISPLAY_TEXT_PREV_DEFAULT);


  if (menu_get_object()) {
    $node = menu_get_object();
    $nid = $node->nid;
    $n_nid = prev_next_nid($nid, 'next');
    $p_nid = prev_next_nid($nid, 'prev');

    if ($n_nid || $p_nid) {

      if ($p_nid && $prev_display && $prev_text != '') {
        $p_node = node_load($p_nid);
        $link = token_replace($prev_text, array('node' => $p_node));
        $options = array('html' => TRUE);
        $content .= '<li class="prev-next-link-prev">' . l($link, "node/$p_nid", $options) . '</li>';
      }

      if ($n_nid && $next_display && $next_text != '') {
        $n_node = node_load($n_nid);
        $link = token_replace($next_text,  array('node' => $n_node));
        $options = array('html' => TRUE);
        $content .= '<li class="prev-next-link-next">' . l($link, "node/$n_nid", $options) . '</li>';
      }

      $block = array(
        'subject' => t('Prev/Next links'),
        'content' => '<ul class="prev-next-links">' . $content . '</ul>',
      );

    }

  }

  return $block;
}

