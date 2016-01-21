<?php

/**
 * @file
 * Default theme implementation for rendering contact centre messages.
 *
 * This template is used when view individual messages within the contact 
 * centre admin.
 *
 * Available variables:
 * - $book_menus: Array of book outlines keyed to the parent book ID. Call
 *   render() on each to print it as an unordered list.
 *
 * - $message_id        : (int) The ID of the message being viewed.
 * - $contact_category  : (str) The contact category selected by the user.
 * - $uid               : (int) User ID of the user or 0 for anonymous.
 * - $user_name         : (str) Name entered on the contact form.
 * - $user_email        : (str) Email address entered on the contact form.
 * - $subject           : (str) Subject entered on the contact form.
 * - $message           : (str) The message.
 * - $created           : (int) Timestamp when message was created.
 * - $opened            : (int) Timestamp when message was opened or 0
 *                              for unopened.
 * - $resolved          : (int) Timestamp when message was resolved or 0
 *                              for unresolved.
 * - $notes             : (ary) An Array of notes recorded against the
 *                              message.
 *
 */
?>
<div class="contact-centre-message">
  <h2><?php print t('Message ID: !mid', array('!mid' => $message_id)); ?></h2>
  <?php

    // output the message details
    print '<h3>' . t('Message Details') . '</h3>';
    print theme(
      'table',
      array(
        'header' => array(
          array('data' => t('Contact Category')),
          array('data' => t('User name')),
          array('data' => t('Email address')),
          array('data' => t('Created')),
          array('data' => t('Opened')),
          array('data' => t('Resolved')),
        ),
        'rows' => array(
          'data' => array(
            array('data' => t('!contact_category', array('!contact_category' => $contact_category))),
            array('data' => check_plain($user_name)),
            array('data' => check_plain($user_email)),
            array('data' => $created),
            array('data' => t('!opened', array('!opened' => $opened))),
            array('data' => t('!resolved', array('!resolved' => $resolved))),
          ),
        ),
      )
    );

    // output the subject and message
    print '<h3>' . t('Message') . '</h3>';
    print theme(
      'table',
      array(
        'rows' => array(
          array(
            'data' => array(
              array('data' => t('Subject'), 'class' => 'contact-centre-message-shaded-cell'),
              array('data' => check_plain($subject), 'class' => 'contact-centre-message-unshaded-cell'),
            ),
            'no_striping' => TRUE,
          ),
          array(
            'data' => array(
              array('data' => t('Message'), 'class' => 'contact-centre-message-shaded-cell'),
              array('data' => check_plain($message), 'class' => 'contact-centre-message-unshaded-cell'),
            ),
            'no_striping' => TRUE,
          ),
        ),
      )
    );

    print '<h3>' . t('Notes') . '</h3>';
    print theme(
      'table',
      array(
        'header' => array(
          array('data' => t('Created')),
          array('data' => t('User')),
          array('data' => t('Note')),
          array('data' => t('Sent to Sender')),
        ),
        'rows' => $notes,
        'empty' => t('There are no notes recorded for this message.'),
      )
    );

  ?>
</div>
