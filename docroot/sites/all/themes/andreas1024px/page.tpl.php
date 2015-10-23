<?php print render($page['head']); ?>
<?php print render($page['top']); ?>
  <div id="wrapper-3">
    <div id="wrapper-2">
      <div id="wrapper">
        <div id="header">
          <?php print render($page['header']); ?>

          <?php if ($logo): ?>
            <a href="<?php print check_url($front_page); ?>" title="<?php print check_plain($site_name); ?>">
            <img src="<?php print check_url($logo); ?>" alt="<?php print check_plain($site_name); ?>" id="logo" />
            </a>
          <?php endif; ?>
          <?php print '<h1><a href="'. check_url($front_page) .'" title="'. check_plain($site_name) .'">';
          if ($site_name) {
            print '<span id="sitename">'. check_plain($site_name) .'</span>';
          }
          ?>
          </a></h1>
          <?php
          if ($site_slogan) {
            print '<p>'. check_plain($site_slogan) .'</p>';
          }
          ?>
          <div class="clear"></div>
        </div> <!-- /#header -->
        <div id="container" class="clear">
          <?php if ($page['sidebar_first']): ?>
            <div id="sidebar-first" class="sidebar">
              <?php print render($page['sidebar_first']); ?>
            </div> <!-- /#sidebar-first -->
          <?php endif; ?>
          <div id="center">
            <?php print $breadcrumb; ?>
            <?php if ($page['highlighted']): ?><div id="highlight"><?php render($page['highlight']); ?></div><?php endif; ?>
	  <a id="main-content"></a>
            <?php if ($tabs): print '<div id="tabs-wrapper" class="clearfix">'; endif; ?>
            <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
            <?php if ($tabs): print '<ul class="tabs primary">'. render($tabs) .'</ul></div>'; endif; ?>
            <?php if (isset($tabs2)): print '<ul class="tabs secondary">'. render($tabs2) .'</ul>'; endif; ?>
            <?php if ($show_messages && $messages): print $messages; endif; ?>
	  <?php if ($page['help']): ?>
	  <?php print render($page['help']); ?>
	  <?php endif; ?>
            <a name="content"></a>
            <?php print render($page['content']); ?>
          </div> <!-- /#center -->
          <?php if (!empty($page['sidebar_second'])): ?>
            <div id="sidebar-second" class="sidebar">
              <?php print render($page['sidebar_second']); ?>
            </div> <!-- /#sidebar-last -->
          <?php endif; ?>
          <div id="footer" class="clear">
	  <?php if ($page['footer']): ?>
            <?php print render($page['footer']); ?>
	  <?php endif; ?>
            <p>Original design by <a href='http://andreasviklund.com/'>Andreas Viklund</a> | Drupal port by <a href='http://www.nickbits.co.uk'>Nick Young</a></p>
          </div> <!-- /#footer -->
        </div> <!-- /#container -->
      </div> <!-- /#wrapper -->
    </div> <!-- /#wrapper2 -->
  </div> <!-- /#wrapper3 -->
  <!-- /layout -->
  <?php print render($page['bottom']); ?>
</body>
</html>