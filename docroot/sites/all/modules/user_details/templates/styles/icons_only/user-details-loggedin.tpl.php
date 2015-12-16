<?php
GLOBAL $user;
/**
 * @file
 * Default theme implementation to present all variables to the theme layer for
 * the logged-in user block of the user_details module.
 *
 * Useable variables: (syntax: <?php print variable; ?>)
 * User Stats
 * - $user_details_loggedin_picture (user picture of the logged-in user)
 * - $user_details_loggedin_created (the date the logged-in user's account was created)
 * - $user_details_loggedin_posts (the number of posts the logged-in user has created)
 * - $user_details_loggedin_comments (the number of comments the logged-in user has created)
 * - $user_details_loggedin_combined (combines the number of posts and the number of comments into one interger)
 * - $user_details_loggedin_points_title (the "branding" from the user_points module ie. points)
 * - $user_details_loggedin_points (the number of user_points the logged-in user has)
 * - $user_details_loggedin_role (the highest role name the logged-in user has)
 * - $user_details_loggedin_privatemsg_unread (the number of unread private messages the logged-in user has)
 * - $user_details_loggedin_front_imgsrc (the image for the front link)
 * - $user_details_loggedin_profile_url (the logged-in users profile url)
 * Built-in Links
 * - Front
 * -- $user_details_loggedin_front_imgsrc (the image source for the "Front" link)
 * - Profile
 * -- $user_details_loggedin_profile_imgsrc (the image source for the "Profile" link)
 * - Edit Profile
 * -- $user_details_loggedin_profile_edit_imgsrc (the image source for the "Profile Edit" link)
 * - Private Messages
 * -- $user_details_loggedin_private_message_imgsrc (the image source for the "Private Messages" link)
 * - Blog
 * -- $user_details_loggedin_blog_imgsrc (the image source for the "Blog" link)
 * - Create Content
 * -- $user_details_loggedin_create_imgsrc (the image source for the "Create Content" link)
 * - Ubercart cart
 * -- $user_details_loggedin_ubercart_cart_imgsrc (the image source for the "Ubercart Cart" link)
 * - Search
 * -- $user_details_loggedin_search_imgsrc (the image source for the "Search" link)
 * - Logout
 * -- $user_details_loggedin_logout_imgsrc (the image source for the "Logout" link)
 * Custom Links
 * - Link One
 * -- $user_details_loggedin_link_one_url (the URL for the "Link One" link)
 * -- $user_details_loggedin_link_one_imgsrc (the image source for the "Link One" link)
 * -- $user_details_loggedin_link_one_title (the title for the "Link One" image)
 * - Link Two
 * -- $user_details_loggedin_link_two_url (the URL for the "Link Two" link)
 * -- $user_details_loggedin_link_two_imgsrc (the image source for the "Link Two" link)
 * -- $user_details_loggedin_link_two_title (the title for the "Link Two" image)
 * - Link Three
 * -- $user_details_loggedin_link_three_url (the URL for the "Link Three" link)
 * -- $user_details_loggedin_link_three_imgsrc (the image source for the "Link Three" link)
 * -- $user_details_loggedin_link_three_title (the title for the "Link three" image)
 * - Link Four
 * -- $user_details_loggedin_link_four_url (the URL for the "Link Four" link)
 * -- $user_details_loggedin_link_four_imgsrc (the image source for the "Link Four" link)
 * -- $user_details_loggedin_link_four_title (the title for the "Link Four" image)
 * - Link Five
 * -- $user_details_loggedin_link_five_url (the URL for the "Link Five" link)
 * -- $user_details_loggedin_link_five_imgsrc (the image source for the "Link Five" link)
 * -- $user_details_loggedin_link_five_title (the title for the "Link Five" image)
 * Admin Links
 * - Admin page
 * -- $user_details_loggedin_admin_imgsrc (the image source for the "Admin" link)
 * - User Details admin
 * -- $user_details_loggedin_user_details_imgsrc (the image source for the "User Details" link)
 * - Panels admin
 * -- $user_details_loggedin_panels_imgsrc (the image source for the "Panels" link)
 * - Views admin
 * -- $user_details_loggedin_views_imgsrc (the image source for the "Views" link)
 * - Performance admin
 * -- $user_details_loggedin_performance_imgsrc (the image source for the "Performance" link)
 * User Content
 * - $user_details_loggedin_content (used to push the "User Content" to the theme layer)
 */
