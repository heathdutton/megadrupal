<?php
/**
 * @file
 * Definition of maildigest_digest_ui class.
 */

/**
 * Represents a digest.
 */
class maildigest_digest_ui extends ctools_export_ui {

  /**
   * Implements ctools_export_ui::edit_form().
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);
	extract($form_state['item']->settings);
    module_load_include('inc', 'token', 'token.pages');
	$form['info']['admin_title']['#description'] = t('The name of this digest.');
    $form['info']['admin_title']['#attributes'] = array('class' => array('maildigest-name'));
    $form['info']['name']['#attributes'] = array('class' => array('maildigest-id'));
    $form['digest']['#tree'] = FALSE;
    $form['digest']['#after_build'] = array('_maildigest_load_javascript_after_build_form');
    $form['digest']['settings']= array(
      '#type' => 'fieldset',
      '#title' => t('Digest settings'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
    );
    $form['digest']['settings']['sender'] = array(
      '#type' => 'textfield',
      '#title' => t('Digest sender'),
      '#default_value' => $sender,
    );
    $form['digest']['settings']['frequency'] = array(
      '#type' => 'select',
      '#title' => t('Send newsletter every'),
      '#options' => array('daily' => t('Day'), 'weekly' => t('Week'), 'monthly' => t('Month')),
      '#default_value' => $frequency,
      '#attributes' => array('onchange' => 'maildigest_frequency_callback(this)')
    );
    $form['digest']['settings']['weekly'] = array(
      '#type' => 'select',
      '#title' => t('Send digest on'),
      '#description' => t('Day of the week on which digest is sent'),
      '#options' => array(t('Sunday'), t('Monday'), t('Tuesday'), t('Wednesday'), t('Thursday'), t('Friday'), t('Saturday')),
      '#default_value' => $weekly,
      '#prefix' => '<div class="maildigest-frequency-field" id="maildigest-weekly-wrapper" style="' . (($frequency && $frequency == 'weekly' && isset($weekly)) ? 'display: block;' : 'display: none;') . '">',
      '#suffix' => '</div>'
     );
    $form['digest']['settings']['monthly'] = array(
      '#type' => 'select',
      '#title' => t('Send digest on'),
      '#description' => t('Day of the month on which digest is sent'),
      '#options' => array('First day' => t('First day'), '15th' => t('15th'), 'Last day' => t('Last day'), 'First Sun' => t('First Sunday'), 'First Mon' => t('First Monday'), 'First Tue' => t('First Tuesday'), 'First Wed' => t('First Wednesday'), 'First Thu' => t('First Thursday'), 'First Fri' => t('First Friday'), 'First Sat' => t('First Saturday'), 'Second Sun' => t('Second Sunday'), 'Second Mon' => t('Second Monday'), 'Second Tue' => t('Second Tuesday'), 'Second Wed' => t('Second Wednesday'), 'Second Thu' => t('Second Thursday'), 'Second Fri' => t('Second Friday'), 'Second Sat' => t('Second Saturday'), 'Third Sun' => t('Third Sunday'), 'Third Mon' => t('Third Monday'), 'Third Tue' => t('Third Tuesday'), 'Third Wed' => t('Third Wednesday'), 'Third Thu' => t('Third Thursday'), 'Third Fri' => t('Third Friday'), 'Third Sat' => t('Third Saturday'), 'Fourth Sun' => t('Fourth Sunday'), 'Fourth Mon' => t('Fourth Monday'), 'Fourth Tue' => t('Fourth Tuesday'), 'Fourth Wed' => t('Fourth Wednesday'), 'Fourth Thu' => t('Fourth Thursday'), 'Fourth Fri' => t('Fourth Friday'), 'Fourth Sat' => t('Fourth Saturday')),
      '#default_value' => $monthly,
      '#prefix' => '<div class="maildigest-frequency-field" id="maildigest-monthly-wrapper" style="' . (($frequency && $frequency == 'monthly' && isset($monthly)) ? 'display: block;' : 'display: none;') . '">',
      '#suffix' => '</div>'
     );
     $form['digest']['settings']['hour'] = array(
      '#type' => 'select',
      '#title' => t('Hour'),
      '#options' => array(0 => '00:00', 3600 => '01:00', 7200 => '02:00', 10800 => '03:00', 14400 => '04:00', 18000 => '05:00', 21600 => '06:00', 25200 => '07:00', 28800 => '08:00', 32400 => '09:00', 36000 => '10:00', 39600 => '11:00', 43200 => '12:00',  46800 => '13:00', 50400 => '14:00', 54000 => '15:00', 57600 => '16:00', 61200 => '17:00', 64800 => '18:00', 68400 => '19:00', 72000 => '20:00', 75600 => '21:00', 79200 => '22:00', 82800 => '23:00'),
      '#default_value' => $hour,
      '#description' => t('Select the hour of the day to automatically send the digest newsletter. The first cron run after this hour will trigger the mailing.'),
     );
     foreach (language_list() as $langcode => $language) {
       $subject = str_replace('-', '_', 'subject_' . $langcode);
       $form['digest']['settings'][$subject] = array(
         '#type' => 'textfield',
         '#title' => t('Subject (!name)', array('!name' => $language->name)),
         '#description' => t('You can use the following tokens in your subject: <fieldset class="collapsible collapsed"><legend><span class="fieldset-legend">Available tokens</span></legend><div class="fieldset-wrapper">!tokens</div></fieldset>', 
           array('!tokens' => theme('token_tree', array('token_types' => array('node', 'user'))))
         ),
         '#default_value' => $$subject,
       );
       $message = str_replace('-', '_', 'message_' . $langcode);
       $message_data = $$message;
       $form['digest']['settings'][$message] = array(
         '#type' => 'text_format',
         '#title' => t('Message (!name)', array('!name' => $language->name)),
         '#description' => t('You can use the following tokens in your message: <fieldset class="collapsible collapsed"><legend><span class="fieldset-legend">Available tokens</span></legend><div class="fieldset-wrapper">!tokens</div></fieldset>', 
           array('!tokens' => theme('token_tree', array('token_types' => array('node', 'user'))))
         ),
         '#default_value' => $message_data['value'],
         '#format' => $message_data['format'],
       );
     }
  }
  
  function edit_save_form($form_state) {
    parent::edit_save_form($form_state);
    $item = &$form_state['item'];
    if (variable_get('maildigest_digest_last_sent_' . $item->name, 0) == 0) {
      variable_set('maildigest_digest_last_sent_' . $item->name, time());
      maildigest_digest_set_next_times($item);
    }
  }

}
