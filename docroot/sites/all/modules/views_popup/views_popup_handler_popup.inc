<?php

/**
 * @file
 * Definition of views_handler_field_custom.
 */

define('VIEWS_POPUP_TARGET_PREFIX', 'views-popup-target-');
define('VIEWS_POPUP_ROW','-row-');

/**
 * A handler to provide a field that is completely custom by the administrator.
 *
 * @ingroup views_field_handlers
 */
class views_popup_handler_popup extends views_handler_field { 
  public static function saved_popups($value='',$new_settings=array(), $target='', $ui_name = ''){
    static $popups = array();
    static $settings = array();
    static $type = '';
    static $name = '';
    if ($value) {
      if ( $popups ) // cannot mix fields and row or multiple row definitions
        if ( $type == VIEWS_POPUP_ROW) { // row in use, ignore fields or other rows
          if (( $target != VIEWS_POPUP_ROW ) || ( $name != $ui_name ))
            return; 
        } else if ( $target == VIEWS_POPUP_ROW) // field in use, ignore rows
          return;

      $popups[] = $value;
      $settings = $new_settings;
      $type     = $target;
      $name     = $ui_name;
    } else {      
      drupal_add_js($settings, 'setting');
      drupal_add_js(drupal_get_path('module', 'views_popup') . '/views_popup.js');      
      $custom = drupal_get_path('module', 'views_popup') . '/css/custom.css';
      if (file_exists ($custom))
      	drupal_add_css($custom);

      $result = implode('',$popups);
      $popups = array ();
      return $result;
    }
  }
  
  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['exclude']      = array('default' => TRUE);
    
    $options['popup_target'] = array('default' => '');
    $options['popup_fields'] = array('default' => array());
    $options['popup_format'] = array('default' => '');
    $options['popup_header'] = array('default' => '');
    $options['popup_footer'] = array('default' => '');
    $options['popup_empty']  = array('default' => '');
    $options['popup_class']  = array('default' => '');
    $options['popup_delay']  = array('default' => '');
    $options['popup_reset']  = array('default' => '');
    $options['popup_mode']   = array('default' => '');    
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $settings = views_popup_settings();

    // Remove excess
    foreach($form as $key => $value) {
      if (($value['#type']=='fieldset') || $value['#fieldset']){
        unset($form[$key]);
      }  
    }
    
    unset($form['exclude']);
    unset($form['custom_label']);
    unset($form['label']);   
    unset($form['element_label_colon']);
    
    $form['exclude']['#description'] = t('Enable to load this field as hidden. The contents of this field will be hidden but must be included either in a group field or as a token. It should be included only once.');
    
