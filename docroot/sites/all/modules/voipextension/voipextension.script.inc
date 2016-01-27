<?php

/**
 * Basic prompt to input extension number for an extension field.
 */
function _voipextension_basic_menu_script($field_name) {
  $script = new VoipScript('voipextension_basic_menu_script');

  $prompt = variable_get('voipextension_prompt_msg', t('Please enter the desired extension number followed by the pound key. Or press star to go back.'));

  $script->addLabel('request_extension');
  $max_length = voipextension_get_maxlength($field_name);
  $script->addGetInput($prompt, 5, '#', $max_length);
  $script->addGotoIf('no_input_received', "^%input_digits == ''");
  $script->addGotoIf('go_back', "^%input_digits == '*'");
  $script->addGosub('voipextension_play_extension_script', array('field_name' => $field_name, 'extension_number' => "%input_digits"));
  $script->addGoto('request_extension');

  $script->addLabel('no_input_received');
  $script->addSay(variable_get('voipextension_noinput_msg', t('No input received. Please try again.')));
  $script->addGoto('request_extension');

  $script->addLabel('go_back');
  $script->addReturn();

  return $script;
}

/**
 * Retrieve extension script and GoSub to it.
 */
function _voipextension_play_extension_script($field_name, $extension_number) {
  $script = new VoipScript('voipextension_play_extension_script');

  // using voipscript %extension_number replacement
  $script->setVar('extension_number', $extension_number);

  $script_name = '';
  list($entity_type, $entity) = voipextension_get_entity($field_name, $extension_number);
  if (! empty($entity)) {
    $extension = voipextension_get_entity_extension($entity_type, $entity, $field_name);
    if (! empty($extension)) {
      $script_name = voipextension_get_script($entity_type, $entity, $extension, $field_name);
    }
  }

  if ($script_name != '' && $extension['enabled']) {
    if (! $transferring = variable_get('voipextension_transferring_msg', FALSE)) {
      $transferring = t('Transferring to extension %extension_number. ');
    }
    $script->addSay($transferring);
    list($entity_id,,) = entity_extract_ids($entity_type, $entity);
    $arguments = array('entity_type' => $entity_type, 'entity_id' => $entity_id, 'field_name' => $field_name, 'extension' => $extension);
    $script->addGosub($script_name, $arguments);
  }
  else {
    if (! $unknown = variable_get('voipextension_unknown_msg', FALSE)) {
      $unknown = t('Unknown extension %number. ');
    }
    $script->addSay($unknown);
  }
  $script->addReturn();

  return $script;
}

/**
 * Basic extension script, used as fallback if no script is set.
 */
function _voipextension_default_extension_script($entity_type, $entity_id, $field_name, $extension) {
  $script = new VoipScript('voipextension_default_extension_script');
  $entity = entity_load($entity_type, array($entity_id));
  $entity = reset($entity);

  if (! empty($entity->title)) {
    $render_array = field_view_field($entity_type, $entity, 'title');
  }
  elseif (! empty($entity->name)) {
    $render_array = field_view_field($entity_type, $entity, 'name');
  }
  if (isset($render_array)) {
    $script->addSay(check_plain(render($render_array)));
  }
  else {
    $script->addSay('Unknown extension title');
  }

  $script->addReturn();

  return $script;
}
