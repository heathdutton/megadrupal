<div id="page" class="container-12 clearfix">
  <div id="site-header" class="clearfix">
    <div id="pre-brand" class="grid-12"><?php print render($page['pre_brand']); ?> </div>
    <div id="branding" class="grid-12 clearfix">
    <?php if ($linked_logo_img): ?>
      <span id="logo" class="grid-2 alpha"><?php print $linked_logo_img; ?></span>
    <?php endif; ?>
    <?php if ($linked_site_name): ?>
      <h1 id="site-name" class="grid-10 omega"><?php print $linked_site_name; ?></h1>
    <?php endif; ?>
    <?php if ($site_slogan): ?>
      <div id="site-slogan" class="grid-10 prefix-2 alpha clearfix"><?php print $site_slogan; ?></div>
    <?php endif; ?>
    </div>
   <div id="post-brand" class="grid-12">
     <?php print render($page['post_brand']); ?>
     <?php print $messages; ?>
   </div>
  </div>

<div id="columns-content" class="clearfix">
<?php if ($page['sidebar_first']): ?>
  <div id="sidebar-left" class="grid-3">
    <?php print render($page['sidebar_first']); ?>
  </div>
<?php endif; ?>

<?php if ($page['sidebar_second']): ?>
  <div id="sidebar-right" class="grid-3">
    <?php print render($page['sidebar_second']); ?>
  </div>
<?php endif; ?>

  <div id="main-content" class="<?php print ns('grid-12', $page['sidebar_first'], 3, $page['sidebar_second'], 3); ?>">
  <?php if (strpos($breadcrumb, "forum")) { print $breadcrumb; } ?>
  <?php if ($page['above_content_first']): ?>
    <div id="above-content-first" class="alpha <?php print ns('grid-6', $page['above_content_second'], 3); ?>">
      <?php print render($page['above_content_first']); ?>
    </div>
  <?php endif; ?>
    
  <?php if ($page['above_content_second']): ?>
    <div id="above-content-second" class="omega clearfix <?php print ns('grid-6', $page['above_content_first'], 3); ?>">
      <?php print render($page['above_content_second']); ?>
    </div>
  <?php endif; ?>

  <div id="content" class="<?php print ns('grid-12', $page['sidebar_first'], 3, $page['sidebar_second'], 3); ?> clearfix">
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>      
    <?php if ($tabs): ?>
      <div class="tabs"><?php print render($tabs); ?></div>
    <?php endif; ?>
    <?php print render($page['help']); ?>
    <?php print render($page['content']); ?>
   </div>
    <?php print $feed_icons; ?>
  </div>
</div>

  <div id="footer" class="prefix-1 suffix-1">
    <?php if ($page['footer']): ?>
      <div id="footer" class="region grid-10 clearfix">
        <?php print render($page['footer']); ?>
      </div>
    <?php endif; ?>
  </div>

</div>
