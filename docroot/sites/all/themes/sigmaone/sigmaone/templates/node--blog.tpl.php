<?php
// $Id: node.tpl.php,v 1.2 2010/12/01 00:18:15 webchick Exp $

/**
 * @file
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $owner_picture: The node author's picture from user-picture.tpl.php.
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
 * SigmaOne extra variables
 * - $owner: node owner user object
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

  <?php
  if ($teaser || !empty($content['comments']['comment_form']) || (arg(0) == 'comment' && arg(1) == 'reply')) {
    unset($content['links']['comment']['#links']['comment-add']);
  }
  $links = render($content['links']);
	?>

	<?php if (!$teaser) : ?>
  	<div class="node-left-info left">
  		<div class="node-calendar round">
  			<div class="date"><?php print format_date($node->created, 'custom', 'd'); ?>
  			</div><div class="month"><?php print format_date($node->created, 'custom', 'M'); ?>
  			</div>
  		</div>

    	<?php if ($display_submitted) : ?>
    		<div class="node-info round">
    		  <?php print $user_picture; ?>
    			<p><?php print $owner->name ;?></p>

    			<?php if ($user->uid != $owner->uid) : ?>
    			  <?php print l(t('View Profile'), 'user/' . $node->uid); ?>
    			  <?php if (drupal_valid_path('user/' . $node->uid , $dynamic_allowed = FALSE)) print l(t('Send Message'), 'user/' . $node->uid); ?>
    			<?php endif; ?>

    			<?php if ($comment != 0) : ?>
    			   <p><?php empty($comment_count) ? print t('No comment yet') : print format_plural($comment_count, '1 comment', '@count comments'); ?></p>
    			<?php endif;?>

    			<?php if (!empty($terms)): ?>
    			   <div class="terms">
    			    <?php print $terms ?>
    			   </div>
    			<?php endif;?>
    			<?php if ($links): ?>
    			   <div class="links">
    			      <?php print $links; ?>
    			   </div>
    			<?php endif; ?>
    		</div>
    	<?php endif; ?>
  	</div>

  	<div class="node-right-info"<?php print $content_attributes; ?>>
    	<?php hide($content['comments']); hide($content['links']); print render($content); ?>
  	</div>

	<?php endif; ?>

	<?php if ($teaser) : ?>
  	<div class="node-content"<?php print $content_attributes; ?>>
      <?php if (!empty($content['field_image'])) : ?>
         <?php print render($content['field_image']); ?>
      <?php endif; ?>

      <?php print render($title_prefix); ?>
      <h2 class="title"<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted) : ?>
        <?php $user = user_load($node->uid) ;?>
          <div class="node-created">
            <?php print $submitted; ?>
          </div>
      <?php endif; ?>

      <?php hide($content['comments']); ?>
      <?php hide($content['links']); ?>
      <?php print render($content); ?>
  	</div>

    <?php if ($links): ?>
      <div class="node-links">
        <?php print $links; ?>
      </div>
    <?php endif; ?>
	<?php endif; ?>

  <?php print render($content['comments']);?>

</article>
