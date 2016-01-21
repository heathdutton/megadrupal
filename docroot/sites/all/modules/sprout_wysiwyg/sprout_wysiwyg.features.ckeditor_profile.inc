<?php
/**
 * @file
 * sprout_wysiwyg.features.ckeditor_profile.inc
 */

/**
 * Implements hook_ckeditor_profile_defaults().
 */
function sprout_wysiwyg_ckeditor_profile_defaults() {
  $data = array(
    'Basic' => array(
      'name' => 'Basic',
      'settings' => array(
        'ss' => 2,
        'toolbar' => '[
    [\'Bold\',\'Italic\'],
    [\'linkit\',\'Unlink\'],
    [\'BulletedList\',\'NumberedList\'],
    [\'Blockquote\',\'IMCE\'],
    [\'Source\']
]',
        'expand' => 't',
        'default' => 't',
        'show_toggle' => 'f',
        'uicolor' => 'default',
        'uicolor_user' => 'default',
        'width' => '100%',
        'lang' => 'en',
        'auto_lang' => 't',
        'language_direction' => 'default',
        'allowed_content' => 't',
        'extraAllowedContent' => '',
        'enter_mode' => 'p',
        'shift_enter_mode' => 'br',
        'font_format' => 'p;div;pre;address;h1;h2;h3;h4;h5;h6',
        'custom_formatting' => 'f',
        'formatting' => array(
          'custom_formatting_options' => array(
            'indent' => 'indent',
            'breakBeforeOpen' => 'breakBeforeOpen',
            'breakAfterOpen' => 'breakAfterOpen',
            'breakAfterClose' => 'breakAfterClose',
            'breakBeforeClose' => 0,
            'pre_indent' => 0,
          ),
        ),
        'css_mode' => 'theme',
        'css_path' => '',
        'css_style' => 'theme',
        'styles_path' => '',
        'filebrowser' => 'none',
        'filebrowser_image' => '',
        'filebrowser_flash' => '',
        'UserFilesPath' => '%b%f/',
        'UserFilesAbsolutePath' => '%d%b%f/',
        'forcePasteAsPlainText' => 't',
        'html_entities' => 'f',
        'scayt_autoStartup' => 'f',
        'theme_config_js' => 'f',
        'js_conf' => '',
        'loadPlugins' => array(
          'imce' => array(
            'name' => 'imce',
            'desc' => 'Plugin for inserting files from IMCE without image dialog',
            'path' => '%plugin_dir%imce/',
            'buttons' => array(
              'IMCE' => array(
                'label' => 'IMCE',
                'icon' => 'images/icon.png',
              ),
            ),
            'default' => 'f',
          ),
          'linkit' => array(
            'name' => 'linkit',
            'desc' => 'Support for Linkit module',
            'path' => '%base_path%profiles/sprout/modules/contrib/linkit/editors/ckeditor/',
            'buttons' => array(
              'linkit' => array(
                'label' => 'Linkit',
                'icon' => 'icons/linkit.png',
              ),
            ),
          ),
        ),
      ),
      'input_formats' => array(
        'basic' => 'Basic Editor',
      ),
    ),
    'WYSIWYG' => array(
      'name' => 'WYSIWYG',
      'settings' => array(
        'ss' => 2,
        'toolbar' => '[
    [\'Source\',\'Maximize\'],
    [\'Bold\',\'Italic\',\'Underline\',\'Strike\'],
    [\'JustifyLeft\',\'JustifyCenter\',\'JustifyRight\',\'JustifyBlock\'],
    [\'BulletedList\',\'NumberedList\',\'Outdent\',\'Indent\'],
    [\'Image\',\'Youtube\',\'MediaEmbed\',\'linkit\',\'Unlink\'],
    \'/\',
    [\'Format\',\'Styles\'],
    [\'RemoveFormat\',\'PasteText\',\'PasteFromWord\'],
    [\'Blockquote\',\'DrupalBreak\',\'PageBreak\'],
    [\'Table\',\'Anchor\',\'Templates\',\'SpecialChar\']
]',
        'expand' => 't',
        'default' => 't',
        'show_toggle' => 'f',
        'uicolor' => 'default',
        'uicolor_user' => 'default',
        'width' => '100%',
        'lang' => 'en',
        'auto_lang' => 't',
        'language_direction' => 'default',
        'allowed_content' => 't',
        'extraAllowedContent' => '',
        'enter_mode' => 'br',
        'shift_enter_mode' => 'p',
        'font_format' => 'p;div;pre;address;h1;h2;h3;h4;h5;h6',
        'custom_formatting' => 'f',
        'formatting' => array(
          'custom_formatting_options' => array(
            'indent' => 'indent',
            'breakBeforeOpen' => 'breakBeforeOpen',
            'breakAfterOpen' => 'breakAfterOpen',
            'breakBeforeClose' => 'breakBeforeClose',
            'breakAfterClose' => 'breakAfterClose',
            'pre_indent' => 0,
          ),
        ),
        'css_mode' => 'theme',
        'css_path' => '',
        'css_style' => 'theme',
        'styles_path' => '',
        'filebrowser' => 'imce',
        'filebrowser_image' => 'imce',
        'filebrowser_flash' => 'imce',
        'UserFilesPath' => '%b%f/',
        'UserFilesAbsolutePath' => '%d%b%f/',
        'forcePasteAsPlainText' => 't',
        'html_entities' => 'f',
        'scayt_autoStartup' => 't',
        'theme_config_js' => 'f',
        'js_conf' => '',
        'loadPlugins' => array(
          'codemirror' => array(
            'name' => 'codemirror',
            'desc' => 'Plugin file: codemirror',
            'path' => '%plugin_dir_extra%codemirror/',
            'buttons' => FALSE,
            'default' => 'f',
          ),
          'drupalbreaks' => array(
            'name' => 'drupalbreaks',
            'desc' => 'Plugin for inserting Drupal teaser and page breaks.',
            'path' => '%plugin_dir%drupalbreaks/',
            'buttons' => array(
              'DrupalBreak' => array(
                'label' => 'DrupalBreak',
                'icon' => 'images/drupalbreak.png',
              ),
            ),
            'default' => 't',
          ),
          'imce' => array(
            'name' => 'imce',
            'desc' => 'Plugin for inserting files from IMCE without image dialog',
            'path' => '%plugin_dir%imce/',
            'buttons' => array(
              'IMCE' => array(
                'label' => 'IMCE',
                'icon' => 'images/icon.png',
              ),
            ),
            'default' => 'f',
          ),
          'linkit' => array(
            'name' => 'linkit',
            'desc' => 'Support for Linkit module',
            'path' => '%base_path%profiles/sprout/modules/contrib/linkit/editors/ckeditor/',
            'buttons' => array(
              'linkit' => array(
                'label' => 'Linkit',
                'icon' => 'icons/linkit.png',
              ),
            ),
          ),
          'mediaembed' => array(
            'name' => 'mediaembed',
            'desc' => 'Plugin for inserting Drupal embeded media',
            'path' => '%plugin_dir%mediaembed/',
            'buttons' => array(
              'MediaEmbed' => array(
                'label' => 'MediaEmbed',
                'icon' => 'images/icon.png',
              ),
            ),
            'default' => 'f',
          ),
          'picture_ckeditor' => array(
            'name' => 'picture_ckeditor',
            'desc' => 'Support responsive images with the Picture module.',
            'buttons' => FALSE,
            'path' => '%base_path%profiles/sprout/modules/contrib/picture/ckeditor/plugins/',
            'default' => 't',
          ),
          'youtube' => array(
            'name' => 'youtube',
            'desc' => 'Plugin file: youtube',
            'path' => '%plugin_dir_extra%youtube/',
            'buttons' => array(
              'Youtube' => array(
                'label' => 'Youtube',
                'icon' => 'images/icon.png',
              ),
            ),
            'default' => 'f',
          ),
        ),
      ),
      'input_formats' => array(
        'wysiwyg' => 'WYSIWYG',
      ),
    ),
  );
  return $data;
}
