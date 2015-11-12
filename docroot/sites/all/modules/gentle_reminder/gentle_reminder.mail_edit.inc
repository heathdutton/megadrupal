<?php
/**
 * @file
 * Integration with Mail Editor module.
 */

/**
 * Implements hook_mailkeys().
 */
function gentle_reminder_mailkeys() {
  $items = array();

  foreach (module_implements('gr_template') as $module) {
    $items[$module] = t('Reminder by %module module', array('%module' => $module));
  }

  return $items;
}

/**
 * Implements hook_mail_edit_text().
 */
function gentle_reminder_mail_edit_text($mailkey, $language = NULL) {
  $info = module_invoke($mailkey, 'gr_template');

  $item_body = _gentle_reminder_get_items_template($mailkey);

  return array(
    'subject' => $info['subject'] ?: '',
    'body' => $info['body'] ?: '',
    'item_body' => $item_body ?: $info['item_body'],
  );
}

/**
 * Implements hook_mail_edit_token_types().
 */
function gentle_reminder_mail_edit_token_types($mail_id) {
  return array('gentle_reminder');
}

/**
 * Implements hook_mail_edit_form_extra().
 *
 * Customize Mail Editor's edit template page.
 */
function gentle_reminder_mail_edit_form_extra(&$form, &$form_state, $module, $template) {
  $form['mail']['item_body'] = array(
    '#title' => t('Item'),
    '#type' => 'textarea',
    '#default_value' => $template['item_body'],
    '#rows' => 5,
  );

  $info = module_invoke($module, 'gr_info');
  if ($info['needs-data']) {
    $form['mail']['tokens_item'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tokens for item'),
      '#weight' => 6,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'tokens' => array(
        '#theme' => 'token_tree',
        '#token_types' => $info['needs-data'],
        '#global_types' => TRUE,
        '#click_insert' => TRUE,
      ),
    );
  }

  $form['mailkey'] = array(
    '#type' => 'value',
    '#value' => $module
  );

  $form['op']['#submit'][] = 'gentle_reminder_mail_edit_form_extra_submit';
}

/**
 * Submit handler to save Mail Editor's edit form.
 */
function gentle_reminder_mail_edit_form_extra_submit(array $form, array &$state) {
  $values = $state['values'];
  $mailkey = $values['mailkey'];
  $body = $values['item_body'];
  $language = 'en';

  db_merge('gentle_reminder_items')
    ->key(array(
      'mailkey' => $mailkey,
      'language' => $language,
    ))
    ->fields(array(
      'mailkey' => $mailkey,
      'language' => $language,
      'body' => $body,
    ))
    ->execute();
}

/**
 * Get template for items.
 *
 * @param $mail_id
 * @return string
 */
function _gentle_reminder_get_items_template($mailkey) {
  return db_select('gentle_reminder_items', 'i')
    ->fields('i', array('body'))
    ->condition('i.mailkey', $mailkey)
    ->execute()
    ->fetchField();
}
