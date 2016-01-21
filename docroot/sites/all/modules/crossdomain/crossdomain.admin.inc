<?php

/**
 * Manages the domains for the crossdomain.xml file.
 */
function crossdomain_form($form, &$form_state) {
  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['domains_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domains to Allow For Flash Files'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="domains-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  $form['domains_fieldset']['description'] = array(
    '#markup' => 'Add domains that will be used to build a crossdomain.xml file so flash files can use it.',
  );

  // Get our current domains and set the number of domains.
  $domains = variable_get('crossdomain', array());
  if (empty($form_state['input']) && !empty($domains)) {
    $form_state['num_domains'] = count($domains);
  }
  else if (!isset($form_state['num_domains'])) {
    $form_state['num_domains'] = 1;
  }

  // Loop through all the current number of fields and build the form.
  for ($i = 0; $i < $form_state['num_domains']; $i++) {

    // Makes our fields inline so remove is next to the input field.
    $form['domains_fieldset']['element'][$i] = array(
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );

    // Field where the domain will be entered.
    $form['domains_fieldset']['element'][$i]['domain'] = array(
      '#type' => 'textfield',
    );

    if (empty($form_state['input']) && !empty($domains)) {
      $form['domains_fieldset']['element'][$i]['domain']['#default_value'] = $domains[$i];
    }

    if ($form_state['num_domains'] > 1) {
      $form['domains_fieldset']['element'][$i]['remove_button'] = array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#name' => 'remove-button-' . $i,
        '#submit' => array('crossdomain_form_remove_one'),
        '#ajax' => array(
          'callback' => 'crossdomain_form_callback',
          'wrapper' => 'domains-fieldset-wrapper',
        ),
      );
    }
  }
  $form['domains_fieldset']['add_domain'] = array(
    '#type' => 'submit',
    '#value' => t('Add another'),
    '#submit' => array('crossdomain_form_add_one'),
    '#ajax' => array(
      'callback' => 'crossdomain_form_callback',
      'wrapper' => 'domains-fieldset-wrapper',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the domains in it.
 */
function crossdomain_form_callback($form, $form_state) {
  return $form['domains_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function crossdomain_form_add_one($form, &$form_state) {
  $form_state['num_domains']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function crossdomain_form_remove_one($form, &$form_state) {
  // This is the id of the element to remove.
  $element_id = $form_state['clicked_button']['#parents'][2];

  if ($form_state['num_domains'] > 1) {
    $form_state['num_domains']--;
  }

  // Unset the values and reindex the variables.
  unset($form_state['values']['domains_fieldset']['element'][$element_id]);
  $form_state['values']['domains_fieldset']['element'] = array_values($form_state['values']['domains_fieldset']['element']);
  unset($form_state['input']['domains_fieldset']['element'][$element_id]);
  $form_state['input']['domains_fieldset']['element'] = array_values($form_state['input']['domains_fieldset']['element']);

  // Rebuild the form.
  $form_state['rebuild'] = TRUE;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function crossdomain_form_submit($form, &$form_state) {
  // Loop through domains and set our variable.
  $items = array();
  foreach($form_state['values']['domains_fieldset']['element'] as $field) {
    if (!empty($field['domain'])) {
      $items[] = $field['domain'];
    }
  }

  variable_set('crossdomain', $items);
}
