<?php

function godwins_law_admin_settings() {
  $form = array();

  $form['godwins_law_description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>'. t('Godwin\'s law states that "As an online discussion grows longer, the probability of a comparison involving Nazis or Hitler approaches 1." It is often used to state that in any given discussion, whoever is the first to make a comparison to Hitler or the Nazis loses the argument, by default. When Godwin\'s Law is invoked on this site, choose how discussions should automatically behave.') . '</p>',
  );

  // Set the default action. By default, it is to close comments for the node.
  $form['godwins_law_action'] = array(
    '#title' => t('Action'),
    '#description' => t('Choose how to respond when Godwin\'s Law has been invoked.'),
    '#type' => 'select',
    '#options' => array(
      GODWINS_LAW_NOACTION => t('No action'),
      COMMENT_NODE_CLOSED => t('Close comments for the node'),
      COMMENT_NODE_HIDDEN => t('Hide all comments for the node'),
    ),
    '#default_value' => variable_get('godwins_law_action', COMMENT_NODE_CLOSED),
  );

  // Choose which keywords invokes Godwin's Law.
  $keywords = variable_get('godwins_law_keywords', array('hitler', 'nazi'));
  $form['godwins_law_keywords'] = array(
    '#title' => t('Keywords'),
    '#description' => t('Set the keywords which will invoke Godwin\'s Law. Seperate keywords with a new line. Spaces are allowed if they are on the same line. Keywords will be case-insensitive.'),
    '#type' => 'textarea',
    '#default_value' => implode("\n", $keywords),
  );

  // Get a list of all content types to choose from
  $types = node_type_get_types();
  $options = array();
  foreach ($types as $machine_name => $type) {
    $options[$machine_name] = $type->name;
  }
  $form['godwins_law_types'] = array(
    '#title' => t('Content types'),
    '#description' => t('Decide which content types should have their comments governed by Godwin\'s Law.'),
    '#type' => 'select',
    '#multiple' => 10,
    '#options' => $options,
    '#default_value' => variable_get('godwins_law_types', array_keys($options)),
  );

  // Set some default messages in the even that Godwin's Law is invoked.
  $form['godwins_law_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#description' => t('Message to display when Godwin\'s Law has been invoked. "%node_title" will be replaced by the node\'s title.'),
    '#default_value' => variable_get('godwins_law_message', t('This comment has invoked Godwin\'s Law. Comments for %node_title are now closed.')),
  );
  $form['godwins_law_message_noaction'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#description' => t('Message to display when Godwin\'s Law has been invoked, but no action has been automatically taken (see above).'),
    '#default_value' => variable_get('godwins_law_message_noaction', t('This comment has invoked Godwin\'s Law. No other action has been taken.')),
  );

  return system_settings_form($form);
}


function godwins_law_admin_settings_validate(&$elements, &$form_state) {
  // Split keywords into an array for system variable storage.
  $elements['godwins_law_keywords']['#value'] = preg_split('/\r\n|\n|\r/', $elements['godwins_law_keywords']['#value'], -1, PREG_SPLIT_NO_EMPTY);
  $form_state['values']['godwins_law_keywords'] = $elements['godwins_law_keywords']['#value'];
}
