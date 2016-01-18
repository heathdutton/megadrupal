<?php

/**
 * @file
 * lom resource add/edit form part: for contact info tab
 */

/**
 * part of lom resource add/edit form
 *  tab: contact info
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_proposal_contact_info(&$form, &$form_state, &$lom, $language) {
  if (archibald_is_proposal($lom)) {

    $form['contactInfo'] = array(
      '#type' => 'fieldset',
      '#title' => ($lom->getLomId() != '') ? t('Proposal contact') : t('Contact information'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
    );


    $form['contactInfo']['proposalContactEmail'] = array(
      '#type' => 'textfield',
      '#title' => archibald_get_field_label('contactInfo.proposalContactEmail'),
      '#maxlength' => 50,
      '#required' => FALSE,
    );

    if ($lom->getLomId() != '') {
      $lom_stats = archibald_get_lom_stats($lom->getLomId(), TRUE);

      $form['contactInfo']['proposalContactEmail'] = array(
        '#type' => 'item',
        '#title' => archibald_get_field_label('contactInfo.proposalContactEmail'),
        '#markup' => $lom_stats->proposal_contact_email,
      );
      $form['contactInfo']['proposalContactName'] = array(
        '#type' => 'item',
        '#title' => archibald_get_field_label('contactInfo.proposalContactName'),
        '#markup' => $lom_stats->proposal_contact_name,
      );
      $form['contactInfo']['proposalContactPhone'] = array(
        '#type' => 'item',
        '#title' => archibald_get_field_label('contactInfo.proposalContactPhone'),
        '#markup' => $lom_stats->proposal_contact_phone,
      );
    }
    else {
      $form['contactInfo']['proposalContactEmail'] = array(
        '#type' => 'textfield',
        '#title' => archibald_get_field_label('contactInfo.proposalContactEmail') . archibald_get_require_type_value('contactInfo.proposalContactEmail'),
        '#maxlength' => 50,
        '#default_value' => '',
        '#required' => FALSE,
      );

      $form['contactInfo']['proposalContactName'] = array(
        '#type' => 'textfield',
        '#title' =>  archibald_get_field_label('contactInfo.proposalContactName') . archibald_get_require_type_value('contactInfo.proposalContactName'),
        '#default_value' => '',
        '#maxlength' => 100,
        '#required' => FALSE,
      );

      $form['contactInfo']['proposalContactPhone'] = array(
        '#type' => 'textfield',
        '#title' =>  archibald_get_field_label('contactInfo.proposalContactPhone') . archibald_get_require_type_value('contactInfo.proposalContactPhone'),
        '#default_value' => '',
        '#maxlength' => 100,
        '#required' => FALSE,
      );
    }

    if (!archibald_can_only_propose()) {
      $form['contactInfo']['addVCard'] = array(
        '#markup' => '<a class="button ctools-use-modal" href="' . url('archibald_contributor/add/chooser/nojs') . '">' . t('Add vCard') . '</a>',
        '#required' => FALSE,
      );
    }
  }
}