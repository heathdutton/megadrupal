<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <div class="date">
    <?php
    print format_date($node->changed, 'date_only'); ?>
  </div>

  <div class="content clearfix"<?php print $content_attributes; ?>>

    <h2<?php print $title_attributes; ?>><a class="recruiter-job-link" href="<?php print $job_url; ?>"><?php print $title; ?></a></h2>

    <div class="description">
      <?php
        print render($teaser_organization);
        if (!empty($teaser_organization) && !empty($teaser_location)) {
          print ', ';
        }
        print render($teaser_location);
      ?>
    </div>

    <div class="terms">
      <?php print render($teaser_terms); ?>
    </div>

  </div>

  <div class="link-wrapper">
    <?php print render($content['links']); ?>
    <?php print render($rules_links); ?>
  </div>

</div>
