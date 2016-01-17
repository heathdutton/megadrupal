<?php

/**
 * @file
 * Provides entity integration for ecard module
 */

/**
 * Implements hook_entity_info.
 */
function ecard_entity_info() {
  return array(
    'ecard' => array(
      'label' => t('Ecard'),
      'entity class' => 'Entity',
      'controller class' => 'EntityAPIController',
      'base table' => 'ecard',
      'module' => 'ecard',
      'entity keys' => array(
        'id' => 'hash',
      ),
    ),
  );
}

/**
 * Creates and saves an ecard entity.
 *
 * @param $ecard
 *  An array of ecard values.
 *   Necessary keys:
 *    nid:        The node id refernce.
 *    mail_from:  the mail of the sender.
 *    name_from:  the name of the sender.
 *    mail_to:    the mail of the recipient.
 *    name_to:    the name of the recipient.
 *    text:       the content of the ecard.
 *
 *   Optional keys:
 *    hash:            the unique identifier of the ecard. Pass for update.
 *    pick_up:         flag if the ecard already picked up. Default 0.
 *    sent:            flag if the ecard is sent. Default 0.
 *    notify:          flag if the sender should be notified when the ecard is picked up. Default 0.
 *    sent_time:       timestamp of when the card was sent. Default to 0.
 *    scheduled_time:  timestamp of when the card is/was scheduled to be sent. Default to 0.
 *
 * @return
 *  Returns the ecard object if the entity was saved successfully.
 *  Otherwise it returns FALSE.
 */
function ecard_create($ecard) {
  $ecard['created'] = time();
  if (!ecard_check_hash($ecard)) {
    $hash_base = $ecard['mail_from'] . uniqid();
    $ecard['hash'] = md5($hash_base);
    $ecard = entity_create('ecard', $ecard);
  }
  if (is_array($ecard)) {
    $ecard = (object) $ecard;
  }

  if (entity_save('ecard', $ecard)) {
    return $ecard;
  }
  return FALSE;
}

/**
 * Deletes an ecard entity by given hash value.
 *
 * @param $hash
 *  The hash value of the etity to be deleted.
 *
 * @return $status
 *  Returns TRUE if the entity is deleted successfully. Otherwise it returns FALSE.
 */
function ecard_delete($hash) {
  entity_delete('ecard', $hash);
}

/**
 * Deletes multiple ecard entities by given hash values.
 *
 * @param array $hash
 *  An array of hash values of the etities to be deleted.
 *
 * @return $status
 *  Returns TRUE if the entity is deleted successfully. Otherwise it returns FALSE.
 */
function ecard_delete_multiple(array $hashes) {
  entity_delete_multiple('ecard', $hashes);
}

/**
 * Returns an ecard entity by given hash value.
 *
 * @param $hash
 *  The hash value of the entity to be returned.
 *    Leave it NULL to receive all ecards.
 *    Pass a string to receive one ecard.
 *    Pass an array of hashes to receive X ecards.
 *
 * @return
 *  An array with the called ecard entities.
 *  Returns FALSE if there are no entities with the given hash value.
 */
function ecard_read($hash = FALSE) {
  if (is_string($hash)) {
    $hash = array($hash);
  }

  return entity_load('ecard', $hash);
}

/**
 * Updates an ecard entity.
 *
 * @param $ecard
 *  An array of ecard values.
 *   Necessary keys:
 *    hash:       the unique identifier of the ecard.
 *
 *   Optional keys:
 *    mail_from:  the mail of the sender.
 *    nid:        The node id refernce.
 *    name_from:  the name of the sender.
 *    mail_to:    the mail of the recipient.
 *    name_to:    the name of the recipient.
 *    text:       the content of the ecard.
 *    pick_up:    flag if the ecard alredy picked up. Default 0.
 *    sent:       flag if the ecard is sent. Default 0.
 *    notify:     flag if the sender should be notified when the ecard is picked up. Default 0.
 *
 * @return
 *  Returns the ecard object if the entity was saved successfully.
 *  Otherwise it returns FALSE.
 */
function ecard_update($ecard) {
  if (is_array($ecard)) {
    $ecard = (object) $ecard;
  }
  if (entity_save('ecard', $ecard)) {
    return $ecard;
  }
  return FALSE;
}

/**
 * Helper function to check the hash value of an ecard.
 *
 * @param $ecard
 *   An array with the ecard to check.
 *
 * @return
 *    FALSE if the hash value is not existing or with incorrect value.
 *    Otherwise it returns TRUE.
 */
function ecard_check_hash($ecard) {
  if (!array_key_exists('hash', $ecard)) {
    return FALSE;
  }
  if (!preg_match('~^[0-9a-f]{32}$~', $ecard['hash'])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Render the given ecard
 *
 * @param $hash
 *  The ecard to render.
 *
 * @return
 *  The html of the ecard value.
 *
 * @todo
 *  Implementing theme functions.
 */
function ecard_render_ecard($hash, $args, $options) {
  $ecard_render = '';
  if ($options['show_form'] === 1 && $options['form_position'] === 'above') {
    $ecard_render .= drupal_render(drupal_get_form('ecard_form_create_ecard', $args));
  }
  if ($options['show_ecard'] === 1) {

    $ecards = ecard_read($hash);

    foreach ($ecards as $ecard) {

      if ($ecard->entity_type === $args['entity_type']
        && $ecard->bundle === $args['bundle']
        && $ecard->entity_id == $args['entity_id']) {

        $ecard->text = token_replace($ecard->text, (array) $ecard);
        $variables['ecard'] = $ecard;
        $variables['entity'] = entity_load($ecard->entity_type, array($ecard->entity_id));
        $variables['name_to'] = t('Hey @name_to,', array('@name_to' => $ecard->name_to));
        $variables['content'] = $ecard->text;
        $variables['footer'] = t('Regards @name_from', array('@name_from' => $ecard->name_from));
        $ecard_render .= theme('ecard_formatter_view', $variables);
      }
    }
  }
  if ($options['show_form'] === 1 && $options['form_position'] === 'below') {
    $ecard_render .= drupal_render(drupal_get_form('ecard_form_create_ecard', $args));
  }
  return $ecard_render;
}
