<?php

/**
 * Settings form.
 */
function simpleantispam_settings_form($form, $form_state) {
  $forms = array();
  foreach (variable_get('simpleantispam_forms', array()) as $form_id => $weight) {
    $forms[] = $form_id . '|' . $weight;
  }

  $form['simpleantispam_forms'] = array(
    '#type' => 'textarea',
    '#title' => 'Form ID\'s',
    '#description' => t('List of form id\'s that are protected by Simple Anti-Spam module. Enter one value per line, in the format form_id|weight.'),
    '#default_value' => implode("\n", $forms),
  );

  $form['simpleantispam_check_stopwords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block forms by stop words'),
    '#default_value' => variable_get('simpleantispam_check_stopwords', FALSE),
  );

  $form['simpleantispam_stopwords'] = array(
    '#type' => 'textarea',
    '#title' => 'Stop words',
    '#description' => t('List of stop words. Enter one value per line.'),
    '#default_value' => implode("\n", variable_get('simpleantispam_stopwords', array())),
    '#states' => array(
      'visible' => array(
        'input[name="simpleantispam_check_stopwords"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['simpleantispam_check_length'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block forms by text length'),
    '#default_value' => variable_get('simpleantispam_check_length', FALSE),
  );

  $form['simpleantispam_max_text_length'] = array(
    '#type' => 'textfield',
    '#title' => 'Max text length',
    '#default_value' => variable_get('simpleantispam_max_text_length', 2000),
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        'input[name="simpleantispam_check_length"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['simpleantispam_check_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block forms by number of links'),
    '#default_value' => variable_get('simpleantispam_check_links', FALSE),
  );

  $form['simpleantispam_max_links_count'] = array(
    '#type' => 'textfield',
    '#title' => 'Max links count',
    '#default_value' => variable_get('simpleantispam_max_links_count', 5),
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        'input[name="simpleantispam_check_links"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['simpleantispam_check_php'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block forms by php code'),
    '#default_value' => variable_get('simpleantispam_check_php', FALSE),
  );

  $form['simpleantispam_php_code'] = array(
    '#type' => 'textarea',
    '#title' => 'PHP code',
    '#description' => '
      PHP code without &lt;?php ?&gt; tags. Return error message if form is blocked.
      Example: <code>if (preg_match(\'#&lt;embed.+&gt;#\', $text)) return \'text contains embeded content\';</code>
    ',
    '#default_value' => variable_get('simpleantispam_php_code', ''),
    '#states' => array(
      'visible' => array(
        'input[name="simpleantispam_check_php"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['simpleantispam_ban'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ban spammers by IP'),
    '#default_value' => variable_get('simpleantispam_ban', FALSE),
  );
  
  $form['simpleantispam_ban_submissions_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of fail form submissions'),
    '#description' => t('Ban spamers after number of fail form submissions'),
    '#default_value' => variable_get('simpleantispam_ban_submissions_count', 3),
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        'input[name="simpleantispam_ban"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['simpleantispam_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log blocked form submissions'),
    '#default_value' => variable_get('simpleantispam_log', TRUE),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Settings form submit callback.
 */
function simpleantispam_settings_form_submit($form, $form_state) {
  // Forms
  $forms = array();
  foreach (explode("\n", trim($form_state['values']['simpleantispam_forms'])) as $line) {
    list($form_id, $weight) = explode('|', $line);
    $forms[$form_id] = (float)$weight;
  }
  variable_set('simpleantispam_forms', $forms);

  // Stop words
  $stopwords = array();
  foreach (explode("\n", $form_state['values']['simpleantispam_stopwords']) as $stopword) {
    if (trim($stopword)) {
      $stopwords[] = str_replace("\r", '', $stopword);
    }
  }
  variable_set('simpleantispam_stopwords', $stopwords);

  // Other settings
  variable_set('simpleantispam_log',                   (bool)$form_state['values']['simpleantispam_log']);
  variable_set('simpleantispam_check_stopwords',       (bool)$form_state['values']['simpleantispam_check_stopwords']);
  variable_set('simpleantispam_check_length',          (bool)$form_state['values']['simpleantispam_check_length']);
  variable_set('simpleantispam_max_text_length',       (int)$form_state['values']['simpleantispam_max_text_length']);
  variable_set('simpleantispam_check_links',           (bool)$form_state['values']['simpleantispam_check_links']);
  variable_set('simpleantispam_max_links_count',       (int)$form_state['values']['simpleantispam_max_links_count']);
  variable_set('simpleantispam_check_php',             (bool)$form_state['values']['simpleantispam_check_php']);
  variable_set('simpleantispam_php_code',              $form_state['values']['simpleantispam_php_code']);
  variable_set('simpleantispam_ban',                   (bool)$form_state['values']['simpleantispam_ban']);
  variable_set('simpleantispam_ban_submissions_count', (int)$form_state['values']['simpleantispam_ban_submissions_count']);

  drupal_set_message(t('Configuration saved.'));
}