?>
<div id="user-details">
  <div id="loggedin">
    <div class="stats">
    <?php if (variable_get('user_details_loggedin_picture_view') != 0): ?>
      <div class="avatar"><?php print $user_details_loggedin_picture; ?></div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_created_view')!= 0): ?>
      <div class="joined">
        <div class="title">Created:</div>
        <div class="result"><?php print $user_details_loggedin_created; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_posts_view') != 0): ?>
      <div class="posts-count">
        <div class="title">Posts:</div>
        <div class="result"><?php print $user_details_loggedin_posts; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_comments_view') != 0): ?>
      <div class="comment-count">
        <div class="title">Comments:</div>
        <div class="result"><?php print $user_details_loggedin_comments; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_combined_view') != 0): ?>
      <div class="post-comment-combine">
        <div class="title">Posts & Comments:</div>
        <div class="result"><?php print $user_details_loggedin_combined; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_points_view') != 0 && module_exists('userpoints')): ?>
      <div class="points">
        <div class="title"><?php print $user_details_loggedin_points_title; ?>:</div>
        <div class="result"><?php print $user_details_loggedin_points; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_role_view') != 0): ?>
      <div class="rank">
        <div class="title">Role:</div>
        <div class="result"><?php print $user_details_loggedin_role; ?></div>
      </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_privatemsg_unread_view') != 0 && module_exists('privatemsg')): ?>
    <div class="private-message-count">
      <div class="title">Private Messages:</div>
      <div class="result"><?php print $user_details_loggedin_privatemsg_unread; ?></div>
    </div>
    <?php endif; ?>
	</div>
	<?php if (variable_get('user_details_loggedin_front_view') != 0 || variable_get('user_details_loggedin_profile_view') != 0 || variable_get('user_details_loggedin_profile_edit_view') != 0 || variable_get('user_details_loggedin_private_message_view') != 0 || variable_get('user_details_loggedin_create_view') != 0 || variable_get('user_details_loggedin_ubercart_cart_view') != 0 || variable_get('user_details_loggedin_logout_view') != 0 || variable_get('user_details_loggedin_link_one_view') == 2 || variable_get('user_details_loggedin_link_two_view') == 2 || variable_get('user_details_loggedin_link_three_view') == 2 || variable_get('user_details_loggedin_link_four_view') == 2 || variable_get('user_details_loggedin_link_five_view') == 2): ?>
      <hr>
	<?php endif; ?>
    <div class="user-quick-links">
	  <?php if (variable_get('user_details_loggedin_front_view') != 0 || variable_get('user_details_loggedin_profile_view') != 0 || variable_get('user_details_loggedin_profile_edit_view') != 0 || variable_get('user_details_loggedin_private_message_view') != 0 || variable_get('user_details_loggedin_create_view') != 0 || variable_get('user_details_loggedin_ubercart_cart_view') != 0 || variable_get('user_details_loggedin_logout_view') != 0 || variable_get('user_details_loggedin_link_one_view') == 2 || variable_get('user_details_loggedin_link_two_view') == 2 || variable_get('user_details_loggedin_link_three_view') == 2 || variable_get('user_details_loggedin_link_four_view') == 2 || variable_get('user_details_loggedin_link_five_view') == 2): ?>
        <div class="title">User Links</div>
	  <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_front_view') != 0): ?>
        <div class="front">
          <a href="/"><img src="<?php print $user_details_loggedin_front_imgsrc; ?>" title="Home" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_profile_view') != 0): ?>
        <div class="profile">
          <a href="<?php print $user_details_loggedin_profile_url; ?>"><img src="<?php print $user_details_loggedin_profile_imgsrc; ?>" title="My Profile" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_profile_edit_view') != 0): ?>
        <div class="profile-edit">
          <a href="<?php print $user_details_loggedin_profile_edit_url; ?>"><img src="<?php print $user_details_loggedin_profile_edit_imgsrc; ?>" title="Edit My Profile" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_private_message_view') != 0 && module_exists('privatemsg')): ?>
        <div class="privatemsg">
          <a href="<?php print $user_details_loggedin_private_message_url; ?>"><img src="<?php print $user_details_loggedin_private_message_imgsrc; ?>" title="My Private Messages" width="20px" height="20px" /></a>            
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_blog_view') != 0 && module_exists('blog')): ?>
        <div class="blog">
          <a href="<?php print $user_details_loggedin_blog_url; ?>"><img src="<?php print $user_details_loggedin_blog_imgsrc; ?>" title="My Blog" width="20px" height="20px" /></a>            
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_create_view') != 0): ?>
        <div class="create">
          <a href="/node/add"><img src="<?php print $user_details_loggedin_create_imgsrc; ?>" title="Create Content" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_ubercart_cart_view') != 0 && module_exists('uc_cart')): ?>
        <div class="ubercart-cart">
          <a href="/cart"><img src="<?php print $user_details_loggedin_ubercart_cart_imgsrc; ?>" title="My Cart" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_search_view') != 0 && module_exists('search')): ?>
        <div class="search">
          <a href="/search"><img src="<?php print $user_details_loggedin_search_imgsrc; ?>" title="Search" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_logout_view') != 0): ?>
        <div class="logout">
          <a href="/user/logout"><img src="<?php print $user_details_loggedin_logout_imgsrc; ?>" title="Logout of the Site" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_one_type') == '2'): ?>
        <div class="link-one">
          <a href="<?php print $user_details_loggedin_link_one_url; ?>"><img src="<?php print $user_details_loggedin_link_one_imgsrc; ?>" title="<?php print $user_details_loggedin_link_one_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_two_type') == '2'): ?>
        <div class="link-two">
          <a href="<?php print $user_details_loggedin_link_two_url; ?>"><img src="<?php print $user_details_loggedin_link_two_imgsrc; ?>" title="<?php print $user_details_loggedin_link_two_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_three_type') == '2'): ?>
        <div class="link-three">
          <a href="<?php print $user_details_loggedin_link_three_url; ?>"><img src="<?php print $user_details_loggedin_link_three_imgsrc; ?>" title="<?php print $user_details_loggedin_link_three_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_four_type') == '2'): ?>
        <div class="link-four">
          <a href="<?php print $user_details_loggedin_link_four_url; ?>"><img src="<?php print $user_details_loggedin_link_four_imgsrc; ?>" title="<?php print $user_details_loggedin_link_four_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_five_type') == '2'): ?>
        <div class="link-five">
          <a href="<?php print $user_details_loggedin_link_five_url; ?>"><img src="<?php print $user_details_loggedin_link_five_imgsrc; ?>" title="<?php print $user_details_loggedin_link_five_title; ?>" width="20px" height="20px" /></a>
        </div>
      <?php endif; ?>
    </div>
    <div class="block-end"></div>
    <?php if (user_access('administor content')): ?>
	  <div class="admin-quick-links">
        <?php if (variable_get('user_details_loggedin_admin_view') !=0 || variable_get('user_details_loggedin_panels_view') != 0 || variable_get('user_details_loggedin_views_view') || variable_get('user_details_loggedin_link_one_type') == '3' || variable_get('user_details_loggedin_link_two_type') == '3' || variable_get('user_details_loggedin_link_three_type') == '3' || variable_get('user_details_loggedin_link_four_type') == '3' || variable_get('user_details_loggedin_link_five_type') == '3'): ?>
          <div class="title">Admin Links</div>
		<?php endif; ?>
        <?php if (variable_get('user_details_loggedin_admin_view') != 0): ?>
          <div class="admin-link">
            <a href="/admin"><img src="<?php print $user_details_loggedin_admin_imgsrc; ?>" title="Administration" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_user_details_view') != 0 && module_exists('user_details')): ?>
          <div class="user-details-link">
            <a href="/admin/config/user-interface/user-details"><img src="<?php print $user_details_loggedin_user_details_imgsrc; ?>" title="User Details" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_panels_view') != 0 && module_exists('panels')): ?>
          <div class="panels-link">
            <a href="/admin/structure/panels"><img src="<?php print $user_details_loggedin_panels_imgsrc; ?>" title="Panels" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_views_view') != 0 && module_exists('views')): ?>
          <div class="views-link">
            <a href="/admin/structure/views"><img src="<?php print $user_details_loggedin_views_imgsrc; ?>" title="Views" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_performance_view') != 0): ?>
          <div class="performance-link">
            <a href="/admin/config/development/performance"><img src="<?php print $user_details_loggedin_performance_imgsrc; ?>" title="Performance" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_link_one_type') == '3'): ?>
          <div class="custom-link-one">
            <a href="<?php print $user_details_loggedin_link_one_url; ?>"><img src="<?php print $user_details_loggedin_link_one_imgsrc; ?>" title="<?php print $user_details_loggedin_link_one_title; ?>" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_link_two_type') == '3'): ?>
          <div class="custom-link-two">
            <a href="<?php print $user_details_loggedin_link_two_url; ?>"><img src="<?php print $user_details_loggedin_link_two_imgsrc; ?>" title="<?php print $user_details_loggedin_link_two_title; ?>" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_link_three_type') == '3'): ?>
          <div class="custom-link-three">
            <a href="<?php print $user_details_loggedin_link_three_url; ?>"><img src="<?php print $user_details_loggedin_link_three_imgsrc; ?>" title="<?php print $user_details_loggedin_link_three_title; ?>" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_link_four_type') == '3'): ?>
          <div class="custom-link-four">
            <a href="<?php print $user_details_loggedin_link_four_url; ?>"><img src="<?php print $user_details_loggedin_link_four_imgsrc; ?>" title="<?php print $user_details_loggedin_link_four_title; ?>" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_link_five_type') == '3'): ?>
          <div class="custom-link-one">
            <a href="<?php print $user_details_loggedin_link_five_url; ?>"><img src="<?php print $user_details_loggedin_link_five_imgsrc; ?>" title="<?php print $user_details_loggedin_link_five_title; ?>" width="20px" height="20px" /></a>
          </div>
        <?php endif; ?>
      </div>
      <div class="block-end"></div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_content_view') != 0 && variable_get('user_details_loggedin_content_amount') != 0): ?>
      <hr>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_content_view') != 0 && variable_get('user_details_loggedin_usercontent_amount') != 0): ?>
      <div class="user-content">
        <div class="title">My Content</div>
        <ul class="result">
          <?php print $user_details_loggedin_content; ?>
        </ul>
	  </div>
    <?php endif; ?>
    <div class="block-end"></div>
  </div>
</div>