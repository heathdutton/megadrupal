<div id="node-<?php print $node->nid; ?>" class="node <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="node-inner">

    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($user_picture): ?>
      <div class="user-picture">
        <?php print $user_picture; ?>
      </div>
    <?php endif; ?>

    <?php if ($display_submitted): ?>
      <span class="submitted"><?php
        print t('created by !username on !datetime',
          array('!username' => $name, '!datetime' => $date));
      ?></span>
    <?php endif; ?>

    <div class="content clearfix"<?php print $content_attributes; ?>>
      <?php
  	    // We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        print render($content);
       ?>
    </div>

    <?php
      // Remove the "Add new comment" link on the teaser page or if the comment
      // form is being displayed on the same page.
      if ($teaser || !empty($content['comments']['comment_form'])) {
        unset($content['links']['comment']['#links']['comment-add']);
      }
      // Only display the wrapper div if there are links.
      $links = render($content['links']);
      if ($links):
    ?>
      <div class="link-wrapper">
        <?php print $links; ?>
      </div>
    <?php endif; ?>

  </div> <!-- /node-inner -->
</div> <!-- /node-->

<?php print render($content['comments']); ?>