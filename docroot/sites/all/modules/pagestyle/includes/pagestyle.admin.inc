<?php
// $Id: pagestyle.admin.inc,v 1.8 2011/01/04 23:14:03 christianzwahlen Exp $

/**
 * @file
 * Provides the Page Style administrative interface.
 */

/**
 * Implement hook_help().
 */
function pagestyle_help($path, $arg) {
  $path_pagestyle = base_path() . drupal_get_path('module', 'pagestyle');
  $abbr_php = '<abbr lang="en" xml:lang="en" title="' . t('PHP: Hypertext Preprocessor') . '">PHP</abbr>';
  $abbr_php_small = '<abbr lang="en" xml:lang="en" title="' . t('PHP: Hypertext Preprocessor') . '">php</abbr>';
  $abbr_mysql = '<abbr title="' . t('My Structured Query Language') . '">MySQL</abbr>';
  $abbr_xhtml = '<abbr lang="en" xml:lang="en" title="' . t('Extensible Hypertext Markup Language') . '">XHTML</abbr>';
  $abbr_css = '<abbr lang="en" xml:lang="en" title="' . t('Cascading Style Sheets') . '">CSS</abbr>';
  $abbr_css_small = '<abbr lang="en" xml:lang="en" title="' . t('Cascading Style Sheets') . '">css</abbr>';
  $abbr_wcag = '<abbr lang="en xml:lang="en" title="' . t('Web Content Accessibility Guidelines') . '">WCAG</abbr>';
  $abbr_bitv = '<abbr lang="de" xml:lang="de" title="' . t('Barrierefreie Informationstechnik-Verordnung') . '">BITV</abbr>';
  $abbr_png = '<abbr lang="en" xml:lang="en" title="' . t('Portable Network Graphics') . '">PNG</abbr>';
  $abbr_gif = '<abbr lang="en" xml:lang="en" title="' . t('Graphic Interchange Format') . '">GIF</abbr>';
  $abbr_svg = '<abbr lang="en" xml:lang="en" title="' . t('Scalable Vector Graphics') . '">SVG</abbr>';
  $abbr_xcf_small = '<abbr lang="en" xml:lang="en" title="' . t('eXperimental Computing Facility') . '">xcf</abbr>';
  $abbr_id = '<abbr lang="en" xml:lang="en" title="' . t('Identification') . '">ID</abbr>';
  $abbr_tpl_php = '<abbr title="' . t('PHP: Hypertext Preprocessor Template', array('!abbr_php' => $abbr_php)) . '">tpl.php</abbr>';
  $link_wcag = l(t('Web Content Accessibility Guidelines'), 'http://www.w3.org/TR/WCAG/', array('attributes' => array('title' => t('External link to:') . ' ' . t('Web Content Accessibility Guidelines'))));
  $link_bitv= l(t('Barrierefreie Informationstechnik-Verordnung'), 'http://bundesrecht.juris.de/bitv/anlage_8.html', array('attributes' => array('title' => t('External link to:') . ' ' . t('Barrierefreie Informationstechnik-Verordnung'))));
  $link_ff = l(t('Mozilla Firefox'), 'http://www.mozilla.com/en-US/firefox', array('attributes' => array('title' => t('External link to:') . ' ' . t('Mozilla Firefox'))));
  $link_ps_perm = l(t('Permissions'), 'admin/config/people/permissions', array('attributes' => array('title' => t('Page Styleconfiguration')), 'fragment' => 'module-pagestyle'));
  $link_block = l(t('Blocks'), 'admin/structure/block', array('attributes' => array('title' => t('Page Style configuration'))));
  $link_ps_config = l(t('Page Style configuration'), 'admin/config/content/pagestyle', array('attributes' => array('title' => t('Page Style configuration'))));
  $link_po_import = l(t('Import'), 'admin/config/regional/translate/import', array('attributes' => array('title' => t('Translate interface') . ', ' . t('Import'))));
  $link_ps = l(t('Download Themes') . ': ' . t('Page Style home page'), 'http://www.zwahlendesign.ch/pagestyle', array('attributes' => array('title' => t('External link to:') . ' ' . t('Page Style home page'), 'class' => 'ps_download_button')));
  $link_jquery_cookie = l(t('jQuery plugins'), 'http://drupal.org/project/jquery_plugin');
  $link_footnote_1 = l('1', 'admin/help/pagestyle', array('attributes' => array('title' =>  t('Footnote') . ' 1'), 'fragment' => 'footnote-1'));
  $link_footnote_2 = l('2', 'admin/help/pagestyle', array('attributes' => array('title' =>  t('Footnote') . ' 2'), 'fragment' => 'footnote-2'));
  switch ($path) {
    case 'admin/help#pagestyle':
     $output = '<p><img src="' . $path_pagestyle . '/images/pagestyle_list.png" alt="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Select menu') . '." class="ps_float_right" /></p>' . "\n";
      $output .= '<p>' . t('This module display a style changer on the page and in the browser menue for a better web accessibility.') . '</p>' . "\n";
      $output .= '<h3>' . t('Advantages') . '</h4>' . "\n";
      $output .= '<ol>' . "\n";
      $output .= '  <li><strong>' . t('Lesser effort.') . '</strong> ' . t('No building of extra themes required!') . ' ' . t('Black') . '/' . t('White') . ', ' . t('White') . '/' . t('Black') . ' ' . t('and') . ' ' .  t('Yellow') . '/' . t('Blue') . '.</li>' . "\n";
      $output .= '  <li><strong>' . t('Style changer in the browser available.') . '</strong></li>' . "\n";
      $output .= '  <li><strong>' . t('One click function.') . '</strong> ' . t('No tutorial required.') . '</li>' . "\n";
      $output .= '  <li>' . t('Same handling in any browser (shortcuts).') . '</li>' . "\n";
      $output .= '  <li>' . t('Adjustable.') . '</li>' . "\n";
      $output .= '  <li>' . t('Select menu availabe.') . '</li>' . "\n";
      $output .= '  <li>' . t('Display current page style.') . '</li>' . "\n";
      $output .= '  <li>' . t('Browser independent.') . '</li>' . "\n";
      $output .= '</ol>' . "\n";
      $output .= '<hr />' . "\n";
      $output .= '<h3>' . t('Features') . '</h4>' . "\n";
      $output .= '<p><img src="' . $path_pagestyle . '/images/pagestyle_select.png" alt="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Select menu') . '." class="ps_float_right" /></p>' . "\n";
      $output .= '<ul>' . "\n";
      $output .= '<li>' . t('!abbr_wcag and !abbr_bitv conform. More Information on: !link_wcag and !link_bitv.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv, '!link_wcag' => $link_wcag, '!link_bitv' => $link_bitv)) . ' ' . t('Keep the conditions:') . ' ' . "\n";
      $output .= '  <ol>' . "\n";
      $output .= '    <li>' . t('!abbr_wcag/!abbr_bitv 1.1: Short text equivalents for images.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</li>' . "\n";
      $output .= '    <li>' . t('!abbr_wcag/!abbr_bitv 3.1: Standard links with text without images in the source code.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</li>' . "\n";
      $output .= '    <li>' . t('!abbr_wcag/!abbr_bitv 6.3: No !javascript required.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv, '!javascript' => '<em lang="en">JavaScript</em>')) . '</li>' . "\n";
      $output .= '    <li>' . t('!abbr_wcag/!abbr_bitv 11.1: Open and standard code.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</li>' . "\n";
      $output .= '    <li>' . t('!abbr_wcag/!abbr_bitv 13.6: Links in a list.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</li>' . "\n";
      $output .= '  </ol>' . "\n";
      $output .= '</li>' . "\n";
      $output .= '<li>' . t('No !javascript required. !jquery suppport.', array('!javascript' => '<em lang="en">JavaScript</em>', '!jquery' => '<em lang="en">jQuery</em>')) . '</li>' . "\n";
      $output .= '<li>' . t('No !abbr_css required.', array('!abbr_css' => $abbr_css)) . '</li>' . "\n";
      $output .= '<li>' . t('Style display settings') . '.</li>' . "\n";
      $output .= '<li>' . t('Caching mode: "Normal" and "Aggressive" compatible.') . '</li>' . "\n";
      $output .= '<li>' . t('Uniform markin of menu, based on the open source browser Firefox 2.0 (!link_ff).', array('!link_ff' => $link_ff)) . '</li>' . "\n";
      $output .= '<li>' . t('Block type') . ': ' . "\n";
      $output .= '<a href="' . $path_pagestyle . '/images/pagestyle_list.png" title="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Text links') . '." >' . t('Text links') . '</a> [' . $link_footnote_1 . '], ';
      $output .= '<a href="' . $path_pagestyle . '/images/pagestyle_img.png" title="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Image links') . '." >' . t('Image links') . '</a> [' . $link_footnote_2 . '], ';
      $output .= '<a href="' . $path_pagestyle . '/images/pagestyle_select.png" title="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Select menu') . '." >' . t('Select menu') . '</a>.';
      $output .= '  </li>' . "\n";
      $output .= '<li>' . t('Themable.') . '</li>' . "\n";
      $output .= '</ul>' . "\n";
      $output .= '<p id="footnote-1">[1] ' . t('Images are showed with !abbr_css.', array('!abbr_css' => $abbr_css)) . ' <span class="ps_ok">' . t('Ideal for Web Accessibility.') . '</span></p>' . "\n";
      $output .= '<p id="footnote-2">[2] ' . t('No Link text, only images in the !abbr_xhtml source code with standard 0 pixel margin.', array('!abbr_xhtml' => $abbr_xhtml)) . ' <span class="warning">' . t('Warning') . ': ' . t('Not for: !abbr_wcag/!abbr_bitv 3.1! If JavaScript is not activated: No image change action if the cursor move over the images!', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</span></p>' . "\n";
      $output .= '<hr />' . "\n";

      // Drupal version specific
      $output .= '<h3>' . t('Requirements') . '</h4>' . "\n";
      $output .= '<ol>' . "\n";
      $output .= '  <li>Drupal 7.</li>' . "\n";
      $output .= '</ol>' . "\n";
      $output .= '<hr />' . "\n";
      $output .= '<h3>' . t('Settings') . '</h4>' . "\n";
      $output .= '<p><img src="' . $path_pagestyle . '/images/pagestyle_img.png" alt="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Image links') . '." class="ps_float_right" /></p>' . "\n";
      $output .= '<ol>' . "\n";
      $output .= '  <li>' . t('Go to:') . ' "' . t('Administer') . '", "' . t('Configuration and modules') . '", "' . t('People and permissions') . '", "' . $link_ps_perm . '" ("<em lang="en">admin/config/people/permissions#module-pagestyle</em>").' . t('Set the different roles.') . '</li>' . "\n";
      $output .= '  <li>' . t('Go to:') . ' "' . t('Administer') . '", "' . t('Structure') . '", "' . $link_block . '" ("<em lang="en">admin/structure/block</em>").' . t('Make the block "Page Style" visibile.') . '</li>' . "\n";
      $output .= '  <li>' . t('Go to:') . ' "' . t('Administer') . '", "' . t('Site configuration') . '", "' . t('Content authoring') . '", "' . $link_ps_config . '" ("<em lang="en">admin/config/content/pagestyle</em>"). ' . t('Change the settings if you want.') . '</li>' . "\n";
      $output .= '  <li>' . t('Go to:') . ' "' . t('Administer') . '", "' . t('Site building') . '", "' . t('Translate interface') . '", "' . $link_po_import . '" ("<em lang="en">admin/config/regional/translate/import</em>"). ' . t('Import your language file (for German "!de_po") if it is necessary.', array('!de_po' => '<em lang="en">' . drupal_get_path('module', 'pagestyle') . '/translations/<abbr lang="de" title="Deutsch">de</abbr>.<abbr title="PO translation file">po</abbr></em>')) . '</li>' . "\n";
      $output .= '</ol>' . "\n";
      $output .= '<hr />' . "\n";
      $output .= '<h3>' . t('Theming') . '</h3>' . "\n";
      $output .= '<h4>' . t('Create Themes') . '</h4>' . "\n";
      $output .= '<p><img src="' . $path_pagestyle . '/images/pagestyle_img2.png" alt="' . t('Screenshot of block:') . ' ' . t('Page Style') . ', ' . t('Image links, not in a list') . '" class="ps_float_right" /></p>' . "\n";
      $output .= '<ol>' . "\n";
      $output .= '  <li>' . t('Copy the files/directorys in your theme:') . "\n";
      $output .= '    <ol>' . "\n";
      $output .= '      <li lang="en">pagestyle-text.' . $abbr_tpl_php . '</li>' . "\n";
      $output .= '      <li lang="en">pagestyle-image.' . $abbr_tpl_php . '</li>' . "\n";
      $output .= '      <li lang="en">pagestyle-form.' . $abbr_tpl_php . '</li>' . "\n";
      $output .= '      <li lang="en">pagestyle.' . $abbr_css_small . '</li>' . "\n";
      $output .= '      <li lang="en">images' . '</li>' . "\n";
      $output .= '    </ol>' . "\n";
      $output .= '  </li>' . "\n";
      $output .= '  <li>' . t('Edit the !abbr_php and !abbr_css code in the files.', array('!abbr_php' => $abbr_php, '!abbr_css' => $abbr_css)) . ' ' . t('Example:') . ' ' . t('Image links, not in a list') . ': ' . t('Change the following elements:') . ' <del>&#060;ul&#062;/&#060;/ul&#062;</del> ' . t('to') . ': <ins>&#060;p&#062;/&#060;/p&#062;</ins><br /> <del>&#060;li&#062;&#060;/li&#062;</del> ' . t('to') . ': <ins>&#060;span class="display_hidden"&#062;&#160;&#124;&#160;&#060;/span&#062;</ins>.<br /><span class="info">' . t('!abbr_wcag/!abbr_bitv 10.5: Adjacent hyperlinks have to be separated by printable characters surrounded by spaces.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</span> <span class="warning">' . t('Warning') . ': ' . t('Not for: !abbr_wcag/!abbr_bitv 13.6! Links are not in a list.', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv)) . '</span></p>' . "\n";
      $output .= '  <li>' . t('Edit the images. Create your own icons. The source file is in the directory:') . ' "<em lang="en">/images/source/pagestyle.' . $abbr_xcf_small . '</em>". ' .  t('!abbr_png images are for new browsers and !abbr_gif images for old browsers (!internet_explorer 6).', array('!abbr_png' => $abbr_png, '!abbr_gif' => $abbr_gif, '!internet_explorer' => '<em lang="en">Internet Explorer</em>')) . '</li>' . "\n";
      $output .= '</ol>' . "\n";
      // Drupal version specific end

      $output .= '<h4>' . t('Download Themes') . '</h4>' . "\n";
      $output .= '<p>' . t('Edited templates are available from the developers website:') . ' ' . $link_ps . '</p>' . "\n";
      $output .= '<hr />' . "\n";
      $output .= '<h3>' . t('Screenshots') . '</h3>' . "\n";
      $output .= '<p><img src="' . $path_pagestyle . '/images/pagestyle.png" alt="' . t('Page Style') . ' ' . t('Screenshots') . '." />' . '</p>' . "\n";
      $output .= '<hr />' . "\n";
      return $output;
    break;
  }
}

/**
 * Builds and returns the pagestyle settings form.
 *
 * @ingroup forms
 * @see pagestyle_validate_form()
 */
function pagestyle_admin_settings() {
  $abbr_php = '<abbr lang="en" xml:lang="en" title="' . t('PHP: Hypertext Preprocessor') . '">PHP</abbr>';
  $abbr_xhtml = '<abbr lang="en" xml:lang="en" title="' . t('Extensible Hypertext Markup Language') . '">XHTML</abbr>';
  $abbr_css = '<abbr lang="en" xml:lang="en" title="' . t('Cascading Style Sheets') . '">CSS</abbr>';
  $abbr_wcag = '<abbr lang="en xml:lang="en" title="' . t('Web Content Accessibility Guidelines') . '">WCAG</abbr>';
  $abbr_bitv = '<abbr lang="de" xml:lang="de" title="' . t('Barrierefreie Informationstechnik-Verordnung') . '">BITV</abbr>';
  $abbr_id = '<abbr lang="en" xml:lang="en" title="' . t('Identification') . '">ID</abbr>';
  $link_footnote_1 = l('1', 'admin/config/content/pagestyle', array('attributes' => array('title' =>  t('Footnote') . ' 1'), 'fragment' => 'footnote-1'));
  $link_footnote_2 = l('2', 'admin/config/content/pagestyle', array('attributes' => array('title' =>  t('Footnote') . ' 2'), 'fragment' => 'footnote-2'));
  $link_footnote_3 = l('3', 'admin/config/content/pagestyle', array('attributes' => array('title' =>  t('Footnote') . ' 3'), 'fragment' => 'footnote-3'));
  $pagestyle_fontweight = array(
      'lighter' => t('Lighter'),
      'normal' => t('Normal'),
      'bold' => t('Bold'),
      'bolder' => t('Bolder'),
      '100' => 100 . ' (' . t('Lighter') . ')',
      '200' => 200,
      '300' => 300,
      '400' => 400 . ' (' . t('Normal') . ')',
      '500' => 500 . ' (' . t('Medium') . ')',
      '600' => 600,
      '700' => 700 . ' (' . t('Bold') . ')',
      '800' => 800,
      '900' => 900 . ' (' . t('Bolder') . ')',
    );
  $form['pagestyle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pagestyle']['pagestyle_block_title'] = array(
    '#type' => 'radios',
    '#title' => t('Block title'),
    '#options' => array(
      'page_style' => t('Page Style'),
      'theme' => t('Theme'),
    ),
    '#default_value' => variable_get('pagestyle_block_title', 'page_style'),
    '#description' => t('Select the block title.') . ' ' . t('Default') . ': "' . t('Page Style') . '".',
  );
  $form['pagestyle']['pagestyle_block_type'] = array(
    '#type' => 'radios',
    '#title' => t('Block type'),
    '#options' => array(
      'text' => t('Text links') . ' [' . $link_footnote_1 . ']',
      'image' => t('Image links') . ' [' . $link_footnote_2 . ']',
      'select' => t('Select menu'),
    ),
    '#default_value' => variable_get('pagestyle_block_type', 'text'),
    '#description' => t('Select the type of block.') . ' ' . t('Default') . ': "' . t('Text links') . '&#160;<br /><span id="footnote-1">[1] ' . t('Images are showed with !abbr_css.', array('!abbr_css' => $abbr_css)) . ' <span class="ps_ok">' . t('Ideal for Web Accessibility.') . '</span></span><br /><span id="footnote-2">[2] ' . t('Images in the !abbr_xhtml source code.', array('!abbr_xhtml' => $abbr_xhtml)) . ' <span class="warning">' . t('Warning') . ': ' . t('Not for: !abbr_wcag/!abbr_bitv 3.1! No hover action if !javascript is not activated!', array('!abbr_wcag' => $abbr_wcag, '!abbr_bitv' => $abbr_bitv, '!javascript' => '<em lang="en">JavaScript</em>')) . '</span></span>',
  );
  $form['pagestyle_adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pagestyle_adv']['pagestyle_normal'] = array(
    '#type' => 'select',
    '#title' => t('Normal Page Style'),
    '#options' => array(
      'black_white' => t('Black') . '/' . t('White'),
      'white_black' => t('White') . '/' . t('Black'),
      'yellow_blue' => t('Yellow') . '/' . t('Blue'),
      'standard' => t('Standard')
    ),
    '#default_value' => variable_get('pagestyle_normal', 'standard'),
    '#description' => t('Select the normal page style.') . ' ' . t('Default') . ': "' . t('Standard') . '".',
  );
  $form['pagestyle_adv']['pagestyle_javascript'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support !javascript', array('!javascript' => '<em lang="en">JavaScript</em>')),
    '#default_value' => variable_get('pagestyle_javascript', 1),
    '#description' => t('Support !javascript (!jquery) or only !abbr_php for caching mode: "Normal" and "Aggressive" and anonymous users.', array('!abbr_php' => $abbr_php, '!javascript' => '<em lang="en">JavaScript</em>', '!jquery' => '<em lang="en">jQuery</em>')) . ' ' . t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_adv']['pagestyle_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display message after changing page style'),
    '#default_value' => variable_get('pagestyle_message', 1),
    '#description' => t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_adv']['pagestyle_cookie_expires'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Expires'),
    '#size' => 4,
    '#maxlength' => 4,
    '#field_suffix' => t('days'),
    '#default_value' => variable_get('pagestyle_cookie_expires', 365),
    '#description' => t('Default') . ': "365" ' . t('days') . '. ' . t('Allowed values') . ': 1-3650 ' . t('days') . '.',
    '#required' => TRUE,
  );
  $form['pagestyle_adv']['pagestyle_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Domain'),
    '#size' => 40,
    '#maxlength' => 100,
    '#field_prefix' => 'http://' . $_SERVER['SERVER_NAME'],
    '#default_value' => variable_get('pagestyle_cookie_domain', base_path()),
    '#description' => t('Default') . ': "' . base_path() . '". <span class="warning">' . t('Warning') . ': ' . t('After a change, the user must store the cookie again.') . '</span>',
    '#required' => TRUE,
  );
  $form['pagestyle_theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme settings'),
    '#description' => t('Enable, disable or set the display of certain page styles and elements.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pagestyle_theme']['pagestyle_display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Toggle display'),
    '#description' => t('Enable or disable the display of certain page styles and elements.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_display_subtitle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subtitle'),
    '#default_value' => variable_get('pagestyle_display_subtitle', 0),
    '#description' => t('Ideal for image links and the select menue (title as label).') . ' ' . t('Default') . ': ' . t('Not checked.'),
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_black_white'] = array(
    '#type' => 'checkbox',
    '#title' => t('Black') . '/' . t('White'),
    '#default_value' => variable_get('pagestyle_black_white', 1),
    '#description' => t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_white_black'] = array(
    '#type' => 'checkbox',
    '#title' => t('White') . '/' . t('Black'),
    '#default_value' => variable_get('pagestyle_white_black', 1),
    '#description' => t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_yellow_blue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Yellow') . '/' . t('Blue'),
    '#default_value' => variable_get('pagestyle_yellow_blue', 1),
    '#description' => t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_standard'] = array(
    '#type' => 'checkbox',
    '#title' => t('Standard'),
    '#description' => '<span class="info">' . t('Minimum 2 styles are required.') . '</span>',
    '#default_value' => variable_get('pagestyle_standard', 1),
    '#description' => t('Default') . ': ' . t('Checked.'),
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_display_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link text'),
    '#default_value' => variable_get('pagestyle_display_links', 1),
    '#description' => t('Default') . ': ' . t('Checked.') . ' <span class="warning">' . t('Warning') . ': ' . t('Not for block type "Image links" with images in the !abbr_xhtml source code.', array('!abbr_xhtml' => $abbr_xhtml)) . '</span>',
  );
  $form['pagestyle_theme']['pagestyle_display']['pagestyle_display_current_text_value'] = array(
    '#type' => 'radios',
    '#title' => t('Display current page style text and value'),
    '#options' => array(
      'text_value' => t('Text and value') . ' (' . t('for example') . ': "' . t("Current Style") . ': ' . t('Standard') . '")',
      'value' => t('Only the value') . ' (' . t('for example') . ': "<span title="' . t("Current Style") . ': ' . t('Standard') . '" class="pagestyle_current_help">' . t('Standard') . '</span>")',
      'hidden' => t('Hide text and value'),
      'remove' => t('Remove text and value in the source code') . ' [' . $link_footnote_3 . ']',
    ),
    '#default_value' => variable_get('pagestyle_display_current_text_value', 'text_value'),
    '#description' => t('Default') . ': "' . t('Text and value') . '". <br /><span id="footnote-3" class="warning">[3] ' . t('Warning') . ': ' . t('Not for screen readers or text browsers (text is not in the !abbr_xhtml source code).', array('!abbr_xhtml' => $abbr_xhtml)) . '</span>',
  );
  $form['pagestyle_theme']['pagestyle_fontweight'] = array(
    '#type' => 'fieldset',
    '#title' => t('Font weight'),
    '#description' => t('Define the font weights for the certain page styles.') . ' <br /><span class="warning">' . t('Warning') . ': ' . t('The values 100 - 900 are not for all fonts.') . '</span>',
    '#attributes' => array('id' => 'pagestyle-fontweight'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pagestyle_theme']['pagestyle_fontweight']['pagestyle_fontweight_black_white'] = array(
    '#type' => 'select',
    '#title' => t('Black') . '/' . t('White') . ' ' . t('font weight'),
    '#options' => $pagestyle_fontweight,
    '#default_value' => variable_get('pagestyle_fontweight_black_white', 'normal'),
    '#description' => t('Default') . ': "' . t('Normal') . '".',
  );
  $form['pagestyle_theme']['pagestyle_fontweight']['pagestyle_fontweight_white_black'] = array(
    '#type' => 'select',
    '#title' => t('Black') . '/' . t('White') . ' ' . t('font weight'),
    '#options' => $pagestyle_fontweight,
    '#default_value' => variable_get('pagestyle_fontweight_white_black', 'bold'),
    '#description' => t('Default') . ': "' . t('Bold') . '". <span class="ok">' . t('Optimal') . ' "' . t('Bold') . '" ' . t('or') . ' "' .  t('Bolder') . '</span>',
  );
  $form['pagestyle_theme']['pagestyle_fontweight']['pagestyle_fontweight_yellow_blue'] = array(
    '#type' => 'select',
    '#title' => t('Black') . '/' . t('White') . ' ' . t('font weight'),
    '#options' => $pagestyle_fontweight,
    '#default_value' => variable_get('pagestyle_fontweight_yellow_blue', 'bold'),
    '#description' => t('Default') . ': "' . t('Bold') . '". <span class="ok">' . t('Optimal') . ' "' . t('Bold') . '" ' . t('or') . ' "' .  t('Bolder') . '</span>',
  );
  $form['pagestyle_theme']['pagestyle_fontweight']['pagestyle_fontweight_standard'] = array(
    '#type' => 'select',
    '#title' => t('Black') . '/' . t('White') . ' ' . t('font weight'),
    '#options' => $pagestyle_fontweight,
    '#default_value' => variable_get('pagestyle_fontweight_standard', 'normal'),
    '#description' => t('Default') . ': "' . t('Normal') . '".',
  );
  $form['pagestyle_theme']['pagestyle_inline'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display inline'),
    '#description' => t('Set the display of certain page style elements to inline or normal.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pagestyle_theme']['pagestyle_inline']['pagestyle_display_list_inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subtitle, list and select menu'),
    '#default_value' => variable_get('pagestyle_display_list_inline', 0),
    '#description' => t('Default') . ': ' . t('Not checked.'),
  );
  $form['pagestyle_theme']['pagestyle_inline']['pagestyle_display_current_inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Current page style'),
    '#default_value' => variable_get('pagestyle_display_current_inline', 0),
    '#description' => t('Default') . ': ' . t('Not checked.'),
  );
  return system_settings_form($form);
}

