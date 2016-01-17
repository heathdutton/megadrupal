<?php

/**
 * @file
 * Form builder and submit functions for the character edit form.
 */

/**
 * Form constructor for the character edit form.
 *
 * Enables/disables a character, changes its css class.
 *
 * @param object $char
 *   A character object.
 *
 * @see iconfonts_menu()
 * @see iconfonts_char_edit_form_submit()
 * @ingroup forms
 */
function iconfonts_char_edit_form($form, $form_state, $char) {

  iconfonts_enable_all_chars(TRUE);

  $form = array(
    'cid' => array(
      '#type' => 'hidden',
      '#value' => $char->cid,
    ),
    'fid' => array(
      '#type' => 'hidden',
      '#value' => $char->fid,
    ),
    'character' => array(
      '#type' => 'item',
      '#title' => t('Edit this character'),
      // Loads the appropriate css file in order to display the icon here.
      '#markup' => '<div class="' . iconfonts_get_generic_char_class($char) . ' icon"></div>',
    ),
    'css_selector' => array(
      '#type' => 'textarea',
      '#title' => t('CSS selector'),
      '#resizable' => FALSE,
      '#rows' => 3,
      '#default_value' => $char->css_selector,
      '#description' => t('Use commas or newlines to separate multiple selectors.
      If you don\'t append <em>:before</em> or <em>:after</em>, <em>:before</em> will be auto-appended.'),
    ),
    'enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $char->enabled,
    ),
    'buttons' => array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save character settings'),
      ),
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel changes'),
      ),
    ),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'iconfonts') . '/iconfonts-admin.css';

  return $form;

}

/**
 * Form submission handler for iconfonts_char_edit_form().
 */
function iconfonts_char_edit_form_submit($form, &$form_state) {

  if ($form_state['clicked_button']['#value'] == t('Cancel changes')) {

    drupal_goto('admin/appearance/fontyourface/edit/' . $form_state['values']['fid']);

  }

  // Retrives the character data from the database in order to compare database
  // values and form_state values.
  $char = iconfonts_get_char($form['cid']['#value']);

  if ($form['enabled']['#value'] > $char->enabled) {

    iconfonts_enable_char($char);

  }

  elseif ($form['enabled']['#value'] < $char->enabled) {

    iconfonts_disable_char($char);

  }

  if ($char->css_selector != $form['css_selector']['#value']) {

    iconfonts_set_css_selector($char, $form['css_selector']['#value']);

  }

}
