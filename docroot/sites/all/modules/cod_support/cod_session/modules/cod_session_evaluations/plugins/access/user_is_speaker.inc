<?php

/**
 * @file
 * Plugin to provide access control based upon entity bundle.
 */

$plugin = array(
  'title' => t("User is speaker"),
  'description' => t('Control access by speakers for a field'),
  'callback' => 'cod_session_evaluations_speaker_access_check',
  'summary' => 'cod_session_evaluations_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'default' => array('type' => array()),
);

/**
 * Check for access.
 */
function cod_session_evaluations_speaker_access_check($conf, $context, $plugin) {
  global $user;
  if ((!is_object($context)) || (empty($context->data))) {
    // If the context doesn't exist -- for example, a newly added entity
    // reference is used as a pane visibility criteria -- we deny access.
    return FALSE;
  }

  // Only work with node contexts.
  if (!isset($context->keyword) || $context->keyword != 'node') {
    return FALSE;
  }

  // And make sure we have a speaker field
  if (!isset($context->data->field_speakers)) {
    return FALSE;
  }

  // Yay we have speakers, we're a node, and a valid context. Match to the user.
  foreach($context->data->field_speakers[LANGUAGE_NONE] AS $speaker) {
    if ($speaker['target_id'] == $user->uid) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the specified context.
 */
function cod_session_evaluations_access_summary($conf, $context) {
  return t('Current user is a speaker for referenced node argument.');
}

