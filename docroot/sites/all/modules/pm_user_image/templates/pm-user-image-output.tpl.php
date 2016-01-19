<?php
/**
 * @file
 * Returns HTML for a pm_autocomplete_output.
 *
 * @Variables
 *   $display_string : string text to be passed by user
 *   $user_object  : User Object
 */
?>
<?php
global $base_url;
$user_data = $user_object;
$external_image = variable_get('pm_user_image_default_external', '');
$internal_image = variable_get('pm_user_image_default_internal', '');
$image_style = variable_get('pm_user_image_image_style', 'thumbnail');
?>
<?php if (isset($user_data->picture) && !empty($user_data->picture)) : ?>
  <div class='pm-user-image'>
    <?php
    print theme('image_style', array(
                'style_name' => $image_style,
                'path' => $user_data->picture->uri,
                'title' => t('user-image'),
                'getsize' => TRUE,
                'attributes' => array(
                    'class' => 'image-pm-user-image',
                    'width' => check_plain(variable_get('pm_user_image_width', 50)),
                    'height' => check_plain(variable_get('pm_user_image_height', 50)),
                ),
                    )
            );
    ?>
    <span class='user-pm-user-image'>
      <?php print $display_string; ?>
    </span>
  </div>
<?php else : ?>
  <div class='pm-user-image'>
    <?php if (variable_get('pm_user_image_path_type', 2) == 1) : ?>
      <?php if (!empty($external_image)) : ?>
        <?php
        print theme('image', array(
                    'path' => $external_image,
                    'height' => check_plain(variable_get('pm_user_image_height', 50)),
                    'width' => check_plain(variable_get('pm_user_image_width', 50)),
                    'attributes' => array('class' => 'image-pm-user-image'),
                    'title' => t('user-image'),
                ));
        ?>
      <?php endif; ?>
    <?php else : ?>
      <?php if (!empty($internal_image)) : ?>
        <?php
        print theme('image_style', array(
                'style_name' => $image_style,
                'path' => file_load($internal_image)->uri,
                'title' => t('user-image'),
                'getsize' => TRUE,
                'attributes' => array(
                    'class' => 'image-pm-user-image',
                    'width' => check_plain(variable_get('pm_user_image_width', 50)),
                    'height' => check_plain(variable_get('pm_user_image_height', 50)),
                ),
                    )
            );
        ?>
      <?php endif; ?>
    <?php endif; ?>
    <span class='user-pm-user-image'><?php print $display_string; ?></span>
  </div>
<?php endif; ?>