/**
 * Validate the pagestyle settings form.
 *
 * @see pagestyle_admin_settings()
 */
function pagestyle_admin_settings_validate($form, &$form_state) {
  $bw = $form_state['values']['pagestyle_black_white'];
  $wb = $form_state['values']['pagestyle_white_black'];
  $yb = $form_state['values']['pagestyle_yellow_blue'];
  $st = $form_state['values']['pagestyle_standard'];
  $normal = $form_state['values']['pagestyle_normal'];
  $ps_co_ex = $form_state['values']['pagestyle_cookie_expires'];
  $ps_co_do = $form_state['values']['pagestyle_cookie_domain'];
  $ps_fw_wb = $form_state['values']['pagestyle_fontweight_white_black'];
  $ps_fw_yb = $form_state['values']['pagestyle_fontweight_yellow_blue'];
  $normal_text = array(
    'black_white' => t('Black') . '/' . t('White'),
    'white_black' => t('White') . '/' . t('Black'),
    'yellow_blue' => t('Yellow') . '/' . t('Blue'),
    'standard' => t('Standard'),
  );
  if (($bw + $wb + $yb + $st) < 2) {
    form_set_error('pagestyle_standard', t('Minimum 2 styles are required.'));
  }
  if ($form_state['values']['pagestyle_' . $normal] != 1) {
    form_set_error('pagestyle_normal', t('The style %style is not available in "Style display settings".', array('%style' => $normal_text[$normal])));
  }
  // Page Style Cookie Expires
  if (!is_numeric($ps_co_ex) || $ps_co_ex != round($ps_co_ex) || $ps_co_ex{0} == "+") {
    form_set_error('pagestyle_cookie_expires', t('Please enter a integer in:') . ' "' . t('Cookie Expires') . '".');
  }
  if ($ps_co_ex < 1 || $ps_co_ex{0} == "-") {
    form_set_error('pagestyle_cookie_expires', t('The minimum is 1 day in:') . ' "' . t('Cookie Expires') . '".');
  }
  if ($ps_co_ex > 3650) {
    form_set_error('pagestyle_cookie_expires', t('The maximum is 3650 day in:') . ' "' . t('Cookie Expires') . '".');
  }
  // Page Style Cookie Domain
  if (empty($ps_co_do{0})) {
    form_set_error('pagestyle_cookie_domain', t('The cookie domain can not have a empty space (" ") as first in:') . ' "' . t('Cookie Domain') . '".');
  }
  if (!empty($ps_co_do{0})) {
    if ($ps_co_do{0} != "/") {
      form_set_error('pagestyle_cookie_domain', t('Please enter a slash "/" as first in:') . ' "' . t('Cookie Domain') . '".');
    }
    elseif ($ps_co_do{0} == "/") {
      if (!empty($ps_co_do{1})) {
        if ($ps_co_do{1} == "/") {
          form_set_error('pagestyle_cookie_domain', t('Please enter only one slash "/" as first in:') . ' "' . t('Cookie Domain') . '".');
        }
        elseif ($ps_co_do{1} != "/") {
          if ($ps_co_do != base_path()) {
            drupal_set_message(t('Warning') . ': ' . t('The cookie domain: "!ps_co_do" is specific!', array('!ps_co_do' => $ps_co_do)), "warning");
          }
          elseif ($ps_co_do != variable_get('pagestyle_cookie_domain', base_path())) {
          drupal_set_message(t('Warning') . ': ' . t('The cookie domain has been changed.') . ' ' . t('After a change, the user must store the cookie again.'), "warning");
          }
        }
      }
    }
  }
  // White/Black
  if (!is_numeric($ps_fw_wb)) {
    if ($ps_fw_wb != 'bold' && $ps_fw_wb != 'bolder') {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_wb" for the style Black/White is not "Bold" or "Bolder"!', array('!ps_fw_wb' => $ps_fw_wb)), "warning");
    }
  }
  elseif (is_numeric($ps_fw_wb)) {
    if ($ps_fw_wb < 700) {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_wb" for the style Black/White is not "Bold" or "Bolder"!', array('!ps_fw_wb' => $ps_fw_wb)) . ' ' . t('The values 100 - 900 are not for all fonts.') . ' ' . t('The value is smaller than 700.'), "warning");
    }
    elseif ($ps_fw_wb >= 700) {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_wb" for the style Black/White is not "Bold" or "Bolder"!', array('!ps_fw_wb' => $ps_fw_wb)) . ' ' . t('The values 100 - 900 are not for all fonts.') , "warning");
    }
  }
  // Yellow/Blue
  if (!is_numeric($ps_fw_yb)) {
    if ($ps_fw_yb != 'bold' && $ps_fw_yb != 'bolder') {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_yb" for the style Yellow/Blue is not "Bold" or "Bolder"!', array('!ps_fw_yb' => $ps_fw_yb)), "warning");
    }
  }
  elseif (is_numeric($ps_fw_yb)) {
    if ($ps_fw_yb < 700) {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_yb" for the style Yellow/Blue is not "Bold" or "Bolder"!', array('!ps_fw_yb' => $ps_fw_yb)) . ' ' . t('The values 100 - 900 are not for all fonts.') . ' ' . t('The value is smaller than 700.'), "warning");
    }
    elseif ($ps_fw_yb >= 700) {
      drupal_set_message(t('Warning') . ': ' . t('The font weight: "!ps_fw_yb" for the style Yellow/Blue is not "Bold" or "Bolder"!', array('!ps_fw_yb' => $ps_fw_yb)) . ' ' . t('The values 100 - 900 are not for all fonts.') , "warning");
    }
  }
}