<div class='drupal_wall' id="<?php print 'commune_post_nid_' . $nid; ?>">
    <div class="commune-post-content">
    
	    <!-- Edit - delete button form button: starts here ! -->
	    <div class="edit_delete">
		      <?php 
		        if (user_access('delete any ' . $type . ' content', $user) || (user_access('delete own ' . $type . ' content', $user) && ($user->uid == $node->uid))) {
		          $wall_edit_delete = drupal_get_form('_commune_delete_edit_node_form', $nid, $user->uid);
		          print drupal_render($wall_edit_delete);
		        }
		      ?> 
	    </div>
		<!-- Edit - delete button: ends here ! -->
      
		<!-- Left Image Icon : starts here ! -->
		<div class='wallContent_left'>
		    <?php print $user_picture; ?>
	    </div>
	    <!-- Left Image Icon : ends here ! -->

	    <!-- Wall right content Block : starts here ! -->
	    <div class="wallContent_right">
		    
	      <!-- Headline Block : Starts here ! -->
	      <strong><?php print $name; ?></strong>
	      <span class="headline">
	        <?php 
		        if ($node->field_commune_url):
		            print 'shared a <a href="/node/' . $nid . '" >new link</a>';
		        elseif ($node->field_commune_upload):
		            print 'shared a <a href="/node/' . $nid . '" >new file</a>';
		        else :
		            print 'shared a <a href="/node/' . $nid . '" >post</a>';
		        endif;
		        
				if($node->context_id) {
					$to = $node->context_id;
					if($to && $to != $uid) {
						$user_to = user_load($to);
						if(isset($user_to->name)) {
							print ' with ';
							print theme('username', array('account' => $user_to));
						}
		  			}
	        	}
	        ?>
	        <div class="caption">
	          <?php
	              print date('F j, Y', $created);
	              print ' at ' . date('h:ia', $created);
	          ?>
	        </div>
	      </span>
	      <!-- Headline block : ends here ! -->
	
	      <!-- User Content block : starts here ! -->
	      <div class="userContent"> 
	         <?php 
		         print render($content['body']);
		         //print_r($content);
	         ?>
	      </div>
	       <!-- User Content block : ends here ! -->
	
	      <?php if ($node->field_commune_upload): ?>
		       <div class="photo_status">
		          <?php
			        print render($content['field_commune_upload']);
		          ?>
		       </div>
	       <?php endif; ?>
	
	        <?php if($node->field_commune_url) : ?>
		    	<div class="video_status">
					<?php print render($content['field_commune_url']); ?>
	    		</div>
			<?php endif; ?>
	    	</div> <!--  commune post content -->
		</div>
        <div>
    	<!-- Likes block : starts here ! -->
		<div class="commune-links">
			<?php
	            $flag_name = variable_get('commune_likes_node');
	            $flag_type = flag_get_flag($flag_name);
	            if ($uid != 0 && $type == 'commune_post') :
	            	if(module_exists('flag') && variable_get('commune_likes_post') == 1 && $flag_type != NULL) :  ?>
	            	
				        <div class="likes" style="height:15px">
				            <?php
				              if ($flag_type != NULL && in_array('commune_post', $flag_type->types)) :
				                print flag_create_link($flag_name, $nid);
				              endif;
				
				              $count_flag = flag_get_counts('node', $nid);
				              if (isset($count_flag[$flag_name]) && $count_flag[$flag_name] > 0) :
				                print $count_flag[$flag_name] . ' people like this.';
				              endif;
				            ?>
			            </div>
					<?php endif;
				endif;
             ?>
        </div>
        <!-- Likes block : ends here ! -->
        <div class="commentView">
	        <?php if ($comment_count > 0) : ?>
	              <?php print l('View all ' . $comment_count . ' comments', "node/$nid"); ?>
	        <?php else: ?>
	        	  <?php print t('No comments'); ?>
	        <?php endif; ?>
        </div>
     </div>   
     <?php print render($content['comments']); ?>
</div>