<?php
/**
 * @file.
 * Handles Rules chatterbox admin configuration settings
 */

/**
 * View and update chennel list.
 */
function nodejs_rules_chatterbox_channels() {
  $query  =  db_select('nodejs_rules_chatterbox_channels','channel')
    ->fields('channel',array('cid','channel','title'));
  $query->extend('PagerDefault')
    ->limit(1);
  $result = $query->execute();
  $rows = array();
  while ($row = $result->fetchObject()) {
    $actions = l("Configure Block",'admin/structure/block/manage/nodejs_rules_chatterbox/'.$row->channel.'/configure').' &nbsp;';
	$actions .= l("Flush Messages",'admin/config/nodejs/rules/channels/'.$row->cid.'/flush').' &nbsp;';
	$actions .= l("Delete Channel",'admin/config/nodejs/rules/channels/'.$row->cid.'/delete');
    $rows[] = array($row->channel,$row->title,$actions);
  }
 
  $header = array('Channel','Block Title','Actions');	
  $op = theme_table(array('header' => $header,'rows' => $rows,"attributes" => array(),"caption" => "","colgroups" => array(),"sticky" => TRUE, "empty" => t('Channels not created yet.')));
  $op .= theme('pager',array("quantity"=>1));
  
  return $op;
}

/**
 * Channel creation form
 */
function nodejs_rules_chatterbox_channels_form(){
  $form = array();
  $form['channel'] = array(
  	'#type' => 'textfield',
	'#title' => t('Channel Name'),
	'#description' => t('A unique channel machine name. It must only contain lowercase letters, numbers, and underscores.'),
	'#required' => TRUE
  );
  $form['block'] = array(
  	'#type' => 'textfield',
	'#title' => t('Block Name'),
	'#description' => t('Title for the notification block.'),
	'#required' => TRUE
  );
  $form['limit'] = array(
  	'#type' => 'textfield',
	'#title' => t('Total items to display'),
	'#description' => t('Total number of items per page.'),
	'#required' => TRUE
  );
  $form['submit'] = array(
  	'#type' => 'submit',
	'#value' => t('Submit'),
  );
  return $form;
}
/**
 * Channel creation form validation
 */
function nodejs_rules_chatterbox_channels_form_validate($form,&$form_state) {
  $query = "SELECT COUNT(*) AS total FROM {nodejs_rules_chatterbox_channels} WHERE channel = ':channel'";
  $result = db_query($query,array(':channel' => $form_state['values']['channel']))->fetchObject();
  if($result->total >0) {
	form_set_error('channel',t('Channel already exists.'));
  }
  if (preg_match('/[^a-z_0-9]/i', $form_state['values']['channel'])) {
    form_set_error('channel',t('Channel must only contain lowercase letters, numbers, and underscores.'));
  }
  if (strlen($form_state['values']['channel']) > 25) {
  	form_set_error('channel',t('Maximum 25 characters are allowed.'));
  }
  if (strlen($form_state['values']['channel']) < 5) {
  	form_set_error('channel',t('Minimum 4 characters required.'));
  }
  if (!is_numeric($form_state['values']['limit'])) {
  	form_set_error('limit',t('Please specify a numeric value.'));
  }
}
/**
 * Channel creattion form submit handler
 */
function nodejs_rules_chatterbox_channels_form_submit($form,&$form_state) {
  $settings = serialize(array("limit" => $form_state['values']['limit']));
  $result = db_insert('nodejs_rules_chatterbox_channels')
    ->fields(array(
	  'channel' => $form_state['values']['channel'],
	  'title' => $form_state['values']['block'],
	  'settings' =>  $settings
	))
	->execute();
  if($result > 0) {
  	drupal_set_message(t('Channel Created Successfully.'));
  }
  $form_state['redirect'] = 'admin/config/nodejs/rules/channels';
}

/**
 * Confirmation for Flush messages in a channel.
 */
function nodejs_rules_chatterbox_channel_flush($form,$form_state,$cid) {
  $form['cid'] = array(
    '#type' => 'hidden',
	'#value' => $cid,
	'#default_value' => $cid,
  );
  return confirm_form($form, t('Are you sure you want to delete all messages?'), 'admin/config/nodejs/rules/channels', t('This action cannot be undone.'), t('Flush'), t('Cancel'));
}

/**
 * Flush message form submit handler.
 */
function nodejs_rules_chatterbox_channel_flush_submit($form,&$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('nodejs_rules_chatterbox_message')
	  ->condition('cid',$form_state['values']['cid'],'=')
	  ->execute();
	drupal_set_message(t('Messages has been deleted successfully.'));
  }
  $form_state['redirect'] = 'admin/config/nodejs/rules/channels';
}

/**
 * Confirm for for delete channel.
 */
function nodejs_rules_chatterbox_channel_delete($form,&$form_state,$cid) {
  $form['cid'] = array(
    '#type' => 'hidden',
	'#value' => $cid,
	'#default_value' => $cid,
  );
  return confirm_form(
    $form, 
	t('Are you sure you want to delete this channel?'), 
	'admin/config/nodejs/rules/channels', 
	theme_item_list(
	  array(
	    'items' => array(
	      t('All notification rules created on this channel will be deleted.'),
	      t('The notification block for this channel will be deleted.'),
		  t('This action cannot be undone.'),
		),
		'title' => '',
		'type' => 'ul',
		'attributes' => array(),
	  )
	), 
	t('Delete'), 
	t('Cancel')
  );
}

/**
 * Delete channel submit form handler.
 */
function nodejs_rules_chatterbox_channel_delete_submit($form,&$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('nodejs_rules_chatterbox_message')
	  ->condition('cid',$form_state['values']['cid'],'=')
	  ->execute();
    db_delete('nodejs_rules_chatterbox_channels')
	  ->condition('cid',$form_state['values']['cid'],'=')
	  ->execute();  
    cache_clear_all(NULL, 'cache_block');
  }
  $form_state['redirect'] = 'admin/config/nodejs/rules/channels';
}

?>