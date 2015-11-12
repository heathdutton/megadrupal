<?php

/**
 * @file
 * Template for topic_map association
 *
 * Available variables:
 * - $type: typeof association(Optional)
 * - $roles: Members of the assocation(Mandatory)
 */
?>
<association>
  <?php if (!empty($type)): ?>
    <type>
      <topicRef href="#<?php echo $type; ?>" />
    </type>
  <?php endif; ?>
  <?php foreach ($roles as $key => $role): ?>
    <role>
      <type>
        <topicRef href="#<?php echo $role->role ?>"/>
      </type>
      <topicRef href="#<?php echo $role->actor ?>"/>
    </role>
  <?php endforeach; ?>
</association>
