<article<?php print $attributes; ?>>
  <div class="content">
    <?php print $user_picture; ?>
    <?php print render($title_prefix); ?>
    <?php if ((!$page && $title) || $display_submitted): ?>
    <header>
      <?php if ($display_submitted): ?>
      <time class="submitted"  datetime="<?php print format_date($created, 'custom', 'Y-m-d');  ?>" pubdate="pubdate">
        <div class="dia datas"><?php print format_date($created, 'custom', 'd'); ?></div>
        <div class="mes datas"><?php print format_date($created, 'custom', 'M'); ?></div>
        <div class="no datas"><?php print format_date($created, 'custom', 'Y'); ?></div>
      </time>
      <?php endif; ?>
      <?php if (!$page && $title): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
      <?php endif; ?>
    </header>
    <?php endif; ?>
    <?php print render($title_suffix); ?>


    <div<?php print $content_attributes; ?>>
      <?php
        // We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        print render($content);
      ?>
    </div>

    <?php if (!empty($content['links'])): ?>
    <footer class="clearfix">
      <?php
        // rewrite the links url to show the form in the right place
        if (isset($content['links']['comment']['#links']['comment-add']['href']) && $content['links']['comment']['#links']['comment-add']['href']) {
          $content['links']['comment']['#links']['comment-add']['href'] = 'node/'. $node->nid;
        }
      ?>
        <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    </footer>
    <?php endif; ?>
  </div>
  <div class="bottom"><span>&nbsp</span></div>
</article>

<?php if (isset($content['comments']['comments'])): ?>
<article>
  <div class="top"></div>
    <div class="content">
      <?php print render($content['comments']); ?>
    </div>
  <div class="bottom"></div>
</article>
<?php endif; ?>