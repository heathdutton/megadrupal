// $Id$

/**
 * @file
 * OM Grids Scripts
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
jQuery(document).ready(function($) {

  var column = getUrlVars()["column"]; 
  var overlay = getUrlVars()["overlay"];         
  var info = getUrlVars()["info"];         
  var screen_center = $(window).width()/2;
  var screen_height = $('body').height();
  
  // when the screen is less then 960, layout will not go negative  
  if(screen_center < 480) {
    screen_center = 480;
  }
  
  // make sure disabled is check on default
  if(column == null) {
    column = 'disabled';
  }

  // make sure disabled is check on default
  if(overlay == null) {
    overlay = 'false';
  }
  
  // make sure disabled is check on default
  if(info == null) {
    info = 'false';
  }  
    
  // getting url column value
  function getUrlVars() {
    var vars = {};
    var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) { vars[key] = value; });
    return vars;
  }
  
  // reaction to window resize
  $(window).resize(function() {  
    screen_center = $(window).width()/2;
    screen_height = $('body').height();
    $("#om-grid-guide-12").css('left', (screen_center - 480) + 'px').css('height', screen_height + 'px');
    $("#om-grid-guide-16").css('left', (screen_center - 480) + 'px').css('height', screen_height + 'px');
    $("#om-grid-guide-24").css('left', (screen_center - 480) + 'px').css('height', screen_height + 'px');
  });
  
  // reaction to button change
  $('#om-grid-guide-buttons input[type=radio]').change(function() {
    var button = $(this).val();
    //alert(button);
    if (button != '') {
      $('.om-grid-guide').remove();
      $('body').append('<div id="om-grid-guide-' + button + '" class="om-grid-guide" style="left: ' + (screen_center - 480) + 'px; height: ' + screen_height + 'px"></div>');
      $('#om-grid-guide-' + button).css('left', (screen_center - 480) + 'px').css('height', screen_height + 'px');
    }
    else {
      $('.om-grid-guide').remove();    
      button = 'disabled'; 
      overlay = 'false';
    }      
    document.location.href = '?column=' + button + '&overlay=' + overlay + '&info=' + info;      
  }); 
  
  // page refresh
  $('.om-grid-guide').remove();
  if (column != 'disabled') {
    $('body').append('<div id="om-grid-guide-' + column + '" class="om-grid-guide" style="left: ' + (screen_center - 480) + 'px; height: ' + screen_height + 'px"></div>');
    $('#om-grid-guide-' + column).css('left', (screen_center - 480) + 'px').css('height', screen_height + 'px');
  }
  $('#om-grid-guide-button-' + column).attr('checked', 'checked'); 
  
  // reaction to button change
  $('#om-grid-guide-overlay').click(function() {
		//alert($(this).attr('checked'));  
	  if(($(this).attr('checked') == true) || ($(this).attr('checked') == 'checked')) {
      $('.om-grid-guide').css('z-index', 1500);
      overlay = 'true';
    }
    else {
      $('.om-grid-guide').css('z-index', -1);
      overlay = 'false';
    }  
    document.location.href = '?column=' + column + '&overlay=' + overlay + '&info=' + info;
  }); 

  // check button state
	if(overlay == 'true') {
    $('.om-grid-guide').css('z-index', 1500);
    $('#om-grid-guide-overlay').attr('checked', 'checked');
  }
  else {
    $('.om-grid-guide').css('z-index', -1);  
  }
    
  // button block
  $('#om-grid-guide-buttons').bind('mouseenter', function(){
    $(this).stop(true);  
    $(this).css('left', 0);    
  }).bind('mouseleave', function(){
    $(this).stop(true);
    $(this).animate({left: '-110px'}, 500);      
  }); 


  // reaction to button change
  $('#om-grid-guide-info').click(function() {
		//alert($(this).attr('checked')); 
		//$(this).attr('checked').toggle(); 
	  if(($(this).attr('checked') == true) || ($(this).attr('checked') == 'checked')) {
      $('.region').addClass('region-info-active');
      $('.wrapper').addClass('wrapper-info-active'); 
      info = 'true';
    }
    else {
      $('.region').removeClass('region-info-active');
      $('.wrapper').removeClass('wrapper-info-active'); 
      info = 'false';
    }      
    document.location.href = '?column=' + column + '&overlay=' + overlay + '&info=' + info;
  }); 

  // check button state
	if(info == 'true') {
    $('.region').addClass('region-info-active');
    $('.wrapper').addClass('wrapper-info-active'); 
    $('#om-grid-guide-info').attr('checked', 'checked');
  }
  else {
    $('.region').removeClass('region-info-active');
    $('.wrapper').removeClass('wrapper-info-active');   
  }
      
  // region info
  $('.region-info-active').hover(function() {
    var region_id = '#' + $(this).attr('id');
    var region_tag = $(this).get(0).tagName;     
    var region_w  = $(this).width(); 
    var region_h  = $(this).height();    
    var region_padding_top    = parseInt($(this).css('padding-top'));
    var region_padding_right  = parseInt($(this).css('padding-right'));    
    var region_padding_bottom = parseInt($(this).css('padding-bottom'));    
    var region_padding_left   = parseInt($(this).css('padding-left'));                    
    var region_margin_top     = parseInt($(this).css('margin-top'));
    var region_margin_right   = parseInt($(this).css('margin-right'));    
    var region_margin_bottom  = parseInt($(this).css('margin-bottom'));    
    var region_margin_left    = parseInt($(this).css('margin-left'));                    
    var region_w_total        = region_w + region_padding_right + region_padding_left + region_margin_right + region_margin_left;
    var region_h_total        = region_h + region_padding_top + region_padding_bottom + region_margin_top + region_margin_bottom;
        
    var parent_id  = '#' + $(this).parent().attr('id');  
    var parent_tag = $(this).parent().get(0).tagName; 
    var parent_w   = $(this).parent().width();           
    var parent_h   = $(this).parent().height();   
    var parent_padding_top    = parseInt($(this).parent().css('padding-top'));
    var parent_padding_right  = parseInt($(this).parent().css('padding-right'));    
    var parent_padding_bottom = parseInt($(this).parent().css('padding-bottom'));    
    var parent_padding_left   = parseInt($(this).parent().css('padding-left'));                    
    var parent_margin_top     = parseInt($(this).parent().css('margin-top'));
    var parent_margin_right   = parseInt($(this).parent().css('margin-right'));    
    var parent_margin_bottom  = parseInt($(this).parent().css('margin-bottom'));    
    var parent_margin_left    = parseInt($(this).parent().css('margin-left'));  
    var parent_w_total        = parent_w + parent_padding_right + parent_padding_left + parent_margin_right + parent_margin_left;
    var parent_h_total        = parent_h + parent_padding_top + parent_padding_bottom + parent_margin_top + parent_margin_bottom;
                                
    $region_info = $('<div class="region-info"><h4>Region Info</h4></div>');
    $region_info.append('<div class="info"><span class="label">region:</span> ' + region_tag + region_id + '</div>');
    $region_info.append('<div class="info"><span class="label">width:</span> ' + region_w + 'px</div>');
    $region_info.append('<div class="info"><span class="label">total width:</span> ' + region_w_total + 'px</div>');
    $region_info.append('<div class="info"><span class="label">height:</span> ' + region_h + 'px</div>');
    $region_info.append('<div class="info"><span class="label">total height:</span> ' + region_h_total + 'px</div>');    

    $region_info.append('<div class="info"><span class="label">padding:</span> ' + region_padding_top + 'px ' + region_padding_right + 'px ' + region_padding_bottom + 'px ' + region_padding_left + 'px</div>'); 
    $region_info.append('<div class="info"><span class="label">margin:</span> ' + region_margin_top + 'px ' + region_margin_right + 'px ' + region_margin_bottom + 'px ' + region_margin_left + 'px</div>'); 

    $region_info.append('<br /><h4>Wrapper Info</h4>');            
    $region_info.append('<div class="info"><span class="label">wrapper:</span> ' + parent_tag + parent_id + '</div>');        
    $region_info.append('<div class="info"><span class="label">width:</span> ' + parent_w + 'px</div>');  
    $region_info.append('<div class="info"><span class="label">total width:</span> ' + parent_w_total + 'px</div>');          
    $region_info.append('<div class="info"><span class="label">height:</span> ' + parent_h + 'px</div>'); 
    $region_info.append('<div class="info"><span class="label">total height:</span> ' + parent_h_total + 'px</div>');                   

    $region_info.append('<div class="info"><span class="label">padding:</span> ' + parent_padding_top + 'px ' + parent_padding_right + 'px ' + parent_padding_bottom + 'px ' + parent_padding_left + 'px</div>'); 
    $region_info.append('<div class="info"><span class="label">margin:</span> ' + parent_margin_top + 'px ' + parent_margin_right + 'px ' + parent_margin_bottom + 'px ' + parent_margin_left + 'px</div>'); 

    $('body').append($region_info);   
  },
  function() {
    $('.region-info').remove();
  }); 
  
  // region info
  $('.wrapper-info-active').hover(function() {
    var parent = $(this),
        count  = parent.children().length,
        index  = parent.children('.region-info-active').index();
  
    //alert(index);
    if (index == -1) {
      var wrapper_id  = '#' + $(this).attr('id');
      var wrapper_tag = $(this).get(0).tagName;
      var wrapper_w   = $(this).width(); 
      var wrapper_h   = $(this).height();    
      var wrapper_padding_top    = parseInt($(this).css('padding-top'));
      var wrapper_padding_right  = parseInt($(this).css('padding-right'));    
      var wrapper_padding_bottom = parseInt($(this).css('padding-bottom'));    
      var wrapper_padding_left   = parseInt($(this).css('padding-left'));                    
      var wrapper_margin_top     = parseInt($(this).css('margin-top'));
      var wrapper_margin_right   = parseInt($(this).css('margin-right'));    
      var wrapper_margin_bottom  = parseInt($(this).css('margin-bottom'));    
      var wrapper_margin_left    = parseInt($(this).css('margin-left'));                    
      var wrapper_w_total        = wrapper_w + wrapper_padding_right + wrapper_padding_left + wrapper_margin_right + wrapper_margin_left;
      var wrapper_h_total        = wrapper_h + wrapper_padding_top + wrapper_padding_bottom + wrapper_margin_top + wrapper_margin_bottom;
                                
      $wrapper_info = $('<div class="wrapper-info"><h4>Wrapper Info</h4></div>');
      $wrapper_info.append('<div class="info"><span class="label">wrapper:</span> ' + wrapper_tag + wrapper_id + '</div>');
      $wrapper_info.append('<div class="info"><span class="label">width:</span> ' + wrapper_w + 'px</div>');
      $wrapper_info.append('<div class="info"><span class="label">total width:</span> ' + wrapper_w_total + 'px</div>');
      $wrapper_info.append('<div class="info"><span class="label">height:</span> ' + wrapper_h + 'px</div>');
      $wrapper_info.append('<div class="info"><span class="label">total height:</span> ' + wrapper_h_total + 'px</div>');    

      $wrapper_info.append('<div class="info"><span class="label">padding:</span> ' + wrapper_padding_top + 'px ' + wrapper_padding_right + 'px ' + wrapper_padding_bottom + 'px ' + wrapper_padding_left + 'px</div>'); 
      $wrapper_info.append('<div class="info"><span class="label">margin:</span> ' + wrapper_margin_top + 'px ' + wrapper_margin_right + 'px ' + wrapper_margin_bottom + 'px ' + wrapper_margin_left + 'px</div>'); 
    
      $('body').append($wrapper_info);   
    }  
  },
  function() {
    $('.wrapper-info').remove();
  });                 
}); 



