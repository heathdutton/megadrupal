<?php

/**
 * @file
 * Functions to create/link/delete discussions.
 */


/**
 * Create a discussion comment form.
 *
 * @param $form_state The current state of the form
 * @param $nid The node that is being discussed
 *
 * @return The form in HTML
 */
function discussthis_create_discussion_form($form, &$form_state, $nid) {
  // the user usually enters this function because the topic does not
  // exist yet, this form asks for the comment with our own discussthis
  // form (i.e. avoid creating a new forum node until the person posts
  // his/her comment out)
  //
  // IMPORTANT: we do NOT check whether the topic already exists because
  // it might when the person clicks on Preview. Please, see the submit
  // function for more information in that regard. Just don't add a test
  // for the topic ID in this function...

  $title = variable_get('discussthis_new_post_title', '');
  if ($title) {
    drupal_set_title($title);
  }
  
  global $user;

  $op = empty($form_state['post']['op']) ? '' : $form_state['post']['op'];
  $is_ready = $op == t('Preview') || $op == t('Save');
  if ($is_ready) {
    $comment = $user;
    $comment->comment = check_markup($form_state['post']['comment'], $form_state['post']['format'], $langcode = '' /* TODO Set this variable. */, FALSE);
    $comment->timestamp = REQUEST_TIME;
    $comment->new = FALSE; // this is not displayed in a very good way, in general
    $comment->preview = TRUE;
    $comment->subject = $form_state['post']['subject'];
    $node = array(
      'type' => 'forum',
    );
    $node = (object) $node;
    $form['preview_comment'] = array(
      '#title' => t('Preview'),
      '#value' => theme('comment', $comment, $node),
    );
  }

  $comment_anonymous_forum = variable_get('comment_anonymous_forum', COMMENT_ANONYMOUS_MAYNOT_CONTACT);
  if (!$user->uid && $comment_anonymous_forum != COMMENT_ANONYMOUS_MAYNOT_CONTACT) {
    drupal_add_js(drupal_get_path('module', 'comment') . '/comment.js');
  }

  if ($user->uid) {
    $form['_author'] = array(
      '#type' => 'item',
      '#title' => t('Your name'),
      '#value' => theme('username', array('account' => $user)),
    );
    $form['author'] = array(
      '#type' => 'value',
      '#value' => $user->name,
    );
  }
  elseif ($comment_anonymous_forum == COMMENT_ANONYMOUS_MAY_CONTACT) {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Your name'),
      '#maxlength' => 60,
      '#size' => 30,
      '#default_value' => variable_get('anonymous', t('Anonymous')),
    );

    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#maxlength' => 64,
      '#size' => 30,
      '#description' => t('The content of this field is kept private and will not be shown publicly.'),
    );

    $form['homepage'] = array(
      '#type' => 'textfield',
      '#title' => t('Homepage'),
      '#maxlength' => 255,
      '#size' => 30,
    );
  }
  elseif ($comment_anonymous_forum == COMMENT_ANONYMOUS_MUST_CONTACT) {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Your name'),
      '#maxlength' => 60,
      '#size' => 30,
      '#default_value' => variable_get('anonymous', t('Anonymous')),
      '#required' => TRUE,
    );

    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#maxlength' => 64,
      '#size' => 30,
      '#description' => t('The content of this field is kept private and will not be shown publicly.'),
      '#required' => TRUE,
    );

    $form['homepage'] = array(
      '#type' => 'textfield',
      '#title' => t('Homepage'),
      '#maxlength' => 255,
      '#size' => 30,
    );
  }

  if (variable_get('comment_subject_field_forum', 1) == 1) {
    $form['subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#maxlength' => 64,
    );
  }

  $form['comment_filter']['comment'] = array(
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#title' => t('Comment'),
      '#rows' => 15,
      '#required' => TRUE,
      '#format' => isset($edit['format']) ? $edit['format'] : NULL,
  );
  
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['discussthis_nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 19,
  );
  $form['#submit'][] = 'discussthis_create_discussion_form_submit';

  $form['preview'] = array(
    '#type' => 'button',
    '#value' => t('Preview'),
    '#weight' => 20,
  );

  return $form;
}



