<?php
?>
<?php
  global $base_path;
  global $theme_path;
?>

<?php if ($teaser): ?>
  <div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) {  print ' sticky'; } ?>
    <?php if (!$status) { print ' node-unpublished'; } ?>">
      <?php print render($title_prefix); ?>
        <div class="art-PostMetadataHeader"> <h2 class="art-PostHeader"><a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a></h2></div> 
        <!--<div class="title"><h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2></div>-->
        <?php if ($submitted): ?>
          <div class="art-PostHeaderIcons art-metadata-icons">
            <span class="submitted">
              <a href="user/<?php print $node->uid ;?>"><img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostAuthorIcon.png"></a>
              <?php print t('Written by ') ;?><a href="user/<?php print $node->uid ;?>"><?php print ($node->name) ;?></a> | 
              <img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostDateIcon.png">
              <?php print format_date($node->created, 'custom', 'l, d M Y H:i');?>
              <?/*php print $submitted; */?>
            </span>
          </div>
        <?php endif; ?>
        <div class="art-PostContent content clear-block">
          <?php
            // We hide the comments and links now so that we can render them later.
            hide($content['comments']);
            hide($content['links']);
            hide($content['field_tags']);
            print render($content);
          ?>
        </div>
        <div class="clear-block">
          <div class="meta">
          </div>
          <?php
            // Remove the "Add new comment" link on the teaser page or if the comment
            // form is being displayed on the same page.
            if ($teaser || !empty($content['comments']['comment_form'])) {
              unset($content['links']['comment']['#links']['comment-add']);
            }
            // Only display the wrapper div if there are links.
            $links = render($content['links']);
            if ($links): ?>
              <div class="links">
                <?php print $links; ?>
              </div>
            <?php endif; ?>
            <?php print render($content['comments']); ?>
        </div>
  </div>


<?php else: ?>


  <div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) {  print ' sticky'; } ?>
    <?php if (!$status) { print ' node-unpublished'; } ?>">
      <?php print render($title_prefix); ?>
        <?php if ($submitted): ?>
          <div class="art-PostHeaderIcons art-metadata-icons">
            <span class="submitted">
              <a href="user/<?php print $node->uid ;?>"><img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostAuthorIcon.png"></a>
              <?php print t('Written by ') ;?><a href="user/<?php print $node->uid ;?>"><?php print ($node->name) ;?></a> | 
              <img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostDateIcon.png">
              <?php print format_date($node->created, 'custom', 'l, d M Y H:i');?>
              <?/*php print $submitted; */?>
            </span>
          </div>
        <?php endif; ?>
        <div class="art-PostContent content clear-block">
          <?php
            // We hide the comments and links now so that we can render them later.
            hide($content['comments']);
            hide($content['links']);
            hide($content['field_tags']);
            print render($content);
          ?>
        </div>
        <div class="clear-block">
          <div class="meta">
          </div>
          <?php
            // Remove the "Add new comment" link on the teaser page or if the comment
            // form is being displayed on the same page.
            if ($teaser || !empty($content['comments']['comment_form'])) {
              unset($content['links']['comment']['#links']['comment-add']);
            }
            // Only display the wrapper div if there are links.
            ?>
            <?php $field_tags = render($content['field_tags']);
              if ($field_tags): ?>
               <div class="post-tags">
               <div class="terms"><?php print $field_tags ?></div>
               </div>
              <?php endif;
            $links = render($content['links']);
            if ($links): ?>
              <div class="links">
                <?php print $links; ?>
              </div>
            <?php endif; ?>
            <?/*php $field_tags = render($content['field_tags']);
              if ($field_tags): ?>
                 <div class="terms"><?php print $field_tags ?></div>
            <?php endif; */?>
            <?php print render($content['comments']); ?>
           <!--<?php if ($links): ?>
             <div class="links"><?php print $links; ?></div> 
           <?php endif; ?>--> 
        </div>
  </div>
      
<?php endif; ?>
