<?php
GLOBAL $user;
/**
 * @file
 * Default theme implementation to present all variables to the theme layer for
 * the authored user block of the user_details module.
 *
 * Useable variables: (syntax: <?php print variable; ?>)
 * User Stats
 * - $user_details_authored_picture (user picture of the authored user)
 * - $user_details_authored_created (the date the authored user's account was created)
 * - $user_details_authored_posts (the number of posts the authored user has created)
 * - $user_details_authored_comments (the number of comments the authored user has created)
 * - $user_details_authored_combined (combines the number of posts and the number of comments into one number)
 * - $user_details_authored_points_title (the "branding" from the user_points module ie. points)
 * - $user_details_authored_points (the number of "User Points" the authored user has)
 * - $user_details_authored_role (the highest role name the authored user has)
 * Built-in Links
 * - $user_details_authored_profile_url (the URL of the authored users profile)
 * - $user_details_authored_profile_imgsrc (the image of the authored users profile link)
 * - $user_details_authored_privatemsg_url (the URL of the authored users profile private message tab)
 * - $user_details_authored_privatemsg_imgsrc (the image of the authored users profile private message link)
 * - $user_details_authored_blog_url (the URL of the authored users blog page)
 * - $user_details_authored_blog_imgsrc (the image of the authored users blog link)
 * - $user_details_authored_logout_imgsrc (the image of the authored users logout link)
 * Custom Links
 * - $user_details_authored_link_one_url (the URL for the "Link One" link)
 * - $user_details_authored_link_one_imgsrc (the image for the "Link One" link)
 * - $user_details_authored_link_one_title (the title for the "Link One" link)
 * - $user_details_authored_link_two_url (the URL for the "Link Two" link)
 * - $user_details_authored_link_two_imgsrc (the image for the "Link Two" link)
 * - $user_details_authored_link_two_title (the title for the "Link Two" link)
 * - $user_details_authored_link_three_url (the URL for the "Link Three" link)
 * - $user_details_authored_link_three_imgsrc (the image for the "Link Three" link)
 * - $user_details_authored_link_three_title (the title for the "Link Three" link)
 * - $user_details_authored_link_four_url (the URL for the "Link Four" link)
 * - $user_details_authored_link_four_imgsrc (the image for the "Link Four" link)
 * - $user_details_authored_link_four_title (the title for the "Link Four" link)
 * - $user_details_authored_link_five_url (the URL for the "Link Five" link)
 * - $user_details_authored_link_five_imgsrc (the image for the "Link Five" link)
 * - $user_details_authored_link_five_title (the title for the "Link Five" link)
 * User Content
 * - $user_details_authored_content (used to push the "User Content" content list to the theme layer)
 */
