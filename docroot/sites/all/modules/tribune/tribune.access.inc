<?php

/**
 * Configuration page for defining user access for one tribune
 */
function tribune_access_page($tribune) {
  return drupal_get_form('_tribune_access_page_form', $tribune);
}

function _tribune_access_page_form($form, $form_state, $tribune) {
  $form = array(
    'tribune' => array(
      '#type' => 'value',
      '#value' => $tribune,
    ),
    'read' => array(
      '#type' => 'fieldset',
      '#title' => t('Read access'),
      '#tree' => TRUE,

      'type' => array(
        '#type' => 'radios',
        '#default_value' => $tribune->tribune_permissions['read'],
        '#options' => array(
          'all' => t('All users'),
          'registered' => t('Authenticated users'),
          'owner' => t('Owner only'),
          'nobody' => t('Nobody'),
        ),
      ),
    ),
    'write' => array(
      '#type' => 'fieldset',
      '#title' => t('Write access'),
      '#tree' => TRUE,

      'type' => array(
        '#type' => 'radios',
        '#default_value' => $tribune->tribune_permissions['write'],
        '#options' => array(
          'all' => t('All users'),
          'registered' => t('Authenticated users'),
          'owner' => t('Owner only'),
          'nobody' => t('Nobody'),
        ),
      ),
    ),
    'custom' => array(
      '#type' => 'fieldset',
      '#title' => t('Custom access rights'),
      '#description' => t('Here you can define custom access rights for users.<br />
        These rights override the settings you might have chosen before, ie. if nobody has the right
        to post in this tribune, you can still choose to allow it for some users in this section.'),
      '#tree' => TRUE,

      'users' => array(
        '#type' => 'item',
        '#title' => t('Add custom access rights for user:'),

        'new' => array(
          '#type' => 'textfield',
          '#autocomplete_path' => 'user/autocomplete',
          '#ajax' => array(
            'callback' => 'tribune_access_add_user',
            'wrapper' => 'tribune_access_users_wrapper',
            'method' => 'append',
            'effect' => 'fade',
          ),
        ),
        'ajax-wrapper' => array(
          '#markup' => '<div id="tribune_access_users_wrapper"></div>',
        ),
      ),
    ),
    'bans' => array(
      '#type' => 'fieldset',
      '#title' => t('IP/User-agent banning'),
      '#description' => t('Here you can ban some users depending on their IP or user-agent.<br />
        Banned users will not be able to post messages to this tribune, however if using Node.js
        they will still receive new messages for up to ten minutes or until they try to reload the page manually.<br />
        These bans override any other setting and apply on authentified users as well as anonymous.'),
      '#tree' => TRUE,

      'patterns' => array(
        '#type' => 'item',
        '#title' => t('Ban this combination of IP and user-agent:'),

        'new' => array(
          'ip' => array(
            '#type' => 'textfield',
            '#attributes' => array(
              'placeholder' => t('IP'),
            ),
            '#description' => t('Use * and ? as wildcards to match ranges of IP addresses (e.g <em>192.168.1.*</em>) or leave empty to ignore.'),
          ),
          'ua' => array(
            '#type' => 'textfield',
            '#attributes' => array(
              'placeholder' => t('User-agent'),
            ),
            '#description' => t('Use * and ? as wildcards to match ranges of IP addresses (e.g <em>*OS X*Firefox/19*</em>) or leave empty to ignore.'),
          ),
          'add' => array(
            '#type' => 'button',
            '#value' => t('Add'),
            '#ajax' => array(
              'callback' => 'tribune_access_add_ban',
              'wrapper' => 'tribune_access_ban_wrapper',
              'method' => 'replace',
              'effect' => 'fade',
            ),
          ),
        ),
        'list' => array(
          '#type' => 'item',
          '#prefix' => '<div id="tribune_access_ban_wrapper">',
          '#suffix' => '</div>',
          'table' => array(
            '#markup' => '',
          ),
          'values' => array(
          ),
        ),
      ),
    ),
    'censorship' => array(
      '#type' => 'fieldset',
      '#title' => t('Censored words'),
      '#description' => t('Here you can censor bad words.'),
      '#tree' => TRUE,

      'type' => array(
        '#type' => 'select',
        '#title' => t('Censorship type'),
        '#options' => array(
          'stars' => t('Stars'),
          'reject' => t('Reject post'),
        ),
        '#default_value' => isset($tribune->tribune_censorship['type']) ? $tribune->tribune_censorship['type'] : 'stars',
      ),

      'patterns' => array(
        '#type' => 'textarea',
        '#title' => t('Censor these words:'),
        '#description' => t('List here all the words or expressions you want to censor, one per line.<br />
          Use "*" as a wildcard to match any word character for any length, and "?" to match any word character once.<br />
          For example, "banana*" would match and censor "banana", "bananas", "bananases", and "m?n" would match "man" and "men" but not "mean".'),
        '#default_value' => isset($tribune->tribune_censorship['patterns']) ? implode("\n", $tribune->tribune_censorship['patterns']) : '',
      ),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('_tribune_access_page_form_submit'),
    ),
  );

  if ($tribune->tribune_public) {
    $form['read']['#disabled'] = TRUE;
    $form['read']['#description'] = t('This tribune is set as publicly readable, disable this option in the settings if you want to control read access.');
  }

  $info = tribune_info($tribune);

  foreach ($tribune->tribune_permissions['custom'] as $uid => $permissions) {
    $user = user_load($uid)->name;
    $form['custom']['users'][$user] = array(
      '#name' => 'custom[users]['.$user.']',
      '#title' => t('Custom access rights for user: @name', array('@name' => $user)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'mod' => array(
        '#type' => 'checkbox',
        '#title' => t('moderation'),
        '#name' => 'custom[users]['.$user.'][mod]',
        '#default_value' => $permissions['mod'],
      ),
      'write' => array(
        '#type' => 'checkbox',
        '#title' => t('write'),
        '#name' => 'custom[users]['.$user.'][write]',
        '#default_value' => $permissions['write'],
      ),
      'read' => array(
        '#type' => 'checkbox',
        '#title' => t('read'),
        '#name' => 'custom[users]['.$user.'][read]',
        '#default_value' => $permissions['read'],
      ),
      'banned' => array(
        '#type' => 'checkbox',
        '#title' => t('ban this user'),
        '#name' => 'custom[users]['.$user.'][banned]',
        '#default_value' => $permissions['banned'],
      ),
      'remove' => array(
        '#type' => 'submit',
        '#value' => t('Remove any special access for this user.'),
        '#attached' => array(
          'js' => array(drupal_get_path('module', 'tribune') . '/tribune-admin.js'),
        ),
      ),
    );

    if (!$info['moderated']) {
      unset($form['custom']['users'][$user]['mod']);
    }
  }

  if (count($tribune->tribune_bans)) {
    $data = array();

    foreach ($tribune->tribune_bans as $id => $ban) {
      $form['bans']['patterns']['list']['values'][$id] = array(
        'ip' => array(
          '#type' => 'hidden',
          '#value' => $ban['ip'],
        ),
        'ua' => array(
          '#type' => 'hidden',
          '#value' => $ban['ua'],
        ),
      );
    }
    $table = tribune_access_ban_table($tribune, $tribune->tribune_bans);
    $form['bans']['patterns']['list']['table'] = array(
      '#markup' => $table,
    );
  }

  if (isset($form_state['values']['bans']['patterns']['new']['ip'])) {
    $ip = $form_state['values']['bans']['patterns']['new']['ip'];
    $ua = $form_state['values']['bans']['patterns']['new']['ua'];

    if ($ip or $ua) {
      $id = max(array_keys($form_state['values']['bans']['patterns']['list']['values'])) + 1;
      $form['bans']['patterns']['list']['values'][$id]['ip'] = array(
        '#type' => 'hidden',
        '#value' => $ip,
      );
      $form['bans']['patterns']['list']['values'][$id]['ua'] = array(
        '#type' => 'hidden',
        '#value' => $ua,
      );
    }
  }

  if (isset($form_state['values']['bans']['patterns']['list']['values'])) {
    foreach ($form_state['values']['bans']['patterns']['list']['values'] as $id => $pattern) {
      $form['bans']['patterns']['list']['values'][$id]['ip'] = array(
        '#type' => 'hidden',
        '#value' => $pattern['ip'],
      );
      $form['bans']['patterns']['list']['values'][$id]['ua'] = array(
        '#type' => 'hidden',
        '#value' => $pattern['ua'],
      );
    }
  }

  return $form;
}

function tribune_access_add_user($form, $form_state) {
  $user = $form_state['values']['custom']['users']['new'];
  $tribune = $form_state['values']['tribune'];

  $info = tribune_info($tribune);

  if ($user) {
    $form = array(
      '#name' => 'custom[users]['.$user.']',
      '#title' => t('Custom access rights for user: @name', array('@name' => $user)),
      '#type' => 'fieldset',
      'mod' => array(
        '#type' => 'checkbox',
        '#title' => t('moderation'),
        '#name' => 'custom[users]['.$user.'][mod]',
      ),
      'write' => array(
        '#type' => 'checkbox',
        '#title' => t('write'),
        '#name' => 'custom[users]['.$user.'][write]',
        '#default_value' => 1,
        '#attributes' => array('checked' => 'checked'),
      ),
      'read' => array(
        '#type' => 'checkbox',
        '#title' => t('read'),
        '#name' => 'custom[users]['.$user.'][read]',
        '#default_value' => 1,
        '#attributes' => array('checked' => 'checked'),
      ),
      'banned' => array(
        '#type' => 'checkbox',
        '#title' => t('ban this user'),
        '#name' => 'custom[users]['.$user.'][banned]',
        '#default_value' => 0,
      ),
    );

    if (!$info['moderated']) {
      unset($form['mod']);
    }

    return $form;
  }
}

function tribune_access_unban($tribune, $id) {
  unset($tribune->tribune_bans[$id]);
  tribune_update($tribune);
  drupal_goto('node/' . $tribune->nid . '/tribune-access', array('fragment' => 'edit-bans'));
}

function tribune_access_ban_table($tribune, $bans) {
  foreach ($bans as $id => $pattern) {
    $unban_link = l(t('Unban'), 'node/' . $tribune->nid . '/tribune-access/unban/' . $id);
    $data[] = array(
      $pattern['ip'],
      $pattern['ua'],
      $unban_link,
    );
  }

  return theme('table', array(
    'rows' => $data,
    'header' => array(
      t('IP'),
      t('User-Agent'),
      t('Remove'),
    ),
  ));
}

function tribune_access_add_ban($form, $form_state) {
  $form = array();

  $form['bans']['patterns']['list'] = array(
    '#prefix' => '<div id="tribune_access_ban_wrapper">',
    '#suffix' => '</div>',
  );

  $tribune = $form_state['values']['tribune'];
  $table = tribune_access_ban_table($tribune, $form_state['values']['bans']['patterns']['list']['values']);

  $form['bans']['patterns']['list']['table'] = array(
    '#markup' => $table,
  );

  return $form['bans']['patterns']['list'];
}

function _tribune_access_page_form_submit($form, $form_state) {
  $custom_permissions = array();
  foreach ($form_state['input']['custom']['users'] as $user => $permissions) {
    $empty = TRUE;
    if (is_array($permissions)) foreach ($permissions as $perm) {
      if ($perm) {
        $empty = FALSE;
      }
    }
    if (!$empty and $user != 'new') {
      $uid = user_load_by_name($user)->uid;

      if ($uid) {
        $custom_permissions[$uid] = $permissions;
      }
    }
  }
  $tribune = $form_state['values']['tribune'];
  $tribune->tribune_permissions = array(
    'mod' => 'owner',
    'write' => $form_state['values']['write']['type'],
    'read' => $form_state['values']['read']['type'],
    'custom' => $custom_permissions,
  );

  $bans = array();
  if (isset($form_state['values']['bans']['patterns']['list'])) {
    $bans = array_values($form_state['values']['bans']['patterns']['list']['values']);
  }
  $tribune->tribune_bans = $bans;

  if (isset($form_state['values']['censorship']['patterns'])) {
    $patterns = explode("\n", $form_state['values']['censorship']['patterns']);
  }
  $tribune->tribune_censorship = array(
    'patterns' => $patterns,
    'type' => $form_state['values']['censorship']['type'],
  );

  tribune_update($tribune);
}

