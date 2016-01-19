<?php

/**
* @file
* Administration page callbacks for the riveroftweets module.
*/
/**
* Form builder. Configure annotations.
*
* @ingroup forms
* @see system_settings_form().
*/
function riveroftweets_admin_settings() {
  $form['tweets'] = array(
    '#type' => 'fieldset',
    '#title' => 'Tweets',
    '#description' => 'You can display tweets from either a username,
                       list, or a search query.',
  );
  
  $form['tweets']['riveroftweets_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter username'),
    '#default_value' => variable_get('riveroftweets_username'),
    '#required' => TRUE,
  );
  
  $form['tweets']['riveroftweets_list'] = array(
    '#type' => 'textfield',
    '#title' => t('List'),
    '#default_value' => variable_get('riveroftweets_list'),
    '#description' => 'Enter the name of a twitter list associated with the 
                       username\'s account.',
  );
  
  $form['tweets']['riveroftweets_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query'),
    '#default_value' => variable_get('riveroftweets_query'),
    '#description' => 'Enter a twitter search query',
  );
  
  $form['join'] = array(
    '#type' => 'fieldset',
    '#title' => 'Joint Text',
    '#description' => 'Text added to output',
  );
  
  $form['join']['riveroftweets_joindefault'] = array(
    '#type' => 'textfield',
    '#title' => t('Default'),
    '#default_value' => variable_get('riveroftweets_joindefault', ' I said, '),
    '#description' => '[string]   auto text for non verb: "I said" bullocks',
  );
  
  $form['join']['riveroftweets_joined'] = array(
    '#type' => 'textfield',
    '#title' => t('ed'),
    '#default_value' => variable_get('riveroftweets_joined', ' I '),
    '#description' => '[string]   auto text for past tense: "I" surfed',
  );
  
  $form['join']['riveroftweets_joining'] = array(
    '#type' => 'textfield',
    '#title' => t('ing'),
    '#default_value' => variable_get('riveroftweets_joining', ' I am '),
    '#description' => '[string]   auto tense for present tense: "I was" surfing',
  );
  
  $form['join']['riveroftweets_joinreply'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply'),
    '#default_value' => variable_get('riveroftweets_joinreply', ' I replied to '),
    '#description' => '[string]   auto tense for replies: "I replied to" @someone "with"',
  );
  
  $form['join']['riveroftweets_joinurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => variable_get('riveroftweets_joinurl', ' I was looking at '),
    '#description' => '[string]   auto tense for urls: "I was looking at" http:...',
  );
  
  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => 'Other',
    '#description' => 'Other options',
  );
  
  $form['other']['riveroftweets_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Template'),
    '#default_value' => variable_get('riveroftweets_template', '{text} ({time})'),
    '#description' => 'Available tokens are {avatar} {join} {text} {time}',
    '#required' => TRUE,
  );
  
  $form['other']['riveroftweets_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of tweets'),
    '#default_value' => variable_get('riveroftweets_count', '5'),
    '#description' => 'Enter a number of tweets to display',
    '#required' => TRUE,
  );
  
  $form['other']['riveroftweets_avatar'] = array(
    '#type' => 'textfield',
    '#title' => t('Avatar size'),
    '#default_value' => variable_get('riveroftweets_avatar', '48'),
    '#description' => 'Enter a number in pixels',
  );
  
  $form['other']['riveroftweets_refresh'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh interval'),
    '#default_value' => variable_get('riveroftweets_refresh'),
    '#description' => 'Enter in number of seconds',
  );
  
  $form['#submit'][] = 'riveroftweets_admin_settings_submit';

  return system_settings_form($form);
}


function riveroftweets_admin_settings_submit()
{
  return;
}
