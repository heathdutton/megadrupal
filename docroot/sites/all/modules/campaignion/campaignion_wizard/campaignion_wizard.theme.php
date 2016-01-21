<?php
/**
 * Theme callback implementation
 * Copied from webform.emails.inc and adapted
 */
function theme_campaignion_wizard_email_form($variables) {
  $form = $variables['form'];

  // Loop through fields, rendering them into radio button options.
  foreach (array('from_address', 'from_name') as $field) {

    $form[$field . '_custom']['#attributes']['class'] = array('webform-set-active');
    $form[$field . '_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
    $form[$field . '_option']['custom']['#inline_element'] = drupal_render($form[$field . '_custom']);

    if (isset($form[$field . '_option']['#options']['default'])) {
      $form[$field . '_option']['default']['#theme_wrappers'] = array('webform_inline_radio');
    }
  }

  $details = '';
  $details .= drupal_render($form['from_address_option']);
  $details .= drupal_render($form['from_name_option']);
  $details .= drupal_render($form['subject_custom']);

  $form['details'] = array(
    '#type'        => 'fieldset',
    '#title'       => '',
    '#weight'      => 10,
    '#children'    => $details,
    '#collapsible' => FALSE,
    '#parents'     => array('details'),
    '#groups'      => array('details' => array()),
    '#attributes'  => array(),
  );

  // Ensure templates are completely hidden.
  $form['templates']['#prefix'] = '<div id="webform-email-templates" style="display: none">';
  $form['templates']['#suffix'] = '</div>';

  // Re-sort the elements since we added the details fieldset.
  $form['#sorted'] = FALSE;
  $children = element_children($form, TRUE);
  return drupal_render_children($form, $children);

}

