<?php
// $Id$

/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'markov_chain',
  'access' => 'administer markov chains',
  'menu' => array(
    'menu item' => 'markov_chain',
    'menu title' => 'Markov Chains',
    'menu description' => 'Create, update and delete Markov chains.',
  ),

  'title singular' => t('chain'),
  'title plural' => t('chains'),
  'title singular proper' => t('Markov chain'),
  'title plural proper' => t('Markov chains'),

  // - Abbreviated. - //

  'form' => array(
    'settings' => 'markov_chains_ui_form',
    'validate' => 'markov_chains_ui_form_validate',
    'submit' => 'markov_chains_ui_form_submit',
  ),
);

/**
* Define the preset add/edit form.
*/
function markov_chains_ui_form(&$form, &$form_state) {

  $chain = $form_state['item'];

  $chain->settings['view'] = isset($chain->settings['view']) ? $chain->settings['view'] : array();
  $chain->settings['field'] = isset($chain->settings['field']) ? $chain->settings['field'] : array();
  $chain->settings['persist'] = isset($chain->settings['persist']) ? $chain->settings['persist'] : TRUE;
  $chain->settings['context'] = isset($chain->settings['context']) ? $chain->settings['context'] : 2;
  $chain->settings['size_low'] = isset($chain->settings['size_low']) ? $chain->settings['size_low'] : 5;
  $chain->settings['size_high'] = isset($chain->settings['size_high']) ? $chain->settings['size_high'] : 10;

  $views = array_keys(views_get_all_views());
  $views = array_combine($views, $views);

  $fields = array_keys(field_read_fields());
  $fields = array_combine($fields, $fields);

  $form['view'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#description' => t('.'),
    '#default_value' => $chain->settings['view'],
    '#options' => $views,
    '#required' => TRUE,
  );

  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#description' => t('The field.'),
    '#default_value' => $chain->settings['field'],
    '#options' => $fields,
    '#required' => TRUE,
  );

  $form['persist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save'),
    '#description' => t('Whether to save the generated chain.'),
    '#default_value' => $chain->settings['persist'],
  );

  $form['context'] = array(
    '#type' => 'textfield',
    '#title' => t('Context'),
    '#description' => t(''),
    '#default_value' => $chain->settings['context'],
    '#required' => TRUE,
  );

  $form['size_low'] = array(
    '#type' => 'textfield',
    '#title' => t('Low Generate Bound'),
    '#description' => t(''),
    '#default_value' => $chain->settings['size_low'],
    '#required' => TRUE,
  );

  $form['size_high'] = array(
    '#type' => 'textfield',
    '#title' => t('High generate bound'),
    '#description' => t(''),
    '#default_value' => $chain->settings['size_high'],
    '#required' => TRUE,
  );
}

/**
 * Validate handler.
 */
function markov_chains_ui_form_validate($form, &$form_state) {
  $item = $form_state['item'];
  $values = $form_state['values'];
  $item->settings['view'] = $values['view'];
  $item->settings['persist'] = (bool) $values['persist'];
  $item->settings['context'] = $values['context'];
  $item->settings['size_low'] = $values['size_low'];
  $item->settings['size_high'] = $values['size_high'];
  $item->settings['field'] = $values['field'];

  if ($values['persist']) {
    $m = new MarkovChain($values['context']);
    $m->load_from_view($values['view'], $values['field']);
    $item->chain = $m->get_table();
  }
}

/**
* Submit handler for the edit form.
*/
function markov_chains_ui_form_submit($form, &$form_state) {

}
