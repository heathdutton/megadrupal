<?php // $Id: $ ?>
  <div class="PageBackgroundSimpleGradient"></div>
    <div class="PageBackgroundGlare">
      <div class="PageBackgroundGlareImage"></div>
    </div>
    <div class="Main">
      <div class="Sheet">
        <div class="Sheet-tl"></div>
        <div class="Sheet-tr"></div>
        <div class="Sheet-bl"></div>
        <div class="Sheet-br"></div>
        <div class="Sheet-tc"></div>
        <div class="Sheet-bc"></div>
        <div class="Sheet-cl"></div>
        <div class="Sheet-cr"></div>
        <div class="Sheet-cc"></div>
        <div class="Sheet-body">
          <?php if (!empty($page['navigation'])): ?>
            <div class="nav">
              <div class="l"></div>
              <div class="r"></div>
              <?php print render($page['navigation']); ?>
            </div>
          <?php endif; ?>
          <div class="Header">
            <div class="logo">
              <?php if ($logo): ?>
                <a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
              <?php endif; ?>
            </div>
          </div>
          <div id="featured">
              <?php if (array_key_exists('featured', $page)): print render($page['featured']); endif; ?>
          </div>
          <div class="contentLayout">
            <?php if ($page['left']) print '<div class="sidebar1">' . render($page['left']) . "</div>";
                  else if (array_key_exists('sidebar_first', $page)) print '<div class="sidebar1">' . render($page['sidebar_first']) . "</div>";?>
            <div class="<?php $l = NULL; if ($page['left']) $l = 'left'; else if (array_key_exists('sidebar_first', $page)) $l = 'sidebar_first';
                              $r = NULL; if ($page['right']) $r = 'right'; else if (array_key_exists('sidebar_second', $page)) $r = 'sidebar_second';
                              print artxGetContentCellStyle($l, $r, $page['content']); ?>">
              <div class="Post">
                <div class="Post-tl"></div>
                <div class
="Post-tr"></div>
                <div class="Post-bl"></div>
                <div class="Post-br"></div>
                <div class="Post-tc"></div>
                <div class="Post-bc"></div>
                <div class="Post-cl"></div>
                <div class="Post-cr"></div>
                <div class="Post-cc"></div>
                <div class="Post-body">
                  <div class="Post-inner">
                    <div class="PostContent">
                      <?php if ($breadcrumb){ print $breadcrumb; } ?>
                      <?php if ($tabs){ print '<div id="tabs-wrapper" class="clear-block">'; } ?>
                      <?php if ($title){ print '<h2 class="PostHeaderIcon-wrapper'. ($tabs ? ' with-tabs' : '') .'">'. $title .'</h2>'; } ?>
                      <?php if ($tabs){ print render($tabs) . '</div>'; } ?>
                      <!--?php if ($tabs['#secondary']): print '<ul class="tabs secondary">'. render($tabs['#secondary']) .'</ul>'; endif; ?-->
                      <!--?php if ($mission){ print '<div id="mission">' . $mission . '</div>'; } ?-->
                      <?php if (array_key_exists('help', $page)){ print render($page['help']); } ?>
                      <?php if ($show_messages && $messages){ print $messages; } ?>
                      <?php print art_content_replace(render($page['content'])); ?>
                    </div>
                    <div class="cleared"></div>
                  </div>
                </div>
              </div>
            </div>
            <?php if ($page['right']) print '<div class="sidebar2">' . render($page['right']) . "</div>";
                  else if (array_key_exists('sidebar_second', $page)) print '<div class="sidebar2">' . render($page['sidebar_second']) . "</div>";?>
          </div>
          <div class="cleared"></div>
          <div class="Footer">
            <div class="Footer-inner">
              <a href="<?php $feedsUrls = array_keys(drupal_add_feed()); if(isset($feedsUrls[0]) && strlen($feedsUrls[0])>0) {print $feedsUrls[0];} ?>" class="rss-tag-icon" title="RSS"></a>
              <div class="Footer-text">
                <?php if ($page['footer']) { print render($page['footer']); } ?>
              </div>
            </div>
          <div class="Footer-background"></div>
        </div>
      </div>
    </div>
    <div class="cleared"></div>
      <p class="page-footer"><!--?php print $footer_message; ?--></p>
    </div>
    <!--?php print $closure; ?-->
