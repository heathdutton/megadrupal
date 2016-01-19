<?php
/**
 * @file
 * Theme the sildeshow threesixty field.
 *
 * Available variables:
 * - $image (array): An array containing multiple image.
 * - $settings (array): An array containing field display settings.
 * - - $settings['width'] , $settings['height'], $settings['navigation'].
 * - $id an incremental id in case if it renders more than once on the page.
 */

$arrayclass = array(
  "threesixty_images",
  "threesixty_images-" . $id,
  "width-" . $settings['width'],
  "height-" . $settings['height'],
  "navigation-" . $settings['navigation'],
);
?>
<?php if (isset($image)): ?>
  <div id='container-<?php print $id ?>' class="container">
    <div class="threesixty product-<?php print $id ?> ">
      <div class="spinner spinner-<?php print $id ?> ">
        <span>0%</span>
      </div>
      <ol class="<?php print implode(" ", $arrayclass)?>">
      <?php
        foreach($image as $pic):
          print '<li>' . $pic['data'] . '</li>';
        endforeach;
      ?>
      </ol>
    </div>
  </div>
<?php endif;?>
