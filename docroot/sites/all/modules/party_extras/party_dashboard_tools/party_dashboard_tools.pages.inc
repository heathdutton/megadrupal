<?php

/**
 * Page callback for the attached entity edit form modal.
 *
 * @param $party
 * @param $party_data_set
 * @param $delta
 * @param $js
 */
function party_dashoard_tools_data_set_edit_modal($js, $party, $data_set, $delta = NULL) {
  if (!$js) {
    if (isset($delta)) {
	    return drupal_goto('party/' . $party->pid . '/' . $data_set['path element'] . '/edit/' . $delta);
	  }
    else {
      return drupal_goto('party/' . $party->pid . '/' . $data_set['path element'] . '/add');
    }
  }

  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('party.pages', 'party', '');

  $form_state = array(
    'title' => t('Edit @label', array('@label' => $data_set['label'])),
	  'ajax' => TRUE,
	  'build_info' => array(
	    'args' => array(
	      $party,
		    $data_set,
		    $delta,
	    ),
    ),
  );

  $commands = ctools_modal_form_wrapper('party_attached_entity_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Overwrite the output if form submission was successfully executed.
    $commands = array();
    $commands[] = ctools_ajax_command_reload();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}