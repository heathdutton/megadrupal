<?php
// $Id$

/**
 * @file
 *
 */

/**
 * Implements hook_skinr_skin_info().
 */
function incubator_prograde_skinr_skin_info() {
  
  // PAGE 
  // ===========================================================================
  $skins['incubator_prograde_page_options'] = array(
    'title' => t('Page options'),
    'type' => 'checkboxes',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-page-options.css'
      )
    ),
    'options' => array(
      'inset-header' => array('title' => t('Inset header'), 'class' => array('page-options-inset-header')),
      'header-gradient' => array('title' => t('Header gradient'), 'class' => array('page-options-header-gradient')),
      'shadowed-page' => array('title' => t('Raised page'), 'class' => array('page-options-shadowed-page')),
      'spaced-header' => array('title' => t('Space before header'), 'class' => array('page-options-spaced-header')),
      'flatten-content' => array('title' => t('Flatten main content'), 'class' => array('main-block-no-styling')),
    )
  );
  
  $skins['incubator_prograde_sitename_color'] = array(
    'title' => t('Site name color'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'options' => array(
      '' => array('title' => t('Title color'), 'class' => array('sitename-title')),
      'main' => array('title' => t('Main color'), 'class' => array('sitename-main')),
      'secondary' => array('title' => t('Secondary color'), 'class' => array('sitename-secondary')),
      'bg' => array('title' => t('Background color'), 'class' => array('sitename-bg')),
    )
  );
  
  $skins['incubator_prograde_sitename'] = array(
    'title' => t('Site name style'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-sitename.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Flat'), 'class' => array('sitename-nostyle')),
      'glow' => array('title' => t('Glow : White'), 'class' => array('sitename-glow')),
      'glowsecondary' => array('title' => t('Glow : Secondary color'), 'class' => array('sitename-glowsecondary')),
      'softshadow' => array('title' => t('Soft shadow'), 'class' => array('sitename-softshadow')),
      'strongshadow' => array('title' => t('Strong shadow'), 'class' => array('sitename-strongshadow')),
      'raisedshadow' => array('title' => t('Raised shadow'), 'class' => array('sitename-raisedshadow')),
      'threedraised' => array('title' => t('3d : Raised'), 'class' => array('sitename-threedraised')),
      'threedinset' => array('title' => t('3d : Inset'), 'class' => array('sitename-threedinset')),
    )
  );
  
  // TYPOGRAPHY 
  // ===========================================================================
  $skins['incubator_prograde_typography'] = array(
    'title' => t('Font styles'),
    'group' => 'typography',
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'options' => array(
      '' => array('title' => t('Default'), 'class' => array('typ-default')),
      'prograde' => array(
        'title' => t('Prograde design'),
        'class' => array('typ-prograde'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Roboto+Slab:400,700|Noto+Serif:400,400italic,700&subset=latin,cyrillic'
        ))
      ),
      'newsy' => array(
        'title' => t('Newsy'), 
        'class' => array('typ-newsy'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Playfair+Display+SC:700',
        )),
      ),
      'modern' => array(
        'title' => t('Modern'), 
        'class' => array('typ-modern'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Economica|Julius+Sans+One|Gafata&subset=latin,latin-ext',
        )),
      ),
      'baskerville' => array(
        'title' => t('Baskerville'),
        'class' => array('typ-baskerville'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Libre+Baskerville:400,i|Ubuntu:400,i,b,bi'
        ))
      ),
      'bree' => array(
        'title' => t('Bree'),
        'class' => array('typ-bree'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Bree+Serif|Imprima&subset=latin,latin-ext'
        ))
      ),
      'montserrat' => array(
        'title' => t('Montserrat'),
        'class' => array('typ-montserrat'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Montserrat:400|Pontano+Sans'
        ))
      ),
      'oswald' => array(
        'title' => t('Oswald'),
        'class' => array('typ-oswald'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Oswald|Muli:400,300'
        ))
      ),
      'pt' => array(
        'title' => t('PT'),
        'class' => array('typ-pt'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=PT+Serif+Caption|PT+Sans'
        ))
      ),
      'vollkorn' => array(
        'title' => t('Vollkorn'),
        'class' => array('typ-vollkorn'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Vollkorn|Raleway:300,600'
        ))
      ),
      'robotoworld' => array(
        'title' => t("Roboto Int'l"),
        'class' => array('typ-robotoworld'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Roboto&subset=latin,latin-ext,greek,cyrillic,vietnamese'
        ))
      ),
      'josefin' => array(
        'title' => t('Josefin'),
        'class' => array('typ-josefin'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Josefin+Slab:400,700|Maven+Pro'
        ))
      ),
      'paytone' => array(
        'title' => t('Paytone'),
        'class' => array('typ-paytone'),
        'attached' => array('css' => array(
          '//fonts.googleapis.com/css?family=Paytone+One|Droid+Sans'
        ))
      ),
    ),
  );
  foreach ($skins['incubator_prograde_typography']['options'] as $name => $options) {
    if (!empty($name)) {
      $skins['incubator_prograde_typography']['options'][$name]['attached']['css'][] = "css/typography/$name.css";
    }
  }
  
  $skins['incubator_prograde_font_sizing'] = array(
    'title' => t('Font size'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default_status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-font-sizing.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Normal'), 'class' => array('font-sizing-normal')),
      'pct105' => array('title' => t('105%'), 'class' => array('font-sizing-105')),
      'pct110' => array('title' => t('110%'), 'class' => array('font-sizing-110')),
      'pct115' => array('title' => t('115%'), 'class' => array('font-sizing-115')),
      'pct120' => array('title' => t('120%'), 'class' => array('font-sizing-120')),
      'pct125' => array('title' => t('125%'), 'class' => array('font-sizing-125')),
    )
  );
  
  // BLOCKS 
  // ===========================================================================
  $skins['incubator_prograde_block_bg'] = array(
    'title' => t('Block color'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-block-bg.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Border, very light'), 'class' => array('block-bg-lighter')),
      'light' => array('title' => t('Border, light'), 'class' => array('block-bg-light')),
      'white' => array('title' => t('White'), 'class' => array('block-bg-white')),
      'pagebg' => array('title' => t('Page background'), 'class' => array('block-bg-pagebg')),
      'border' => array('title' => t('Border'), 'class' => array('block-bg-border')),
    )
  );
  
  $skins['incubator_prograde_block_styles'] = array(
    'title' => t('Block styles'),
    'type' => 'checkboxes',
    'theme hooks' => array('html', 'block', 'region'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-block-styles.css'
      )
    ),
    'options' => array(
      'no-borders' => array('title' => t('No borders'), 'class' => array('block-styles-no-borders')),
      'no-background' => array('title' => t('No background'), 'class' => array('block-styles-no-background')),
      'rounded' => array('title' => t('Rounded'), 'class' => array('block-styles-rounded')),
      'heavy' => array('title' => t('Heavy'), 'class' => array('block-styles-heavy')),
      'shadowed' => array('title' => t('Shadowed'), 'class' => array('block-styles-shadowed')),
    )
  );
  
  $skins['incubator_prograde_headers'] = array(
    'title' => t('Block headers'),
    'type' => 'checkboxes',
    'theme hooks' => array('html', 'block'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-headers.css',
      )
    ),
    'options' => array(
      'centered' => array('title' => t('Centered'), 'class' => array('headers-centered')),
      'colored' => array('title' => t('Colored background'), 'class' => array('headers-colored')),
      'top-aligned' => array('title' => t('Aligned with top'), 'class' => array('headers-top-aligned')),
      'link-text' => array('title' => t('Link color text'), 'class' => array('headers-link-text')),
      'link-bg' => array('title' => t('Link color background'), 'class' => array('headers-link-bg')),
      'secondary-text' => array('title' => t('Secondary color text'), 'class' => array('headers-secondary-text')),
      'secondary-bg' => array('title' => t('Secondary color background'), 'class' => array('headers-secondary-bg')),
      'banner' => array('title' => t('Banner'), 'class' => array('headers-banner')),
      'banner-right' => array('title' => t('Banner right'), 'class' => array('headers-banner-right', 'headers-banner-wide')),
      'banner-wide' => array('title' => t('Full-width banner'), 'class' => array('headers-banner-wide')),
    ),
  );
  
  $skins['incubator_prograde_block_separator_color'] = array(
    'title' => t('Separator color'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-block-separators.css',
      )
    ),
    'options' => array(
      '' => array('title' => t('Block border'), 'class' => array('block-separators-block-border')),
      'lightgray' => array('title' => t('Light gray'), 'class' => array('block-separators-lightgray')),
      'gray' => array('title' => t('Medium gray'), 'class' => array('block-separators-gray')),
      'darkgray' => array('title' => t('Dark gray'), 'class' => array('block-separators-darkgray')),
      'main' => array('title' => t('Main color'), 'class' => array('block-separators-main')),
      'secondary' => array('title' => t('Secondary color'), 'class' => array('block-separators-secondary')),
    )
  );
  
  $skins['incubator_prograde_block_separators'] = array(
    'title' => t('Separator style'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-block-separators.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Solid'), 'class' => array('block-separators-solid')),
      'dotted' => array('title' => t('Dotted'), 'class' => array('block-separators-dotted')),
      'dashed' => array('title' => t('Dashed'), 'class' => array('block-separators-dashed')),
      'none' => array('title' => t('None'), 'class' => array('block-separators-none')),
    )
  );
  
  // MENUS
  // ===========================================================================
  $skins['incubator_prograde_main_menu_colors'] = array(
    'title' => t('Main menu colors'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'options' => array(
      '' => array('title'=> t('Base'), 'class' => array('main-menu-basecolor')),
      'linkcolor' => array('title'=> t('Link'), 'class' => array('main-menu-linkcolor')),
      'titlecolor' => array('title'=> t('Title'), 'class' => array('main-menu-titlecolor')),
      'secondarycolor' => array('title'=> t('Secondary'), 'class' => array('main-menu-secondarycolor')),
    ),
  );
  
  $skins['incubator_prograde_main_menu'] = array(
    'title' => t('Main menu styles'),
    'type' => 'checkboxes',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-main-menu.css'
      )
    ),
    'options' => array(
      'no-tabs' => array('title' => t('No tab coloring'), 'class' => array('main-menu-no-tabs')),
      'block' => array('title' => t('Block style'), 'class' => array('main-menu-block', '.main-menu-no-lineunder')),
      'overlay' => array('title' => t('Overlay header'), 'class' => array('main-menu-overlay', 'main-menu-no-lineunder')),
      'narrow-space' => array('title' => t('Narrow space'), 'class' => array('main-menu-narrow-space', 'main-menu-no-separators')),
      'wide-space' => array('title' => t('Wider space'), 'class' => array('main-menu-wide-space', 'main-menu-no-separators')),
      'narrow-items' => array('title' => t('Narrow items'), 'class' => array('main-menu-narrow-items')),
      'full-width' => array('title' => t('Full width'), 'class' => array('main-menu-full-width')),
      'dot-separators' => array('title' => t('Dot separators'), 'class' => array('main-menu-dot-separators', 'main-menu-no-separators')),
      'no-lineunder' => array('title' => t('No line under'), 'class' => array('main-menu-no-lineunder')),
      'underline-items' => array('title' => t('Underline items'), 'class' => array('main-menu-underline-items', 'main-menu-no-lineunder', 'main-menu-no-separators')),
    )
  );
  
  $skins['incubator_prograde_main_menu_side'] = array(
    'title' => t('Main menu position'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-main-menu.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Normal'), 'class' => array('main-menu-side-normal')),
      'left' => array('title' => t('Left'), 'class' => array('main-menu-side-left')),
      'center' => array('title' => t('Center'), 'class' => array('main-menu-side-center')),
      'right' => array('title' => t('Right'), 'class' => array('main-menu-side-right')),
    )
  );
  
  $skins['incubator_prograde_user_menu_style'] = array(
    'title' => t('User menu style'),
    'type' => 'select',
    'theme hooks' => array('html'),
    'default status' => 0,
    'status' => array(
      'incubator_prograde' => 1,
    ),
    'attached' => array(
      'css' => array(
        'css/skinr-user-menu.css'
      )
    ),
    'options' => array(
      '' => array('title' => t('Links'), 'class' => array('user-menu-style-links')),
      'tabs' => array('title' => t('Tabs'), 'class' => array('user-menu-style-tabs')),
      'bar' => array('title' => t('Bar'), 'class' => array('user-menu-style-bar')),
    ),
  );
  
  return $skins;
}

function incubator_prograde_skinr_group_info() {
  return array(
    'incubator_prograde_general' => array(
      'title' => t('General'),
      'description' => t('Choose styles for page layout and typography.'),
    ),
    'incubator_prograde_blocks' => array(
      'title' => t('Blocks'),
      'description' => t('Choose styles for blocks of content.'),
    ),
    'incubator_prograde_menu' => array(
      'title' => t('Main menu'),
      'description' => t('Choose styles for the main menu.'),
    ),
    'incubator_prograde_color' => array(
      'title' => t('Secondary color'),
      'description' => t('Choose where the secondary color is used.'),
    ),
  );
}
