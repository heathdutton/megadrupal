<?php
/**
 * @file
 * Hooks and API provided by the Slick module.
 *
 * Modules may implement any of the available hooks to interact with Slick.
 */

/**
 * Slick may be configured using the web interface via sub-modules.
 *
 * However if you want to code it, use slick_build(), or build it from the
 * available data for more advanced slick such as asNavFor, see slick_fields.
 *
 * The example is showing a customized views-view-unformatted--ticker.tpl.php.
 * Practically any content-related .tpl.php file where you have data to print.
 * Do preprocess, or here a direct .tpl.php manipulation for quick illustration.
 *
 * The goal is to create a vertical newsticker, or tweets, with pure text only.
 * First, create an unformatted Views block, says 'Ticker' containing ~ 10
 * titles, or any data for the contents -- using EFQ, or static array will do.
 */
// 1.
// Provides HTML settings with optionset name and ID, none of JS related.
// See slick_get_element_default_settings() for more supported keys.
// To add JS key:value pairs, use #options at theme_slick() below instead.
// If you provide ID, be sure unique per instance as it is cached.
// Leave empty to be provided by the module.
$id = 'slick-ticker';
$settings = array(
// Optional optionset name, otherwise fallback to default.
// 'optionset' => 'default',
// Optional skin name fetched from hook_slick_skins_info(), otherwise none.
// 'skin' => 'default',
// Note we add attributes to the settings, not as theme key here, to allow
// various scenarios before being passed to the actual #attributes property.
// Or ignore this, if the only attribute is just $id, and the $id is set.
// @see README.txt for the HTML structure.
// Leave empty to be provided by the module.
  'attributes' => array(
    'id' => $id,
  ),
  // content_attributes ID is for nested, or asNavFor slicks.
  // Pass the proper ID to asnavfor_target, see slick_fields/slick_views.
  // Leave empty to be provided by the module.
  'content_attributes' => array(
    'id' => $id . '-slider',
  ),
);

// 3.
// Prepare $items contents, note the 'slide' key is to hold the actual slide
// which can be pure and simple text, or any image/media file. Meaning $rows can
// be text only, or image/audio/video, or a combination of both.
// To add caption/overlay, use 'caption' key with the supported sub-keys:
// title, alt, link, overlay, editor, or data for complex content.
// Sanitize each sub-key content accordingly.
// See template_preprocess_slick_item() for more info.
$items = array();
foreach ($rows as $key => $row) {
  $items[] = array(
    'slide' => $row,
    // If the slide is an image, to add text caption, use:
    // 'caption' => 'some-caption data',
    // Individual slide supports some useful settings like layout, classes, etc.
    // Meaning each slide can have different layout, or classes.
    // See sub-modules implementation.
    'settings' => array(
      'layout' => 'bottom',
      'slide_classes' => 'slide--custom-class--' . $key,
      'has_pattern' => TRUE,
    ),
  );
}

// 4.
// Optional JS and CSS assets loader, see slick_attach(). An empty array should
// suffice for the most basic slick with no skin at all.
// @see slick_fields/slick_views for the real world samples.
$attach = array();
// Add more assets using supported slick_attach() keys.
$attachments = slick_attach($attach);
// Add more attachments using regular library keys just as freely:
$attachments['css'] += array(HOOK_PATH . '/css/zoom.css' => array('weight' => 9));
$attachments['js'] += array(HOOK_PATH . '/js/zoom.min.js' => array('weight' => -5));

// 5.
// Optional specific Slick JS options, if no optionset provided above.
// Play with speed and options to achieve desired result.
// @see slick_get_options()
$options = array(
  'arrows' => FALSE,
  'autoplay' => TRUE,
  'vertical' => TRUE,
  'draggable' => FALSE,
);

// 6.A.
// Build the slick, note key 0 just to mark the thumbnail asNavFor with key 1.
$slick[0] = array(
  '#theme' => 'slick',
  '#items' => $items,
  '#options' => $options,
  '#settings' => $settings,
  // Attach the Slick library, see slick_attach() for more options.
  // At D8, #attached is obligatory to avoid issue with caching.
  '#attached' => $attachments,
);

// Optionally build an asNavFor with $slick[1], and both should be passed to
// theme_slick_wrapper(), otherwise a single theme_slick() will do.
// See slick_fields, or slick_views sub-modules for asNavFor samples.
// All is set, render the Slick.
print render($slick);

