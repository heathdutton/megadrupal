 <?php $cid = $comment->cid; ?>
 <div class="comment" id="<?php print 'commune_comment_cid_' . $cid; ?>">
    <div class="comment_left">
        <?php print $picture;?>
    </div> 
                  
    <div class="comment_right">
        <!-- Delete Comment Block : Starts here ! -->
        <div class="comment_delete">
            <?php
	            global $user;
                $wall_delete = drupal_get_form('_commune_delete_comment_form', $cid, $user->uid);
		        print drupal_render($wall_delete); 
		    ?>
		</div>
		<!-- Delete Comment Block : ends here ! -->
			
		<strong>
			<?php print $author; ?>
		</strong>
		
		<?php print render($content); ?>
        
        <div class="caption">
            <?php
				print date('F j, Y', $comment->created);
				print ' at ' . date('h:ia', $comment->created);
	                       
	            // Liking option for Comments.
	            if(module_exists('flag') && variable_get('commune_likes_post') == 1) :
		             $comment_flag_name = variable_get('commune_likes_comment');
		             $commment_count_flag = flag_get_counts('comment', $cid);
		             if (isset($commment_count_flag[$comment_flag_name]) && $commment_count_flag[$comment_flag_name] > 0) :
		               print '<span class="like_comment">' . $commment_count_flag[$comment_flag_name] . '&nbsp;';
		             endif;
	
	                 $commment_flag_type = flag_get_flag($comment_flag_name);
	                 if ($commment_flag_type != NULL && in_array('comment_node_commune', $commment_flag_type->types)) :
			            print flag_create_link($comment_flag_name, $cid()) . '</span>';
		             endif;
				endif;
	        ?>
        </div> <!-- caption -->
    </div> <!-- comment right div -->
</div>
