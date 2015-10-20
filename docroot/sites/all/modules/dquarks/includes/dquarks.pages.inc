<?php

/**
 * @file
 * Menu callbacks and functions for configuring and editing dquarkss.
 */

/**
 * Theme the redirection setting on the dquarks node form.
 */
function theme_dquarks_advanced_redirection_form($variables) {
  $form = $variables['form'];
  // Add special class for setting the active radio button.
  $form['redirect_url']['#attributes']['class'] = array('dquarks-set-active');

  // Remove title and description for Redirect URL field.
  $form['redirect_url']['#title'] = NULL;
  $form['redirect_url']['#description'] = NULL;

  $form['redirect']['confirmation']['#theme_wrappers'] = array('dquarks_inline_radio');
  $form['redirect']['url']['#theme_wrappers'] = array('dquarks_inline_radio');
  $form['redirect']['none']['#theme_wrappers'] = array('dquarks_inline_radio');
  $form['redirect']['url']['#inline_element'] = $form['redirect']['url']['#title'] . ': ' . drupal_render($form['redirect_url']);
  $form['redirect']['url']['#title'] = NULL;

  return drupal_render_children($form);
}

/**
 * Theme the total submit limit fieldset on the dquarks node form.
 */
function theme_dquarks_advanced_total_submit_limit_form($variables) {
  $form = $variables['form'];
  $form['total_submit_limit']['#attributes']['class'] = array('dquarks-set-active');
  $form['total_submit_interval']['#attributes']['class'] = array('dquarks-set-active');
  // Remove div wrappers around limit options.
  $form['total_submit_limit']['#theme_wrappers'] = array();
  $form['total_submit_interval']['#theme_wrappers'] = array();
  $replacements = array(
    '!count' => drupal_render($form['total_submit_limit']),
    '!timespan' => drupal_render($form['total_submit_interval']),
  );

  $form['enforce_total_limit']['no']['#theme_wrappers'] = array('dquarks_inline_radio');
  $form['enforce_total_limit']['yes']['#title'] = NULL;
  $form['enforce_total_limit']['yes']['#inline_element'] = t('Limit to !count total submission(s) !timespan', $replacements);
  $form['enforce_total_limit']['yes']['#theme_wrappers'] = array('dquarks_inline_radio');

  return drupal_render_children($form);
}
