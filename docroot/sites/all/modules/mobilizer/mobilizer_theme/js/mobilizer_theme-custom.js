jQuery(document).ready(function($) {
  $('.nav-toggle').click(function() {
    $('#main-menu div ul:first-child').slideToggle(250);
    return false;
  });
  if( ($(window).width() > 640) || ($(document).width() > 640) ) {
      $('#main-menu li').mouseenter(function() {
        $(this).children('ul').css('display', 'none').stop(true, true).slideToggle(250).css('display', 'none').children('ul').css('display', 'none');
      });
      $('#main-menu li').mouseleave(function() {
        $(this).children('ul').stop(true, true).fadeOut(250).css('display', 'block');
      })
        } else {
    $('#main-menu li').each(function() {
      if($(this).children('ul').length)
        $(this).append('<span class="drop-down-toggle"><span class="drop-down-arrow"></span></span>');
    });
    $('.drop-down-toggle').click(function() {
      $(this).parent().children('ul').slideToggle(250);
    });
  }
 $('.menu-three-drop').click(function() {
  $('.dropdown').slideToggle("slow");
    });
    
  $('.more-link').click(function() {
    $('.hide').toggle();
    if ($('li.more-link span').html() == 'More Links') {
      $('li.more-link span.label').html('');
      $('li.more-link span.label').html('Less Links');
      var arrow_icon_src = $('li.more-link span.arrow img').attr("src");
      var arrow_icon_src1 = arrow_icon_src.replace("arrow_down.png","arrow_up.png");
      $('li.more-link span.arrow img').attr("src", arrow_icon_src1);
    } else {
      $('li.more-link span.label').html('');
      $('li.more-link span.label').html('More Links');
      var arrow_icon_src = $('li.more-link span.arrow img').attr("src");
      var arrow_icon_src1 = arrow_icon_src.replace("arrow_up.png","arrow_down.png");
      $('li.more-link span.arrow img').attr("src", arrow_icon_src1);
    }
  });
  
  $('.region-content .custom-rss-feed img').each(function(){
    $(this).css('width', '100%');
  })
});

function failureFunction(){
  var $ = jQuery;
  if ($.browser.safari || $.browser.opera) {
    return true;
  }
  else if ($.browser.msie) {
    try {
      if (new ActiveXObject("Skype.Detection")) return true;
    } catch(e) { }
  }
  else {
    if (navigator.mimeTypes["application/x-skype"] != undefined) {
      return true;
    }
    else {
      return false;
    }
  }
}

// function to display share data popup
function mobilizer_show_popup(obj){
  var obj_div = jQuery(obj).parent();
  jQuery(obj_div).find('div').show();
}

// function to hide share data popup
function share_popup_close(obj){
	jQuery(obj).parent().parent().parent().hide();
}