// 6.B.
// Or recommended, use slick_build() so you can cache the slick instance easily.
// If it is a hardly updated content, such as profile videos, logo carousels, or
// more permanent home slideshows, select "Persistent", otherwise time.
// Cache the slick for 1 hour and fetch fresh contents when the time reached.
// If stale cache is not cleared, slick will keep fetching fresh contents.
$settings['cache'] = 3600;
// Or cache the slick and keep stale contents till the next cron runs.
$settings['cache'] = 'persistent';
// One cron hits, slick will use the new cached version regardless of expiration
// time due to the nature of render cache.
// Add a custom unique cache ID.
$settings['cid'] = 'my-extra-unique-id';
// Where the parameters as described above:
$slick = slick_build($items, $options, $settings, $attachments, $id);
// All is set, render the Slick.
print render($slick);

/**
 * Registers Slick skins.
 *
 * This function may live in module file, or my_module.slick.inc if you have
 * many skins.
 *
 * This hook can be used to register skins for the Slick. Skins will be
 * available when configuring the Optionset, Field formatter, or Views style.
 *
 * Slick skins get a unique CSS class to use for styling, e.g.:
 * If your skin name is "my_module_slick_carousel_rounded", the class is:
 * slick--skin--my-module-slick-carousel-rounded
 *
 * A skin can specify some CSS and JS files to include when Slick is displayed,
 * except for a thumbnail skin which accepts CSS only.
 *
 * Each skin supports 5 keys:
 * - name: The human readable name of the skin.
 * - description: The description about the skin, for help and manage pages.
 * - css: An array of CSS files to attach.
 * - js: An array of JS files to attach, e.g.: image zoomer, reflection, etc.
 * - inline css: An optional flag to determine whether the image is turned into
 *   CSS background rather than image with SRC, see fullscreen skin.
 *
 * @see hook_hook_info()
 * @see slick_example.module
 * @see slick.slick.inc
 */
function hook_slick_skins_info() {
  // The source can be theme or module.
  $theme_path = drupal_get_path('theme', 'my_theme');

  return array(
    'skin_name' => array(
      // Human readable skin name.
      'name' => t('Skin name'),
      // Description of the skin.
      'description' => t('Skin description.'),
      'css' => array(
        // Full path to a CSS file to include with the skin.
        $theme_path . '/css/my-theme.slick.theme--slider.css' => array('weight' => 10),
        $theme_path . '/css/my-theme.slick.theme--carousel.css' => array('weight' => 11),
      ),
      'js' => array(
        // Full path to a JS file to include with the skin.
        $theme_path . '/js/my-theme.slick.theme--slider.js',
        $theme_path . '/js/my-theme.slick.theme--carousel.js',
        // If you want to act on afterSlick event, or any other slick events,
        // put a lighter weight before slick.load.min.js (0).
        $theme_path . '/js/slick.skin.menu.min.js' => array('weight' => -2),
      ),
    ),
  );
}

/**
 * Registers Slick dot skins.
 *
 * The provided dot skins will be available at sub-module interfaces.
 * A skin dot named 'hop' will have a class 'slick-dots--hop' for the UL.
 *
 * The array is similar to the hook_slick_skins_info(), excluding JS.
 */
function hook_slick_dots_info() {
  // Create an array of dot skins.
}

/**
 * Registers Slick arrow skins.
 *
 * The provided arrow skins will be available at sub-module interfaces.
 * A skin arrow named 'slit' will have a class 'slick__arrow--slit' for the NAV.
 *
 * The array is similar to the hook_slick_skins_info(), excluding JS.
 */
function hook_slick_arrows_info() {
  // Create an array of arrow skins.
}

/**
 * Alter Slick skins.
 *
 * This function lives in a module file, not my_module.slick.inc.
 * Overriding skin CSS can be done via theme.info, hook_css_alter(), or below.
 *
 * @param array $skins
 *   The associative array of skin information from hook_slick_skins_info().
 *
 * @see hook_slick_skins_info()
 * @see slick_example.module
 */
