<?php
/**
 * @file
 * This is the template file description for Comment Stats module.
 *
 * This file renders comments in the form of posts, replies and unapproved
 * comments.
 */
if(count($all_comments) > 0):
   foreach($all_comments as $comment_details):

      $content_id = $comment_details->nid;
      $comment_id = $comment_details->cid;
      $subject = $comment_details->subject;
      $content_body = isset($comment_details->comment_body[LANGUAGE_NONE][0]['safe_value']) ? $comment_details->comment_body[LANGUAGE_NONE][0]['safe_value'] : '';
      $created_user_id = $comment_details->uid;
      $user_fields = user_load($created_user_id);

      $created_user_name = $user_fields->name;
      if(is_object($user_fields->picture)):
         $picture_user = image_style_url("thumbnail", $user_fields->picture->uri);
         $user_url = url('user/' . $created_user_id);
      else:
         $user_default_image = base_path() . drupal_get_path('module', 'comment_stats') . '/images/user.png';
         $picture_user = $user_default_image;
         if($created_user_id == 0):
            $user_url = "javascript:void(0)";
         else:
            $user_url = url('user/' . $created_user_id);
         endif;
      endif;

      $time_show = format_interval((time() - $comment_details->changed), 2) . t('ago');
      $delete_url = url('comment/' . $comment_id . '/delete');
      $edit_url = url('comment/' . $comment_id . '/edit');
      $reply_url = url('comment/reply/' . $content_id . '/' . $comment_id);
      $url_alias = drupal_get_path_alias('node/' . $content_id);
      $subject_url = url($url_alias, array('query' => array('comment' => "comment/" . $comment_id), 'fragment' => 'comment-' . $comment_id));
      $status = $comment_details->status;
      if(!$status):
         $approval_url = url('comment/' . $comment_id . '/approve', array('query' => array('token' => drupal_get_token("comment/$comment_id/approve"))));
      endif;
      $output = "
               <div class='comment_wrapper flt'>

                    <div class='info_wrapper'>
                        <div class='user-picture'>
                           <a href='$user_url' title='View user profile.'><img src='$picture_user' /></a>
                        </div>
                           <div><a href='$user_url' title='View user profile'>$created_user_name</a></div>
                           <div>$time_show</div>
                     </div>
                     <div class='content_wrapper'>
                        <div class='subject_wrapper'>
                           <a href='$subject_url' target='_blank'>$subject</a>
                        </div>
                        <div class='content_wrapper_body'>$content_body</div>
                        <ul class='links inline'>
                           <li class='comment-delete first'><a href='$delete_url' >delete</a></li>
                           <li class='comment-edit'><a href='$edit_url' >edit</a></li>";
      if($status):
        $output .= "        <li class='comment-reply last'><a href='$reply_url' >reply</a></li>";
      else:
        $output .= "        <li class='comment-approve last'><a href='$approval_url' >approve</a></li>";
      endif;
      $output .= "      </ul>
                     </div>
               </div>";
      print $output;

   endforeach;
else:
   print t("No record found");
endif;
