<?php

/**
 * @file Parachat admin functions
 */

/**
 * Parachat Settings
 */
function parachat_admin_settings() {

  $data = variable_get('parachat_settings', NULL);
  $data = isset($data) ? unserialize($data) : array();

  $form['applet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Applet Codebase')
  );

  $form['applet']['codebase'] = array(
    '#type' => 'textfield',
    '#title' => t('Applet Codebase URL'),
    '#description' => t('This is found in the Applet Code and should be something like: http://host7.parachat.com/pchat/applet'),
    '#default_value' => isset($data['codebase']) ? $data['codebase'] : '',
    '#required' => TRUE
  );

  $form['applet']['hint'] = array(
    '#value' => '
<p>You should have received an email from Parachat with applet code similar
to what you see below.  The Applet Codebase URL is bolded below.</p>
<p>&lt;APPLET CODEBASE=&quot;<b>http://host7.parachat.com/pchat/applet</b>&quot;<br />
 ARCHIVE=papplet.jar CODE=pclient.main.ChatClient.class HEIGHT=400 WIDTH=600&gt;<br />
&lt;PARAM NAME=&quot;cabbase&quot; VALUE=&quot;papplet.cab&quot;&gt;<br />
&lt;PARAM NAME=&quot;Net.Site&quot; VALUE=&quot;12345&quot;&gt;<br />

&lt;PARAM NAME=&quot;Net.Room&quot; VALUE=&quot;Lobby&quot;&gt;<br />
Sorry, your browser is not Java enabled, please visit<br />
&lt;a href=&quot;http://www.parachat.com/faq/java.html&quot;&gt;our java support pages&lt;/a&gt;<br />
&lt;/APPLET&gt;</p>'
  );


  $form['site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#description' => t('Sent with Parachat welcome email. i.e. 1234_demo'),
    '#default_value' => isset($data['site_id']) ? $data['site_id'] : '',
    '#required' => TRUE
    );

  $form['room'] = array(
    '#type' => 'textfield',
    '#title' => 'Default Room',
    '#description' => t('Name of the default room users will join'),
    '#default_value' => isset($data['room']) ? $data['room'] : '',
    '#required' => TRUE
    );

  $options = array(0=>t('No'), 1=>t('Yes'));

  $form['auth'] = array(
    '#type' => 'radios',
    '#title' => 'External DB Authentication',
    '#description' => t('This is for added security, but requires more setup in the Parachat UI. External db must be enabled and you need to set the URL to: http://example.com/chat/auth'),
    '#options' => $options,
    '#default_value' => isset($data['auth']) && $data['auth'] == 1 ? 1 : 0,
    '#required' => TRUE
    );

  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Width of the Parachat applet in pixels'),
    '#default_value' => isset($data['width']) ? $data['width'] : 750,
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE
    );

  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Height of the Parachat applet in pixels'),
    '#default_value' => isset($data['height']) ? $data['height'] : 500,
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE
    );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
    );

    return $form;
}

/**
 * Implementation of hook_submit
 */
function parachat_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  $data = serialize($values);
  variable_set('parachat_settings', $data);
  drupal_set_message(t('Parachat settings successfully saved.'));
}
