<?php
/** Template for user_information theme
 * used to theme the new user block & online user block
 * available variable :
 * $user_picture
 * $user_name
 * $user_info
 * $user_contact
 */
?>
<div class="comment-block clearfix">
  <?php if (!empty($user_picture)) print $user_picture; ?>
  <div class="content-comment <?php if (empty($user_picture)) print 'no-picture'; ?>">
  	<h6><?php if (!empty($user_name)) print $user_name; ?> </h6>
  	<p><?php if (!empty($user_info)) print $user_info; ?></p>
  	<p><?php if (!empty($user_contact)) print $user_contact; ?></p>
  </div>
</div>
