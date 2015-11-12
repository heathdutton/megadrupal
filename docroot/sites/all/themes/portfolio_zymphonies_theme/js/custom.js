/* --------------------------------------------- 

* Filename:     custom.js
* Version:      1.0.0 (2015-04-03)
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
  
  // Front page blocks

  var i = 1;
  
  $('.front-blocks .region').each(function() {
     $(this).addClass('frontuniqueblocks'+i++);
  });

  $('.not-front div').removeClass('zymphonies');

  $('.nav-toggle').click(function() {
    $(this).toggleClass('dropdownactive');
    $('#main-menu').toggleClass('staticmenu');
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

  $('.frontblockwrap .block >.content').addClass('zymphonies');
  $('.region-aboutme .block >.content').addClass('bounceIn');
  $('.portfolio-list').addClass('zymphonies bounceInDown');
  $('.region-keyskills .block >.content').addClass('bounceInUp');
  $('.region-education .block >.content').addClass('bounceIn');
  $('.region-contact .block >.content').addClass('bounceInUp');
  $('#post-content .content > article').addClass('zymphonies bounceInDown');

});

// Animation

wow = new WOW({
  boxClass: 'zymphonies',      
  animateClass: 'animated',
  offset: 0 
});

wow.init();