<?php


/**
 * @file
 * recipe.admin.inc -contains all admin pages, settings, and validate.
 *
 * This file will only be parsed when processing admin menu options.
 * See http://drupal.org/node/146172
 */


/**
 * Settings form for menu callback
 */
function recipe_admin_settings() {
  $form['recipe_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#default_value' => variable_get('recipe_help', ''),
    '#cols' => 55,
    '#rows' => 4,
    '#description' => t('This text will be displayed at the top of the recipe submission form. Useful for helping or instructing your users.'),
  );

  // Ingredient section
  $form['recipe_ingredients'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recipe ingredient section'),
  );
  $form['recipe_ingredients']['recipe_fraction_display'] = array(
    '#type' => 'textfield',
    '#title' => t('Fractions display string'),
    '#default_value' => variable_get('recipe_fraction_display', t('{%d }%d&frasl;%d')),
    '#size' => 35,
    '#maxlength' => 255,
    '#description' => t('How fractions should be displayed. Leave blank to display as decimals. Each incidence of %d will be replaced by the whole number, the numerator, and the denominator in that order. Anything between curly braces will not be displayed when the whole number is equal to 0. Recommended settings are "{%d }%d&amp;frasl;%d" or "{%d }&lt;sup&gt;%d&lt;/sup&gt;/&lt;sub&gt;%d&lt;/sub&gt;"'),
  );
  $form['recipe_ingredients']['recipe_unit_display'] = array(
    '#type' => 'radios',
    '#title' => t('Ingredient unit display'),
    '#default_value' => variable_get('recipe_unit_display', 0),
    '#options' => array(t('Abbreviation'), t('Full name')),
    '#description' => t('Display ingredient units like Tbsp or Tablespoon.'),
    '#required' => TRUE,
  );

  // System of measurement section
  $form['recipe_ingredients']['system_of_measurement'] = array(
    '#type' => 'fieldset',
    '#title' => t('System of measurement'),
  );
  $form['recipe_ingredients']['system_of_measurement']['recipe_preferred_system_of_measure'] = array(
    '#type' => 'radios',
    '#title' => t('Preferred system of measure'),
    '#default_value' => variable_get('recipe_preferred_system_of_measure', 0),
    '#options' => array(t('U.S. customary units'), t('SI/Metric')),
    '#description' => t('Which system of measure should be preferred where it is ambiguous.'),
    '#required' => TRUE,
  );
  $form['recipe_ingredients']['system_of_measurement']['recipe_default_unit'] = array(
    '#type' => 'select',
    '#title' => t('Default unit type for ingredients'),
    '#default_value' => variable_get('recipe_default_unit', ''),
    '#options' => recipe_unit_options(),
    '#description' => t('The default unit for new ingredients on the recipe edit screen.'),
  );
  $form['recipe_ingredients']['system_of_measurement']['recipe_preferred_system_of_measure_limit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit UI to the preferred system of measure'),
    '#default_value' => variable_get('recipe_preferred_system_of_measure_limit', 0),
    '#return_value' => 1,
    '#description' => t('Limit unit selectbox to only preferred system of measure.  Does not affect import routines.'),
  );

  $form['recipe_ingredients']['recipe_ingredient_name_normalize'] = array(
    '#type' => 'radios',
    '#title' => t('Ingredient name normalization'),
    '#default_value' => variable_get('recipe_ingredient_name_normalize', 0),
    '#options' => array(t('Leave as entered'), t('Convert to lowercase')),
    '#description' => t('When recipes are entered, should ingredient names be converted or left alone?'),
    '#required' => TRUE,
  );
  $form['recipe_ingredients']['recipe_add_more_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Add more ingredient count'),
    '#default_value' => variable_get('recipe_add_more_count', 5),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('How many ingredients to add when you click "more ingredients" on the recipe edit screen.'),
  );


  // Summary Section
  $form['recipe_summary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recipe summary'),
    '#description' => t('The recipe summary contains the yield, source, and prep time values.')
  );
  $form['recipe_summary']['recipe_summary_location'] = array(
    '#type' => 'radios',
    '#title' => t('Recipe summary location'),
    '#return_value' => 1,
    '#default_value' => variable_get('recipe_summary_location', 0),
    '#options' => array(t('Node content'), t('Block'), t('Hidden')),
    '#description' => t('Where to show the recipe summary information.'),
    '#required' => TRUE
  );
  $form['recipe_summary']['recipe_summary_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipe summary title'),
    '#default_value' => variable_get('recipe_summary_title', t('Summary')),
    '#size' => 35,
    '#maxlength' => 255,
    '#description' => t('The title shown above the recipe summary.'),
  );


  $form['recipe_recent_box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recent recipe box'),
  );
  $form['recipe_recent_box']['recipe_recent_box_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show recent recipes box'),
    '#return_value' => 1,
    '#default_value' => variable_get('recipe_recent_box_enable', 1),
    '#description' => t('Show the recent recipes box on the recipes menu page.'),
    '#required' => FALSE,
  );
  $form['recipe_recent_box']['recipe_recent_box_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Box title'),
    '#default_value' => variable_get('recipe_recent_box_title', t('Latest recipes')),
    '#size' => 35,
    '#maxlength' => 255,
    '#description' => t('Title of the recent recipes box.'),
  );
  $form['recipe_recent_box']['recipe_recent_display'] = array(
    '#type' => 'select',
    '#title' => t('Recipes to display'),
    '#default_value' => variable_get('recipe_recent_display', 5),
    '#options' => drupal_map_assoc(array(0, 5, 10, 15)),
    '#description' => t('Sets the number of recent recipes that will be displayed in the Recent Recipes box. (0 = not displayed).'),
  );

  return system_settings_form($form);
}


