<?php
/**
 * @file
 *   Admin page callbacks and related function for the CakeMail Relay module.
 */

/**
 * Form build callback for CakeMail Relay settings.
 */
function cakemail_relay_settings_page($form, &$form_state) {
  $form['cakemail_relay_user_key'] = array(
    '#type' => 'textfield',
    '#title' => t('User key'),
    '#default_value' => isset($form_state['values']['cakemail_relay_user_key']) ? $form_state['values']['cakemail_relay_user_key'] : variable_get('cakemail_relay_user_key'),
    '#ajax' => array(
      'callback' => 'cakemail_relay_settings_ajax',
      'wrapper' => 'cakemail-relay-template-id-wrapper',
      'method' => 'replace',
    ),
  );
  $form['cakemail_relay_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => isset($form_state['values']['cakemail_relay_client_id']) ? $form_state['values']['cakemail_relay_client_id'] : variable_get('cakemail_relay_client_id'),
    '#ajax' => array(
      'callback' => 'cakemail_relay_settings_ajax',
      'wrapper' => 'cakemail-relay-template-id-wrapper',
      'method' => 'replace',
    ),
  );
  $form['cakemail_relay_template_id'] = array(
    '#type' => 'select',
    '#title' => t('Template'),
    '#default_value' => isset($form_state['values']['cakemail_relay_template_id']) ? $form_state['values']['cakemail_relay_template_id'] : variable_get('cakemail_relay_template_id'),
    '#options' => array(),
    '#prefix' => '<div id="cakemail-relay-template-id-wrapper">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'cakemail_relay_settings_ajax',
      'wrapper' => 'cakemail-relay-template-preview-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['cakemail_relay_template_preview'] = array(
    '#type' => 'markup',
    '#markup' => '',
    '#prefix' => '<div id="cakemail-relay-template-preview-wrapper">',
    '#suffix' => '</div>',
  );
  try {
    $response = cakemail_api()->TemplateV2->GetTemplates(array(
      'user_key' => $form['cakemail_relay_user_key']['#default_value'],
      'client_id' => $form['cakemail_relay_client_id']['#default_value'],
    ));
    if ($response->templates) {
      foreach ($response->templates as $template) {
        $form['cakemail_relay_template_id']['#options'][$template->id] = $template->name;
        if ($form['cakemail_relay_template_id']['#default_value'] == $template->id) {
          $form['cakemail_relay_template_preview']['#markup'] = "<img src='{$template->thumbnail}'/>";
        }
      }
    }
  }
  catch (\Drupal\cakemail_api\APIException $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  return system_settings_form($form);
}

function cakemail_relay_settings_ajax($form, $form_state) {
  switch ($form_state['triggering_element']['#name']) {
    case 'cakemail_relay_template_id':
      return $form['cakemail_relay_template_preview'];

    default:
      return $form['cakemail_relay_template_id'];
  }
}