  <div id="<?php if ($page['right']): print "two-column-"; endif;?>wrapper">
    <div id="container" class="clearfix">
      <div id="menu">
        <?php if (isset($main_menu)) { ?>
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); }?>
        <div class="feed">
          <?php print $feed_icons ?>
        </div>
      </div>
      <div id="logo-floater">
        <?php if ($logo || $site_title): ?>
          <div id="blog_title"><h1 class="blog_title"><a href="<?php print $front_page ?>" title="<?php print $site_title ?>">
          <?php if ($logo): ?>
            <img src="<?php print $logo ?>" alt="<?php print $site_title ?>" id="logo" />
          <?php endif; ?>
          <?php print $site_html ?>
          </a></h1></div>
        <?php endif; ?>
      </div>
      <?php if ($page['green']): ?><div id="mission"><?php print render($page['green']); ?></div><?php endif; ?>
      <?php print render($title_prefix); ?>

      <?php if ($title): ?>
        <h2 class="top-title">
          <?php print $title ?>
        </h2>
      <?php endif; ?>


      <div id="primary">
        <?php if ($tabs): ?><div id="tabs-wrapper" class="clearfix"><?php endif; ?>
        <?php if ($secondary_nav): print $secondary_nav; endif; ?>
        <?php if ($tabs): ?><ul class="tabs primary"><?php print render($tabs) ?></ul></div><?php endif; ?>
        <?php if ($tabs2): ?><ul class="tabs secondary"><?php print render($tabs2) ?></ul><?php endif; ?>
        <?php if ($show_messages && $messages): print $messages; endif; ?>
        
        <div class="clearfix page-body">
          <?php print render($page['content']); ?>
        </div>
      </div>

      <div id="footer"><div class="to_top"><a href="#menu" title="Top of Page">&nbsp;</a></div><?php if ($page['footer']) { print render($page['footer']); } ?></div>


    </div> <!-- /#container -->
      <?php if ($page['right']): ?><div id="sidebar-right"><?php print render($page['right']); ?></div><?php endif; ?>

  </div> <!-- /#wrapper -->

