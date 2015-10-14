<?php

/**
 * @file
 * Hatch's theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
 
?>

 <?php
    if ($teaser):
      unset($content['links']['comment']['#links']['comment-add']);
      //$img=$node->field_image['und'][0]['uri'];  
      //$nid=$node->nid;
      //$image=image_style_url('medium',$img);
     // print_r($image);
  ?>
  <div  class="grid-3 image" id="teaserimg" ><div id="node-<?php print $node->nid; ?>"  <?php print $attributes; ?>  >
    <a href="<?php print $node_url;?>"><img src="<?php print $image;?>" title="<?php print $title;?>"  /></a>
    <h2 id="teaser-title" class="teaser-title" style=""><a title="<?php print $title;?>" href="<?php print $node_url;?>" ><?php print $title?></a></h2>
  </div></div>
  <?php else:?>
   <div  id='article' class="grid-12"><div id="node-<?php print $node->nid; ?>"  <?php print $attributes; ?>  >
      <div class="post-content ">
        <div class="post-aside ">								
				  <h1 class="post-title entry-title"><?php print $title;?></h1>						
					  <div class="byline">Date: <abbr class="published" title="<?php print date('l, M dS, Y, g:ha',$created);?>"><?php print date('M j, Y',$created);?></abbr></div>								
					    <div class="byline">Author: <span class="author vcard"><?php print $name;?></span></div>
                             
              
 
					    <div class="entry-meta" id="tags">
					      <span class="post_tag">
					        <span class="before">
					           
					          <?php print render($content['field_tags']) ?>
					        </span>
					      </span>
					    </div>				
					    	<?php if($nextandprev):?>
								  <?php print render($nextandprev);	?>
								<?php endif;?>									
				</div>
		    <?php
          
           unset($content['links']['comment']['#links']['comment-add']);
                    
        ?>
        <img src="<?php print $image ;?>"  />
        <div class="entry-content"><?php print render($content['body']);?></div>
        
        <?php print render($content['comments']);?>
        </div>
      </div>
    </div>
    <?php endif;?>	
