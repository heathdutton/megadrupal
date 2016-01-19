<?php
/**
 * @file
 * Subs Premium admin UI.
 */

/**
 * Form callback.
 * Subs Premium settings form.
 *
 * @see subs_premium_menu()
 */
function subs_premium_settings_form() {
  $form = array();

  // Check for compatible modules before doing anything else.
  $profile2 = module_exists('profile2');
  $nopremium = module_exists('nopremium');
  // @TODO support Organic Groups?

  if (!$profile2 && !$nopremium) {
    $form['empty'] = array(
      '#markup' => t("In order to take advantage of Subs Premium and allow access to restricted content based on a user's subscription status, you must use one of the following modules: <a href=\"@nopremium\">Node Option Premium</a>, <a href=\"@profile2\">Profile2</a> or <a href=\"@og\">Organic Groups</a>.", array('@profile2' => 'http://drupal.org/project/profile2', '@nopremium' => 'http://drupal.org/project/nopremium', '@og' => 'http://drupal.org/project/og')),
    );
  }

  // Load all subscription types.
  $types = subs_get_types();

  // Support revealing private profile fields to users with active subscriptions.
  if ($profile2) {
    $form['profile2'] = array(
      '#type' => 'fieldset',
      '#title' => t('Profile2'),
      '#description' => t('Integration with @profile2 module. Display private profile fields to users with active subscriptions.', array('@profile2' => t('Profile2'))),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $profiles = profile2_get_types();

    foreach ($types as $name => $type) {
      $form['profile2'][$name] = array(
        '#type' => 'fieldset',
        '#title' => $type->label,
        '#description' => t("Select which profiles users with an active @name subscription can view private fields from.", array('@name' => $type->label)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      foreach ($profiles as $profile_name => $profile_type) {
        $varname = "subs_premium_profile_{$name}_{$profile_name}";

        $form['profile2'][$name][$varname] = array(
          '#title' => $profile_type->label,
          '#type' => 'checkbox',
          '#default_value' => variable_get($varname, FALSE),
        );
      }
    }
  }

  if ($nopremium) {
    $form['nopremium'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Option Premium'),
      '#description' => t('Integration with @nopremium module. Let users with active subscriptions view premium content.', array('@nopremium' => t('Node Option Premium'))),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $content_types = node_type_get_types();

    foreach ($types as $name => $type) {
      $form['nopremium'][$name] = array(
        '#type' => 'fieldset',
        '#title' => $type->label,
        '#description' => t("Select which type of premium content users with an active @name subscription can view.", array('@name' => $type->label)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      foreach ($content_types as $type_name => $content_type) {
        $varname = "subs_premium_content_{$name}_{$type_name}";

        $form['nopremium'][$name][$varname] = array(
          '#title' => $content_type->name,
          '#type' => 'checkbox',
          '#default_value' => variable_get($varname, FALSE),
        );
      }
    }
  }

  return system_settings_form($form);
}