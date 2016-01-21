<?php

/**
 * @file
 * Administration pages and forms for Publish to Apple News component text styles.
 */

/**
 * Admin list of an article type's text styles.
 */
function publish_to_apple_news_component_text_styles_page($article_type) {
  $render = array();
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_component_text_style')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($result['publish_to_apple_news_component_text_style'])) {
    $textstyles = entity_load('publish_to_apple_news_component_text_style', array_keys($result['publish_to_apple_news_component_text_style']));
    $header = array('Text Style Name', 'Operations');
    $rows = array();
    
    foreach ($textstyles as $textstyle) {
      $edit_link = l(t('Edit'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/textstyles/' . $textstyle->tsid . '/edit');
      $delete_link = l(t('Delete'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/textstyles/' . $textstyle->tsid . '/delete');
      
      $rows[] = array(
        $textstyle->identifier,
        "$edit_link | $delete_link",
      );
    }
    
    $render['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  } else {
    $render = array(
      '#markup' => '<p>No component text styles found.</p>',
    );
  }
  
  return $render;
}

/**
 * Form for creating a Publish to Apple News component text style.
 */
function publish_to_apple_news_component_text_style_form($form, &$form_state, $article_type, $component_text_style = NULL) {
  drupal_add_css(drupal_get_path('module', 'publish_to_apple_news') . '/css/forms.css');
  
  $form_state['publish_to_apple_news_article_type'] = $article_type;
  
  // Prevent 0's from appearing in integer fields
  if (isset($component_text_style)) {
    foreach ($component_text_style as &$property) {
      if (is_numeric($property) && !$property) {
        $property = NULL;
      }
    }
  }
  
  $form_state['publish_to_apple_news_component_text_style'] = isset($component_text_style) ? $component_text_style : entity_create('publish_to_apple_news_component_text_style', array());
  
  if (!isset($form_state['entity_type'])) {
    $form_state['entity_type'] = 'publish_to_apple_news_component_text_style';
  }
  
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#description' => t('The unique identifier for this style to be referenced by components.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->identifier : '',
    '#required' => TRUE,
  );
  
  $fonts = array(
    'AcademyEngravedLetPlain',
    'AlNile-Bold',
    'AlNile',
    'AmericanTypewriter',
    'AmericanTypewriter-Bold',
    'AmericanTypewriter-Condensed',
    'AmericanTypewriter-CondensedBold',
    'AmericanTypewriter-CondensedLight',
    'AmericanTypewriter-Light',
    'AppleColorEmoji',
    'AppleSDGothicNeo-Thin',
    'AppleSDGothicNeo-Light',
    'AppleSDGothicNeo-Regular',
    'AppleSDGothicNeo-Medium',
    'AppleSDGothicNeo-SemiBold',
    'AppleSDGothicNeo-Bold',
    'AppleSDGothicNeo-Medium',
    'ArialMT',
    'Arial-BoldItalicMT',
    'Arial-BoldMT',
    'Arial-ItalicMT',
    'ArialHebrew',
    'ArialHebrew-Bold',
    'ArialHebrew-Light',
    'ArialRoundedMTBold',
    'Avenir-Black',
    'Avenir-BlackOblique',
    'Avenir-Book',
    'Avenir-BookOblique',
    'Avenir-Heavy',
    'Avenir-HeavyOblique',
    'Avenir-Light',
    'Avenir-LightOblique',
    'Avenir-Medium',
    'Avenir-MediumOblique',
    'Avenir-Oblique',
    'Avenir-Roman',
    'AvenirNext-Bold',
    'AvenirNext-BoldItalic',
    'AvenirNext-DemiBold',
    'AvenirNext-DemiBoldItalic',
    'AvenirNext-Heavy',
    'AvenirNext-HeavyItalic',
    'AvenirNext-Italic',
    'AvenirNext-Medium',
    'AvenirNext-MediumItalic',
    'AvenirNext-Regular',
    'AvenirNext-UltraLight',
    'AvenirNext-UltraLightItalic',
    'AvenirNextCondensed-Bold',
    'AvenirNextCondensed-BoldItalic',
    'AvenirNextCondensed-DemiBold',
    'AvenirNextCondensed-DemiBoldItalic',
    'AvenirNextCondensed-Heavy',
    'AvenirNextCondensed-HeavyItalic',
    'AvenirNextCondensed-Italic',
    'AvenirNextCondensed-Medium',
    'AvenirNextCondensed-MediumItalic',
    'AvenirNextCondensed-Regular',
    'AvenirNextCondensed-UltraLight',
    'AvenirNextCondensed-UltraLightItalic',
    'BanglaSangamMN',
    'BanglaSangamMN-Bold',
    'Baskerville',
    'Baskerville-Bold',
    'Baskerville-BoldItalic',
    'Baskerville-Italic',
    'Baskerville-SemiBold',
    'Baskerville-SemiBoldItalic',
    'BodoniOrnamentsITCTT',
    'BodoniSvtyTwoITCTT-Bold',
    'BodoniSvtyTwoITCTT-Book',
    'BodoniSvtyTwoITCTT-BookIta',
    'BodoniSvtyTwoOSITCTT-Bold',
    'BodoniSvtyTwoOSITCTT-Book',
    'BodoniSvtyTwoOSITCTT-BookIt',
    'BodoniSvtyTwoSCITCTT-Book',
    'BradleyHandITCTT-Bold',
    'ChalkboardSE-Bold',
    'ChalkboardSE-Light',
    'ChalkboardSE-Regular',
    'Chalkduster',
    'Cochin',
    'Cochin-Bold',
    'Cochin-BoldItalic',
    'Cochin-Italic',
    'Copperplate',
    'Copperplate-Bold',
    'Copperplate-Light',
    'Courier',
    'Courier-Bold',
    'Courier-BoldOblique',
    'Courier-Oblique',
    'CourierNewPS-BoldItalicMT',
    'CourierNewPS-BoldMT',
    'CourierNewPS-ItalicMT',
    'CourierNewPSMT',
    'DBLCDTempBlack',
    'DINAlternate-Bold',
    'DINCondensed-Bold',
    'DamascusBold',
    'Damascus',
    'DamascusLight',
    'DamascusMedium',
    'DamascusSemiBold',
    'DevanagariSangamMN',
    'DevanagariSangamMN-Bold',
    'Didot',
    'Didot-Bold',
    'Didot-Italic',
    'DiwanMishafi',
    'EuphemiaUCAS',
    'EuphemiaUCAS-Bold',
    'EuphemiaUCAS-Italic',
    'Farah',
    'Futura-CondensedExtraBold',
    'Futura-CondensedMedium',
    'Futura-Medium',
    'Futura-MediumItalic',
    'GeezaPro',
    'GeezaPro-Bold',
    'Georgia',
    'Georgia-Bold',
    'Georgia-BoldItalic',
    'Georgia-Italic',
    'GillSans',
    'GillSans-Bold',
    'GillSans-BoldItalic',
    'GillSans-Italic',
    'GillSans-Light',
    'GillSans-LightItalic',
    'GujaratiSangamMN',
    'GujaratiSangamMN-Bold',
    'GurmukhiMN',
    'GurmukhiMN-Bold',
    'STHeitiSC-Light',
    'STHeitiSC-Medium',
    'STHeitiTC-Light',
    'STHeitiTC-Medium',
    'Helvetica',
    'Helvetica-Bold',
    'Helvetica-BoldOblique',
    'Helvetica-Light',
    'Helvetica-LightOblique',
    'Helvetica-Oblique',
    'HelveticaNeue',
    'HelveticaNeue-Bold',
    'HelveticaNeue-BoldItalic',
    'HelveticaNeue-CondensedBlack',
    'HelveticaNeue-CondensedBold',
    'HelveticaNeue-Italic',
    'HelveticaNeue-Light',
    'HelveticaNeue-LightItalic',
    'HelveticaNeue-Medium',
    'HelveticaNeue-MediumItalic',
    'HelveticaNeue-UltraLight',
    'HelveticaNeue-UltraLightItalic',
    'HelveticaNeue-Thin',
    'HelveticaNeue-ThinItalic',
    'HiraKakuProN-W3',
    'HiraKakuProN-W6',
    'HiraMinProN-W3',
    'HiraMinProN-W6',
    'HoeflerText-Black',
    'HoeflerText-BlackItalic',
    'HoeflerText-Italic',
    'HoeflerText-Regular',
    'IowanOldStyle-Bold',
    'IowanOldStyle-BoldItalic',
    'IowanOldStyle-Italic',
    'IowanOldStyle-Roman',
    'Kailasa',
    'Kailasa-Bold',
    'KannadaSangamMN',
    'KannadaSangamMN-Bold',
    'KhmerSangamMN',
    'KohinoorDevanagari-Book',
    'KohinoorDevanagari-Light',
    'KohinoorDevanagari-Medium',
    'LaoSangamMN',
    'MalayalamSangamMN',
    'MalayalamSangamMN-Bold',
    'Marion-Bold',
    'Marion-Italic',
    'Marion-Regular',
    'Menlo-BoldItalic',
    'Menlo-Regular',
    'Menlo-Bold',
    'Menlo-Italic',
    'MarkerFelt-Thin',
    'MarkerFelt-Wide',
    'Noteworthy-Bold',
    'Noteworthy-Light',
    'Optima-Bold',
    'Optima-BoldItalic',
    'Optima-ExtraBlack',
    'Optima-Italic',
    'Optima-Regular',
    'OriyaSangamMN',
    'OriyaSangamMN-Bold',
    'Palatino-Bold',
    'Palatino-BoldItalic',
    'Palatino-Italic',
    'Palatino-Roman',
    'Papyrus',
    'Papyrus-Condensed',
    'PartyLetPlain',
    'SanFranciscoDisplay-Black',
    'SanFranciscoDisplay-Bold',
    'SanFranciscoDisplay-Heavy',
    'SanFranciscoDisplay-Light',
    'SanFranciscoDisplay-Medium',
    'SanFranciscoDisplay-Regular',
    'SanFranciscoDisplay-Semibold',
    'SanFranciscoDisplay-Thin',
    'SanFranciscoDisplay-Ultralight',
    'SanFranciscoRounded-Black',
    'SanFranciscoRounded-Bold',
    'SanFranciscoRounded-Heavy',
    'SanFranciscoRounded-Light',
    'SanFranciscoRounded-Medium',
    'SanFranciscoRounded-Regular',
    'SanFranciscoRounded-Semibold',
    'SanFranciscoRounded-Thin',
    'SanFranciscoRounded-Ultralight',
    'SanFranciscoText-Bold',
    'SanFranciscoText-BoldG1',
    'SanFranciscoText-BoldG2',
    'SanFranciscoText-BoldG3',
    'SanFranciscoText-BoldItalic',
    'SanFranciscoText-BoldItalicG1',
    'SanFranciscoText-BoldItalicG2',
    'SanFranciscoText-BoldItalicG3',
    'SanFranciscoText-Heavy',
    'SanFranciscoText-HeavyItalic',
    'SanFranciscoText-Light',
    'SanFranciscoText-LightItalic',
    'SanFranciscoText-Medium',
    'SanFranciscoText-MediumItalic',
    'SanFranciscoText-Regular',
    'SanFranciscoText-RegularG1',
    'SanFranciscoText-RegularG2',
    'SanFranciscoText-RegularG3',
    'SanFranciscoText-RegularItalic',
    'SanFranciscoText-RegularItalicG1',
    'SanFranciscoText-RegularItalicG2',
    'SanFranciscoText-RegularItalicG3',
    'SanFranciscoText-Semibold',
    'SanFranciscoText-SemiboldItalic',
    'SanFranciscoText-Thin',
    'SanFranciscoText-ThinItalic',
    'SavoyeLetPlain',
    'SinhalaSangamMN',
    'SinhalaSangamMN-Bold',
    'SnellRoundhand',
    'SnellRoundhand-Black',
    'SnellRoundhand-Bold',
    'Superclarendon-Regular',
    'Superclarendon-BoldItalic',
    'Superclarendon-Light',
    'Superclarendon-BlackItalic',
    'Superclarendon-Italic',
    'Superclarendon-LightItalic',
    'Superclarendon-Bold',
    'Superclarendon-Black',
    'Symbol',
    'TamilSangamMN',
    'TamilSangamMN-Bold',
    'TeluguSangamMN',
    'TeluguSangamMN-Bold',
    'Thonburi',
    'Thonburi-Bold',
    'Thonburi-Light',
    'TimesNewRomanPS-BoldItalicMT',
    'TimesNewRomanPS-BoldMT',
    'TimesNewRomanPS-ItalicMT',
    'TimesNewRomanPSMT',
    'Trebuchet-BoldItalic',
    'TrebuchetMS',
    'TrebuchetMS-Bold',
    'TrebuchetMS-Italic',
    'Verdana',
    'Verdana-Bold',
    'Verdana-BoldItalic',
    'Verdana-Italic',
    'ZapfDingbatsITC',
    'Zapfino',
  );
  
  $font_options = array(
    '' => 'Select...',
  );
  foreach ($fonts as $font) {
    $font_options[$font] = $font;
  }
  
  
  $boolean_options = array(
    0 => 'False',
    1 => 'True',
  );
  
  $form['font_name'] = array(
    '#type' => 'select',
    '#options' => $font_options,
    '#title' => t('Font name'),
    '#description' => t('The PostScript name of the font to apply.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->font_name : '',
  );
  $form['font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Font size'),
    '#description' => t('The size of the font, in points.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->font_size : '',
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['line_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Line height'),
    '#description' => t('The default line height, in points.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->line_height : '',
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#description' => t("The text color, defined as a 3- to 8-character RGBA hexadecimal string. Should include '#'."),
    '#default_value' => isset($component_text_style) ? $component_text_style->text_color : '#000',
  );
  $form['text_transform'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'uppercase' => t('Uppercase'),
      'lowercase' => t('Lowercase'),
      'capitalize' => t('Capitalize (title case)'),
    ),
    '#title' => t('Text transform'),
    '#description' => t('The transform to apply to the text.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->text_transform : 'none',
  );
  $form['underline'] = array(
    '#type' => 'select',
    '#options' => $boolean_options,
    '#title' => t('Underline'),
    '#description' => t('Whether to underline the text.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->underline : 0,
  );
  $form['strikethrough'] = array(
    '#type' => 'select',
    '#options' => $boolean_options,
    '#title' => t('Strikethrough'),
    '#description' => t('Whether to have a strike through the text.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->strikethrough : 0,
  );
  $form['background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t("The background color for text lines, defined as a 3- to 8-character RGBA hexadecimal string. Should include '#'."),
    '#default_value' => isset($component_text_style) ? $component_text_style->background_color : '',
  );
  $form['vertical_alignment'] = array(
    '#type' =>  'select',
    '#options' => array(
      'baseline' => t('Baseline'),
      'superscript' => t('Superscript'),
      'subscript' => t('Subscript'),
    ),
    '#title' => t('Vertical alignment'),
    '#description' => t('The vertical alignment of the text, allowing for superscript and subscript.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->vertical_alignment : 'baseline',
  );
  $form['tracking'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking'),
    '#description' => t('The amount of tracking for characters in text, in percentage of the font size, as a decimal. Should be between 0.0 and 1'),
    '#default_value' => isset($component_text_style) ? $component_text_style->tracking : '',
    '#element_validate' => array('element_validate_number'),
  );
  $form['text_alignment'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'left' => t('Left'),
      'center' => t('Center'),
      'right' => t('Right'),
      'justified' => t('Justified'),
    ),
    '#title' => t('Text alignment'),
    '#description' => t('Justification (horizontal alignment) style for all text within the component.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->text_alignment : 'none',
  );
  $form['hyphenation'] = array(
    '#type' => 'select',
    '#options' => $boolean_options,
    '#title' => t('Hyphenation'),
    '#description' => t('Indicates whether the text should hyphenate when necessary.'),
    '#default_value' => isset($component_text_style) ? $component_text_style->hyphenation : 0,
  );
  $form['link_style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link style'),
    '#description' => t('Text styling for all links within this component text style, if applicable.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'link_style_font_name' => array(
      '#type' => 'select',
      '#options' => $font_options,
      '#title' => t('Font name'),
      '#default_value' => isset($component_text_style) ? $component_text_style->link_style_font_name : '',
    ),
    'link_style_font_size' => array(
      '#type' => 'textfield',
      '#title' => t('Font size'),
      '#description' => t('The font size, in points.'),
      '#default_value' => isset($component_text_style) ? $component_text_style->link_style_font_size : '',
    ),
    'link_style_text_color' => array(
      '#type' => 'textfield',
      '#title' => t('Text color'),
      '#description' => t("Should include '#'."),
      '#default_value' => isset($component_text_style) ? $component_text_style->link_style_text_color : '',
    ),
    'link_style_underline' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Underline'),
      '#default_value' => isset($component_text_style) ? $component_text_style->link_style_underline : 0,
    ),
  );
  $form['dropcap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Drop cap'),
    '#description' => t('Defines a drop cap that can be used by this component text style.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'dropcap_number_of_lines' => array(
      '#type' => 'textfield',
      '#title' => t('Number of lines'),
      '#description' => t('Indicates the number of text lines this drop cap should span. Minimum is 2, maximum is 6.'),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_number_of_lines : '',
    ),
    'dropcap_number_of_characters' => array(
      '#type' => 'textfield',
      '#title' => t('Number of characters'),
      '#description' => t('Indicates the number of characters this drop cap should use. Minimum is 1, maximum is 4.'),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_number_of_characters : '',
    ),
    'dropcap_font_name' => array(
      '#type' => 'select',
      '#options' => $font_options,
      '#title' => t('Font name'),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_font_name : '',
    ),
    'dropcap_text_color' => array(
      '#type' => 'textfield',
      '#title' => t('Text color'),
      '#description' => t("Should include '#'."),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_text_color : '',
    ),
    'dropcap_background_color' => array(
      '#type' => 'textfield',
      '#title' => t('Background color'),
      '#description' => t("Should include '#'."),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_background_color : '',
    ),
    'dropcap_padding' => array(
      '#type' => 'textfield',
      '#title' => t('Padding'),
      '#description' => t('Sets the padding of the drop cap in points.'),
      '#default_value' => isset($component_text_style) ? $component_text_style->dropcap_padding : '',
    ),
  );
  
  
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  
  return $form;
}


/**
 * Submit handler for selecting component type when adding a new text style.
 */
function publish_to_apple_news_component_text_style_form_type_submit($form, &$form_state) {
  $form_state['new_component_type'] = $form_state['values']['component_type'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation for creating/editing a component text style.
 */
function publish_to_apple_news_component_text_style_form_validate($form, &$form_state) {
  // Make sure no other component text style in this article type has the same identifier
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'publish_to_apple_news_component_text_style')
    ->propertyCondition('tid', $form_state['publish_to_apple_news_article_type']->tid)
    ->propertyCondition('identifier', $form_state['values']['identifier']);
  
  if (!empty($form_state['publish_to_apple_news_component_text_style']->tsid)) {
    $query->propertyCondition('tsid', $form_state['publish_to_apple_news_component_text_style']->tsid, '!=');
  }
  
  $result = $query->execute();
  
  if (isset($result['publish_to_apple_news_component_text_style'])) {
    form_set_error('identifier', t('Another component text style in this article type has the same identifier. The identifier must be unique.'));
  }
  
  // Validate min and max allowed values for dropcap number of lines and characters
  if (!empty($form_state['values']['dropcap_number_of_lines'])) {
    $number_lines = $form_state['values']['dropcap_number_of_lines'];
    if ($number_lines < 2 || $number_lines > 6) {
      form_set_error('dropcap_number_of_lines', t('The drop cap number of lines must be between 2 and 6.'));
    }
  }
  if (!empty($form_state['values']['dropcap_number_of_characters'])) {
    $number_chars = $form_state['values']['dropcap_number_of_characters'];
    if ($number_chars < 1 || $number_chars > 4) {
      form_set_error('dropcap_number_of_characters', t('The drop cap number of characters must be between 1 and 4.'));
    }
  }
}

/**
 * Submit handler for creating/editing a component style.
 */
function publish_to_apple_news_component_text_style_form_submit(&$form, &$form_state) {  
  $component_text_style = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);
  
  // Save article type relationship
  $component_text_style->tid = $form_state['publish_to_apple_news_article_type']->tid;
  
  if ($component_text_style->save()) {
    drupal_set_message(t('Your component text style has been saved.'));
  }
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $form_state['publish_to_apple_news_component_text_style']->tid . '/textstyles';
}

/**
 * Confirm form for deleting a component text style.
 */
function publish_to_apple_news_component_text_style_delete_form($form, &$form_state, $article_type, $component_text_style) {
  $form_state['article_type'] = $article_type;
  $form_state['component_text_style'] = $component_text_style;
  
  return confirm_form(
    $form,
    t('Are you sure you want to delete this component text style?'),
    'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/textstyles',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting a component style.
 */
function publish_to_apple_news_component_text_style_delete_form_submit(&$form, &$form_state) {
  $article_type = $form_state['article_type'];
  $component_text_style = $form_state['component_text_style'];
  
  $component_text_style->delete();
  
  drupal_set_message(t('Your component text style has been deleted.'));
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/textstyles';
}



























