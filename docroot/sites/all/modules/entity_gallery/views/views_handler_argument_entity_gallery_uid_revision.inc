<?php

/**
 * @file
 * Definition of views_handler_argument_entity_gallery_uid_revision.
 */

/**
 * Filter handler to accept a user id to check for entity galleries that
 * user posted or created a revision on.
 */
class views_handler_argument_entity_gallery_uid_revision extends views_handler_argument_comment_user_uid {
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $placeholder = $this->placeholder();
    $this->query->add_where_expression(0, "$this->table_alias.uid = $placeholder OR ((SELECT COUNT(*) FROM {entity_gallery_revision} egr WHERE egr.uid = $placeholder AND egr.egid = $this->table_alias.egid) > 0)", array($placeholder => $this->argument));
  }
}
