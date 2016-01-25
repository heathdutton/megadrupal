<?php

/**
 * @file
 * Needs to be documented.
 */
?>
<article class="node-forum clearfix">
  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <div class="topic-content"<?php print $content_attributes; ?>>
  <?php
    // We hide the comments and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    print render($content['body']);
  ?>
  </div>

  <?php
  // Remove the "Add new comment" link on the teaser page or if the comment
  // form is being displayed on the same page.
  if ($teaser || !empty($content['comments']['comment_form'])):
    unset($content['links']['comment']['#links']['comment-add']);
  endif;
  // Only display the wrapper div if there are links.
  $links = render($content['links']);
  if ($links):
  ?>
    <div class="link-wrapper">
      <?php print $links; ?>
    </div>
    <?php if ($display_submitted): ?>
      <div class="meta">
        <?php print $user_picture; ?>
        <?php print $submitted; ?>
      </div>
    <?php endif; ?>
  <?php endif; ?>
</article>
<?php print render($content['comments']); ?>
