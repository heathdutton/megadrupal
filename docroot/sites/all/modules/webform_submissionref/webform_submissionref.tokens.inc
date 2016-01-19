<?php

/**
 * @file
 * Builds placeholder replacement tokens for Webform Submission Reference.
 */

/**
 * Implements hook_token_info().
 */
function webform_submissionref_token_info() {
  $info['tokens']['submission']['referenced-values'] = array(
    'name' => t('Webform submission referenced values'),
    'description' => t('Webform tokens for data referenced by a Webform Submission component. Replace "?" with "source_key:target_key" where "source_key" is the Field Key of the Webform Submission component and "target_key" is the Field Key of a component on the target webform.'),
    'dynamic' => TRUE,
  );
  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @see webform_tokens().
 */
function webform_submissionref_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Get [submission:*] tokens.
  if ($type == 'submission' && !empty($data['webform-submission'])) {
    $submission = $data['webform-submission'];
    $node = isset($data['node']) ? $data['node'] : node_load($submission->nid);
    $email = isset($data['webform-email']) ? $data['webform-email'] : NULL;
    $format = !empty($options['sanitize']) ? 'html' : 'text';

    // Get [submission:referenced-values:*] tokens.
    if ($reference_tokens = token_find_with_prefix($tokens, 'referenced-values')) {
      foreach ($node->webform['components'] as $cid => $component) {
        // Get [submission:referenced-values:my_component:*] tokens.
        if ($component_tokens = token_find_with_prefix($reference_tokens, $component['form_key'])) {
          foreach ($component_tokens as $name => $original) {
            // Get [submission:referenced-values:my_submissionref_component:*] tokens.
            if ($component['type'] == 'submissionref') {
              // Get target submission.
              list($target_nid) = unserialize($component['value']);
              $target_sid = $submission->data[$cid][0];
              if (empty($target_sid)) {
                break;
              }
              $target_submission = webform_get_submission($target_nid, $target_sid);
              // Get target webform.
              // No need to check if the webform actually exists.
              // The component stores only valid webform nids.
              $target_webform = node_load($target_nid);
              // Get target component.
              $target_cid = _webform_submissionref_get_cid($target_webform, $name);
              if (empty($target_cid)) {
                break;
              }
              // Replace token by value of referenced submission/component.
              $target_renderable = webform_submission_render($target_webform, $target_submission, $email, $format);
              $target_renderable[$name]['#access'] = TRUE;
              $target_renderable[$name]['#theme_wrappers'] = array();
              $replacements[$original] = render($target_renderable[$name]);
            }
          }
        }
      }
    }
  }

  return $replacements;
}

/**
 * Given a webform and the machine name of a component, return component id.
 *
 * @param $webform
 *   Webform node.
 * @param $name
 *   Component name.
 *
 * @return
 *   Component id if exists. FALSE otherwise.
 */
function _webform_submissionref_get_cid($webform, $name) {
  foreach ($webform->webform['components'] as $cid => $component) {
    if ($component['form_key'] == $name) {
      return $cid;
    }
  }
  return FALSE;
}
