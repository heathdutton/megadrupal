<?php
/**
 * @file
 */

/**
 * Alter the name of the sender for a message sent through the CakeMail MailSystem.
 *
 * @param $name
 *   The name of the sender.
 * @param $address
 *   The email address of the sender.
 */
function hook_cakemail_relay_sender_name(&$name, $address) {

}

/**
 * Alter the template ID used for a message sent through the CakeMail MailSystem.
 *
 * @param $template_id
 *   The ID of the template to use.
 * @param $message
 *   A message array, as described in hook_mail_alter().
 */
function hook_cakemail_relay_template_id(&$template_id, $message) {

}

/**
 * Alter the variables used when applying a template to a message send through
 * the CakeMail MailSystem.
 *
 * @param $variables
 *   An associative array of variables to use when applying the template to the
 *   message. Keys are the variable name used in the template, without the
 *   enclosing brackets ('[' and ']').
 * @param $message
 *   A message array, as described in hook_mail_alter().
 */
function hook_cakemail_relay_template_variables(&$variables, $message) {

}