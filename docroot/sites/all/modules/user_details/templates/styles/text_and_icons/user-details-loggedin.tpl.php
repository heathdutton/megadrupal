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
 * - $user_details_loggedin_combined (combines the number of posts and the number of comments into one number)
 * - $user_details_loggedin_points_title (the "branding" from the user_points module ie. points)
 * - $user_details_loggedin_points (the number of "User Points" the logged-in user has)
 * - $user_details_loggedin_privatemsg_unread (the number of unread private messages the logged-in user has)
 * - $user_details_loggedin_role (the highest role name the logged-in user has)
 * Built-in Links
 * - $user_details_loggedin_front_imgsrc (the image for the front link)
 * - $user_details_loggedin_profile_url (the URL of the logged-in users profile)
 * - $user_details_loggedin_profile_imgsrc (the image of the logged-in users profile link)
 * - $user_details_loggedin_profile_edit_url (the URL of the logged-in users profile edit tab)
 * - $user_details_loggedin_profile_edit_imgsrc (the image of the logged-in users profile edit link)
 * - $user_details_loggedin_privatemsg_url (the URL of the logged-in users profile private message tab)
 * - $user_details_loggedin_privatemsg_imgsrc (the image of the logged-in users profile private message link)
 * - $user_details_loggedin_blog_url (the URL of the logged-in users blog page)
 * - $user_details_loggedin_blog_imgsrc (the image of the logged-in users blog link)
 * - $user_details_loggedin_create_imgsrc (the image of the create link)
 * - $user_details_loggedin_ubercart_cart_imgsrc (the image of the logged-in users cart link)
 * - $user_details_loggedin_search_imgsrc (the image of the search link)
 * - $user_details_loggedin_logout_imgsrc (the image of the logged-in users logout link)
 * - $user_details_loggedin_admin_imgsrc (the image of the  admin link)
 * - $user_details_loggedin_user_details_imgsrc (the image of the user details link)
 * - $user_details_loggedin_panels_imgsrc (the image of the panels link)
 * - $user_details_loggedin_views_imgsrc (the image of the views link)
 * - $user_details_loggedin_performance_imgsrc (the image of the performance link)
 * Custom Links
 * - $user_details_loggedin_link_one_url (the URL for the "Link One" link)
 * - $user_details_loggedin_link_one_imgsrc (the image for the "Link One" link)
 * - $user_details_loggedin_link_one_title (the title for the "Link One" link)
 * - $user_details_loggedin_link_two_url (the URL for the "Link Two" link)
 * - $user_details_loggedin_link_two_imgsrc (the image for the "Link Two" link)
 * - $user_details_loggedin_link_two_title (the title for the "Link Two" link)
 * - $user_details_loggedin_link_three_url (the URL for the "Link Three" link)
 * - $user_details_loggedin_link_three_imgsrc (the image for the "Link Three" link)
 * - $user_details_loggedin_link_three_title (the title for the "Link Three" link)
 * - $user_details_loggedin_link_four_url (the URL for the "Link Four" link)
 * - $user_details_loggedin_link_four_imgsrc (the image for the "Link Four" link)
 * - $user_details_loggedin_link_four_title (the title for the "Link Four" link)
 * - $user_details_loggedin_link_five_url (the URL for the "Link Five" link)
 * - $user_details_loggedin_link_five_imgsrc (the image for the "Link Five" link)
 * - $user_details_loggedin_link_five_title (the title for the "Link Five" link)
 * User Content
 * - $user_details_loggedin_content (used to push the "User Content" content list to the theme layer)
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
    <?php if (variable_get('user_details_loggedin_privatemsg_unread_view') != 0 && module_exists('privatemsg')): ?>
    <div class="private-message-count">
      <div class="title">Private Messages:</div>
      <div class="result"><?php print $user_details_loggedin_privatemsg_unread; ?></div>
    </div>
    <?php endif; ?>
    <?php if (variable_get('user_details_loggedin_role_view') != 0): ?>
      <div class="rank">
        <div class="title">Role:</div>
        <div class="result"><?php print $user_details_loggedin_role; ?></div>
      </div>
    <?php endif; ?>
	</div>
	<?php if (variable_get('user_details_loggedin_front_view') != 0 || variable_get('user_details_loggedin_profile_view') != 0 || variable_get('user_details_loggedin_profile_edit_view') != 0 || variable_get('user_details_loggedin_blog_view') != 0 || variable_get('user_details_loggedin_private_message_view') != 0 || variable_get('user_details_loggedin_create_view') != 0 || variable_get('user_details_loggedin_ubercart_cart_view') != 0 || variable_get('user_details_loggedin_logout_view') != 0 || variable_get('user_details_loggedin_link_one_view') == 2 || variable_get('user_details_loggedin_link_two_view') == 2 || variable_get('user_details_loggedin_link_three_view') == 2 || variable_get('user_details_loggedin_link_four_view') == 2 || variable_get('user_details_loggedin_link_five_view') == 2): ?>
      <hr>
	<?php endif; ?>
    <div class="user-quick-links">
	  <?php if (variable_get('user_details_loggedin_front_view') != 0 || variable_get('user_details_loggedin_profile_view') != 0 || variable_get('user_details_loggedin_profile_edit_view') != 0 || variable_get('user_details_loggedin_blog_view') || variable_get('user_details_loggedin_private_message_view') != 0 || variable_get('user_details_loggedin_create_view') != 0 || variable_get('user_details_loggedin_ubercart_cart_view') != 0 || variable_get('user_details_loggedin_logout_view') != 0 || variable_get('user_details_loggedin_link_one_view') == 2 || variable_get('user_details_loggedin_link_two_view') == 2 || variable_get('user_details_loggedin_link_three_view') == 2 || variable_get('user_details_loggedin_link_four_view') == 2 || variable_get('user_details_loggedin_link_five_view') == 2): ?>
        <div class="title">User Links</div>
	  <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_front_view') != 0): ?>
        <div class="front">
          <div class="link"><a href="/"><img src="<?php print $user_details_loggedin_front_imgsrc; ?>" title="Homepage" alt="Homepage" width="20px" height="20px" /></a></div>
          <div class="tooltip">Homepage</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_profile_view') != 0): ?>
        <div class="profile">
          <div class="link"><a href="<?php print $user_details_loggedin_profile_url; ?>"><img src="<?php print $user_details_loggedin_profile_imgsrc; ?>" title="My Profile" alt="My Profile" width="20px" height="20px" /></a></div>
          <div class="tooltip">My Profile</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_profile_edit_view') != 0): ?>
        <div class="profile-edit">
          <div class="link"><a href="<?php print $user_details_loggedin_profile_edit_url; ?>"><img src="<?php print $user_details_loggedin_profile_edit_imgsrc; ?>" title="Edit My Profile" alt="Edit My Profile" width="20px" height="20px" /></a></div>
          <div class="tooltip">Edit My Profile</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_private_message_view') != 0 && module_exists('privatemsg')): ?>
        <div class="privatemsg">
          <div class="link"><a href="<?php print $user_details_loggedin_private_message_url; ?>"><img src="<?php print $user_details_loggedin_private_message_imgsrc; ?>" title="My Private Messages" alt="My Private Messages" width="20px" height="20px" /></a></div>
          <div class="tooltip">Private Messages</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_blog_view') != 0 && module_exists('blog')): ?>
        <div class="blog">
          <div class="link"><a href="<?php print $user_details_loggedin_blog_url; ?>"><img src="<?php print $user_details_loggedin_blog_imgsrc; ?>" title="My Blog" alt="My Blog" width="20px" height="20px" /></a></div>
          <div class="tooltip">My Blog</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_create_view') != 0): ?>
        <div class="create">
          <div class="link"><a href="/node/add"><img src="<?php print $user_details_loggedin_create_imgsrc; ?>" title="Create Content" alt="Create Content" width="20px" height="20px" /></a></div>
          <div class="tooltip">Create Content</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_ubercart_cart_view') != 0 && module_exists('uc_cart')): ?>
        <div class="ubercart-cart">
          <div class="link"><a href="/cart"><img src="<?php print $user_details_loggedin_ubercart_cart_imgsrc; ?>" title="My Shopping Cart" alt="My Shopping Cart" width="20px" height="20px" /></a></div>
          <div class="tooltip">My Cart</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_search_view') != 0 && module_exists('search')): ?>
        <div class="search">
          <div class="link"><a href="/search"><img src="<?php print $user_details_loggedin_search_imgsrc; ?>" title="Search" alt="Search" width="20px" height="20px" /></a></div>
          <div class="tooltip">Search</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_logout_view') != 0): ?>
        <div class="logout">
          <div class="link"><a href="/logout"><img src="<?php print $user_details_loggedin_logout_imgsrc; ?>" title="Logout" alt="Logout" width="20px" height="20px" /></a></div>
          <div class="tooltip">Logout</div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_one_type') == '2'): ?>
        <div class="link-one">
          <div class="link"><a href="<?php print $user_details_loggedin_link_one_url; ?>"><img src="<?php print $user_details_loggedin_link_one_imgsrc; ?>" title="<?php print $user_details_loggedin_link_one_title; ?>" alt="<?php print $user_details_loggedin_link_one_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_one_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_two_type') == '2'): ?>
        <div class="link-two">
          <div class="link"><a href="<?php print $user_details_loggedin_link_two_url; ?>"><img src="<?php print $user_details_loggedin_link_two_imgsrc; ?>" title="<?php print $user_details_loggedin_link_two_title; ?>" alt="<?php print $user_details_loggedin_link_two_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_two_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_three_type') == '2'): ?>
        <div class="link-three">
          <div class="link"><a href="<?php print $user_details_loggedin_link_three_url; ?>"><img src="<?php print $user_details_loggedin_link_three_imgsrc; ?>" title="<?php print $user_details_loggedin_link_three_title; ?>" alt="<?php print $user_details_loggedin_link_three_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_three_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_four_type') == '2'): ?>
        <div class="link-four">
          <div class="link"><a href="<?php print $user_details_loggedin_link_four_url; ?>"><img src="<?php print $user_details_loggedin_link_four_imgsrc; ?>" title="<?php print $user_details_loggedin_link_four_title; ?>" alt="<?php print $user_details_loggedin_link_four_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_four_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_five_type') == '2'): ?>
        <div class="link-five">
          <div class="link"><a href="<?php print $user_details_loggedin_link_five_url; ?>"><img src="<?php print $user_details_loggedin_link_five_imgsrc; ?>" title="<?php print $user_details_loggedin_link_five_title; ?>" alt="<?php print $user_details_loggedin_link_five_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tootip"><?php print $user_details_loggedin_link_five_title; ?></div>
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
            <div class="link"><a href="/admin"><img src="<?php print $user_details_loggedin_admin_imgsrc; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip">Admin
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_user_details_view') != 0 && module_exists('user_details')): ?>
          <div class="user-details-link">
            <div class="link"><a href="/admin/config/user-interface/user-details"><img src="<?php print $user_details_loggedin_user_details_imgsrc; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip">User Details
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_panels_view') != 0 && module_exists('panels')): ?>
          <div class="panels-link">
            <div class="link"><a href="/admin/structure/panels"><img src="<?php print $user_details_loggedin_panels_imgsrc; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip">Panels
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_views_view') != 0 && module_exists('views')): ?>
          <div class="views-link">
            <div class="link"><a href="/admin/structure/views"><img src="<?php print $user_details_loggedin_views_imgsrc; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip">Views
          </div>
        <?php endif; ?>
        <?php if (variable_get('user_details_loggedin_performance_view') != 0): ?>
          <div class="performance-link">
            <div class="link"><a href="/admin/config/development/performance"><img src="<?php print $user_details_loggedin_performance_imgsrc; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip">Performance
          </div>
        <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_one_type') == '1'): ?>
        <div class="link-one">
          <div class="link"><a href="<?php print $user_details_loggedin_link_one_url; ?>"><img src="<?php print $user_details_loggedin_link_one_imgsrc; ?>" title="<?php print $user_details_loggedin_link_one_title; ?>" alt="<?php print $user_details_loggedin_link_one_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_one_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_two_type') == '1'): ?>
        <div class="link-two">
          <div class="link"><a href="<?php print $user_details_loggedin_link_two_url; ?>"><img src="<?php print $user_details_loggedin_link_two_imgsrc; ?>" title="<?php print $user_details_loggedin_link_two_title; ?>" alt="<?php print $user_details_loggedin_link_two_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_two_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_three_type') == '1'): ?>
        <div class="link-three">
          <div class="link"><a href="<?php print $user_details_loggedin_link_three_url; ?>"><img src="<?php print $user_details_loggedin_link_three_imgsrc; ?>" title="<?php print $user_details_loggedin_link_three_title; ?>" alt="<?php print $user_details_loggedin_link_three_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_three_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_four_type') == '1'): ?>
        <div class="link-four">
          <div class="link"><a href="<?php print $user_details_loggedin_link_four_url; ?>"><img src="<?php print $user_details_loggedin_link_four_imgsrc; ?>" title="<?php print $user_details_loggedin_link_four_title; ?>" alt="<?php print $user_details_loggedin_link_four_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tooltip"><?php print $user_details_loggedin_link_four_title; ?></div>
        </div>
      <?php endif; ?>
      <?php if (variable_get('user_details_loggedin_link_five_type') == '1'): ?>
        <div class="link-five">
          <div class="link"><a href="<?php print $user_details_loggedin_link_five_url; ?>"><img src="<?php print $user_details_loggedin_link_five_imgsrc; ?>" title="<?php print $user_details_loggedin_link_five_title; ?>" alt="<?php print $user_details_loggedin_link_five_title; ?>" width="20px" height="20px" /></a></div>
          <div class="tootip"><?php print $user_details_loggedin_link_five_title; ?></div>
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