<?php

/**
 * @file
 *  User-facing page callbacks for cd_sunlight.
 */

/**
 * Congress search form builder.
 *
 * @ingroup forms
 *
 * @see cd_sunlight_search_form_validate()
 * @see cd_sunlight_search_form_submit()
 */
function cd_sunlight_search_form($form, &$form_state, $zip = NULL, $state = NULL, $district = NULL) {
  $form['instructions'] = array(
    '#markup' => t('Enter a zip code to find your members of Congress.'),
  );

  // Do we have access to the Congress page view.
  $congress_path = menu_get_item('congress');
  if (!empty($congress_path['access'])) {
    $form['instructions']['#markup'] .= ' ' .
      t('Or <a href="!url">browse all members of Congress</a>.', array('!url' => url('congress')));
  }
  $form['instructions']['#markup'] = '<p>' . $form['instructions']['#markup'] . '</p>';

  $form['zip'] = array(
    '#title' => t('Zip Code (five digits)'),
    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#default_value' => $zip ? $zip : '',
  );
  $form['submit'] = array(
    '#value' => 'Search',
    '#type' => 'submit',
  );

  // The results.
  if (!empty($state)) {
    $t_arguments = array(
      '%zip' => $zip,
      '%cd' => $state . $district,
    );
    $results = '<h3>' . ($zip ? t('%zip is in %cd', $t_arguments) : t('%cd', $t_arguments)) . '</h3>' .
      views_embed_view('congress', 'default', $state, $district);

    // The more link is retaining the arguments.  Get rid of them.
    $form['results']['#markup'] = preg_replace('!/congress.*?"!', '/congress"', $results);
  }

  return $form;
}

/**
 * Congress search form validator.
 *
 * @ingroup forms
 *
 * @see cd_sunlight_search_form()
 * @see cd_sunlight_search_form_submit()
 */
function cd_sunlight_search_form_validate(&$form, &$form_state) {
  if (!is_numeric($form_state['values']['zip'])) {
    form_set_error('zip', t('The Zip code must be numeric.'));
  }
  if (strlen($form_state['values']['zip']) !== 5) {
    form_set_error('zip', t('The Zip code must be five digits.'));
  }

  // Lookup the district.
  // We do this here so that anything that wants to override the submit handler gets this data.
  $cd = cd_sunlight_api_lookup_contact($form_state['values']);
  if ($cd === FALSE) {
    form_set_error('zip', t('Unfortunately our system is currently having dificulties.  Please try again later.'));
    return;
  }
  elseif (empty($cd)) {
    form_set_error('zip', t('We could not determine who your members of Congress are, perhaps this is not a valid Zip code.'));
    return;
  }
  $form_state['values']['cd'] = $cd;
}

/**
 * Congress search form submit handler.
 *
 * @ingroup forms
 *
 * @see cd_sunlight_search_form()
 * @see cd_sunlight_search_form_validate()
 */
function cd_sunlight_search_form_submit(&$form, &$form_state) {

  $zip = $form_state['values']['zip'];
  $state = $form_state['values']['cd']['state'];
  $district = $form_state['values']['cd']['district'];

  // Redisplay the form, with the results.
  $form_state['redirect'] = "congress-search/$zip/$state/$district";

}
