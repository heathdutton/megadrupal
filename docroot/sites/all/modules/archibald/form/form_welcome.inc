<?php

/**
 * @file
 * lom resource add/edit form part: welcome message for anonymous users
 */

/**
 * part of lom resource add/edit form
 *  welcome message
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_welcome(&$form, &$form_state, &$lom, $language) {
  if (archibald_form_is_proposal(empty($form_state['values']['lom_id']))) {
    $message = t('You will be notified by E-Mail about the progress of your proposal.') . '<br /> <span class="form-required">*</span> ' . t('Mandatory fields') . ' <br> <span class="form-recommended">*</span> ' . t('Recommended fields');

    $form['welcome'] = array(
      '#type' => 'fieldset',
      '#title' => t('Please complete the form below'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#weight' => 98,
    );

    $form['welcome']['message'] = array(
      '#markup' => variable_get('contact_form_welcome', $message),
    );

  }

}
