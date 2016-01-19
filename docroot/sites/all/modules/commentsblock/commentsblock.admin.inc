<?php
/*
* @file
* Administration page callbacks for the commentsblock.module.
*/

/**
 * Menu callback setting
 */

function commentsblock_settings_form() {

  // Do we render comment form only or complete form+comments?
  $options = array(
    0 => t('Load comments + comment form into a block'),
    1 => t('Only load the comment form'),
  );

  $form['commentsblock_display'] = array(
    '#type' => 'select',
    '#title' => t('Render comment form only?'),
    '#description' => t('Should the comments be shown together with the form?'),
    '#options' => $options,
    '#default_value' => variable_get('commentsblock_display', $options[0]),
  );
  $form['commentsblock_display_info'] = array(
    '#type' => 'item',
    '#title' => t('Info'),
    '#markup' => '<p>If you choose <em>Only load the comment form</em>, <strong>ONLY</strong> the comment form is rendered into a block. You can then use the view shipped with the module to display the comments. Otherwise no comments will be shown.</p>',
  );

  $form['commentsblock_display_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide "Add new comment"'),
    '#description' => t('Hides "Add new comment" at the node links. The "Add new comment" only appears if a minimum of one comment has already been posted'),
    '#default_value' => variable_get('commentsblock_display_link', 0),
  );

  return system_settings_form($form);
}