function hook_slick_skins_info_alter(array &$skins) {
  // The source can be theme or module.
  // The CSS is provided by my_theme.
  $path = drupal_get_path('theme', 'my_theme');

  // Modify the default skin's name and description.
  $skins['default']['name'] = t('My Theme: Default');
  $skins['default']['description'] = t('My Theme default skin.');

  // This one won't work.
  // $skins['default']['css'][$path . '/css/slick.theme--base.css'] = array();
  // This one overrides slick.theme--default.css with slick.theme--base.css.
  $skins['default']['css'] = array($path . '/css/slick.theme--base.css' => array('weight' => -22));

  // Overrides skin asNavFor with theme CSS.
  $skins['asnavfor']['name'] = t('My Theme: asnavfor');
  $skins['asnavfor']['css'] = array($path . '/css/slick.theme--asnavfor.css' => array('weight' => 21));

  // Or with the new name.
  $skins['asnavfor']['css'] = array($path . '/css/slick.theme--asnavfor-new.css' => array('weight' => 21));

  // Overrides skin Fullwidth with theme CSS.
  $skins['fullwidth']['name'] = t('My Theme: fullwidth');
  $skins['fullwidth']['css'] = array($path . '/css/slick.theme--fullwidth.css' => array('weight' => 22));
}

/**
 * Alter Slick attach information before they are called.
 *
 * This function lives in a module file, not my_module.slick.inc.
 *
 * @param array $attach
 *   The modified array of $attach information from slick_attach().
 * @param array $settings
 *   An array of settings to check for the supported features.
 *
 * @see slick_attach()
 * @see slick_example.module
 */
function hook_slick_attach_info_alter(array &$attach, $settings) {
  // Disable inline CSS after copying the output to theme at final stage.
  // Inline CSS are only used for 2 cases: Fullscreen and Field collection
  // individual slide color, only if your clients don't change mind much.
  // Use key 'inline css' to register skin that wants inline CSS rather than
  // images when declaring the skins, see fullscreen skin.
  // Use hook_slick_inline_css_info_alter() to modify the output.
  // @see slick_get_inline_css_skins()
  // @see slick_slick_skins_info()
  $attach['attach_inline_css'] = NULL;

  // Disable module JS: slick.load.min.js to use your own slick JS.
  $attach['attach_module_js'] = FALSE;
}

/**
 * Alter Slick load information before they are called.
 *
 * This function lives in a module file, not my_module.slick.inc.
 *
 * @param array $load
 *   The modified array of $load information.
 * @param array $attach
 *   The contextual array of $attach information.
 * @param array $skins
 *   The contextual array of $skins information.
 * @param array $settings
 *   An array of settings to check for the supported features.
 *
 * @see slick_attach()
 * @see slick_example.module
 * @see slick_devel.module
 */
function hook_slick_attach_load_info_alter(&$load, $attach, $skins, $settings) {
  $slick_path = drupal_get_path('module', 'slick');
  $min = $slick_path . '/js/slick.load.min.js';
  $dev = $slick_path . '/js/slick.load.js';

  if (HOOK_DEBUG) {
    // Switch to the non-minified version of the slick.load.min.js.
    $load['js'] += array(
      $dev => array('group' => JS_DEFAULT, 'weight' => 0),
    );
    if (isset($load['js'][$min])) {
      unset($load['js'][$min]);
    }
  }
}

/**
 * Using slick_attach() to a custom theme or renderable array.
 *
 * slick_attach() is just an array as normally used with #attached property.
 * This can be used to merge extra 3d party libraries along with the slick.
 * Previously slick_add() was provided as a fallback. It was dropped since it
 * was never actually used by slick. However you can add slick assets using a
 * few alternatives: drupal_add_library(), drupal_add_js(), or the recommended
 * #attached, the D8 way, just as easily. Hence slick_attach() acts as a
 * shortcut to load the basic Slick assets.
 *
 * Passing an empty array will load 3 basic files:
 *  - slick.min.js, slick.css, slick.load.min.js.
 */
// Empty array for the basic files, or optionallly pass a skin to have a proper
// display where appropriate, see slick_fields/slick_views for more samples.
$attach = array();
$attachments = slick_attach($attach);

// Add another custom library to the array.
$transit = libraries_get_path('jquery.transit') . '/jquery.transit.min.js';
$attachments['js'] += array($transit => array('group' => JS_LIBRARY, 'weight' => -5));

// Add another asset.
$my_module_path = drupal_get_path('module', 'my_module');
$attachments['css'] += array($my_module_path . '/css/my_module.css' => array('weight' => 5));

// Pass the $attachments to theme_slick(), or any theme with bigger scope.
$my_module_theme = array(
  '#theme' => 'my_module_theme',
  // More properties...
  '#attached' => $attachments,
);