?>
<div id="user-details">
  <div id="authored">
    <div class="stats">
    <?php if (variable_get('user_details_authored_picture_view') != 0): ?>
      <div class="avatar"><?php print $user_details_authored_picture; ?></div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_created_view')!= 0): ?>
      <div class="joined">
        <div class="title">Created:</div>
        <div class="result"><?php print $user_details_authored_created; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_posts_view') != 0): ?>
      <div class="posts-count">
        <div class="title">Posts:</div>
        <div class="result"><?php print $user_details_authored_posts; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_comments_view') != 0 && module_exists('comment')): ?>
      <div class="comment-count">
        <div class="title">Comments:</div>
        <div class="result"><?php print $user_details_authored_comments; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_combined_view') != 0 && module_exists('comment')): ?>
      <div class="post-comment-combine">
        <div class="title">Posts & Comments:</div>
        <div class="result"><?php print $user_details_authored_combined; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_points_view') != 0 && module_exists('userpoints')): ?>
      <div class="points">
        <div class="title"><?php print $user_details_authored_points_title; ?>:</div>
        <div class="result"><?php print $user_details_authored_points; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_role_view') != 0): ?>
      <div class="rank">
        <div class="title">Role:</div>
        <div class="result"><?php print $user_details_authored_role; ?></div>
      </div>
    <?php endif; ?>
	</div>
	<?php if (variable_get('user_details_authored_profile_view') != 0 || variable_get('user_details_authored_blog_view') != 0 || variable_get('user_details_authored_privatemsg_view') != 0 || variable_get('user_details_authored_link_one_view') == 2 || variable_get('user_details_authored_link_two_view') == 2 || variable_get('user_details_authored_link_three_view') == 2 || variable_get('user_details_authored_link_four_view') == 2 || variable_get('user_details_authored_link_five_view') == 2): ?>
      <hr>
	<?php endif; ?>
    <div class="user-quick-links">
	  <?php if (variable_get('user_details_authored_profile_view') != 0 || variable_get('user_details_authored_blog_view') != 0 || variable_get('user_details_authored_privatemsg_view') != 0 || variable_get('user_details_authored_link_one_view') == 2 || variable_get('user_details_authored_link_two_view') == 2 || variable_get('user_details_authored_link_three_view') == 2 || variable_get('user_details_authored_link_four_view') == 2 || variable_get('user_details_authored_link_five_view') == 2): ?>
        <div class="title">User Links</div>
	  <?php endif; ?>
      <?php if (variable_get('user_details_authored_profile_view') != 0): ?>
        <div class="profile">
          <a href="<?php print $user_details_authored_profile_url; ?>"><img src="<?php print $user_details_authored_profile_imgsrc; ?>" title="My Profile" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_private_message_view') != 0 && module_exists('privatemsg')): ?>
        <div class="privatemsg">
          <a href="<?php print $user_details_authored_private_message_url; ?>"><img src="<?php print $user_details_authored_private_message_imgsrc; ?>" title="My Private Messages" width="20px" height="20px" /></a>            
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_blog_view') != 0 && module_exists('blog')): ?>
        <div class="blog">
          <a href="<?php print $user_details_authored_blog_url; ?>"><img src="<?php print $user_details_authored_blog_imgsrc; ?>" title="My Blogs" width="20px" height="20px" /></a>            
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_link_one_type') == '2'): ?>
        <div class="link-one">
          <a href="<?php print $user_details_authored_link_one_url; ?>"><img src="<?php print $user_details_authored_link_one_imgsrc; ?>" title="<?php print $user_details_authored_link_one_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_link_two_type') == '2'): ?>
        <div class="link-two">
          <a href="<?php print $user_details_authored_link_two_url; ?>"><img src="<?php print $user_details_authored_link_two_imgsrc; ?>" title="<?php print $user_details_authored_link_two_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_link_three_type') == '2'): ?>
        <div class="link-three">
          <a href="<?php print $user_details_authored_link_three_url; ?>"><img src="<?php print $user_details_authored_link_three_imgsrc; ?>" title="<?php print $user_details_authored_link_three_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_link_four_type') == '2'): ?>
        <div class="link-four">
          <a href="<?php print $user_details_authored_link_four_url; ?>"><img src="<?php print $user_details_authored_link_four_imgsrc; ?>" title="<?php print $user_details_authored_link_four_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_authored_link_five_type') == '2'): ?>
        <div class="link-five">
          <a href="<?php print $user_details_authored_link_five_url; ?>"><img src="<?php print $user_details_authored_link_five_imgsrc; ?>" title="<?php print $user_details_authored_link_five_imgtitle; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
    </div>
    <div class="block-end"></div>
    <?php if (variable_get('user_details_authored_content_view') != 0 && variable_get('user_details_authored_content_amount') != 0 && variable_get('user_details_authored_content_truncate')): ?>
      <hr>
    <?php endif; ?>
    <?php if (variable_get('user_details_authored_content_view') != 0 && variable_get('user_details_authored_content_amount') != 0 && variable_get('user_details_authored_content_truncate') != 0): ?>
      <div class="user-content">
        <div class="title">My Content</div>
        <ul class="result">
          <?php print $user_details_authored_content; ?>
        </ul>
	  </div>
    <?php endif; ?>
    <div class="block-end"></div>
  </div>
</div>