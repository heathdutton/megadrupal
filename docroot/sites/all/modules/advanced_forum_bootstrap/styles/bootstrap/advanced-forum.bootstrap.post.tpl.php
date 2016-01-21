<?php

/**
 * @file
 *
 * Theme implementation: Template for each forum post whether node or comment.
 *
 * All variables available in node.tpl.php and comment.tpl.php for your theme
 * are available here. In addition, Advanced Forum makes available the following
 * variables:
 *
 * - $top_post: TRUE if we are formatting the main post (ie, not a comment)
 * - $reply_link: Text link / button to reply to topic.
 * - $total_posts: Number of posts in topic (not counting first post).
 * - $new_posts: Number of new posts in topic, and link to first new.
 * - $links_array: Unformatted array of links.
 * - $account: User object of the post author.
 * - $name: User name of post author.
 * - $author_pane: Entire contents of the Author Pane template.
 */

?>

<?php if ($top_post): ?>
  <?php print $topic_header ?>
<?php endif; ?>

<div id="<?php print $post_id; ?>" class="<?php print $classes; ?> panel panel-default" <?php print $attributes; ?>>
  <div class="panel-heading">
    <span class="pull-right"><?php print $permalink; ?></span>
    <span class="text-muted"><?php print $date ?></span>

    <?php if (!$top_post): ?>
      <?php if (!empty($new)): ?>
        <a id="new"><span class="new">(<?php print $new ?>)</span></a>
      <?php endif; ?>

      <?php if (!empty($first_new)): ?>
        <?php print $first_new; ?>
      <?php endif; ?>

      <?php if (!empty($new_output)): ?>
        <?php print $new_output; ?>
      <?php endif; ?>
    <?php endif; ?>

    <?php if (!empty($in_reply_to)): ?>
      <span class="forum-in-reply-to"><?php print $in_reply_to; ?></span>
    <?php endif; ?>

    <?php if (!$node->status): ?>
      <span class="unpublished-post-note"><?php print t("Unpublished post") ?></span>
    <?php endif; ?>
  </div>

  <div class="panel-body">
    <div class="media">
      <?php if (!empty($author_pane)): ?>
        <div class="pull-left">
          <?php print $author_pane; ?>
        </div>
      <?php endif; ?>

      <div class="media-body">
        <?php if (!empty($title)): ?>
          <h4 class="media-heading"><?php print $title ?></h4>
        <?php endif; ?>

        <?php
          // We hide the comments and links now so that we can render them later.
          hide($content['taxonomy_forums']);
          hide($content['comments']);
          hide($content['links']);
          if (!$top_post) hide($content['body']);
          print render($content);
        ?>

        <?php if (!empty($post_edited)): ?>
          <div class="post-edited"><?php print $post_edited ?></div>
        <?php endif; ?>

        <?php if (!empty($signature)): ?>
          <div class="author-signature"><?php print $signature ?></div>
        <?php endif; ?>
      </div>
    </div>
  </div>

  <div class="panel-footer text-right">
    <?php print render($content['links']); ?>
  </div>
</div>

<?php print render($content['comments']); ?>
