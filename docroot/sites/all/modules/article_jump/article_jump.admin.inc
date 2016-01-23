<?php
/**
 * @file
 * Article Jump module settings UI.
 */

/**
 * Creates module configuration form.
 */
function article_jump_admin_settings() {
  $form = array();
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Article Jump Settings'),
  );

  // The jquery selector, ie $('.content article').
  $form['display']['article_jump_article_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Article Jump jQuery Selector'),
    '#description' => t("The jQuery Selector(s) to use. Separate selectors with a comma. Do not include quotes."),
    '#default_value' => variable_get('article_jump_article_selector', ".content article, .content .node-teaser"),
  );

  $form['display']['article_jump_next_hotkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Scroll to Next Article Hotkey'),
    '#description' => t("The key combination that will trigger scrolling to previous article. Can use shift, ctrl, alt in combination with other keys: alt+n"),
    '#default_value' => variable_get('article_jump_next_hotkey', "j"),
  );

  $form['display']['article_jump_prev_hotkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Scroll to Previous Article Hotkey'),
    '#description' => t("The key combination that will trigger scrolling to previous article. Can use shift, ctrl, alt in combination with other keys: alt+p"),
    '#default_value' => variable_get('article_jump_prev_hotkey', "k"),
  );

  // The Scrolling Speed.
  $form['display']['article_jump_scroll_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Scroll Speed in Milliseconds'),
    '#description' => t("How many milliseconds to scroll to the next article."),
    '#default_value' => variable_get('article_jump_scroll_speed', '500'),
  );

  return system_settings_form($form);
}

/**
 * Validates the configuration form.
 */
function article_jump_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['article_jump_scroll_speed'] < 0) {
    form_set_error('article_jump_scroll_speed', t('You must enter a speed greater than 0.'));
  }
  if (preg_match('/"|;|\(|\)|\$/', $form_state['values']['article_jump_article_selector'])) {
    form_set_error('article_jump_article_selector', t('jQuery selectors must not include quotes, semicolons, dollar signs or parenthesis.'));
  }
}
