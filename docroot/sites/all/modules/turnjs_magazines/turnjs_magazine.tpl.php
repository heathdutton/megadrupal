<?php
/**
 * @file 
 */
?>

<div id="turnjs-magazine-loading-firstpage" style="width: 50%; margin: 0 auto; opacity: 0.2;">
  <!-- Dynamic preloading image (first page) -->
</div>
<div id="turnjs-magazine-container" style="opacity: 0;">
  <div class="zoom-icon zoom-icon-in"></div>
  <div class="magazine-viewport">
    <div class="magazine-wrapper">
      <div class="magazine">
        <?php
        if (!empty($res_images)) {
          $images_count = count($res_images);
          $page = 0;
          foreach ($res_images as $idx => $res_image) {
            $page++;
            if (!empty($res_image['#item'])) {
              $full_image_uri = $res_image['#item']['uri'];
              $full_image_path = file_create_url($full_image_uri);
              $image_array = array('path' => $full_image_path);
              if (isset($res_image['alt'])) {
                $image_array['alt'] = $res_image['alt'];
              }
              if (isset($res_image['alt'])) {
                $image_array['title'] = $res_image['title'];
              }
              if (isset($res_image['height'])) {
                $image_array['height'] = $res_image['height'];
              }
              if (isset($res_image['width'])) {
                $image_array['width'] = $res_image['width'];
              }
              $image = theme_image($image_array);
              ?>
              <div class="p<?php echo $page; ?> sheet"><?php echo $image ?></div>
              <?php
            }
          }
        }
        ?>
        <div ignore="1" class="next-button"></div>
        <div ignore="1" class="previous-button"></div>
      </div>
    </div>
  </div>

  <!-- Thumbnails -->
  <div class="magazine-thumbnails">
    <div>
      <?php
      $items = array();
      if (!empty($res_images)) {
        $images_count = count($res_images);
        $current_page = 0;
        $image_group = array();
        foreach ($res_images as $idx => $thumbnail) {
          // Pages start with 1.
          $current_page++;
          // Add page number to image classes.
          $thumbnail['#item']['attributes']['class'] = 'page-' . $current_page;
          $image_group[] = render($thumbnail);
          $pages_group[] = $current_page;
          if (count($image_group) > 1 || $current_page == 1 || $current_page == $images_count) {
            $pagecounter = '<span>' . (implode(' &amp; ', $pages_group)) . ' / ' . ($images_count) . '</span>';
            $items[] = array(implode('', $image_group) . $pagecounter);
            // Reset group.
            $image_group = array();
            // Reset pages.
            $pages_group = array();
          }
        }
      }

      $item_list = array(
        'items' => $items,
        'title' => '',
        'type' => 'ul',
        'attributes' => array()
      );
      echo theme_item_list($item_list);
      ?>
      <div>
      </div>
    </div>
  </div>
</div>