/**
 * Save the new comment.
 *
 * This function creates a new forum topic if this is the first
 * comment on this node. Otherwise, it just adds the comment to
 * the existing forum.
 *
 * @todo
 * We've got a problem here, if the administrator wants to moderate
 * these comments, then we should not create the forum node. This
 * is a similar problem to the one we see in creating the forum
 * node before the comment is even posted to us.
 *
 * I guess that the biggest problem is that a comment, to exist,
 * needs to be attached to a node.
 *
 * I see two potential solutions:
 *
 * 1) we handle our own comments
 *
 * 2) we create the comment on the node being discussed and move
 *    it later to the forum
 *
 * @param $form The form info
 * @param $form_state The current state of the form, including the values
 */
function discussthis_create_discussion_form_submit($form, &$form_state) {
  global $user;

  // start a new transaction : if this fails, we should revert it all
  $transaction = db_transaction();
  try {
    // we got a post, so we want to create the forum node
  
    // load the node being discussed
    $node = node_load($form_state['values']['discussthis_nid']);
    if (!$node) {
      // what shall we do here? the admin could delete the node while
      // a user is trying to discuss it...
      drupal_set_message(t('The node being discussed is not available anymore. (nid: @nid)', array('@nid' => $form_state['values']['discussthis_nid'])), 'error');
      drupal_goto('');
    }
  
    
    $language = $node->language;
    $discussthis_types_config = variable_get('discussthis_types_config', array());
    
    // Create the new topic
    $topic = (object)array();
    $topic->uid       = token_replace(variable_get('discussthis_author', '[current-user:uid]'), array('user' => $user));    
    $topic->type       = 'forum';
    $topic->taxonomy_forums['und'][0]['tid'] = isset($discussthis_types_config[$node->type][$node->type . '_forum']) ? $discussthis_types_config[$node->type][$node->type . '_forum'] : 0;
    $topic->created   = time();
    $topic->status     = 1;          // To have published, else use 0
    $topic->promote  = 0;
    $topic->sticky   = 0;
    $topic->revision = 1;
    $topic->language = $language;
    $topic->comment = 2;
    $topic->title    = token_replace(variable_get('discussthis_newsubject', 'Discussion on [node:content-type] : [node:title]'), array('node' => $node));
    $topic->body[$language][0] = array(
        'value'  => token_replace(variable_get('discussthis_newtemplate', DISCUSSTHIS_DEFAULT_NODE_MESSAGE), array('node' => $node)),
        'format' => 'full_html',
    );
    $topic->teaser   = '';
    $topic->log      = 'Auto Created Forum Topic';
    
    node_submit($topic);
    node_save($topic);
  
    if ($topic->nid) {
      // If it did work, associate the node with the topic
      $discussthis_topic['nid'] = $node->nid;
      $discussthis_topic['topic_nid'] = $topic->nid;
      _discussthis_set_topic($discussthis_topic);
    } else {
      // It did not work... hmmm...
      drupal_set_message('Forum post could not be created for your comment to be published.', 'error');
      drupal_goto();
    }
  
    // Create the comment on the new discussion topic
    $comment = (object) array(
        'nid' => $topic->nid,
        'cid' => 0,
        'pid' => 0,
        'uid' => $user->uid,
        'mail' => '',
        'is_anonymous' => user_is_anonymous(),
        'homepage' => '',
        'status' => COMMENT_PUBLISHED,
        'subject' => $form_state['values']['subject'],
        'language' => $node->language,
        'comment_body' => array(
            'und' => array(
                0 => array (
                    'value' => $form_state['values']['comment']['value'],
                    'format' => $form_state['values']['comment']['format'],
                )
            )
        ),
    );
    comment_submit($comment);
    comment_save($comment);
  
    $form_state['redirect'] = 'node/' . $topic->nid;
  
    // if you have boost, we want to reset the discussed node since
    // now it will look "different" (the link and eventually the new
    // comment)
    if (function_exists('boost_expire_node')) {
      boost_expire_node($node);
    }
    
  } catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('discussthis', $e);
  }
}

function template_preprocess_discussion_render_wrapper(&$variables) {
  // Provide contextual information.
  $variables['node'] = $variables['content']['#node'];
  $variables['display_mode'] = variable_get('comment_default_mode_' . $variables['node']->type, COMMENT_MODE_THREADED);
  // The comment form is optional and may not exist.
  $variables['content'] += array('comment_form' => array());
}
function template_preprocess_discussion_render_item(&$variables) {
  template_preprocess_comment($variables);
}