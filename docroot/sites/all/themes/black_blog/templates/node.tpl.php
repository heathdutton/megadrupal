<?php if ($teaser): ?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> front-articleleft alpha clearfix"<?php print $attributes; ?>>
      <?php print render($title_prefix); ?>
      <?php if (!$page): ?>
      <?php if (!empty($thumb)): ?>
       <a href="<?php print $node_url; ?>"><img src="<?php print $thumb; ?>" alt="<?php print $title; ?>" title="<?php print $title; ?>"/></a>
      <?php endif; ?>
        <div class="article_title"><h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2></div>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <div class="clearfix"<?php print $content_attributes; ?>">
      <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_image']);
      hide($content['field_tags']);
      print render($content);
      ?> 
  <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
    // form is being displayed on the same page.
    if ($teaser || !empty($content['comments']['comment_form'])) {
      unset($content['links']['comment']['#links']['comment-add']);
    }
    // Only display the wrapper div if there are links.
    if (!empty($content['field_tags'])): ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    <?php  endif;
    $links = render($content['links']);
    if ($links):
  ?>
    <div class="link-wrapper">
      <?php print $links; ?>
    </div>
  <?php endif; ?>
  

  <?php print render($content['comments']); ?>

</div>
</div>
<?php else:  ?>

<div id="node-<?php print $node->nid; ?>" class="alpha clearfix"<?php print $attributes; ?>>
      <?php print render($title_prefix); ?>
      <?/*php if (!$page): ?>
        <div class="article_title"><h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2></div>
      <?php endif; */?>
      <?php if($page):?>
    <?php if (!empty($thumb)):?>
       <a href="<?php print $node_url; ?>"><img src="<?php print $thumb; ?>" alt="<?php print $title; ?>" title="<?php print $title; ?>"/></a>
    <?php endif;?>
    <div class="article_title"><h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2></div>
    <?php endif;?>
      <?php print render($title_suffix); ?>
     
      <div class="clearfix"<?php print $content_attributes; ?>">
      <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_image']);
      hide($content['field_tags']);
      print render($content);
      ?>
  <?php if ($display_submitted): ?>
    <div id="comment_submitted">cef
      <?php print $user_picture; ?>
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
    // form is being displayed on the same page.
    if ($teaser || !empty($content['comments']['comment_form'])) {
      unset($content['links']['comment']['#links']['comment-add']);
    }
    // Only display the wrapper div if there are links.
    if (!empty($content['field_tags'])): ?>
      <div class="post-tags"><?php  print render($content['field_tags']) ;?></div>
    <?php  endif;
    $links = render($content['links']);
    if ($links):
  ?>
    <div class="link-wrapper">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

  <?php print render($content['comments']); ?>
</div>
</div>
<?php endif; ?>
	
