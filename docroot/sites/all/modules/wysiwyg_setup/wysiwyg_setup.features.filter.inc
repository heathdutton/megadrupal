<?php
/**
 * @file
 * wysiwyg_setup.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function wysiwyg_setup_filter_default_formats() {
  $formats = array();

  // Exported format: HTML filter.
  $formats['filtered_html'] = array(
    'format' => 'filtered_html',
    'name' => 'HTML filter',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_autop' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(
          'filter_url_length' => '72',
        ),
      ),
      'wysiwyg' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(
          'valid_elements' => 'a[!href|style|target<_blank|title],
                    div[style|align<center?justify?left?right],
                    p[style|align<center?justify?left?right],
                    span[style],
                    img[!src|style|width|height|alt|title],
                    br,span,em,strong,u,strike,cite,code,blockquote,ul,ol,li,dl,dt,dd,hr,
                    h1,h2,h3,h4,h5,h6',
          'allow_comments' => '0',
          'nofollow_policy' => 'disabled',
          'nofollow_domains' => array(),
          'style_color' => array(
            'color' => 'color',
            'background' => 'background',
            'background-color' => 'background-color',
            'background-image' => 0,
            'background-repeat' => 0,
            'background-attachment' => 0,
            'background-position' => 0,
          ),
          'style_font' => array(
            'font' => 'font',
            'font-family' => 'font-family',
            'font-size' => 'font-size',
            'font-size-adjust' => 'font-size-adjust',
            'font-stretch' => 'font-stretch',
            'font-style' => 'font-style',
            'font-variant' => 'font-variant',
            'font-weight' => 'font-weight',
          ),
          'style_text' => array(
            'text-align' => 'text-align',
            'text-decoration' => 'text-decoration',
            'text-indent' => 'text-indent',
            'text-transform' => 'text-transform',
            'word-spacing' => 'word-spacing',
            'direction' => 'direction',
            'unicode-bidi' => 'unicode-bidi',
            'letter-spacing' => 0,
            'white-space' => 0,
          ),
          'style_box' => array(
            'margin' => 'margin',
            'margin-top' => 'margin-top',
            'margin-right' => 'margin-right',
            'margin-bottom' => 'margin-bottom',
            'margin-left' => 'margin-left',
            'padding' => 'padding',
            'padding-top' => 'padding-top',
            'padding-right' => 'padding-right',
            'padding-bottom' => 'padding-bottom',
            'padding-left' => 'padding-left',
          ),
          'style_border-1' => array(
            'border' => 'border',
            'border-top' => 'border-top',
            'border-right' => 'border-right',
            'border-bottom' => 'border-bottom',
            'border-left' => 0,
            'border-width' => 0,
            'border-top-width' => 0,
            'border-right-width' => 0,
            'border-bottom-width' => 0,
            'border-left-width' => 0,
          ),
          'style_border-2' => array(
            'border-color' => 'border-color',
            'border-style' => 'border-style',
            'border-top-color' => 0,
            'border-right-color' => 0,
            'border-bottom-color' => 0,
            'border-left-color' => 0,
            'border-top-style' => 0,
            'border-right-style' => 0,
            'border-bottom-style' => 0,
            'border-left-style' => 0,
          ),
          'style_dimension' => array(
            'height' => 'height',
            'line-height' => 'line-height',
            'width' => 'width',
            'max-height' => 0,
            'max-width' => 0,
            'min-height' => 0,
            'min-width' => 0,
          ),
          'style_positioning' => array(
            'vertical-align' => 'vertical-align',
            'bottom' => 0,
            'clip' => 0,
            'left' => 0,
            'overflow' => 0,
            'right' => 0,
            'top' => 0,
            'z-index' => 0,
          ),
          'style_layout' => array(
            'clear' => 0,
            'display' => 0,
            'float' => 0,
            'position' => 0,
            'visibility' => 0,
          ),
          'style_list' => array(
            'list-style' => 0,
            'list-style-image' => 0,
            'list-style-position' => 0,
            'list-style-type' => 0,
          ),
          'style_table' => array(
            'border-collapse' => 'border-collapse',
            'border-spacing' => 0,
            'caption-side' => 0,
            'empty-cells' => 0,
            'table-layout' => 0,
          ),
          'style_user' => array(
            'cursor' => 0,
            'outline' => 0,
            'outline-width' => 0,
            'outline-style' => 0,
            'outline-color' => 0,
            'zoom' => 0,
          ),
          'rule_valid_classes' => array(),
          'rule_valid_ids' => array(),
          'rule_style_urls' => array(),
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  // Exported format: HTML full.
  $formats['full_html'] = array(
    'format' => 'full_html',
    'name' => 'HTML full',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_autop' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(
          'filter_url_length' => '72',
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
