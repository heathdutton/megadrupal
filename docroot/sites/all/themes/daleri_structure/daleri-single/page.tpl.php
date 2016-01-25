<?php print render($page['head']); ?>
<?php print render($page['page_bottom']); ?>
<a name="top"></a>
  <div id="wrapper">
    <?php print render($page['header']); ?>
    <?php if ($logo): ?>
      <a href="<?php print check_url($front_page); ?>" title="<?php print check_plain($site_name); ?>"><img src="<?php print check_url($logo); ?>" alt="<?php print check_plain($site_name); ?>" id="logo" /></a>
    <?php endif; ?>
    <?php 
      if ($site_name) {
        print '<h1><a href="'. check_url($front_page) .'" title="'. check_plain($site_name) .'">'. check_plain($site_name) . '</a></h1>';
      }
      if ($site_slogan) {
        print '<p class=\'slogan\'>'. check_plain($site_slogan) .'</p>';
      }
    ?>  
    
     <?php if (isset($main_menu)) { ?><?php print theme('links', $main_menu, array('class' => 'links', 'id' => 'navlist')) ?><?php } ?>
      
    <div id="content" >
      <?php print $breadcrumb; ?>  
      <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
      <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
      <?php if ($tabs): print '<ul class="tabs primary">'. render($tabs) .'</ul></div>'; endif; ?>
      <?php if (isset($tabs2)): print '<ul class="tabs secondary">'. render($tabs2) .'</ul>'; endif; ?>
      <?php if ($show_messages && $messages): print $messages; endif; ?>
      <?php if ($page['help']): ?><?php print render($page['help']); ?><?php endif; ?>
      <a name="content"></a>
      <?php print render($page['content']); ?>

    </div> <!-- /#center -->

    <div id="footer" class="clear">
      <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
      <?php endif; ?>
      <p><a href="#top">Return to top</a></p>
      <p>Original design by <a href="http://andreasviklund.com/">Andreas Viklund</a> |  Drupal port by <a href="http://www.nickbits.co.uk">Nick Young</a></p>
    </div> <!-- /#footer -->
  </div> 
<?php print render($page['page_bottom']); ?>