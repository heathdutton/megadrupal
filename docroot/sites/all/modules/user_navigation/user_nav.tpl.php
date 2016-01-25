<?php
/**
 * @file
 * Template file for the theming example text form.
 *
 * Available variables:
 * - $user_object: Full object of current user.
 * - $profile_pic: Profile picture of current user.
 * - $search_block: Drupal Search Block.
 * - $menus: Navigation Menus.
 *
 * @see user_nav_page_build()
 */
?>
<div class='user-menu-container'><div class='user-menu-left-container'>
        <?php
        print render($search_block);
        ?>
        <div class = 'user-menu-user-area'>
            <?php
            echo l($profile_pic, 'user/' . $user_object->uid, array('html' => TRUE));
            ?>
            <div>
                <span><?php
                    $user_name = empty($user_object->name) ? '' : check_plain($user_object->name);
                    echo $user_name;
                    ?></span>
                <p><?php echo t('Please click on the image to edit your profile'); ?></p>
            </div>
        </div>
        <div class = 'user-menu-inner-item-container'>  
            <?php
            foreach ($menus as $menu) :
              $title_class = drupal_html_class($menu['link']['link_title']);
              $link = $menu['link']['link_path'];
              if (!$menu['link']['hidden']):
                echo "<div class='user-menu-inner-item user-menu-$title_class'>";
                echo l(drupal_ucfirst($menu['link']['link_title']), $link);
                if (!empty($menu['below'])):
                  echo "<div class='menu-container'>";
                  foreach ($menu['below'] as $inner_menu) :
                    $inner_title = $inner_menu['link']['link_title'];
                    $inner_link = $inner_menu['link']['link_path'];
                    echo l(drupal_ucfirst($inner_title), $inner_link);
                  endforeach;
                  echo '</div>';
                endif;
                echo '</div>';
              endif;
            endforeach;
            ?>
        </div>

        <span id='clock'></span>
    </div>
    <div class='user-nav-slide'></div>
</div>
