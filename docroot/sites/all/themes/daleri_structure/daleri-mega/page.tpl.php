<?php print render($page['head']); ?>
<?php print render($page['top']); ?>
<a name="top"></a>
  <div id="top">
    <div id="sitetitle">
      <?php print render($page['header']); ?>
      <?php if ($logo): ?>
        <a href="<?php print check_url($front_page); ?>" title="<?php print check_plain($site_name); ?>"><img src="<?php print check_url($logo); ?>" alt="<?php print check_plain($site_name); ?>" id="logo" /></a>
      <?php endif; ?>
      <?php 
        if ($site_name) {
          print '<h1><a href="'. check_url($front_page) .'" title="'. check_plain($site_name) .'">'. check_plain($site_name) . '</a></h1>';
        }
        if ($site_slogan) {
          print '<p>'. check_plain($site_slogan) .'</p>';
        }
      ?>  
    </div> <!-- /#sitetitle -->  
  </div>
  <div id="wrapper" class="clear">
    
    <div id="menubar">
       <?php
          if ($page['menubar']):
            print render($page['menubar']);
          endif;
        ?>
    </div> <!-- /#menubar -->  

    <div id="content" >
      <?php print $breadcrumb; ?>  
      <?php if ($page['highlight']): ?><div id="highlight"><?php render($page['highlight']); ?></div><?php endif; ?>
      <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
      <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
      <?php if ($tabs): print '<ul class="tabs primary">'. render($tabs) .'</ul></div>'; endif; ?>
      <?php if (isset($tabs2)): print '<ul class="tabs secondary">'. render($tabs2) .'</ul>'; endif; ?>
      <?php if ($show_messages && $messages): print $messages; endif; ?>
      <?php if ($page['help']): ?><?php print render($page['help']); ?><?php endif; ?>
        <a name="content"></a>
      <?php print render($page['content']); ?>

    </div> <!-- /#center -->

    <div id="sidebar">
	<?php if ($page['info']): ?> <div id="info"><?php print render($page['info']); ?></div> <!-- /#info -->   <?php endif; ?>  
	<?php if (!empty($page['sidebar_first'])): ?>      <div id="sidebar-first" class="sidebar">        <?php print render($page['sidebar_first']); ?>      </div> <!-- /#sidebar-last -->    <?php endif; ?>
	<?php if (!empty($page['sidebar_second'])): ?>      <div id="sidebar-second" class="sidebar">        <?php print render($page['sidebar_second']); ?>      </div> <!-- /#sidebar-last -->    <?php endif; ?>
    </div> <!-- /#sidebar-->
    <div class="clear"></div>
  </div> <!-- /#wrapper -->
  <div id="footer" class="clear">
    <div class="left">  
      <p>Original design by <a href="http://andreasviklund.com/">Andreas Viklund</a> | Drupal port by <a href="http://www.nickbits.co.uk">Nick Young</a></p>
    </div>
    <div class="right">
      <?php if ($page['footer']): ?>
          <?php print render($page['footer']); ?>
        <?php endif; ?>
      <p><a href="#top">Return to top</a></p>
    </div> 
  </div> <!-- /#footer -->
<?php print render($page['page_bottom']); ?>