<?php
/**
 * @file
 * cms_core_blocks.features.uuid_bean.inc
 */

/**
 * Implements hook_uuid_features_default_beans().
 */
function cms_core_blocks_uuid_features_default_beans() {
  $beans = array();

  $beans[] = array(
    'label' => 'Lumi Slideshow',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'lumi-slideshow',
    'view_mode' => 'default',
    'created' => 1416745135,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'de9052a4-2a26-480c-a9ca-ea85b8d0ebb0',
    'uuid' => '0183f0cd-5323-4f46-ad39-7a4a9983e348',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '    <ul class="slides">
        <li>
            <div><img class="float-right no-style" src="http://i.imgur.com/7CquW.png"></div>

            <h1>Luxurious Theme</h1>

            <p>This brilliant looking theme has mobile friendly features and amazing artwork.</p>
        </li>

        <li>[row][column sm="8"]

            <h1>Shortcodes</h1>

            <p>New: This theme comes with a library of shortcodes that makes advanced website building super easy!</p>[/column][column sm="4"][dropshadow]<img src="http://farm3.staticflickr.com/2447/3909683071_e81876de35_z.jpg" title="Some rights reserved by Sergiu Bacioiu">[/dropshadow][/column][/row]
        </li>

        <li>[row][column sm="8"]

            <h1>Skinr Integration</h1>

            <p>This theme uses the Arctica base theme which puts responsive layout design in the hands of the webmaster.</p>[/column][column sm="4"][dropshadow type=3]<img src="http://farm3.staticflickr.com/2720/4327564714_ab1c8abe25_z.jpg" title="Some rights reserved by Sergiu Bacioiu">[/dropshadow][/column][/row]
        </li>
    </ul>',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Glazed img block 1',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'glazed-img-block-1',
    'view_mode' => 'default',
    'created' => 1413829960,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'b978c1b1-4bb8-4ee1-834b-136b5a08c45d',
    'uuid' => '0edee04e-56c9-4c89-a679-f272cb34464a',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[image-box style="romeo" heading="Romeo Animation" text="This is an image-box shortcode" button_text="Read more" button_href="http://sooperthemes.com/"]<img src="http://farm3.staticflickr.com/2277/5793507155_755600efc4_z_d.jpg"> [/image-box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Touchpro Slideshow',
    'description' => NULL,
    'title' => 'Featured Tutorials',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'touchpro-slideshow',
    'view_mode' => 'default',
    'created' => 1416826277,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '8d9fd576-0139-4559-9c7c-fa68e1b3f215',
    'uuid' => '0f15f42d-d284-4c92-a338-659fa9b4e098',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul class="slide">
	<li><img src="sites/default/files/block-attachments/wall-e-2.jpg">
		<p class="caption">This slideshow is flexible in both width and height #responsivedesign</p>
	</li>
	<li><img src="sites/default/files/block-attachments/wall-e-3.jpg">
		<p class="caption">You can totally customize the transitions and other behaviors of the slideshow</p>
	</li>
	<li><img src="sites/default/files/block-attachments/wall-e-1.jpg">
		<p class="caption">It\'s based on the popular jQuery cycle script and includes custom easing</p>
	</li>
	<li><img src="sites/default/files/block-attachments/wall-e-4.jpg">
		<p class="caption">And all essential settings are available in the theme configurator</p>
	</li>
</ul>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Mega Menu Heading Infographical Elements',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'mega-menu-heading-infographical-',
    'view_mode' => 'default',
    'created' => 1431011672,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '6cf078c9-eac1-40c8-8603-0ee3930ecc5d',
    'uuid' => '1ace7c6a-6824-4689-b17f-409ad44f0970',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h4>Infographical Elements</h4>',
          'format' => 'unfiltered_html',
          'safe_value' => '<h4>Infographical Elements</h4>',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'LA Office',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'la-office',
    'view_mode' => 'default',
    'created' => 1413830451,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '1a55ba17-4db1-4c7b-9acf-a943dc11e40a',
    'uuid' => '1aeda41e-ea0f-466c-94bc-99ae7ffe238d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h3>LA Office</h3>

<p>[contact address="Park Lane 5" city=LA zip="12345 AB" phone=555-55-12345 email=info@example.com][/contact]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'lightbox images',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'lightbox-images',
    'view_mode' => 'default',
    'created' => 1416745403,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '58be5954-d430-4ac8-ab9f-794d10f9bc2c',
    'uuid' => '1f3e8624-74bf-45df-a17e-4f97b002e1b2',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[row] [column sm="3"][dropshadow]<a href="http://farm3.staticflickr.com/2542/3953686888_6bb1c91e09_b.jpg" rel="lightbox[preblocks]" title="Photo copyright <a  data-cke-saved-href=\'http://www.flickr.com/photos/sergiu_bacioiu/\' href=\'http://www.flickr.com/photos/sergiu_bacioiu/\'>Sergiu Bacioiu</a> licensed under CC2.0 Attribution"><img src="http://farm3.staticflickr.com/2542/3953686888_6bb1c91e09_m.jpg" height="159" width="240"></a>[/dropshadow][/column][column sm="3"][dropshadow]<a href="http://farm3.staticflickr.com/2470/3930657923_0b0f00f833_b.jpg" rel="lightbox[preblocks]" title="Photo copyright <a  data-cke-saved-href=\'http://www.flickr.com/photos/sergiu_bacioiu/\' href=\'http://www.flickr.com/photos/sergiu_bacioiu/\'>Sergiu Bacioiu</a> licensed under CC2.0 Attribution"><img src="http://farm3.staticflickr.com/2470/3930657923_0b0f00f833_m.jpg" height="159" width="240"></a>[/dropshadow][/column][column sm="3"][dropshadow]<a href="http://farm3.staticflickr.com/2742/4315795138_fd99eeaa54_b.jpg" rel="lightbox[preblocks]" title="Photo copyright <a  data-cke-saved-href=\'http://www.flickr.com/photos/sergiu_bacioiu/\' href=\'http://www.flickr.com/photos/sergiu_bacioiu/\'>Sergiu Bacioiu</a> licensed under CC2.0 Attribution"><img src="http://farm3.staticflickr.com/2742/4315795138_fd99eeaa54_m.jpg" height="159" width="240"></a>[/dropshadow][/column][column sm="3" last=1][dropshadow]<a href="http://farm5.staticflickr.com/4032/4394955222_07c30ed3d6_b.jpg" rel="lightbox[preblocks]" title="Photo copyright <a  data-cke-saved-href=\'http://www.flickr.com/photos/sergiu_bacioiu/\' href=\'http://www.flickr.com/photos/sergiu_bacioiu/\'>Sergiu Bacioiu</a> licensed under CC2.0 Attribution"><img src="http://farm5.staticflickr.com/4032/4394955222_07c30ed3d6_m.jpg" height="159" width="240"></a>[/dropshadow][/column] [/row]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 2: Shortcodes',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-2-shortcodes',
    'view_mode' => 'default',
    'created' => 1415267551,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'aa265a80-5015-4656-a42b-bf39b3b25671',
    'uuid' => '2422704c-78cc-4a59-a4f7-39accd03fd4c',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Amazing Shortcodes" icon="th" layout="icon-inline-title" icon_style="circle"]On top of the fully loaded CKEditor set-up with Media library, we give you shortcodes to create beautiful layouts and engaging content. Our shortcodes allow you to add add rich content to any block, you can even use shortcodes to include all Drupal blocks and views in content![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Social links',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'social-links',
    'view_mode' => 'default',
    'created' => 1413830858,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'fa3cfbdb-6b5f-48e7-8738-a5b6e86fafc1',
    'uuid' => '2440d5fc-e4b5-487c-89ed-bf335499d88a',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[social type="inline" style="circle" color="inherit" size="2x" hover="pulse" Twitter="#" Facebook="#" YouTube="#" Drupal="#|9 Years on D.O.!"][/social]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 1: Time Saver icon-white',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-1-time-saver-icon-white',
    'view_mode' => 'default',
    'created' => 1418141102,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '718ab1e3-dde3-4a10-b026-e7b352b57e4a',
    'uuid' => '28336abd-8e35-4bbc-8cbe-b52ef7b3d887',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="A Real Time-Saver" icon="clock-o" layout="icon-inline-title" icon_style="circle" icon_color="white" icon_border="white"]CMS Powerstart is the ultimate shortcut to a production-ready Drupal website. It saves you those boring hours of initial work and takes care of responsive design, SEO settings, performance settings and comes with advances content types and views out of the box[/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Mega Menu Heading Layout Elements',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'mega-menu-heading-layout-element',
    'view_mode' => 'default',
    'created' => 1431011635,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '223d56f3-1104-4f9f-a084-6af50eaad95d',
    'uuid' => '2de02ddd-2c90-42f6-aae6-cf49483059a8',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h4>Layout Elements</h4>',
          'format' => 'unfiltered_html',
          'safe_value' => '<h4>Layout Elements</h4>',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 3: Bootstrap icon-white',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-3-bootstrap-icon-white',
    'view_mode' => 'default',
    'created' => 1418141294,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '018df400-c98a-450c-8383-e65ca4b8ba12',
    'uuid' => '3bf4538f-18f8-4a6a-96cb-e4caa5830d2b',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Bootstrap 3" icon="twitter" layout="icon-inline-title" icon_style="circle" icon_color="white" icon_border="white"]Bootstrap 3 is so awesome for responsive design! And Powerstart has awesome Bootstrap 3 integration. It is integrated in our views and content types to create responsive, mobile-friendly content. You will also love our Shortcode Generator which supports all Bootstrap 3 elements in a WYSIWYG interface![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Testimonials',
    'description' => NULL,
    'title' => 'Testimonials',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'testimonials',
    'view_mode' => 'default',
    'created' => 1416747987,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'd975a180-6716-4ae8-a665-af3e6438e830',
    'uuid' => '3c5f1bb5-25fa-46e3-8f85-105e24188ab4',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul class="testimonials">
	<li><img src="http://i.imgur.com/OZcatc8.jpg">
		<p>Curabitur aliquam est et tortor consequat consequat. Aenean nec massa sit amet ipsum sagittis ultricies. Proin nisi au gue, volutpat vitae malesuada vel. Nulla non</p>

		<div class="read-more"><a href="#">Read more</a></div>
	</li>
	<li><img src="http://i.imgur.com/FKFi1zx.jpg">
		<p>Curabitur aliquam est et tortor consequat consequat. Aenean nec massa sit amet ipsum sagittis ultricies. Proin nisi au gue, volutpat vitae malesuada vel. Nulla non</p>

		<div class="read-more"><a href="#">Read more</a></div>
	</li>
	<li><img src="http://i.imgur.com/2QkKNO8.jpg">
		<p>Curabitur aliquam est et tortor consequat consequat. Aenean nec massa sit amet ipsum sagittis ultricies. Proin nisi au gue, volutpat vitae malesuada vel. Nulla non</p>

		<div class="read-more"><a href="#">Read more</a></div>
	</li>
</ul>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 2: Shortcodes icon-white',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-2-shortcodes-icon-white',
    'view_mode' => 'default',
    'created' => 1418142697,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'e97e9575-1147-4f86-aa71-5525067c7a33',
    'uuid' => '3cda9c20-3b64-430b-b4cf-7aba88f57b4d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Amazing Shortcodes" icon="th" layout="icon-inline-title" icon_style="circle" icon_color="white" icon_border="white"]On top of the fully loaded CKEditor set-up with Media library, we give you shortcodes to create beautiful layouts and engaging content. Our shortcodes allow you to add add rich content to any block, you can even use shortcodes to include all Drupal blocks and views in content![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Facebook Widget',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'facebook-widget',
    'view_mode' => 'default',
    'created' => 1415129922,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '1ba4fc70-8fff-4752-91ee-be2c1fbe3e7b',
    'uuid' => '4543a147-4d81-44b2-a4b4-6f82ce1ba5a8',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/sdk.js#xfbml=1&appId=568856383201152&version=v2.0";
  fjs.parentNode.insertBefore(js, fjs);
}(document, \'script\', \'facebook-jssdk\'));</script>
<div class="fb-like-box" data-href="https://www.facebook.com/sooperthemes" data-colorscheme="light" data-width="270" data-show-faces="true" data-header="false" data-stream="false" data-show-border="true"></div>',
          'format' => 'unfiltered_html',
          'safe_value' => '<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/sdk.js#xfbml=1&appId=568856383201152&version=v2.0";
  fjs.parentNode.insertBefore(js, fjs);
}(document, \'script\', \'facebook-jssdk\'));</script>
<div class="fb-like-box" data-href="https://www.facebook.com/sooperthemes" data-colorscheme="light" data-width="270" data-show-faces="true" data-header="false" data-stream="false" data-show-border="true"></div>',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 1: Time Saver',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-1-time-saver',
    'view_mode' => 'default',
    'created' => 1415266674,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '7d66b658-b3ef-4102-8a8f-45a0929a290d',
    'uuid' => '52b9c4b5-ffbb-4c46-9b91-a7a2b4725d10',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="A Real Time-Saver" icon="clock-o" layout="icon-inline-title" icon_style="circle"]CMS Powerstart is the ultimate shortcut to a production-ready Drupal website. It saves you those boring hours of initial work and takes care of responsive design, SEO settings, performance settings and comes with advances content types and views out of the box[/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'New York Office',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'new-york-office',
    'view_mode' => 'default',
    'created' => 1413829986,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'b87c50f0-0195-4d53-ac37-6a9b033099e4',
    'uuid' => '53c16019-8308-4638-b664-563e8726decd',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h3>New York Office</h3>

<p>[contact address="5th Avenue 271B" city=LA zip="12345 AB" phone=555-55-12345 email=info@example.com][/contact]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Connect with us',
    'description' => NULL,
    'title' => 'Connect with us',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'connect-with-us',
    'view_mode' => 'default',
    'created' => 1416825863,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'b6aa6820-90d5-4582-8b32-2534ac050136',
    'uuid' => '592742dc-706a-44a0-bf9e-562d7f703bb8',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul><li><a href="https://twitter.com/#!/sooperthemes">Follow us on Twitter</a></li><li><a href="https://www.facebook.com/sooperthemes">Follow us on Facebook</a></li><li><a href="/touchpro/rss.xml/">TouchPro Magazine RSS</a></li><li><a href="/touchpro/contact">Contact us</a></li></ul>',
          'format' => 'wysiwyg_simple',
          'safe_value' => '<ul>
<li><a href="https://twitter.com/#!/sooperthemes">Follow us on Twitter</a></li>
<li><a href="https://www.facebook.com/sooperthemes">Follow us on Facebook</a></li>
<li><a href="/touchpro/rss.xml/">TouchPro Magazine RSS</a></li>
<li><a href="/touchpro/contact">Contact us</a></li>
</ul>
',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Main Features Glazed',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'main-features-glazed',
    'view_mode' => 'default',
    'created' => 1413830134,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '2c3db70d-8e2e-45e1-8a3f-3afc018b383f',
    'uuid' => '5f6b9b82-6095-418c-b062-e0df95b6726c',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[row]<br>
	[column sm="6"]<br>
	[box heading="Title of a feature goes here!!" layout="icon-adjacent" icon="bolt" icon_size="3x" icon_color="accent1"]<br>
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque luctus commodo velit, ac convallis diam molestie at. Sed commodo, mi quis![/box]<br>
	[/column]<br>
	[column sm="6"]<br>
	[box heading="Title of a feature goes here!" layout="icon-adjacent" icon="cloud" icon_size="3x" icon_color="accent1"]<br>
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque luctus commodo velit, ac convallis diam molestie at. Sed commodo, mi quis![/box]<br>
	[/column]<br>
	[/row]<br>
	[row]<br>
	[column sm="6"]<br>
	[box heading="Title of a feature goes here!" layout="icon-adjacent" icon="comment-o" icon_size="3x" icon_color="accent1"]<br>
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque luctus commodo velit, ac convallis diam molestie at. Sed commodo, mi quis![/box]<br>
	[/column]<br>
	[column sm="6"]<br>
	[box heading="Title of a feature goes here!" layout="icon-adjacent" icon="graduation-cap" icon_size="3x" icon_color="accent1"]<br>
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque luctus commodo velit, ac convallis diam molestie at. Sed commodo, mi quis![/box]<br>
	[/column]<br>
	[/row]</p>

<p><br>
	<a class="btn btn-lg btn-primary center-block col-md-4 wow wow-visible rubberBand" href="#">Buy Glazed now</a></p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Glazed img block 3',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'glazed-img-block-3',
    'view_mode' => 'default',
    'created' => 1413830053,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '3111e29a-ed56-4d2a-945c-c96e30b5fe1d',
    'uuid' => '665b5da1-7fc3-4f3a-af8a-d8422cb91fac',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[image-box style="sarah" wow_center_offset="45" heading="Sarah Animation" text="This is an image-box shortcode" button_text="I\'m Ready!" button_href="http://sooperthemes.com/"] <img alt="" src="https://farm4.staticflickr.com/3621/3401365869_6ee875a890_z_d.jpg?zz=1">[/image-box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'dummy ad',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'dummy-ad',
    'view_mode' => 'default',
    'created' => 1416825914,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '7f6733a6-1a42-4b4f-a123-70a7a6d7a839',
    'uuid' => '759bddba-bf7e-4e42-8a1e-b0ba8408affd',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p><a href="http://www.sooperthemes.com"><img alt="Dummy Ad" src="http://i.imgur.com/6CedIGZ.jpg"></a></p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Information',
    'description' => NULL,
    'title' => 'Information',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'information',
    'view_mode' => 'default',
    'created' => 1416825883,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '20829c70-98d2-4422-83d1-264fc89a120a',
    'uuid' => '7eaaae46-4a8c-49c3-ac30-05ebcb871514',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul><li><a href="#">About Us</a></li><li><a href="#">Become a TouchPro author</a></li><li><a href="#">Publishing Policy</a></li><li><a href="#">Advertise with us</a></li></ul>',
          'format' => 'wysiwyg_simple',
          'safe_value' => '<ul>
<li><a href="#">About Us</a></li>
<li><a href="#">Become a TouchPro author</a></li>
<li><a href="#">Publishing Policy</a></li>
<li><a href="#">Advertise with us</a></li>
</ul>
',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Join Our Newsletter',
    'description' => NULL,
    'title' => 'Join Our Newsletter',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'join-our-newsletter',
    'view_mode' => 'default',
    'created' => 1416745536,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '4c1e2218-4f98-4044-83f9-2329384aa507',
    'uuid' => '8410c753-93b7-4b5e-bc1b-2cbb2158292f',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>Lorem ipsum dolor sit amet, consectetuer turpis adipiscing volutpat mattis eros.</p>
<form><input class="form-text" type="text"> <input class="form-submit" value="Subscribe" type="submit"></form>',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => '1 min. installation',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => '1-min-installation',
    'view_mode' => 'default',
    'created' => 1416745168,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'dceb3e43-0898-492a-9e40-1128b79e4da5',
    'uuid' => '8bf7837e-8bc2-4478-b8a4-cba196a6f8ad',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>Install Drupal in record breaking time! Our custom installation scripts are fully tuned for fast installation, making the process faster than installing any other distribution.&lt;/p&gt; You can see a <a href="#">here</a>.</p>
<p>[button url=http://www.sooperthemes.com/drupal-themes/lumi size=3]Buy theme[/button]</p>',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'SEO Optimized',
    'description' => NULL,
    'title' => 'SEO Optimized',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'seo-optimized',
    'view_mode' => 'default',
    'created' => 1416564168,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '370da9d1-d36a-4705-a336-86dab16f0146',
    'uuid' => '90e0f228-17ca-48ab-b87a-1f3e03f23cc4',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>All Sooperthemes products are SEO optimized out of the box.</p>

<p>Clean semantic markup makes sure that search engines understand your content while advanced Drupal backend features take care of url aliases, unique canonical urls, blazing fast pageload times and full control over all HTML.</p>

<p>[button link="http://www.sooperthemes.com/drupal-themes/respondr" type="primary"]Buy theme[/button]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Mega Menu Heading Display Elements',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'mega-menu-heading-display-elemen',
    'view_mode' => 'default',
    'created' => 1431011680,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '783018eb-35c2-4839-87ec-644e258b0290',
    'uuid' => '96624fb1-3da2-4d9a-ace3-a0c71ebb6c7d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h4>Display Elements</h4>',
          'format' => 'unfiltered_html',
          'safe_value' => '<h4>Display Elements</h4>',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Call to action',
    'description' => NULL,
    'title' => 'SEO Optimized',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'call-to-action',
    'view_mode' => 'default',
    'created' => 1416745299,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'be9a35fb-d800-465d-8494-1d1e771e3907',
    'uuid' => '9a1f8906-4a7d-4a36-87c2-1061168456cf',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h2 class="float-left">Insert your favorite call-to-action here!</h2>

<div>[button url="http://www.sooperthemes.com/pricing" type="primary" xlass="float-right"]Buy this theme[/button]</div>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 1: Time Saver icon-link',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-1-time-saver-icon-link',
    'view_mode' => 'default',
    'created' => 1418142872,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '9988deda-838d-4329-979a-6e4f01e6a1b5',
    'uuid' => '9aaeb1a1-1acd-4457-8677-fa1b1ca8f71a',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="A Real Time-Saver" icon="clock-o" layout="icon-inline-title" icon_style="circle" icon_color="link" icon_border="link"]CMS Powerstart is the ultimate shortcut to a production-ready Drupal website. It saves you those boring hours of initial work and takes care of responsive design, SEO settings, performance settings and comes with advances content types and views out of the box[/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'About Drupal CMS',
    'description' => NULL,
    'title' => 'About Drupal CMS',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'about-drupal-cms',
    'view_mode' => 'default',
    'created' => 1416564085,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '1871cc5a-e497-4d32-a3f4-0b8ac60f399a',
    'uuid' => '9f5ea3b1-d044-49d6-b4e6-cedf1d1440d5',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>Drupal\'s excellent modules and active community have made it the choice of CMS for:</p><ul><li><a href="http://www.whitehouse.gov/">The White House</a></li><li><a href="http://www.harvard.edu/">Harvard University</a></li><li><a href="https://developer.linkedin.com/">LinkedIn</a></li><li><a href="http://www.beyonceonline.com/us/home">Sony Music</a></li><li><a href="http://www.infocus.com/">InFocus</a></li></ul>',
          'format' => 'wysiwyg_simple',
          'safe_value' => '<p>Drupal\'s excellent modules and active community have made it the choice of CMS for:</p>
<ul>
<li><a href="http://www.whitehouse.gov/">The White House</a></li>
<li><a href="http://www.harvard.edu/">Harvard University</a></li>
<li><a href="https://developer.linkedin.com/">LinkedIn</a></li>
<li><a href="http://www.beyonceonline.com/us/home">Sony Music</a></li>
<li><a href="http://www.infocus.com/">InFocus</a></li>
</ul>
',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 2: Shortcodes icon-link',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-2-shortcodes-icon-link',
    'view_mode' => 'default',
    'created' => 1418142924,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '6a6c5704-7ccd-48de-bc62-0d80fac13f1c',
    'uuid' => '9f71bc90-6398-4f32-920d-63482fff22ea',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Amazing Shortcodes" icon="th" layout="icon-inline-title" icon_style="circle" icon_color="link" icon_border="link"]On top of the fully loaded CKEditor set-up with Media library, we give you shortcodes to create beautiful layouts and engaging content. Our shortcodes allow you to add add rich content to any block, you can even use shortcodes to include all Drupal blocks and views in content![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Power statement',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'power-statement',
    'view_mode' => 'default',
    'created' => 1416747927,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'fedf4631-b096-48b4-ac72-fea1515a1cfb',
    'uuid' => 'aba35373-3c18-47b9-b2ad-074f8a46eb85',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h2>Improve your ROI with our Deviant Seven Solutions</h2>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Glazed img block 2',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'glazed-img-block-2',
    'view_mode' => 'default',
    'created' => 1413830026,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'eb15de51-9e4c-4456-a9f7-92b4f48eea02',
    'uuid' => 'b5d17b59-c08f-4f73-9167-e620c85b8b1c',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[image-box style="ruby" wow_center_offset="40" heading="Ruby Animation" text="This is an image-box shortcode" button_text="I\'m Ready!" button_href="http://sooperthemes.com/"] <img alt="" src="https://farm3.staticflickr.com/2468/4166013343_8ef03e1799_z_d.jpg">[/image-box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'test',
    'description' => NULL,
    'title' => 'test',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'test',
    'view_mode' => 'default',
    'created' => 1423820687,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'ce79b309-3c71-4b49-92db-cef9c137e201',
    'uuid' => 'b619d935-24d4-4f28-ab7b-7492fc1c1e4d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => 'testt',
          'format' => 'plain_text',
          'safe_value' => '<p>testt</p>
',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Drupal CMS Powerstart',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'drupal-cms-powerstart',
    'view_mode' => 'default',
    'created' => 1413830605,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '115c5d60-cd32-4694-9e74-7d6339a959c6',
    'uuid' => 'b8e41eb4-dda6-43e0-9a2f-3bb68dc62aa3',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h1 style="text-align:center;">Drupal CMS Powerstart Content Types</h1>

<p style="text-align:center;">The backend is based on the CMS Powerstart distribution, <a href="https://www.drupal.org/project/cms" target="_blank&quot;">read more</a></p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 3: Bootstrap icon-link',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-3-bootstrap-icon-link',
    'view_mode' => 'default',
    'created' => 1418143102,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '2c98a428-dadc-41dc-bb46-182f7dc5df30',
    'uuid' => 'bb5a4aae-08f8-4138-b593-05e52f712e83',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Bootstrap 3" icon="twitter" layout="icon-inline-title" icon_style="circle" icon_color="link" icon_border="link"]Bootstrap 3 is so awesome for responsive design! And Powerstart has awesome Bootstrap 3 integration. It is integrated in our views and content types to create responsive, mobile-friendly content. You will also love our Shortcode Generator which supports all Bootstrap 3 elements in a WYSIWYG interface![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Preblock 3: Bootstrap',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'preblock-3-bootstrap',
    'view_mode' => 'default',
    'created' => 1415267904,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '2d6dc266-4dc7-4b81-bf87-c8696c1f418e',
    'uuid' => 'c519df83-3cb8-49d7-935b-c6d08ef81bdc',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p>[box heading="Bootstrap 3" icon="twitter" layout="icon-inline-title" icon_style="circle"]Bootstrap 3 is so awesome for responsive design! And Powerstart has awesome Bootstrap 3 integration. It is integrated in our views and content types to create responsive, mobile-friendly content. You will also love our Shortcode Generator which supports all Bootstrap 3 elements in a WYSIWYG interface![/box]</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Websites we really like',
    'description' => NULL,
    'title' => 'Websites we really like',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'websites-we-really-like',
    'view_mode' => 'default',
    'created' => 1416825843,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '8f68135d-d76f-447f-82af-9e16b6983c1b',
    'uuid' => 'cc50449b-c2a7-403e-844c-8ed2bd0c0c64',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul><li><a href="http://www.sooperthemes.com/">SooperThemes Drupal Themes</a></li><li><a href="http://www.sooperthemes.com/arctica">Arctica base theme</a></li><li><a href="http://drupal.org/">Powered by Drupal</a></li><li><a href="http://www.smashingmagazine.com/">Smashing Magazine</a></li></ul>',
          'format' => 'wysiwyg_simple',
          'safe_value' => '<ul>
<li><a href="http://www.sooperthemes.com/">SooperThemes Drupal Themes</a></li>
<li><a href="http://www.sooperthemes.com/arctica">Arctica base theme</a></li>
<li><a href="http://drupal.org/">Powered by Drupal</a></li>
<li><a href="http://www.smashingmagazine.com/">Smashing Magazine</a></li>
</ul>
',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Glazed Default Home Slider v2',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'glazed-default-home-slider-v2',
    'view_mode' => 'default',
    'created' => 1430222351,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '5f2e1cf6-dc2c-4efe-8019-9fae171f2312',
    'uuid' => 'd27b03e5-9f7b-430a-9702-caa77f6f1c99',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '  <!--
  #################################
    - THEMEPUNCH BANNER -
  #################################
  -->
              <div class="tp-banner-container">
    <div class="tp-banner">
      <ul>

      <!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://localhost/images/tmp/glazed-perspective-demos-0.2.jpg" data-bgposition="center center" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="center top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <h1 style="font-size:90px;font-weight:700;line-height:1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">
          Glazed
        </h1><!-- LAYER NR. 2 -->

        <h2 style="font-weight: 300; font-size:60px; line-height: 1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="500" data-voffset="120" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          Biggest Drupal Theme Ever
        </h2><!-- LAYER NR. 3 -->

        <p style="font-size:21px;color:#333;line-height:1.4em;" class="lead tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="700" data-voffset="180" data-x="center" data-y="center" style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">
          Unique Designs with Full Demo Content!
        </p><!-- LAYER NR. 3 -->

        <div class="tp-caption Miami_nostyle sft fadeout tp-scrollbelowslider" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="300" data-hoffset="0" data-scrolloffset="20" data-speed="600" data-splitin="none" data-splitout="none" data-start="2450" data-voffset="50" data-x="center" data-y="bottom" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="tp-layer-inner-rotation Miami_nostyle rs-slideloop" data-easing="Power3.easeInOut" data-speed="1" data-xe="0" data-xs="0" data-ye="10" data-ys="0" style="">
            <div class="icon-angle-down" style="color: #fff; cursor: pointer; font-size: 45px; font-style: italic; padding: 50px; text-align: center"></div>
          </div>
        </div>
      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7" data-lazyload="http://localhost/images/misc/impossible-2560.jpg" data-bgposition="center top" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="right top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-hoffset="0" data-voffset="50" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="stpe-cta stpe-cta--wide glazed-util-overlay-white" style="padding:60px 80px">
  <h3 class="stpe-cta__heading">Discover the Drupal theme that does <em>Everything</em></h3>
  <a class="btn btn-lg stpe-borderbutton glazed-util-color-primary glazed-util-border-primary glazed-util-hover-background-primary glazed-util-hover-color-white glazed-util-hover-border-primary" href="sooperthemes.com/pricing">Buy Glazed</a>
</div>
        </div><!-- LAYER NR. 3 -->

      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img alt="video_woman_cover2" data-bgfit="cover" data-bgposition="center center" data-bgrepeat="no-repeat" src="http://miamicdn4.themepunch.com/wp-content/uploads/2014/09/video_woman_cover2.jpg"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption tp-fade fadeout fullscreenvideo tp-videolayer" data-aspectratio="16:9" data-autoplay="true" data-autoplayonlyfirsttime="false" data-easing="Power4.easeOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="1500" data-forcecover="1" data-forcerewind="on" data-nextslideatend="true" data-posteronmobile="off" data-speed="1000" data-start="900" data-videocontrols="none" data-videoloop="loopandnoslidestop" data-videomp4="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4" data-videoposter="http://themepunch.com/miami/wp-content/uploads/2014/09/video_woman_cover2.jpg" data-videopreload="auto" data-videowebm="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm" data-videowidth="100%" data-x="0" data-y="0" style="z-index: 5;"></div><!-- LAYER NR. 2 -->

        <div class="tp-caption Miami_darkbgbox2 tp-fade fadeout tp-resizeme" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="300" data-splitin="none" data-splitout="none" data-start="300" data-voffset="20" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;"></div><!-- LAYER NR. 3 -->

        <div class="tp-caption miami_title_60px customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          ENJOY THE SHOW
        </div><!-- LAYER NR. 5 -->

        <h3 class="tp-caption glazed-util-color-white customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="60" data-x="center" data-y="center" style="z-index: 9; max-width: auto; max-height: auto; white-space: nowrap;">
          Now Go Explore the Biggest Drupal Theme Ever!
        </h3><!-- LAYER NR. 6 -->
      </li>
</ul>
<div class="tp-bannertimer"></div>  </div>  </div>
      <script type="text/javascript">

        jQuery(document).ready(function() {
          if (jQuery(window).height() > 900) {
            toggleFullScreen = "off";
          }
          else {
            toggleFullScreen = "on";
          }

          jQuery(\'.tp-banner\').show().revolution(
          {
            dottedOverlay:"none",
            delay:20000,
            startwidth:1170,
            startheight:900,
            hideThumbs:200,
            fullScreen: toggleFullScreen,
            fullScreenOffsetContainer: "",
            thumbWidth:100,
            thumbHeight:50,
            thumbAmount:5,

            navigationType:"bullet",
            navigationArrows:"solo",
            navigationStyle:"preview1",

            touchenabled:"on",
            onHoverStop:"off",

            swipe_velocity: 0.7,
            swipe_min_touches: 1,
            swipe_max_touches: 1,
            drag_block_vertical: false,

                        parallax:"mouse",
            parallaxBgFreeze:"on",
            parallaxLevels:[7,4,3,2,5,4,3,2,1,0],

            keyboardNavigation:"off",

            navigationHAlign:"center",
            navigationVAlign:"bottom",
            navigationHOffset:0,
            navigationVOffset:20,
            soloArrowLeftHalign:"left",
            soloArrowLeftValign:"center",
            soloArrowLeftHOffset:20,
            soloArrowLeftVOffset:0,
            soloArrowRightHalign:"right",
            soloArrowRightValign:"center",
            soloArrowRightHOffset:20,
            soloArrowRightVOffset:0,
            shadow:0,
            spinner:"spinner4",

            stopLoop:"off",
            stopAfterLoops:-1,
            stopAtSlide:-1,

            shuffle:"off",

            autoHeight:"off",
            forceFullWidth:"off",



            hideThumbsOnMobile:"off",
            hideNavDelayOnMobile:1500,
            hideBulletsOnMobile:"off",
            hideArrowsOnMobile:"off",
            hideThumbsUnderResolution:0,

            hideSliderAtLimit:0,
            hideCaptionAtLimit:0,
            hideAllCaptionAtLilmit:0,
            startWithSlide:0,
            videoJsPath:"rs-plugin/videojs/",
            fullScreenOffsetContainer: ""
          });




        }); //ready

      </script>
<style>
.tp-caption.Miami_darkbgbox2,.Miami_darkbgbox2 {
  min-width: 600px;
  max-width: 90%;
  min-height:200px;
  text-decoration:none;
  background:#2d3032;
  background-color:rgba(45,48,50,0.15);
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px {
font-size:60px;
line-height:60px;
font-weight:700;
color:#ffffff;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

</style>
<!-- END REVOLUTION SLIDER -->  ',
          'format' => 'unfiltered_html',
          'safe_value' => '  <!--
  #################################
    - THEMEPUNCH BANNER -
  #################################
  -->
              <div class="tp-banner-container">
    <div class="tp-banner">
      <ul>

      <!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://localhost/images/tmp/glazed-perspective-demos-0.2.jpg" data-bgposition="center center" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="center top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <h1 style="font-size:90px;font-weight:700;line-height:1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">
          Glazed
        </h1><!-- LAYER NR. 2 -->

        <h2 style="font-weight: 300; font-size:60px; line-height: 1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="500" data-voffset="120" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          Biggest Drupal Theme Ever
        </h2><!-- LAYER NR. 3 -->

        <p style="font-size:21px;color:#333;line-height:1.4em;" class="lead tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="700" data-voffset="180" data-x="center" data-y="center" style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">
          Unique Designs with Full Demo Content!
        </p><!-- LAYER NR. 3 -->

        <div class="tp-caption Miami_nostyle sft fadeout tp-scrollbelowslider" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="300" data-hoffset="0" data-scrolloffset="20" data-speed="600" data-splitin="none" data-splitout="none" data-start="2450" data-voffset="50" data-x="center" data-y="bottom" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="tp-layer-inner-rotation Miami_nostyle rs-slideloop" data-easing="Power3.easeInOut" data-speed="1" data-xe="0" data-xs="0" data-ye="10" data-ys="0" style="">
            <div class="icon-angle-down" style="color: #fff; cursor: pointer; font-size: 45px; font-style: italic; padding: 50px; text-align: center"></div>
          </div>
        </div>
      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7" data-lazyload="http://localhost/images/misc/impossible-2560.jpg" data-bgposition="center top" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="right top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-hoffset="0" data-voffset="50" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="stpe-cta stpe-cta--wide glazed-util-overlay-white" style="padding:60px 80px">
  <h3 class="stpe-cta__heading">Discover the Drupal theme that does <em>Everything</em></h3>
  <a class="btn btn-lg stpe-borderbutton glazed-util-color-primary glazed-util-border-primary glazed-util-hover-background-primary glazed-util-hover-color-white glazed-util-hover-border-primary" href="sooperthemes.com/pricing">Buy Glazed</a>
</div>
        </div><!-- LAYER NR. 3 -->

      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img alt="video_woman_cover2" data-bgfit="cover" data-bgposition="center center" data-bgrepeat="no-repeat" src="http://miamicdn4.themepunch.com/wp-content/uploads/2014/09/video_woman_cover2.jpg"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption tp-fade fadeout fullscreenvideo tp-videolayer" data-aspectratio="16:9" data-autoplay="true" data-autoplayonlyfirsttime="false" data-easing="Power4.easeOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="1500" data-forcecover="1" data-forcerewind="on" data-nextslideatend="true" data-posteronmobile="off" data-speed="1000" data-start="900" data-videocontrols="none" data-videoloop="loopandnoslidestop" data-videomp4="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4" data-videoposter="http://themepunch.com/miami/wp-content/uploads/2014/09/video_woman_cover2.jpg" data-videopreload="auto" data-videowebm="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm" data-videowidth="100%" data-x="0" data-y="0" style="z-index: 5;"></div><!-- LAYER NR. 2 -->

        <div class="tp-caption Miami_darkbgbox2 tp-fade fadeout tp-resizeme" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="300" data-splitin="none" data-splitout="none" data-start="300" data-voffset="20" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;"></div><!-- LAYER NR. 3 -->

        <div class="tp-caption miami_title_60px customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          ENJOY THE SHOW
        </div><!-- LAYER NR. 5 -->

        <h3 class="tp-caption glazed-util-color-white customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="60" data-x="center" data-y="center" style="z-index: 9; max-width: auto; max-height: auto; white-space: nowrap;">
          Now Go Explore the Biggest Drupal Theme Ever!
        </h3><!-- LAYER NR. 6 -->
      </li>
</ul>
<div class="tp-bannertimer"></div>  </div>  </div>
      <script type="text/javascript">

        jQuery(document).ready(function() {
          if (jQuery(window).height() > 900) {
            toggleFullScreen = "off";
          }
          else {
            toggleFullScreen = "on";
          }

          jQuery(\'.tp-banner\').show().revolution(
          {
            dottedOverlay:"none",
            delay:20000,
            startwidth:1170,
            startheight:900,
            hideThumbs:200,
            fullScreen: toggleFullScreen,
            fullScreenOffsetContainer: "",
            thumbWidth:100,
            thumbHeight:50,
            thumbAmount:5,

            navigationType:"bullet",
            navigationArrows:"solo",
            navigationStyle:"preview1",

            touchenabled:"on",
            onHoverStop:"off",

            swipe_velocity: 0.7,
            swipe_min_touches: 1,
            swipe_max_touches: 1,
            drag_block_vertical: false,

                        parallax:"mouse",
            parallaxBgFreeze:"on",
            parallaxLevels:[7,4,3,2,5,4,3,2,1,0],

            keyboardNavigation:"off",

            navigationHAlign:"center",
            navigationVAlign:"bottom",
            navigationHOffset:0,
            navigationVOffset:20,
            soloArrowLeftHalign:"left",
            soloArrowLeftValign:"center",
            soloArrowLeftHOffset:20,
            soloArrowLeftVOffset:0,
            soloArrowRightHalign:"right",
            soloArrowRightValign:"center",
            soloArrowRightHOffset:20,
            soloArrowRightVOffset:0,
            shadow:0,
            spinner:"spinner4",

            stopLoop:"off",
            stopAfterLoops:-1,
            stopAtSlide:-1,

            shuffle:"off",

            autoHeight:"off",
            forceFullWidth:"off",



            hideThumbsOnMobile:"off",
            hideNavDelayOnMobile:1500,
            hideBulletsOnMobile:"off",
            hideArrowsOnMobile:"off",
            hideThumbsUnderResolution:0,

            hideSliderAtLimit:0,
            hideCaptionAtLimit:0,
            hideAllCaptionAtLilmit:0,
            startWithSlide:0,
            videoJsPath:"rs-plugin/videojs/",
            fullScreenOffsetContainer: ""
          });




        }); //ready

      </script>
<style>
.tp-caption.Miami_darkbgbox2,.Miami_darkbgbox2 {
  min-width: 600px;
  max-width: 90%;
  min-height:200px;
  text-decoration:none;
  background:#2d3032;
  background-color:rgba(45,48,50,0.15);
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px {
font-size:60px;
line-height:60px;
font-weight:700;
color:#ffffff;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

</style>
<!-- END REVOLUTION SLIDER -->  ',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Respondr Slideshow',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'respondr-slideshow',
    'view_mode' => 'default',
    'created' => 1416512627,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'f0374330-d0b8-4ae9-94e0-780fbceb6573',
    'uuid' => 'd8bc6307-6858-435f-ad53-0098ac9c042d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul class="slides">
	<li><img src="http://i.imgur.com/fTjwm.jpg">
		<div class="text-box">
			<p>Sooperthemes shortcodes allow authors and site builders to create advanced content like [button type="primary" link="#"]Action[/button]&nbsp; [button type="primary"]Buttons[/button]</p>

			<p><a href="content/shortcodes">read more</a></p>
		</div>
	</li>
	<li><img src="http://i.imgur.com/lI4nw.jpg">
		<div class="text-box">
			<p>This slider is touchscreen friendly, press your finger anywhere in the slider to use the 1:1 sliding capability of your device.</p>

			<p><a href="content/features/sliders-swipe-support">read more</a></p>
		</div>
	</li>
	<li><img src="http://i.imgur.com/DUpmP.jpg">
		<div class="text-box">
			<p>Sooperthemes products are great for site builders, thanks to integration with Skinr building mobile friendly layouts is a breeze.</p>

			<p><a href="content/features/site-building-skinr">read more</a></p>
		</div>
	</li>
</ul>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Welcome to Drupal CMS Powerstart',
    'description' => NULL,
    'title' => 'Welcome to Drupal CMS Powerstart',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'welcome-to-drupal-cms-powerstart',
    'view_mode' => 'default',
    'created' => 1413832529,
    'log' => '',
    'uid' => 0,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '814aaf0e-db18-43e4-8fc9-cc1d943e6774',
    'uuid' => 'e35f2bd3-a97c-477b-bd63-583ad0a6b0db',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<p><img alt="Portrait" class="phototron-image" data-license="https://creativecommons.org/licenses/by-sa/2.0/" data-source="https://www.flickr.com/photos/kk/3905212994/in/set-72157622323170600" src="http://i.imgur.com/uaWeCc1.jpg"></p>

<div class="row">
	<div class="col-md-6">
		<p>Explore CMS Kickstart and create engaging Drupal sites with ease. CMS Kickstart works with any theme and comes with handy tools for custom layouts!</p>
	</div>
</div>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Glazed Default Home Slider',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'glazed-default-home-slider',
    'view_mode' => 'default',
    'created' => 1430210463,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'c7292be5-5e68-4cc0-a737-f374c2a4066b',
    'uuid' => 'e417cb11-edca-4a7d-b329-1331656147bf',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<div class="rev_slider_wrapper" id="rev_slider_16_1_wrapper" style="background-color:#333;padding:0px;">
  <!-- START REVOLUTION SLIDER 4.6.5  -->

  <div class="rev_slider fullscreenbanner" id="rev_slider_16_1" style="display:none;">
    <ul>
      <!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" data-lazyload="http://localhost/images/misc/tiled-white70.jpg" data-bgposition="center center" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="110" data-bgpositionend="right center" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <h1 style="font-size:90px;font-weight:700;line-height:1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">
          Glazed
        </h1><!-- LAYER NR. 2 -->

        <h2 style="font-weight: 300; font-size:60px; line-height: 1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="150" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          Biggest Drupal Theme Ever
        </h2><!-- LAYER NR. 3 -->

        <p style="font-size:21px;color:#333;line-height:1.4em;" class="lead tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="180" data-x="center" data-y="center" style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">
          Unique Designs with Full Demo Content!
        </p><!-- LAYER NR. 3 -->

        <div class="tp-caption Miami_nostyle sft fadeout tp-scrollbelowslider" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="300" data-hoffset="0" data-scrolloffset="20" data-speed="600" data-splitin="none" data-splitout="none" data-start="2450" data-voffset="50" data-x="center" data-y="bottom" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="tp-layer-inner-rotation Miami_nostyle rs-slideloop" data-easing="Power3.easeInOut" data-speed="1" data-xe="0" data-xs="0" data-ye="10" data-ys="0" style="">
            <div class="icon-angle-down" style="color: #fff; cursor: pointer; font-size: 45px; font-style: italic; padding: 50px; text-align: center"></div>
          </div>
        </div>
      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" data-lazyload="http://localhost/images/misc/impossible-2560.jpg" data-bgposition="center top" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="right top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-hoffset="0" data-voffset="50" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="stpe-cta stpe-cta--wide glazed-util-overlay-white" style="padding:60px 80px">
  <h3 class="stpe-cta__heading">Discover the Drupal theme that does <em>Everything</em></h3>
  <a class="btn btn-lg stpe-borderbutton glazed-util-color-primary glazed-util-border-primary glazed-util-hover-background-primary glazed-util-hover-color-white glazed-util-hover-border-primary" href="sooperthemes.com/pricing">Purchase Glazed</a>
</div>
        </div><!-- LAYER NR. 3 -->

      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img alt="video_woman_cover2" data-bgfit="cover" data-bgposition="center center" data-bgrepeat="no-repeat" src="http://miamicdn4.themepunch.com/wp-content/uploads/2014/09/video_woman_cover2.jpg"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption tp-fade fadeout fullscreenvideo tp-videolayer" data-aspectratio="16:9" data-autoplay="true" data-autoplayonlyfirsttime="false" data-easing="Power4.easeOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="1500" data-forcecover="1" data-forcerewind="on" data-nextslideatend="true" data-posteronmobile="off" data-speed="1000" data-start="900" data-videocontrols="none" data-videoloop="loopandnoslidestop" data-videomp4="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4" data-videoposter="http://themepunch.com/miami/wp-content/uploads/2014/09/video_woman_cover2.jpg" data-videopreload="auto" data-videowebm="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm" data-videowidth="100%" data-x="0" data-y="0" style="z-index: 5;"></div><!-- LAYER NR. 2 -->

        <div class="tp-caption Miami_darkbgbox2 tp-fade fadeout tp-resizeme" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="300" data-splitin="none" data-splitout="none" data-start="300" data-voffset="20" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;"></div><!-- LAYER NR. 3 -->

        <div class="tp-caption miami_title_60px customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          ENJOY THE SHOW
        </div><!-- LAYER NR. 5 -->

        <h3 class="tp-caption glazed-util-color-white customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="60" data-x="center" data-y="center" style="z-index: 9; max-width: auto; max-height: auto; white-space: nowrap;">
          Now Go Explore the Biggest Drupal Theme Ever!
        </h3><!-- LAYER NR. 6 -->
      </li>
    </ul>

    <div class="tp-bannertimer tp-bottom" style="visibility: hidden !important;"></div>
  </div><script type="text/javascript">

                /******************************************
                    -   PREPARE PLACEHOLDER FOR SLIDER  -
                ******************************************/


                var setREVStartSize = function() {
                    var tpopt = new Object();
                        tpopt.startwidth = 1170;
                        tpopt.startheight = 900;
                        tpopt.container = jQuery(\'#rev_slider_16_1\');
                        // tpopt.fullScreen = "on";
                        tpopt.forceFullWidth="off";
                };

                /* CALL PLACEHOLDER */
                setREVStartSize();
                var tpj=jQuery;
                tpj.noConflict();
                var revapi16;
                tpj(document).ready(function() {
                if(tpj(\'#rev_slider_16_1\').revolution == undefined){
                    revslider_showDoubleJqueryError(\'#rev_slider_16_1\');
                }else{
                   revapi16 = tpj(\'#rev_slider_16_1\').show().revolution(
                    {
                        dottedOverlay:"none",
                        delay:20000,
                        startwidth:1170,
                        startheight:900,
                        hideThumbs:0,

                        thumbWidth:100,
                        thumbHeight:50,
                        thumbAmount:4,
                        simplifyAll:"off",
                        navigationType:"bullet",
                        navigationArrows:"solo",
                        navigationStyle:"preview1",

                        touchenabled:"off",
                        onHoverStop:"on",
                        nextSlideOnWindowFocus:"off",
                        keyboardNavigation:"off",
                        navigationHAlign:"center",
                        navigationVAlign:"bottom",
                        navigationHOffset:0,
                        navigationVOffset:60,
                        soloArrowLeftHalign:"left",
                        soloArrowLeftValign:"center",
                        soloArrowLeftHOffset:40,
                        soloArrowLeftVOffset:0,
                        soloArrowRightHalign:"right",
                        soloArrowRightValign:"center",
                        soloArrowRightHOffset:40,
                        soloArrowRightVOffset:0,
                        shadow:0,
                        fullWidth:"on",
                        fullScreen:"off",
                                                spinner:"spinner2",
                        stopLoop:"off",
                        stopAfterLoops:-1,
                        stopAtSlide:-1,

                        shuffle:"off",
                        forceFullWidth:"off",
                        fullScreenAlignForce:"off",
                        minFullScreenHeight:"",
                        hideTimerBar:"on",
                        hideThumbsOnMobile:"on",
                        hideBulletsOnMobile:"on",
                        hideArrowsOnMobile:"on",
                        hideThumbsUnderResolution:0,
                        fullScreenOffsetContainer: "",
                        fullScreenOffset: "",
                        hideSliderAtLimit:0,
                        hideCaptionAtLimit:481,
                        hideAllCaptionAtLilmit:0,
                        startWithSlide:0                    });


               }
                }); /*ready*/

  </script><style type="text/css">
  #rev_slider_16_1_wrapper .tp-loader.spinner2{ background-color: #FFFFFF !important; }

.largeredbtn {
font-family: "Raleway", sans-serif;
font-weight: 900;
font-size: 16px;
line-height: 60px;
color: #fff !important;
text-decoration: none;
padding-left: 40px;
padding-right: 80px;
padding-top: 22px;
padding-bottom: 22px;
background: rgb(234,91,31); /* Old browsers */
background: -moz-linear-gradient(top,  rgba(234,91,31,1) 0%, rgba(227,58,12,1) 100%); /* FF3.6+ */
background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,rgba(234,91,31,1)), color-stop(100%,rgba(227,58,12,1))); /* Chrome,Safari4+ */
background: -webkit-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* Chrome10+,Safari5.1+ */
background: -o-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* Opera 11.10+ */
background: -ms-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* IE10+ */
background: linear-gradient(to bottom,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* W3C */
filter: progid:DXImageTransform.Microsoft.gradient( startColorstr=\'#ea5b1f\', endColorstr=\'#e33a0c\',GradientType=0 ); /* IE6-9 */
}

.largeredbtn:hover {
background: rgb(227,58,12); /* Old browsers */
background: -moz-linear-gradient(top,  rgba(227,58,12,1) 0%, rgba(234,91,31,1) 100%); /* FF3.6+ */
background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,rgba(227,58,12,1)), color-stop(100%,rgba(234,91,31,1))); /* Chrome,Safari4+ */
background: -webkit-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* Chrome10+,Safari5.1+ */
background: -o-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* Opera 11.10+ */
background: -ms-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* IE10+ */
background: linear-gradient(to bottom,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* W3C */
filter: progid:DXImageTransform.Microsoft.gradient( startColorstr=\'#e33a0c\', endColorstr=\'#ea5b1f\',GradientType=0 ); /* IE6-9 */
}

.fullrounded img {
   -webkit-border-radius: 400px;
-moz-border-radius: 400px;
border-radius: 400px;
  }

.tp-caption a:hover {
color:#296169;
}
.tp-caption a {
color:#296169;
text-shadow:none;
text-decoration:none;
-webkit-transition:all 0.2s ease-out;
-moz-transition:all 0.2s ease-out;
-o-transition:all 0.2s ease-out;
-ms-transition:all 0.2s ease-out;
}

.tp-caption a:hover {
color:#296169;
}
.feature-round {
  color: #000;
  background:#fff;
  background: rgba(255, 255, 255, 0.7);
  font-size: 12px;
  width: 100px;
  height: 100px;
  line-height: 14px;
  text-align: center;
  text-decoration: none;
  box-sizing: border-box;
  padding: 35px 35px 35px 35px;
  background-color: transparent;
  border-radius: 50px 50px 50px 50px;
  border-width: 0px;
  border-color: #000000;
  border-style: none;
}

.tp-caption.miami_title_60px {
color:#ffffff;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_subtitle {
color: #ffffff;
letter-spacing:2px;
text-decoration:none;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.divideline30px {
height:2px;
min-width:30px;
background: #333;
text-decoration:none;
}

.tp-caption.Miami_nostyle {
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.miami_content_light {
font-size:22px;
line-height:28px;
font-weight:400;
font-family:"Source Sans Pro";
color:rgb(255, 255, 255);
text-decoration:none;
letter-spacing:0px;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_title_60px_dark {
font-size:60px;
line-height:60px;
font-weight:700;
font-family:"Source Sans Pro";
color:#333;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_content_dark {
font-size:22px;
line-height:28px;
font-weight:400;
font-family:"Source Sans Pro";
color:#666;
letter-spacing:0px;
text-decoration:none;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.divideline30px_dark {
height:2px;
min-width:30px;
text-decoration:none;
background-color:#333;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.noshadow {
text-shadow:none;
}

.tp-caption.ellipse70px {
min-width:70px;
min-height:70px;
line-height:1px;
text-decoration:none;
cursor:pointer;
background-color:rgb(0, 0, 0);
background-color:rgba(0, 0, 0, 0.14902);
border-radius:50px 50px 50px 50px;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.arrowicon {
line-height:1px;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.noshadow,.noshadow {
  text-shadow:none
}

.tp-caption.miami_subtitle,.miami_subtitle {
  font-size:17px;
  line-height:24px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:rgba(255,255,255,0.65);
  letter-spacing:2px;
  text-decoration:none;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.divideline30px,.divideline30px {
  height:2px;
  min-width:30px;
  background:#fff;
  background-color:#fff;
  text-decoration:none;
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px,.miami_title_60px {
  font-size:60px;
  line-height:60px;
  font-weight:700;
  font-family:"Source Sans Pro";
  color:#fff;
  text-decoration:none;
  text-shadow:none;
  letter-spacing:1px;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.divideline30px_dark,.divideline30px_dark {
  height:2px;
  min-width:30px;
  text-decoration:none;
  background-color:#333;
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px_dark,.miami_title_60px_dark {
  font-size:60px;
  line-height:60px;
  font-weight:700;
  font-family:"Source Sans Pro";
  color:#333;
  text-decoration:none;
  text-shadow:none;
  letter-spacing:1px;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.miami_content_dark,.miami_content_dark {
  font-size:22px;
  line-height:28px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:#666;
  letter-spacing:0;
  text-decoration:none;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.miami_content_light,.miami_content_light {
  font-size:22px;
  line-height:28px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:#fff;
  text-decoration:none;
  letter-spacing:0;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.Miami_nostyle,.Miami_nostyle {
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.Miami_darkbgbox2,.Miami_darkbgbox2 {
  min-width:600px;
  min-height:200px;
  text-decoration:none;
  background:#2d3032;
  background-color:rgba(45,48,50,0.15);
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.slider_greygradient_image,.slider_greygradient_image {
  width:500%;
  height:100%;
  text-decoration:none;
  background:url(http://themepunch.com/miami/wp-content/uploads/2014/10/greygradienttop.png) repeat-x transparent;
  background-color:transparent;
  border-width:0;
  border-color:#222;
  border-style:none
}

  </style>
</div>',
          'format' => 'unfiltered_html',
          'safe_value' => '<div class="rev_slider_wrapper" id="rev_slider_16_1_wrapper" style="background-color:#333;padding:0px;">
  <!-- START REVOLUTION SLIDER 4.6.5  -->

  <div class="rev_slider fullscreenbanner" id="rev_slider_16_1" style="display:none;">
    <ul>
      <!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" data-lazyload="http://localhost/images/misc/tiled-white70.jpg" data-bgposition="center center" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="110" data-bgpositionend="right center" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <h1 style="font-size:90px;font-weight:700;line-height:1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">
          Glazed
        </h1><!-- LAYER NR. 2 -->

        <h2 style="font-weight: 300; font-size:60px; line-height: 1.2em; color: #333;" class="tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="150" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          Biggest Drupal Theme Ever
        </h2><!-- LAYER NR. 3 -->

        <p style="font-size:21px;color:#333;line-height:1.4em;" class="lead tp-caption customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="180" data-x="center" data-y="center" style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">
          Unique Designs with Full Demo Content!
        </p><!-- LAYER NR. 3 -->

        <div class="tp-caption Miami_nostyle sft fadeout tp-scrollbelowslider" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="300" data-hoffset="0" data-scrolloffset="20" data-speed="600" data-splitin="none" data-splitout="none" data-start="2450" data-voffset="50" data-x="center" data-y="bottom" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="tp-layer-inner-rotation Miami_nostyle rs-slideloop" data-easing="Power3.easeInOut" data-speed="1" data-xe="0" data-xs="0" data-ye="10" data-ys="0" style="">
            <div class="icon-angle-down" style="color: #fff; cursor: pointer; font-size: 45px; font-style: italic; padding: 50px; text-align: center"></div>
          </div>
        </div>
      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img  src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" data-lazyload="http://localhost/images/misc/impossible-2560.jpg" data-bgposition="center top" data-bgrepeat="no-repeat" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="105" data-bgpositionend="right top" data-bgfit="cover" data-bgrepeat="no-repeat"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption customin fadeout tp-resizeme" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-hoffset="0" data-voffset="50" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">
          <div class="stpe-cta stpe-cta--wide glazed-util-overlay-white" style="padding:60px 80px">
  <h3 class="stpe-cta__heading">Discover the Drupal theme that does <em>Everything</em></h3>
  <a class="btn btn-lg stpe-borderbutton glazed-util-color-primary glazed-util-border-primary glazed-util-hover-background-primary glazed-util-hover-color-white glazed-util-hover-border-primary" href="sooperthemes.com/pricing">Purchase Glazed</a>
</div>
        </div><!-- LAYER NR. 3 -->

      </li><!-- SLIDE  -->

      <li data-masterspeed="6000" data-saveperformance="off" data-slotamount="7" data-transition="fade">
        <!-- MAIN IMAGE -->
        <img alt="video_woman_cover2" data-bgfit="cover" data-bgposition="center center" data-bgrepeat="no-repeat" src="http://miamicdn4.themepunch.com/wp-content/uploads/2014/09/video_woman_cover2.jpg"> <!-- LAYERS -->
         <!-- LAYER NR. 1 -->

        <div class="tp-caption tp-fade fadeout fullscreenvideo tp-videolayer" data-aspectratio="16:9" data-autoplay="true" data-autoplayonlyfirsttime="false" data-easing="Power4.easeOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-endspeed="1500" data-forcecover="1" data-forcerewind="on" data-nextslideatend="true" data-posteronmobile="off" data-speed="1000" data-start="900" data-videocontrols="none" data-videoloop="loopandnoslidestop" data-videomp4="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4" data-videoposter="http://themepunch.com/miami/wp-content/uploads/2014/09/video_woman_cover2.jpg" data-videopreload="auto" data-videowebm="http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm" data-videowidth="100%" data-x="0" data-y="0" style="z-index: 5;"></div><!-- LAYER NR. 2 -->

        <div class="tp-caption Miami_darkbgbox2 tp-fade fadeout tp-resizeme" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="300" data-splitin="none" data-splitout="none" data-start="300" data-voffset="20" data-x="center" data-y="center" style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;"></div><!-- LAYER NR. 3 -->

        <div class="tp-caption miami_title_60px customin fadeout tp-resizeme" data-customin="x:0;y:-30;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power4.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="0" data-x="center" data-y="center" style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">
          ENJOY THE SHOW
        </div><!-- LAYER NR. 5 -->

        <h3 class="tp-caption glazed-util-color-white customin fadeout tp-resizeme" data-captionhidden="on" data-customin="x:0;y:25;z:0;rotationX:-70;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;" data-easing="Power3.easeInOut" data-elementdelay="0.1" data-endelementdelay="0.1" data-hoffset="0" data-speed="800" data-splitin="none" data-splitout="none" data-start="300" data-voffset="60" data-x="center" data-y="center" style="z-index: 9; max-width: auto; max-height: auto; white-space: nowrap;">
          Now Go Explore the Biggest Drupal Theme Ever!
        </h3><!-- LAYER NR. 6 -->
      </li>
    </ul>

    <div class="tp-bannertimer tp-bottom" style="visibility: hidden !important;"></div>
  </div><script type="text/javascript">

                /******************************************
                    -   PREPARE PLACEHOLDER FOR SLIDER  -
                ******************************************/


                var setREVStartSize = function() {
                    var tpopt = new Object();
                        tpopt.startwidth = 1170;
                        tpopt.startheight = 900;
                        tpopt.container = jQuery(\'#rev_slider_16_1\');
                        // tpopt.fullScreen = "on";
                        tpopt.forceFullWidth="off";
                };

                /* CALL PLACEHOLDER */
                setREVStartSize();
                var tpj=jQuery;
                tpj.noConflict();
                var revapi16;
                tpj(document).ready(function() {
                if(tpj(\'#rev_slider_16_1\').revolution == undefined){
                    revslider_showDoubleJqueryError(\'#rev_slider_16_1\');
                }else{
                   revapi16 = tpj(\'#rev_slider_16_1\').show().revolution(
                    {
                        dottedOverlay:"none",
                        delay:20000,
                        startwidth:1170,
                        startheight:900,
                        hideThumbs:0,

                        thumbWidth:100,
                        thumbHeight:50,
                        thumbAmount:4,
                        simplifyAll:"off",
                        navigationType:"bullet",
                        navigationArrows:"solo",
                        navigationStyle:"preview1",

                        touchenabled:"off",
                        onHoverStop:"on",
                        nextSlideOnWindowFocus:"off",
                        keyboardNavigation:"off",
                        navigationHAlign:"center",
                        navigationVAlign:"bottom",
                        navigationHOffset:0,
                        navigationVOffset:60,
                        soloArrowLeftHalign:"left",
                        soloArrowLeftValign:"center",
                        soloArrowLeftHOffset:40,
                        soloArrowLeftVOffset:0,
                        soloArrowRightHalign:"right",
                        soloArrowRightValign:"center",
                        soloArrowRightHOffset:40,
                        soloArrowRightVOffset:0,
                        shadow:0,
                        fullWidth:"on",
                        fullScreen:"off",
                                                spinner:"spinner2",
                        stopLoop:"off",
                        stopAfterLoops:-1,
                        stopAtSlide:-1,

                        shuffle:"off",
                        forceFullWidth:"off",
                        fullScreenAlignForce:"off",
                        minFullScreenHeight:"",
                        hideTimerBar:"on",
                        hideThumbsOnMobile:"on",
                        hideBulletsOnMobile:"on",
                        hideArrowsOnMobile:"on",
                        hideThumbsUnderResolution:0,
                        fullScreenOffsetContainer: "",
                        fullScreenOffset: "",
                        hideSliderAtLimit:0,
                        hideCaptionAtLimit:481,
                        hideAllCaptionAtLilmit:0,
                        startWithSlide:0                    });


               }
                }); /*ready*/

  </script><style type="text/css">
  #rev_slider_16_1_wrapper .tp-loader.spinner2{ background-color: #FFFFFF !important; }

.largeredbtn {
font-family: "Raleway", sans-serif;
font-weight: 900;
font-size: 16px;
line-height: 60px;
color: #fff !important;
text-decoration: none;
padding-left: 40px;
padding-right: 80px;
padding-top: 22px;
padding-bottom: 22px;
background: rgb(234,91,31); /* Old browsers */
background: -moz-linear-gradient(top,  rgba(234,91,31,1) 0%, rgba(227,58,12,1) 100%); /* FF3.6+ */
background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,rgba(234,91,31,1)), color-stop(100%,rgba(227,58,12,1))); /* Chrome,Safari4+ */
background: -webkit-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* Chrome10+,Safari5.1+ */
background: -o-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* Opera 11.10+ */
background: -ms-linear-gradient(top,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* IE10+ */
background: linear-gradient(to bottom,  rgba(234,91,31,1) 0%,rgba(227,58,12,1) 100%); /* W3C */
filter: progid:DXImageTransform.Microsoft.gradient( startColorstr=\'#ea5b1f\', endColorstr=\'#e33a0c\',GradientType=0 ); /* IE6-9 */
}

.largeredbtn:hover {
background: rgb(227,58,12); /* Old browsers */
background: -moz-linear-gradient(top,  rgba(227,58,12,1) 0%, rgba(234,91,31,1) 100%); /* FF3.6+ */
background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,rgba(227,58,12,1)), color-stop(100%,rgba(234,91,31,1))); /* Chrome,Safari4+ */
background: -webkit-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* Chrome10+,Safari5.1+ */
background: -o-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* Opera 11.10+ */
background: -ms-linear-gradient(top,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* IE10+ */
background: linear-gradient(to bottom,  rgba(227,58,12,1) 0%,rgba(234,91,31,1) 100%); /* W3C */
filter: progid:DXImageTransform.Microsoft.gradient( startColorstr=\'#e33a0c\', endColorstr=\'#ea5b1f\',GradientType=0 ); /* IE6-9 */
}

.fullrounded img {
   -webkit-border-radius: 400px;
-moz-border-radius: 400px;
border-radius: 400px;
  }

.tp-caption a:hover {
color:#296169;
}
.tp-caption a {
color:#296169;
text-shadow:none;
text-decoration:none;
-webkit-transition:all 0.2s ease-out;
-moz-transition:all 0.2s ease-out;
-o-transition:all 0.2s ease-out;
-ms-transition:all 0.2s ease-out;
}

.tp-caption a:hover {
color:#296169;
}
.feature-round {
  color: #000;
  background:#fff;
  background: rgba(255, 255, 255, 0.7);
  font-size: 12px;
  width: 100px;
  height: 100px;
  line-height: 14px;
  text-align: center;
  text-decoration: none;
  box-sizing: border-box;
  padding: 35px 35px 35px 35px;
  background-color: transparent;
  border-radius: 50px 50px 50px 50px;
  border-width: 0px;
  border-color: #000000;
  border-style: none;
}

.tp-caption.miami_title_60px {
color:#ffffff;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_subtitle {
color: #ffffff;
letter-spacing:2px;
text-decoration:none;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.divideline30px {
height:2px;
min-width:30px;
background: #333;
text-decoration:none;
}

.tp-caption.Miami_nostyle {
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.miami_content_light {
font-size:22px;
line-height:28px;
font-weight:400;
font-family:"Source Sans Pro";
color:rgb(255, 255, 255);
text-decoration:none;
letter-spacing:0px;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_title_60px_dark {
font-size:60px;
line-height:60px;
font-weight:700;
font-family:"Source Sans Pro";
color:#333;
text-decoration:none;
text-shadow:none;
letter-spacing:1px;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.miami_content_dark {
font-size:22px;
line-height:28px;
font-weight:400;
font-family:"Source Sans Pro";
color:#666;
letter-spacing:0px;
text-decoration:none;
text-shadow:none;
background-color:transparent;
border-width:0px;
border-color:rgb(0, 0, 0);
border-style:none;
}

.tp-caption.divideline30px_dark {
height:2px;
min-width:30px;
text-decoration:none;
background-color:#333;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.noshadow {
text-shadow:none;
}

.tp-caption.ellipse70px {
min-width:70px;
min-height:70px;
line-height:1px;
text-decoration:none;
cursor:pointer;
background-color:rgb(0, 0, 0);
background-color:rgba(0, 0, 0, 0.14902);
border-radius:50px 50px 50px 50px;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.arrowicon {
line-height:1px;
border-width:0px;
border-color:rgb(34, 34, 34);
border-style:none;
}

.tp-caption.noshadow,.noshadow {
  text-shadow:none
}

.tp-caption.miami_subtitle,.miami_subtitle {
  font-size:17px;
  line-height:24px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:rgba(255,255,255,0.65);
  letter-spacing:2px;
  text-decoration:none;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.divideline30px,.divideline30px {
  height:2px;
  min-width:30px;
  background:#fff;
  background-color:#fff;
  text-decoration:none;
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px,.miami_title_60px {
  font-size:60px;
  line-height:60px;
  font-weight:700;
  font-family:"Source Sans Pro";
  color:#fff;
  text-decoration:none;
  text-shadow:none;
  letter-spacing:1px;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.divideline30px_dark,.divideline30px_dark {
  height:2px;
  min-width:30px;
  text-decoration:none;
  background-color:#333;
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.miami_title_60px_dark,.miami_title_60px_dark {
  font-size:60px;
  line-height:60px;
  font-weight:700;
  font-family:"Source Sans Pro";
  color:#333;
  text-decoration:none;
  text-shadow:none;
  letter-spacing:1px;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.miami_content_dark,.miami_content_dark {
  font-size:22px;
  line-height:28px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:#666;
  letter-spacing:0;
  text-decoration:none;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.miami_content_light,.miami_content_light {
  font-size:22px;
  line-height:28px;
  font-weight:400;
  font-family:"Source Sans Pro";
  color:#fff;
  text-decoration:none;
  letter-spacing:0;
  text-shadow:none;
  background-color:transparent;
  border-width:0;
  border-color:#000;
  border-style:none
}

.tp-caption.Miami_nostyle,.Miami_nostyle {
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.Miami_darkbgbox2,.Miami_darkbgbox2 {
  min-width:600px;
  min-height:200px;
  text-decoration:none;
  background:#2d3032;
  background-color:rgba(45,48,50,0.15);
  border-width:0;
  border-color:#222;
  border-style:none
}

.tp-caption.slider_greygradient_image,.slider_greygradient_image {
  width:500%;
  height:100%;
  text-decoration:none;
  background:url(http://themepunch.com/miami/wp-content/uploads/2014/10/greygradienttop.png) repeat-x transparent;
  background-color:transparent;
  border-width:0;
  border-color:#222;
  border-style:none
}

  </style>
</div>',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'write for us',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'write-for-us',
    'view_mode' => 'default',
    'created' => 1416826239,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => 'bade93a8-bcc4-446e-8777-276addf64fbe',
    'uuid' => 'f2c07164-6d6b-4991-bbd0-2b392148bb95',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<h3>Write for us and get paid!</h3>

<p>If you\'re interested in writing a tutorial, check out the writing guidelines for more info. We accept written articles as well as screencasts. We pay competitive money for competitive content!</p>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'Deviant Slider',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'deviant-slider',
    'view_mode' => 'default',
    'created' => 1416747902,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '4e973071-0e00-4792-888f-385c94ab9c8f',
    'uuid' => 'fc2b4008-4396-4577-a602-1ecdae742398',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '<ul class="slide">
	<li>
		<div style="width: 427px; max-width: 100%; margin: 0 auto;"><img alt="Drupal 7 banner" src="http://i.imgur.com/NbiwIU1.png">
			<div class="button-big" style="float: right;"><a href="node/32">Read more</a></div>
		</div>
	</li>
	<li>
		<h1>Lorem Ipsum Dolor Sit Amet</h1>

		<h2>Consectetur adispiscin. Suspendisse vel nunce</h2>
		<a href="contact">contact us now</a></li>
</ul>
',
          'format' => 'wysiwyg_full',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  $beans[] = array(
    'label' => 'FullWidth Slider',
    'description' => NULL,
    'title' => '',
    'type' => 'content_block',
    'data' => array(
      'view_mode' => 'default',
    ),
    'delta' => 'fullwidth-slider',
    'view_mode' => 'default',
    'created' => 1413830168,
    'log' => '',
    'uid' => 1,
    'default_revision' => 1,
    'revisions' => array(),
    'vuuid' => '9d650d2d-2563-4c3e-a052-ff64c8461a43',
    'uuid' => 'fcb78a8a-4ef3-4fd1-b6ba-888968b5864d',
    'field_block_content' => array(
      'und' => array(
        0 => array(
          'value' => '	<!--
	#################################
		- THEMEPUNCH BANNER -
	#################################
	-->
              <div class="tp-banner-container">
		<div class="tp-banner" >
			<ul>

	<!-- SLIDE  -->
	<li data-transition="fade" data-slotamount="5" data-masterspeed="500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/06/homeslider_thumb1.jpg"  data-saveperformance="on"  data-title="Glazed Theme Freedom">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png"  alt="slidebg1" data-lazyload="http://i.imgur.com/e1W94yJ.jpg" data-bgposition="center center"  data-kenburns="on" data-duration="32000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="110" data-bgpositionend="right center" data-bgfit="cover" data-bgrepeat="no-repeat">
		<!-- LAYERS -->


		<!-- LAYER NR. 9 -->
		<div class="tp-caption medium_bg_orange skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="230"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="500"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"a
			data-captionhidden="on"
			style="z-index: 10">Revolution Slider Included Free!
		</div>

		<!-- LAYER NR. 9 -->
		<div class="tp-caption medium_bg_orange skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="276"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="700"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 10">Save $18 USD!
		</div>

		<!-- LAYER NR. 9 -->
		<div class="tp-caption tp-glazed skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="360"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1500"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 10">The Best Multi-Purpose Theme Ever!
		</div>

		<!-- LAYER NR. 10 -->
		<div class="tp-caption tp-glazed skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="435"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1700"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 11">Total Design Freedom
		</div>

		<!-- LAYER NR. 10 -->
		<div class="tp-caption black_thin_whitebg_30 skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="509"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1900"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 11">Amazing Drupal Site Building Tools
		</div>

		<!-- LAYER NR. 11 -->
		<div class="tp-caption customin tp-resizeme"
			 data-x="0"
			 data-y="600"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="2500"
			data-easing="Power3.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			 data-linktoslide="next"

			style="z-index: 12; max-width: auto; max-height: auto; white-space: nowrap;"><a href=\'#\' class=\'btn btn-lg btn-primary\'>Continue the Glazed tour <img src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" alt="" data-ww="18" data-hh="11" data-lazyload="http://themepunch.com/revolution/wp-content/uploads/2014/05/doublearrow2.png"></a>
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="slideup" data-slotamount="1" data-masterspeed="1500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns4-320x200.jpg" data-delay="15000"  data-saveperformance="off"  data-title="Mountain Freedom">
		<!-- MAIN IMAGE -->
		<img src="http://i.imgur.com/Iq1RVy7.jpg"  alt="kenburns4"  data-bgposition="left center" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="150" data-bgfitend="100" data-bgpositionend="right center">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption fullgradient_overlay tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="1000"
			data-start="100"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="500"
			data-endeasing="Power1.easeOut"
			style="z-index: 2; max-width: auto; max-height: auto; white-space: nowrap;">
		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption white_heavy_70 tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="500"
			data-start="500"
			data-easing="Power4.easeOut"
			data-splitin="chars"
			data-splitout="chars"
			data-elementdelay="0.05"
			data-endelementdelay="0.05"
			data-endspeed="300"
			data-endeasing="Power1.easeOut"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">Next Gen Drupal
		</div>

		<!-- LAYER NR. 3 -->
		<div class="tp-caption whiteline_long customin fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="50"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="500"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="600"
			data-endeasing="Power1.easeOut"
			style="z-index: 4; max-width: auto; max-height: auto; white-space: nowrap;">
		</div>

		<!-- LAYER NR. 4 -->
		<div class="tp-caption light_medium_20 tp-fade fadeout col-sm-9 col-md-7 col-lg-5"
			data-x="center" data-hoffset="50"
			data-y="center" data-voffset="120"
			data-speed="600"
			data-start="800"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="600"
			data-endeasing="Power1.easeOut"
			style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">“Glazed by SooperThemes is the first Drupal theme with features that meet the needs for site builders and marketers in the multi-device age.” <br/>
― Jur
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="slidehorizontal" data-slotamount="1" data-masterspeed="1000" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover3-320x200.jpg"  data-fstransition="fade" data-fsmasterspeed="1000" data-fsslotamount="7" data-saveperformance="off"  data-title="Free on the Beach">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover3.jpg"  alt="video_woman_cover3"  data-bgposition="center center" data-bgfit="cover" data-bgrepeat="no-repeat">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption tp-fade fadeout fullscreenvideo"
			data-x="0"
			data-y="0"
			data-speed="1000"
			data-start="1100"
			data-easing="Power4.easeOut"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1500"
			data-endeasing="Power4.easeIn"
			data-autoplay="true"
			data-autoplayonlyfirsttime="false"
			data-nextslideatend="true"
 data-volume="mute" data-forceCover="1" data-aspectratio="16:9" data-forcerewind="on"			style="z-index: 2;"><video class="" preload="none" width="100%" height="100%"
poster=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover2.jpg\'>
<source src=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4\' type=\'video/mp4\' />
<source src=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm\' type=\'video/webm\' />

</video>

		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="360"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2100"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">Ready to create Magnificent
		</div>

		<!-- LAYER NR. 3 -->
		<div class="tp-caption black_heavy_60 customin ltl tp-resizeme"
			data-x="739"
			data-y="278"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="1500"
			data-start="1100"
			data-easing="Power4.easeOut"
			data-splitin="chars"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 4; max-width: auto; max-height: auto; white-space: nowrap;">HTML5
		</div>

		<!-- LAYER NR. 4 -->
		<div class="tp-caption randomrotate ltl"
			data-x="961"
			data-y="291"
			data-speed="1500"
			data-start="1600"
			data-easing="Power4.easeOut"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 5;"><img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/redbg.png" alt="">
		</div>

		<!-- LAYER NR. 5 -->
		<div class="tp-caption light_heavy_34 customin ltl tp-resizeme"
			data-x="970"
			data-y="296"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="1500"
			data-start="1800"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">Video
		</div>

		<!-- LAYER NR. 6 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="405"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2400"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">Video Sliders that will engage
		</div>

		<!-- LAYER NR. 7 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="450"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2700"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">your Website\'s audience.
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="fade" data-slotamount="1" data-masterspeed="1500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns1-320x200.jpg" data-delay="13000"  data-saveperformance="off"  data-title="Our Workplace">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns1.jpg"  alt="kenburns1"  data-bgposition="left center" data-kenburns="on" data-duration="14000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="130" data-bgpositionend="right center">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption reddishbg_heavy_70 lfb fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="600"
			data-start="500"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.05"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power1.easeOut"
			style="z-index: 2; max-width: auto; max-height: auto; white-space: nowrap;">KEN BURNS SLIDES
		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption borderbox_725x130 tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="600"
			data-start="800"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power1.easeOut"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">

		</div>
	</li>

</ul>
<div class="tp-bannertimer"></div>	</div>  </div>
			<script type="text/javascript">

				jQuery(document).ready(function() {



					jQuery(\'.tp-banner\').show().revolution(
					{
						dottedOverlay:"none",
						delay:60000,
						startwidth:1170,
						startheight:700,
						hideThumbs:200,

						thumbWidth:100,
						thumbHeight:50,
						thumbAmount:5,

						navigationType:"bullet",
						navigationArrows:"solo",
						navigationStyle:"preview1",

						touchenabled:"on",
						onHoverStop:"on",

						swipe_velocity: 0.7,
						swipe_min_touches: 1,
						swipe_max_touches: 1,
						drag_block_vertical: false,

												parallax:"mouse",
						parallaxBgFreeze:"on",
						parallaxLevels:[7,4,3,2,5,4,3,2,1,0],

						keyboardNavigation:"off",

						navigationHAlign:"center",
						navigationVAlign:"bottom",
						navigationHOffset:0,
						navigationVOffset:20,

						soloArrowLeftHalign:"left",
						soloArrowLeftValign:"center",
						soloArrowLeftHOffset:20,
						soloArrowLeftVOffset:0,

						soloArrowRightHalign:"right",
						soloArrowRightValign:"center",
						soloArrowRightHOffset:20,
						soloArrowRightVOffset:0,

						shadow:0,
						fullWidth:"on",
						fullScreen:"off",

						spinner:"spinner4",

						stopLoop:"off",
						stopAfterLoops:-1,
						stopAtSlide:-1,

						shuffle:"off",

						autoHeight:"off",
						forceFullWidth:"off",



						hideThumbsOnMobile:"off",
						hideNavDelayOnMobile:1500,
						hideBulletsOnMobile:"off",
						hideArrowsOnMobile:"off",
						hideThumbsUnderResolution:0,

						hideSliderAtLimit:0,
						hideCaptionAtLimit:0,
						hideAllCaptionAtLilmit:0,
						startWithSlide:0,
						videoJsPath:"rs-plugin/videojs/",
						fullScreenOffsetContainer: ""
					});




				});	//ready

			</script>

<!-- END REVOLUTION SLIDER -->	',
          'format' => 'unfiltered_html',
          'safe_value' => '	<!--
	#################################
		- THEMEPUNCH BANNER -
	#################################
	-->
              <div class="tp-banner-container">
		<div class="tp-banner" >
			<ul>

	<!-- SLIDE  -->
	<li data-transition="fade" data-slotamount="5" data-masterspeed="500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/06/homeslider_thumb1.jpg"  data-saveperformance="on"  data-title="Glazed Theme Freedom">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png"  alt="slidebg1" data-lazyload="http://i.imgur.com/e1W94yJ.jpg" data-bgposition="center center"  data-kenburns="on" data-duration="32000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="110" data-bgpositionend="right center" data-bgfit="cover" data-bgrepeat="no-repeat">
		<!-- LAYERS -->


		<!-- LAYER NR. 9 -->
		<div class="tp-caption medium_bg_orange skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="230"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="500"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"a
			data-captionhidden="on"
			style="z-index: 10">Revolution Slider Included Free!
		</div>

		<!-- LAYER NR. 9 -->
		<div class="tp-caption medium_bg_orange skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="276"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="700"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 10">Save $18 USD!
		</div>

		<!-- LAYER NR. 9 -->
		<div class="tp-caption tp-glazed skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="360"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1500"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 10">The Best Multi-Purpose Theme Ever!
		</div>

		<!-- LAYER NR. 10 -->
		<div class="tp-caption tp-glazed skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="435"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1700"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 11">Total Design Freedom
		</div>

		<!-- LAYER NR. 10 -->
		<div class="tp-caption black_thin_whitebg_30 skewfromleftshort customout"
			data-x="left" data-hoffset="0"
			data-y="509"
			data-customout="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0.75;scaleY:0.75;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="1900"
			data-easing="Back.easeOut"
			data-endspeed="500"
			data-endeasing="Power4.easeIn"
			data-captionhidden="on"
			style="z-index: 11">Amazing Drupal Site Building Tools
		</div>

		<!-- LAYER NR. 11 -->
		<div class="tp-caption customin tp-resizeme"
			 data-x="0"
			 data-y="600"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="2500"
			data-easing="Power3.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			 data-linktoslide="next"

			style="z-index: 12; max-width: auto; max-height: auto; white-space: nowrap;"><a href=\'#\' class=\'btn btn-lg btn-primary\'>Continue the Glazed tour <img src="http://themepunch.com/revolution/wp-content/plugins/revslider/images/dummy.png" alt="" data-ww="18" data-hh="11" data-lazyload="http://themepunch.com/revolution/wp-content/uploads/2014/05/doublearrow2.png"></a>
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="slideup" data-slotamount="1" data-masterspeed="1500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns4-320x200.jpg" data-delay="15000"  data-saveperformance="off"  data-title="Mountain Freedom">
		<!-- MAIN IMAGE -->
		<img src="http://i.imgur.com/Iq1RVy7.jpg"  alt="kenburns4"  data-bgposition="left center" data-kenburns="on" data-duration="16000" data-ease="Linear.easeNone" data-bgfit="150" data-bgfitend="100" data-bgpositionend="right center">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption fullgradient_overlay tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="1000"
			data-start="100"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="500"
			data-endeasing="Power1.easeOut"
			style="z-index: 2; max-width: auto; max-height: auto; white-space: nowrap;">
		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption white_heavy_70 tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="500"
			data-start="500"
			data-easing="Power4.easeOut"
			data-splitin="chars"
			data-splitout="chars"
			data-elementdelay="0.05"
			data-endelementdelay="0.05"
			data-endspeed="300"
			data-endeasing="Power1.easeOut"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">Next Gen Drupal
		</div>

		<!-- LAYER NR. 3 -->
		<div class="tp-caption whiteline_long customin fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="50"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="500"
			data-start="500"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="600"
			data-endeasing="Power1.easeOut"
			style="z-index: 4; max-width: auto; max-height: auto; white-space: nowrap;">
		</div>

		<!-- LAYER NR. 4 -->
		<div class="tp-caption light_medium_20 tp-fade fadeout col-sm-9 col-md-7 col-lg-5"
			data-x="center" data-hoffset="50"
			data-y="center" data-voffset="120"
			data-speed="600"
			data-start="800"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="600"
			data-endeasing="Power1.easeOut"
			style="z-index: 5; max-width: auto; max-height: auto; white-space: nowrap;">“Glazed by SooperThemes is the first Drupal theme with features that meet the needs for site builders and marketers in the multi-device age.” <br/>
― Jur
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="slidehorizontal" data-slotamount="1" data-masterspeed="1000" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover3-320x200.jpg"  data-fstransition="fade" data-fsmasterspeed="1000" data-fsslotamount="7" data-saveperformance="off"  data-title="Free on the Beach">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover3.jpg"  alt="video_woman_cover3"  data-bgposition="center center" data-bgfit="cover" data-bgrepeat="no-repeat">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption tp-fade fadeout fullscreenvideo"
			data-x="0"
			data-y="0"
			data-speed="1000"
			data-start="1100"
			data-easing="Power4.easeOut"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1500"
			data-endeasing="Power4.easeIn"
			data-autoplay="true"
			data-autoplayonlyfirsttime="false"
			data-nextslideatend="true"
 data-volume="mute" data-forceCover="1" data-aspectratio="16:9" data-forcerewind="on"			style="z-index: 2;"><video class="" preload="none" width="100%" height="100%"
poster=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/video_woman_cover2.jpg\'>
<source src=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.mp4\' type=\'video/mp4\' />
<source src=\'http://themepunch.com/revolution/wp-content/uploads/2014/05/woman_sun.webm\' type=\'video/webm\' />

</video>

		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="360"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2100"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">Ready to create Magnificent
		</div>

		<!-- LAYER NR. 3 -->
		<div class="tp-caption black_heavy_60 customin ltl tp-resizeme"
			data-x="739"
			data-y="278"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="1500"
			data-start="1100"
			data-easing="Power4.easeOut"
			data-splitin="chars"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 4; max-width: auto; max-height: auto; white-space: nowrap;">HTML5
		</div>

		<!-- LAYER NR. 4 -->
		<div class="tp-caption randomrotate ltl"
			data-x="961"
			data-y="291"
			data-speed="1500"
			data-start="1600"
			data-easing="Power4.easeOut"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 5;"><img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/redbg.png" alt="">
		</div>

		<!-- LAYER NR. 5 -->
		<div class="tp-caption light_heavy_34 customin ltl tp-resizeme"
			data-x="970"
			data-y="296"
			data-customin="x:0;y:0;z:0;rotationX:0;rotationY:0;rotationZ:0;scaleX:0;scaleY:0;skewX:0;skewY:0;opacity:0;transformPerspective:600;transformOrigin:50% 50%;"
			data-speed="1500"
			data-start="1800"
			data-easing="Power4.easeOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 6; max-width: auto; max-height: auto; white-space: nowrap;">Video
		</div>

		<!-- LAYER NR. 6 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="405"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2400"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 7; max-width: auto; max-height: auto; white-space: nowrap;">Video Sliders that will engage
		</div>

		<!-- LAYER NR. 7 -->
		<div class="tp-caption black_thin_whitebg_30 customin ltl tp-resizeme"
			data-x="730"
			data-y="450"
			data-customin="x:0;y:0;z:0;rotationX:90;rotationY:0;rotationZ:0;scaleX:1;scaleY:1;skewX:0;skewY:0;opacity:0;transformPerspective:200;transformOrigin:50% 0%;"
			data-speed="1500"
			data-start="2700"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.01"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power4.easeIn"
			style="z-index: 8; max-width: auto; max-height: auto; white-space: nowrap;">your Website\'s audience.
		</div>
	</li>
	<!-- SLIDE  -->
	<li data-transition="fade" data-slotamount="1" data-masterspeed="1500" data-thumb="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns1-320x200.jpg" data-delay="13000"  data-saveperformance="off"  data-title="Our Workplace">
		<!-- MAIN IMAGE -->
		<img src="http://themepunch.com/revolution/wp-content/uploads/2014/05/kenburns1.jpg"  alt="kenburns1"  data-bgposition="left center" data-kenburns="on" data-duration="14000" data-ease="Linear.easeNone" data-bgfit="100" data-bgfitend="130" data-bgpositionend="right center">
		<!-- LAYERS -->

		<!-- LAYER NR. 1 -->
		<div class="tp-caption reddishbg_heavy_70 lfb fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="600"
			data-start="500"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.05"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power1.easeOut"
			style="z-index: 2; max-width: auto; max-height: auto; white-space: nowrap;">KEN BURNS SLIDES
		</div>

		<!-- LAYER NR. 2 -->
		<div class="tp-caption borderbox_725x130 tp-fade fadeout tp-resizeme"
			data-x="center" data-hoffset="0"
			data-y="center" data-voffset="0"
			data-speed="600"
			data-start="800"
			data-easing="Power4.easeInOut"
			data-splitin="none"
			data-splitout="none"
			data-elementdelay="0.1"
			data-endelementdelay="0.1"
			data-endspeed="1000"
			data-endeasing="Power1.easeOut"
			style="z-index: 3; max-width: auto; max-height: auto; white-space: nowrap;">

		</div>
	</li>

</ul>
<div class="tp-bannertimer"></div>	</div>  </div>
			<script type="text/javascript">

				jQuery(document).ready(function() {



					jQuery(\'.tp-banner\').show().revolution(
					{
						dottedOverlay:"none",
						delay:60000,
						startwidth:1170,
						startheight:700,
						hideThumbs:200,

						thumbWidth:100,
						thumbHeight:50,
						thumbAmount:5,

						navigationType:"bullet",
						navigationArrows:"solo",
						navigationStyle:"preview1",

						touchenabled:"on",
						onHoverStop:"on",

						swipe_velocity: 0.7,
						swipe_min_touches: 1,
						swipe_max_touches: 1,
						drag_block_vertical: false,

												parallax:"mouse",
						parallaxBgFreeze:"on",
						parallaxLevels:[7,4,3,2,5,4,3,2,1,0],

						keyboardNavigation:"off",

						navigationHAlign:"center",
						navigationVAlign:"bottom",
						navigationHOffset:0,
						navigationVOffset:20,

						soloArrowLeftHalign:"left",
						soloArrowLeftValign:"center",
						soloArrowLeftHOffset:20,
						soloArrowLeftVOffset:0,

						soloArrowRightHalign:"right",
						soloArrowRightValign:"center",
						soloArrowRightHOffset:20,
						soloArrowRightVOffset:0,

						shadow:0,
						fullWidth:"on",
						fullScreen:"off",

						spinner:"spinner4",

						stopLoop:"off",
						stopAfterLoops:-1,
						stopAtSlide:-1,

						shuffle:"off",

						autoHeight:"off",
						forceFullWidth:"off",



						hideThumbsOnMobile:"off",
						hideNavDelayOnMobile:1500,
						hideBulletsOnMobile:"off",
						hideArrowsOnMobile:"off",
						hideThumbsUnderResolution:0,

						hideSliderAtLimit:0,
						hideCaptionAtLimit:0,
						hideAllCaptionAtLilmit:0,
						startWithSlide:0,
						videoJsPath:"rs-plugin/videojs/",
						fullScreenOffsetContainer: ""
					});




				});	//ready

			</script>

<!-- END REVOLUTION SLIDER -->	',
        ),
      ),
    ),
    'field_page_attachments' => array(),
    'url_alias' => array(),
  );
  return $beans;
}