    $fields = array();
    $targets = array( VIEWS_POPUP_ROW => '** Target the row' );
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      if (($handler->definition['handler']!='views_popup_handler_popup')){
        $ui_name=$handler->ui_name();
        $fields[$field] = $ui_name. ( $handler->options['exclude'] ? ' **' : '' );
        if ( ! $handler->options['exclude']) {
          $targets[$field] = $ui_name;
        }
      }
    }

   
    $form['popup_info'] = array (
        '#type' => 'item',
        '#title' => t('Overview'),
        '#description' => t('This field defines the contents of a popup window and its target. It is hidden so you will not see it on a list of visible fields.'),
    );
    
    $form['popup_target'] = array (
      '#type' => 'select',
      '#title' => t('Popup target'),
      '#options' => $targets,
      '#default_value' => $this->options['popup_target'],
      '#description' => t('Field or row assocaiated with the popup window. Do not mix field and row definitions. Multiple fields can be used but only one row. '),
    );

    $form['popup_format'] = array (
      '#type' => 'select',
      '#title' => t('Field format'),
      '#options' => array ( 
          'div' => t('Unformatted list'), 
          'span' => t('Inline'), 
          'ul' => t('HTML unordered list'),
          'ol' => t('HTML ordered list'),
          'table' => t('Table'),
          ),
      '#default_value' => $this->options['popup_format'],
      '#description' => t('This format will be used to render the popup window fields.'),
    );
    
    $form['popup_fields'] = array (
      '#type' => 'checkboxes',
      '#title' => t('Popup fields'),
      '#options' => $fields,
      '#default_value' => $this->options['popup_fields'],
      '#description' => t('These fields will appear in the popup window. (** hidden fields)'),
    );

    $form['customize'] = array (
        '#type' => 'fieldset',
        '#title' => t('Customize'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
            
    $form['popup_class'] = array (
        '#type' => 'textfield',
        '#title' => t('Popup class'),
        '#default_value' => $this->options['popup_class'],
        '#description' => t('Optional CSS class.'),
        '#fieldset' => 'customize',
    );
    
    $form['popup_class_help'] = array_merge (
        array ( '#fieldset' => 'customize' ),
        views_popup_class_hint()
    ); 

    $form['popup_header'] = array (
        '#type' => 'textarea',
        '#title' => t('Popup header'),
        '#default_value' => $this->options['popup_header'],
        '#description' => t('HTML text that appears at the top of the popup window.'),
        '#fieldset' => 'customize',
    );
    
    $form['popup_footer'] = array (
        '#type' => 'textarea',
        '#title' => t('Popup footer'),
        '#default_value' => $this->options['popup_footer'],
        '#description' => t('HTML text that appears at the bottom of the popup window.'),
        '#fieldset' => 'customize',
    );
        
    $form['popup_empty'] = array (
        '#type' => 'textarea',
        '#title' => t('Popup empty'),
        '#default_value' => $this->options['popup_empty'],
        '#description' => t('HTML text to use if all fields are empty.'),
        '#fieldset' => 'customize',
    );
    
        
    $form['popup_settings'] = array (
        '#type' => 'fieldset',
        '#title' => t('Settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        );

    $form['popup_mode'] = array (
        '#type' => 'select',
        '#title' => t('Popup mode'),
        '#options' => array_merge( array ( '' => t('Default')), $settings['popup_mode_options']),
        '#default_value' => $this->options['popup_mode'],
        '#description' => t('Popup mode of operation.'),
        '#fieldset' => 'popup_settings',
    );
    
    
    $form['popup_delay'] = array (
        '#type' => 'textfield',
        '#title' => t('Popup delay'),
        '#default_value' => $this->options['popup_delay'],
        '#description' => t('Delay in msec before popup appears. Leave blank to use default (!default).', array ( '!default' => $settings['popup_delay'])),
        '#fieldset' => 'popup_settings',
    );
        
    $form['popup_reset'] = array (
        '#type' => 'textfield',
        '#title' => t('Popup reset time'),
        '#default_value' => $this->options['popup_reset'],
        '#description' => t('Delay in msec between popups. Leave blank to use default (!default).', array ( '!default' => $settings['popup_reset'])),
        '#fieldset' => 'popup_settings',
    );
  }
  
  function element_wrapper_classes ($row_index = NULL) {
    $parent=$this->options['popup_field'];
    return "views-popup-popup-window views-popup-parent-views-field-$parent ".parent::element_wrapper_classes($row_index);
  }
  
  function token_replace($text) {
    return function_exists('token_replace') ? token_replace($text) : $text;
  }
  
  /**
   * This function accumulates the popup windows using self::saved_popups.
   * views_popup_views_post_render() adds this to the end of the view
   * 
   * @param array $values
   * @return string
   */
  function render ($values) { 
    $settings = views_popup_settings();
       
    $fields = $this->view->display_handler->handlers['field'];
    $result = array();
    foreach ($this->options['popup_fields'] as $ui_name => $value) {
      if ($value) {
        $field = $fields[$ui_name];
        $label = $field->label();
        if($field->options['element_label_colon'])
          $label .= ': ';
        $result[] = array ( 'label' => $label, 'field' => $field->advanced_render($values));
      }
    } 
    
    if (!$result)  
      $result[] = array ( 'label' => '', 'field' => $this->token_replace($this->options['popup_empty']));
      
    $mode   = $this->options['popup_mode'];   
    $delay  = $this->options['popup_delay'];
    $reset  = $this->options['popup_reset'];
    $target = $this->options['popup_target'] ;
    
    if ( !$target ) 
      $target = VIEWS_POPUP_ROW; // force row default if somehow none is specified
        
    $js_settings = array(
        'views_popup'  => array( // pass the module options to the javascript that handles the popups
            'popup_delay'  => $delay ? $delay : $settings['popup_delay'],
            'reset_time'   => $reset ? $reset : $settings['popup_reset'],
            'mode'         => $mode  ? $mode  : $settings['popup_mode'],  
        ));
    
    $extra = ($mode == 'click') ? "<div class='views-popup-window-close-button'></div>" : '';
    
    $window = theme ( 
        'views_popup_'. $this->options['popup_format'],
        array(
          'class'  => "views-popup-window views-popup-views-field-$target ".$this->options['popup_class']. ' '.$settings['popup_class'],
          'header' => $extra . $this->token_replace($this->options['popup_header']),
          'footer' => $this->token_replace($this->options['popup_footer']),
          'items'  => $result,
          ));
      
    self::saved_popups ( $window, $js_settings, $target, $this->options['ui_name']);
    return "<div class='views-popup-dummy'></div>"; // not really used
  }
  
  function ui_name($short = FALSE) {
    if ($short) {
      $fields = $this->view->display_handler->handlers['field'];
      $title  = ($short && isset($this->definition['title short'])) ? $this->definition['title short'] : $this->definition['title'];
      $field  = $this->options['popup_target'];
      
      return t ('!group !title!field',
          array (
              '!group' => $this->definition['group'],
              '!title' => $title,
              '!field' => 
                  ($field == VIEWS_POPUP_ROW)
                ? (' ** '.t('Row'))
                : (   isset($fields[$field])
                    ? ' <'.$fields[$field]->ui_name(TRUE).'>'
                    : '' // sometimes seems to be missing after update,  -- '.$field. ' ' .t('deleted').' --'
                  ) 
          ));
    }
    return parent::ui_name(FALSE) ;
  }
}



