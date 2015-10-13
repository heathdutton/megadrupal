/* --------------------------------------------- 

* Filename:     custom.js
* Version:      1.0.0 (2015-03-08)
* Website:      http://www.zymphonies.com
                http://www.freebiezz.com
* Description:  System JS
* Author:       Zymphonies Dev Team
                info@zymphonies.com

-----------------------------------------------*/

jQuery(document).ready(function($) {
  
  $('.social-icons li').each(function(){
    var url = $(this).find('a').attr('href');
    if(url == ''){
     $(this).hide();
    }
  });

  $('.nav-toggle').click(function() {
    $('#main-menu div ul:first-child').slideToggle(250);
    return false;
  });  
  
  $('#main-menu li').each(function() {
    if($(this).children('ul').length)
      $(this).append('<span class="drop-down-toggle"><span class="drop-down-arrow"></span></span>');
  });
  $('.drop-down-toggle').click(function() {
    $(this).parent().children('ul').slideToggle(250);
  });
 
});