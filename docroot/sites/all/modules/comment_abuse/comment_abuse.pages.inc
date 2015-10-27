<?php

/**
 * @file
 * Provides page callback for COMMENT ABUSE module.
 */

/**
 * Callback function for abuse comment.
 */
function comment_abuse_content($cid, $form_state) {

  // Create ajax commands.
  $commands = array();

  $abuse_result = FALSE;
  if (!empty($cid)) {
    // Add new abuse.
    $abuse_result = comment_abuse_add_comment_abuse($cid, $form_state);
  }

  if ($abuse_result) {
    // Translate message.
    $message = comment_abuse_get_text_for('success');

    // Replace abuse link with recieved message.
    $commands[] = ajax_command_replace(
      '.comment-abuse-' . $cid,
      '<span class="abuse-notice">' . check_plain($message) . '</span>'
    );

    // Invoke rules event.
    if (module_exists('rules')) {
      $comment = comment_load($cid);
      rules_invoke_event('complaint_insert', $comment);
    }
  }
  else {
    // Translate message.
    $message = comment_abuse_get_text_for('fail');

    // Replace abuse link with recieved message.
    $commands[] = ajax_command_replace(
      '.comment-abuse-' . $cid,
      '<span class="abuse-notice">' . check_plain($message) . '</span>'
    );
  }

  // Remove old error messages.
  $commands[] = ajax_command_remove('#modal-content .messages');

  // Complaint form mode.
  if (variable_get('comment_abuse_use_popup', 1)) {    
    $commands[] = ajax_command_replace(
      '#complaint-form-wrapper',
      theme(
        'comment_abuse_complaint_popup_result',
        array(
          'message' => check_plain($message),
          'result' => $abuse_result,
        )
      )
    );    

    print ajax_render($commands);
    drupal_exit();
  }
  // Single link mode.
  else {
    return array('#type' => 'ajax', '#commands' => $commands);
  }
}

/**
 * Return popup with form for complaint on comment.
 */
function comment_abuse_complaint_popup($js = NULL, $cid) {

  if(!$js) {
    return drupal_get_form('comment_abuse_complaint_form', array('cid' => $cid));
  }
  // adelante!
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => comment_abuse_get_text_for('popup_title'),
    'ajax' => TRUE,
    'build_info' => array('args' => array(0 => array('cid' => $cid))),
  );
  $output = ctools_modal_form_wrapper('comment_abuse_complaint_form', $form_state);  
  if (empty($form_state['executed'])) {    
    print ajax_render($output);
    exit;
  } 
}
