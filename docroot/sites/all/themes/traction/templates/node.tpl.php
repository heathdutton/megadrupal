<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <div class="date">
        <div class="day"><?php print $day; ?></div>
        <div class="month"><?php print $my; ?></div>
      </div>
      <?php if ($node_article && $thumb): ?>
       <a href="<?php print $node_url; ?>"><img src="<?php print $thumb; ?>" class="index-node-thm alignleft border" alt="<?php print $title; ?>" title="<?php print $title; ?>" /></a>
      <?php endif; ?>
    <?php else: ?>
    	<h1 class="title with-author"><?php print $title; ?></h1>
    	<div class="author"><?php print $submitted; ?></div>
    <?php endif; ?>
  <?php print render($title_suffix); ?>
	<?php if ($page): ?>
      <div class="content single">
      		<?php if ($node_article && $image_url): ?>
          	<img src="<?php print $image_url; ?>" class="alignright single-node-thm border" alt="<?php print $title; ?>" title="<?php print $title; ?>" />
          <?php endif; ?>
          <?php
               // We hide the comments and links now so that we can render them later.
               hide($content['comments']);
               hide($content['links']);
               hide($content['field_tags']);
               hide($content['field_image']);
               print render($content);
          ?>
          </div><!--end content--> 
  <?php else: ?>
				<div class="content">
				  <h2 class="title" <?php print $title_attributes; ?>>
     	      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
          </h2>
          <?php
               // We hide the comments and links now so that we can render them later.
               hide($content['comments']);
               hide($content['links']);
               hide($content['field_tags']);
               hide($content['field_image']);
               print render($content);
               print $readmore;
          ?>
          </div><!--end content--> 
	<?php endif; ?>
 

						<?php if ($page): ?>														
						<div class="meta clear">
						<?php if (!empty($content['field_tags'])): ?>
							<div class="tags">
  							<div class="post-tags">
	  						  <?php print render($content['field_tags']); ?>
		  					</div>
		  			  </div>
		  			<?php endif; ?>
						</div><!--end meta-->
						<?php endif; ?>
					</div><!--end node-->
					<?php print render($content['comments']); ?>
