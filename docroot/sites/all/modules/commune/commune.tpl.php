<?php
/**
 * @file
 * Default theming to display a Drupal wall status posts in a drupal
 * block.
 *
 * Available variables:
 *
 * Drupal wall - wall post (object array )
 * - $wall_post : Consisit aray of drupal wall content type wall post of the
 *   desire user.
 *   It contains the Node: nid, uid, created, title, body, comment.
 *   
 * Drupal user information (object array) 
 * - $user_info : Consisit of aray of drupal user basic information.
 *   User info => users ID (uid), name, mail & picture.
 *
 * @see _commune_user_profile_picture($user_id)
 */

  global $base_url;
 ?>

<!-- Drupal wall block : starts here ! -->
<?php if (arg(0) == 'drupal-wall'){ $commune_page = "_global";} else { $commune_page = "";} ?>
<div id="commune_append_older_wall_post<?php print $commune_page;?>">
  <?php
    $i = 0;
    global $user;
    foreach($wall_post as $node) {
      $post = entity_metadata_wrapper('node', $node);
      $nid = $post->getIdentifier();
      $type = $post->getBundle();
      $author = $post->author->getIdentifier();;
      $i++;
  ?>
      
      
  <div class='drupal_wall' id="<?php print 'commune_post_nid_' . $nid; ?>">
	  <?php print drupal_render(node_view($node)); ?>
	  <?php print render(commune_page_comments($node)); ?>
       	              <!-- Comment Block : Ends here ! -->
            <!-- For AJAX comment post -->
            <div id="div_append_next_user_comment_<?php echo $nid; ?>"></div>
            <!-- For AJAX comment post : Ends here-->

          <!-- write a comment Block : starts here ! -->
          <?php 
            if ($user->uid != 0 && variable_get('commune_comment_post_textbox') == 1) :
          ?>
          <div class="comment">
            <div class="comment_box">
              <?php
               $wall_comment_post = drupal_get_form('_commune_comment_post_form', $nid, $user->uid);
               print drupal_render($wall_comment_post); ?>
            </div>
          </div>
          <?php
            endif;
              // End outer if.
          ?>
       <!-- write a comment Block: Ends here ! -->
  </div>
             <?php }; ?>
</div>
<!-- Drupal wall block : ends here ! -->
