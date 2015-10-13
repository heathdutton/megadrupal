<?php
/**
 * @file
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
 * - $formatted_created : the node created date formatted display
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
 * Extra classes applied by SigmaOne is set from sigmaone_preprocess_node().
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 * @see sigmaone_preprocess_node().
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if ($teaser) : ?>
    <?php if (!empty($content['comments']['comment_form']) || (arg(0) == 'comment' && arg(1) == 'reply')) : ?>
    <?php unset($content['links']['comment']['#links']['comment-add']); ?>
    <?php endif;?>

    <div class="node-content"<?php print $content_attributes; ?>>

      <?php if (!empty($content['field_image'])) : ?>
        <?php print render($content['field_image']); ?>
      <?php endif; ?>

      <?php print render($title_prefix); ?>
      <header>
        <h2 class="title"<?php print $title_attributes; ?>>
          <a href="<?php print $node_url; ?>"><?php print $title; ?> </a>
        </h2>
      </header>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted) : ?>
          <time class="node-created" datetime="<?php print $formatted_created; ?>">
            <?php print html_entity_decode(t('By @username on @date', array('@username' => l($owner->name, 'user/' . $node->uid), '@date' => $formatted_created))); ?>
          </time>
      <?php endif; ?>

      <?php hide($content['comments']); ?>
      <?php hide($content['links']); ?>
      <?php print render($content); ?>
    </div>

  <?php endif; ?>


  <?php if (!$teaser) : ?>
    <?php if ($display_submitted) : ?>
        <time class="node-created" datetime="<?php print $formatted_created; ?>">
          <?php print html_entity_decode(t('Article by @username on @date', array('@username' => l($owner->name, 'user/' . $node->uid), '@date' => $formatted_created))); ?>
        </time>
    <?php endif; ?>

    <?php if (!empty($terms)): ?>
      <div class="node-terms terms terms-inline">
        <?php print t('Posted in : @terms', array('@terms' => $terms)); ?>
      </div>
    <?php endif;?>

    <div class="node-content not-teaser"<?php print $content_attributes; ?>>
      <?php hide($content['comments']); ?>
      <?php hide($content['links']); ?>
      <?php print render($content); ?>
    </div>

    <?php if (!empty($content['comments']['comment_form'])) : ?>
    <?php unset($content['links']['comment']['#links']['comment-add']); ?>
    <?php endif;?>
  <?php endif; ?>


  <?php $links = render($content['links']);?>

  <?php if ($links): ?>
    <footer class="node-links">
      <?php print $links; ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['comments']);?>

</article>
