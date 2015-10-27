<?php
/**
 * @file
 * Administration page for JuicerIO module.
 */

/**
 * Admin settings form.
 */
function juicerio_settings_form($form, &$form_state) {
  $juicer_name = variable_get('juicerio_feed_id', FALSE);
  $help  = '';

  if ($juicer_name) {
    $default = $juicer_name;
  }
  else {
    $default = '';
    $help .= '<p>' . t('To use the Juicer module for Drupal you first need to create and set up an account with !juicer.', array('!juicer' => l(t('Juicer'), 'https://www.juicer.io'))) . '</p>';
    $help .= '<p>' . t('For help setting up a (free or paid) Juicer account, read !blog, (but skip the last step).', array('!blog' => l(t('this blog post'), 'https://www.juicer.io/blog/how-to-aggregate-and-embed-your-social-media-posts-with-juicer'))) . '</p>';
  }

  $help .= '<p>' . t('All services included in your feed (facebook, twitter, etc) are managed on the !juicer site.', array('!juicer' => l(t('Juicer'), 'https://www.juicer.io'))) . '</p>';
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => $help,
  );
  $form['juicerio_feed_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Juicer Username'),
    '#default_value' => $default,
    '#weight' => 1,
  );
  $form['juicerio_blocks'] = array(
    '#type' => 'select',
    '#title' => t('Number of Juicer feed blocks'),
    '#options' => array(1 => t('One'), 2 => t('Two'), 3 => t('Three')),
    '#default_value' => array(variable_get('juicerio_blocks', 1)),
    '#description' => t('Only paid Juicer accounts can place more than one feed.'),
    '#weight' => 2,
  );

  $form['example'] = array(
    '#type' => 'fieldset',
    '#title' => t('Example'),
    '#collapsible' => TRUE,
    '#weight' => 10,
  );
  $form['example']['example-feed'] = array(
    '#type' => 'markup',
    '#markup' => theme('juicerio_feed', array('feed_id' => $juicer_name)),
  );

  return system_settings_form($form);
}
