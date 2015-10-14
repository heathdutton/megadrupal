<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
	<?php print render($title_prefix); ?>
	<div class="comm_date"><span class="data"><span class="j"><?php print $day;?></span><?php print $my;?></span><span class="nr_comm"><a class="nr_comm_spot" href=""><?php print $comment_count;?></a></span>
	</div>
	
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
	<?php
	      // We hide the comments and links now so that we can render them later.
	      hide($content['comments']);
	      hide($content['links']);
	      hide($content['field_tags']);
	      print render($content);
	    ?>	
	
	 <?php
	    // Remove the "Add new comment" link on the teaser page or if the comment
	    // form is being displayed on the same page.
	    if (!empty($content['links']['comment']['#links']['comment-comments'])) {
	    	 unset($content['links']['comment']['#links']['comment-add']);
	         unset($content['links']['node']['#links']['node-readmore']);
	         
	         
	    }else{
	    	unset($content['links']['node']['#links']['node-readmore']);
	    	
	    }
	    
	   //unset($content['links']['comment']['#links']['comment_forbidden']);
	    // Only display the wrapper div if there are links.
	  //  print "<pre>";  
	   // print_r($content['links']);
	    $links = render($content['links']);
	    if ($links):
	  ?>
	
	<div class="cat_box"  >
    	<div class="tag_cont" >
      	<div class="post-tags">
      	  <?php print render($content['field_tags']); ?>
      	</div>
    	</div>	
    	<div class='comt-test' ><?php print render($content['links']['comment']); ?></div>    	
		
	</div>
	 <?php endif; ?>
	
	<?php if(!drupal_is_front_page()): ?>
		<div class="comt_box">
			<?php print render($content['comments']); ?>
		</div>
	<?php endif; ?>
</div>