function recipe_export_multi($type = NULL) {

  // load supported formats
  $formats = module_invoke_all('recipeio', 'export_multi');
  $o = t('Supported bulk output formats:');
  if ( $type === NULL ) {
    foreach ( $formats as $key => $format ) {
      $format_count = 0;
      if ($format) {
        $o .= '<br/>' . l($format['format_name'], "admin/content/recipe/export_multi/$key");
        $format_count++;
      }
    }
    if ( $format_count == 0 ) {
      $o .= '<br/><p>' . t('You have no export formats available with the bulk export feature.') . '</p>';
    }
    return $o;
  }

  // normalize typed urls
  $type = drupal_strtolower($type);

  // If callback exists, call it, otherwise error out.
  if (isset($formats[$type]) && function_exists($formats[$type]['callback'])) {
    $o = call_user_func($formats[$type]['callback']);
    return $o;
  }
  else {
    drupal_set_message(t('Unknown export format(%the_format).', array('%the_format' => $type)), 'error');
    drupal_not_found();
  }
}

function recipe_import_multi($type = NULL) {

  // load supported formats
  $formats = module_invoke_all('recipeio', 'import_multi');
  $o = t('Supported bulk input formats:');
  if ( $type === NULL ) {
    $format_count = 0;
    foreach ( $formats as $key => $format ) {
      if ($format) {
        $o .= '<br/>' . l($format['format_name'], "admin/content/recipe/import_multi/$key");
        $format_count++;
      }
    }
    if ( $format_count == 0 ) {
      $o .= '<br/><p>' . t('You have no import formats available with the bulk export feature.') . '</p>';
    }
    return $o;
  }

  // normalize typed urls
  $type = drupal_strtolower($type);

  // If callback exists, call it, otherwise error out.
  if (isset($formats[$type]) && function_exists($formats[$type]['callback'])) {
    $o = call_user_func($formats[$type]['callback']);
    return $o;
  }
  else {
    drupal_set_message(t('Unknown export format(%the_format).', array('%the_format' => $type)), 'error');
    drupal_not_found();
  }
}

