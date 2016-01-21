<?php
/**
 * @file
 *   <p>All hooks and code related to defining and exposing Image effects to
 *   the Image API.</p>
 *   
 *   <p>© 2012 RedBottle Design, LLC. All rights reserved.</p>
 *   
 *   <p>All module code is released under the GNU General Public License.
 *   See COPYRIGHT.txt and LICENSE.txt.</p>
 * 
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 * @license GNU GPLv2
 */

/**
 * Implementation of <code>hook_image_effect_info()</code>.
 */
function image_combination_effects_image_effect_info() {
  $effects = array();

  $effects['image_combination_effects_combine'] = array(
    'label'                => t('Combine images'),
    'help'                 => t('Combines several images from the same field into a single image that runs horizontally or vertically.'),
    'effect callback'      => 'image_combination_effects_combine_image',
    'dimensions callback'  => 'image_combination_effects_combine_dimensions',
    'form callback'        => 'image_combination_effects_combine_form',
  );

  return $effects;
}

/**
 * <p>Image effect callback for all combination effects.</p>
 * 
 * <p>This module employs a hook to ensure that only the first delta of the
 * image field is rendered, so <code>$first_image</code> is guaranteed to
 * always be the first image of the field.</p>
 * 
 * @param  stdClass $first_image
 *                  The first delta of the field.
 *                  
 * @param  array    $settings
 *                  An array of the settings for the image effect, as provided
 *                  by the user. This contains the following item:
 *                  - "orientation": The direction in which images should be
 *                  laid out, which is one of:
 *                    - IMCOEF_OPTION_HORIZONTAL_LEFT_TO_RIGHT: left-to-right
 *                    - IMCOEF_OPTION_HORIZONTAL_RIGHT_TO_LEFT: right-to-left
 *                    - IMCOEF_OPTION_VERTICAL_TOP_TO_BOTTOM: top-to-bottom
 *                    - IMCOEF_OPTION_VERTICAL_BOTTOM_TO_TOP: bottom-to-top
 * 
 * @return boolean  <code>TRUE</code> if the effect was applied successfully;
 *                  <code>FALSE</code>, otherwise.
 */
function image_combination_effects_combine_image(stdClass $first_image, array $settings) {
  $result = FALSE;
  
  $orientation = $settings['orientation'];
  
  // Get context about the image.
  $image_context      = imagecache_actions_get_image_context($first_image, $settings);
  $referring_entities = $image_context['referring_entities'];

  $target_info = array(
    IMCOEF_TARGET_WIDTH        => $first_image->info['width'],
    IMCOEF_TARGET_HEIGHT       => $first_image->info['height'],
    IMCOEF_TARGET_ORIENTATION  => $orientation,
  );

  foreach ($referring_entities as $field_name => $field_referring_entities) {
    foreach ($field_referring_entities as $entity_type => $entities) {
      foreach ($entities as $entity_id => $entity) {
        $image_style = image_combination_effects_get_field_image_style($field_name, $entity, $entity_type, 'default');
        
        if (!empty($image_style)) {
          $delta_image_files = field_get_items($entity_type, $entity, $field_name);

          // First image is already taken care of.
          $first_delta_image_file = array_shift($delta_image_files);
          
          foreach ($delta_image_files as $delta => $delta_image_file) {
            $delta_image = imagecache_actions_image_load($delta_image_file['uri'], $first_image->toolkit);
  
            if (!empty($delta_image)) {
              $target_info[IMCOEF_TARGET_DELTA] = $delta;
              
              image_combination_effects_place_image($delta_image, $first_image, $target_info, $image_style);
            }
          }
        }
      }
    }
  }

  if ($result && ($first_image->toolkit == 'GD')) {
    $first_image->info['width']  = imagesx($first_image->resource);
    $first_image->info['height'] = imagesy($first_image->resource);
  }

  return $result;
}

/**
 * Image dimensions callback for all combination effects.
 *  
 * @param  array  $dimensions
 *                An array of the dimensions to be modified as width and height
 *                components, in pixels:
 *                - "width": The image width.
 *                - "height": The image height.
 *               
 * @param  array  $settings
 *                An array of the settings for the image effect, as provided by
 *                the user. This contains the following item:
 *                - "orientation": The direction in which images should be
 *                  laid out in the output image, which is one of:
 *                    - IMCOEF_OPTION_HORIZONTAL_LEFT_TO_RIGHT: left-to-right
 *                    - IMCOEF_OPTION_HORIZONTAL_RIGHT_TO_LEFT: right-to-left
 *                    - IMCOEF_OPTION_VERTICAL_TOP_TO_BOTTOM: top-to-bottom
 *                    - IMCOEF_OPTION_VERTICAL_BOTTOM_TO_TOP: bottom-to-top 
 */
function image_combination_effects_combine_dimensions(array &$dimensions, array $settings) {
  // FIXME: This is a lazy way out.
  $dimensions['width']  = NULL;
  $dimensions['height'] = NULL;
}

/**
 * Image settings form callback for all combination effects.
 * 
 * @param   array  $settings
 *                 The array of existing settings values for the image effect.
 *               
 * @return  array  The settings form array for the image effect.
 */
function image_combination_effects_combine_form(array $settings) {
  $form = array();
  
  if (!isset($settings['orientation'])) {
    $settings['orientation'] = IMCOEF_OPTION_HORIZONTAL_LEFT_TO_RIGHT;
  }
  
  $form['orientation'] = array(
    '#type'        => 'select',
    '#title'       => t('Orientation'),
    '#description' => t('Choose in which direction images should be combined.'),      

    '#options' => array(
      IMCOEF_OPTION_HORIZONTAL_LEFT_TO_RIGHT => t('Horizontal (left-to-right)'),
      IMCOEF_OPTION_HORIZONTAL_RIGHT_TO_LEFT => t('Horizontal (right-to-left)'),
        
      IMCOEF_OPTION_VERTICAL_TOP_TO_BOTTOM   => t('Vertical (top-to-bottom)'),
      IMCOEF_OPTION_VERTICAL_BOTTOM_TO_TOP   => t('Vertical (bottom-to-top)'),
    ),
    
    '#default_value' => $settings['orientation'],
  );
  
  return $form;
}