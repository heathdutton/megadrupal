<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  
    <?php print $user_picture; ?>
  
    <div class="node-meta clearfix">
    
		<?php print render($title_prefix); ?>
        <?php if (!$page): ?>
        <h2<?php print $title_attributes; ?> class="node-title left"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <span class="submitted node-info right"><?php if ($display_submitted): ?><?php print $submitted; ?><?php endif; ?></span>
       
    </div><!--/node-meta-->

    <div class="node-box clearfix">

	  <?php if (module_exists('comment') && $node->comment) { ?>
      <h2 class="comments-header"><?php print $node->comment_count ?> <?php print t('Comments'); ?></h2>
      <?php } ?>
      
      <div class="node-content clearfix"<?php print $content_attributes; ?>>

		<?php
          // We hide the comments and links now so that we can render them later.
          hide($content['comments']);
          hide($content['links']);
          print render($content);
        ?>
        
      </div><!--/node-content-->
    
      <?php if (!empty($content['links'])): ?>
      <div class="node-footer clearfix">
        <div class="meta">

            <div class="links">
            <?php print render($content['links']); ?>
            </div>
  
        </div><!--/meta-->
      </div><!--/node-footer-->
      <?php endif; ?>
      
    </div><!--/node-box-->
  </div>
  <?php if ($content['comments']): ?>
    <?php print render($content['comments']); ?>
  <?php endif; ?>