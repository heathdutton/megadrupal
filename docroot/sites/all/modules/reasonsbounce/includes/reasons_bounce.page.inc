<?php

/**
 * Page callback.
 */
function reasons_bounce_webform_popup_content($js = NULL, $node) {
  $submission =  (object) array(); // empty object required by webform
  $webform_nid = $node->nid;
   
  // React without the modal
  if (!$js) {
    // Webform requires more parameters than standard forms
    return drupal_get_form('webform_client_form_' . $webform_nid, $node, $submission);
  }

  // React with the modal
  // Add modal components
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => $node->title,
    'ajax' => TRUE,
  );
  // Emulate ctools_modal_form_wrapper() form modal.inc because webform can not be triggered trough drupal_build_form
  // If it can, I'd be glad to understand how
  $form_state += array(
    're_render' => FALSE,
    'no_redirect' => !empty($form_state['ajax']),
  );

  // Fetch webform form stuff
  $output = drupal_get_form('webform_client_form_' . $webform_nid, $node, $submission);
  // Merge node content (except webform) and webform form before rendering   
  $node_view = node_view($node); // Prepare webform for rendering
  $node_view['body']['#weight'] = -100; // Ensure node body is at the top
  unset($node_view['webform']); // Prevent displaying twice the webform
  array_unshift($output['submitted'], $node_view['body']); // Merge
  $node->title = ''; // Remove title to prevent displaying twice

  if (!empty($form_state['ajax'])) {
    $output = ctools_modal_form_render($form_state, $output);
  }
  
  // Handle successful submission through session flag - see 'webform_popup_custom_webform_submit' function below
  if (isset($_SESSION['webform_client_form_'.$webform_nid]) && $_SESSION['webform_client_form_'.$webform_nid] == 'submitted') {
    // Delete session flag   
    unset($_SESSION['webform_client_form_'.$webform_nid]);
    // Fetch confirmation message. It will be displayed in the modal window
    $confirmation['#markup'] =
      '<div class="popups-confirmation-wrapper">'.
      check_markup($node_view['#node']->webform['confirmation'], $node_view['#node']->webform['confirmation_format'], '', TRUE).
      '</div>';   
    $output = array(); // Recreate output
    // Overwrite the form output if it was successful.
    $output[] = ctools_modal_command_display('Confirmation', $confirmation);
  }
  
  // Render output in modal window
  print ajax_render($output);
  exit;
}