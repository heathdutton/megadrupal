This module provides control and sending complaints to the comments. It will help you in fighting with spam in comments.

Features:
 - complaints will be sent by AJAX;
 - two modes of send complaints - simple link and popup with complaint form;
 - filter by content types;
 - notification by email about complaints;
 - support views and rules;

Little additional info:

 - Author of comment can not complain about your own comment;
 - Each user can complain about a comment only once:
    for authorized users, verifies the user ID;
    for anonymous users, checked by IP-address.

Dependencies:
 - Ctools
 - Views


For developers:

Link to complaints about the comment will be added automatically, but you can render link for complaint programmatically:

<?php
  print comment_abuse_get_link($link_text, $comment_id);
?>

How to customize popup http://drupal.org/node/1905036

Troubleshooting:
Do not displayed a link to a complaint? The three main reasons:

 - No links will be displayed for comments (check if visible links to edit, delete).
 - You can not complain about your comments and a link to complain about a comment, you can only see on other people's comments.
 - Check permissions

Credits:
Initial development was supported by Drupal Jedi
