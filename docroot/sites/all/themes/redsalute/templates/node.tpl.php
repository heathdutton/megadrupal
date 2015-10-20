<?php
?>
<?php
  global $base_path;
  global $theme_path;
?>
<?php if ($teaser): ?>
<div class="Post">
    <div class="Post-body">
        <div class="Post-inner">
          <div id="node-<?php print $node->nid; ?>" class="node<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
            <?//php print $picture; ?>
            <?php print render($title_prefix); ?>
            <h2 class="PostHeaderIcon-wrapper">
			        <?/*php $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL)); */?>
              <img width="26" height="26" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostHeaderIcon.png">
              <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a>
            </h2>
            <?php if ($submitted): ?>
               <div class="art-PostHeaderIcons art-metadata-icons">
                <span class="submitted">
                  <a href="user/<?php print $node->uid; ?>"><img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostAuthorIcon.png"></a>
                  <?php print t('Written by ') ;?><a href="user/<?php print $node->uid ;?>"><?php print ($node->name) ;?></a> | 
                  <img width="17" height="18" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostDateIcon.png">
                  <?php print format_date($node->created, 'custom', 'l, d M Y H:i');?>
                  <?/*php print $submitted; */?>
                </span>
              </div>
            <?php endif; ?>
            <div class="PostContent">
              <?php
                // We hide the comments and links now so that we can render them later.
                hide($content['comments']);
                hide($content['links']);
                hide($content['field_tags']);
                print render($content);
              ?>
            </div>
            <div class="clear-block">
              <?php
                $links = render($content['links']);
                if ($links): ?>
                  <div class="links">
                    <?php print $links ?>
                  </div>
                <?php endif; ?>
            </div>
          </div>
          <?php print render($content['comments']); ?>
        </div>
    </div>
</div>
<div class="clearfix"></div>

<?php else: ?>

<div id="node-<?php print $node->nid; ?>" class="node<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?//php print $picture; ?>
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
  <div class="PostContent">
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
      <?php $field_tags = render($content['field_tags']);
        if ($field_tags): ?>
          <div class="post-tags">
               <div class="terms"><?php print $field_tags ?></div>
          </div>
        <?php endif; ?>        
    </div>
    <?php print render($content['comments']); ?>
  </div>
</div>

<?php endif; ?>
