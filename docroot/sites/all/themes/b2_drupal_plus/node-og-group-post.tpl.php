<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<h2 class="b2-postheader"<?php print $title_attributes; ?>><?php print render($title_prefix); ?>
<?php echo art_node_title_output($title, $node_url, $page); ?>
<?php print render($title_suffix); ?>
</h2>
<?php if ($display_submitted): ?>
<div class="b2-postheadericons b2-metadata-icons">
<?php echo art_submitted_worker($date, $name); ?>

</div>
<?php endif; ?>
<div class="b2-postcontent">
<?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      $terms = get_terms_D7($content);
      hide($content[$terms['#field_name']]);
      print render($content);
    ?>

</div>
<div class="cleared"></div>
<?php print $user_picture; ?>
<?php if (isset($content['links']) || isset($content['comments'])):
$output = art_links_woker_D7($content);
if (!empty($output)):	?>
<div class="b2-postfootericons b2-metadata-icons">
<?php echo $output; ?>

</div>
<?php endif; endif; ?>

</div>

		<div class="cleared"></div>
    </div>
</div>
