/**
 * Theme custom js
 * @author Pitabas Behera
*/

(function ($) {
  
  Drupal.behaviors.boot_press = {
    attach: function (context, settings) {
      jQuery(window).scroll(function() {
        if(jQuery(this).scrollTop() != 0) {
          jQuery("#boot-press-to-top").fadeIn();	
        } else {
          jQuery("#boot-press-to-top").fadeOut();
        }
      });
      jQuery("#boot-press-to-top").click(function() {
        jQuery("body, html").animate({scrollTop:0},800);
      });
      
      jQuery('.message').find('.close').on('click', function() {
        jQuery(this).parent('.message:first').slideUp('slow');
      });
      jQuery('#main-navigation nav li').hover(function() {
        var $self = jQuery(this);
        if ($self.hasClass('dropdown')) {
          $self.addClass('open');
        }
      }, function() {
        var $self = jQuery(this);
        if ($self.hasClass ('dropdown')) {
          $self.removeClass('open');
        }
      });
      
      /*-----------------------------------------------------------------*/
      /* Header Nav Animate
      /*-----------------------------------------------------------------*/
      if (jQuery(window).width() >= '751') {
        jQuery('#main-navigation nav li').hover(function () {
          var $self = jQuery(this);
          var $menu_item = $self.find('a');
          if ($self.hasClass('dropdown')) {
            $menu_item.attr('data-toggle', '');
          }
          jQuery(this).children('ul').stop(true, true).slideDown(200);
        }, function () {
          jQuery(this).children('ul').stop(true, true).slideUp(200);
        });
      }
    
      jQuery('#main-navigation nav li ul li a').hover(function () {
        jQuery(this).stop(true, true).velocity({paddingLeft: "23px"}, 150);
      }, function () {
        jQuery(this).stop(true, true).velocity({paddingLeft: "20px"}, 150);
      });
      
    
      /*-----------------------------------------------------------------*/
      /* Toggle
      /*-----------------------------------------------------------------*/
      jQuery('.toggle-main .toggle:first-child').addClass('current').children('.toggle-content').css('display', 'block');
      jQuery('.toggle-title').click(function () {
        var parent_toggle = jQuery(this).closest('.toggle');
        if (parent_toggle.hasClass('current')) {
          parent_toggle.removeClass('current').children('.toggle-content').slideUp(300);
        } else {
          parent_toggle.addClass('current').children('.toggle-content').slideDown(300);
        }
      });
    
      /*-----------------------------------------------------------------*/
      /* Accordion
      /*-----------------------------------------------------------------*/
      jQuery('.accordion-main .accordion:first-child').addClass('current').children('.accordion-content').css('display', 'block');
      jQuery('.accordion-title').click(function () {
        var parent_accordion = jQuery(this).closest('.accordion');
        if (parent_accordion.hasClass('current')) {
          parent_accordion.removeClass('current').children('.accordion-content').slideUp(300);
        } else {
          parent_accordion.addClass('current').children('.accordion-content').slideDown(300);
        }
        parent_accordion.siblings('.accordion').removeClass('current').children('.accordion-content').slideUp(300);
      });
      
      /*-----------------------------------------------------------------------------------*/
      /* Tabs
      /*-----------------------------------------------------------------------------------*/
      var $tabsNav    = jQuery('.tabs-nav'),
      $tabsNavLis = $tabsNav.children('li');
    
      $tabsNav.each(function(){
        var $this = jQuery(this);
        $this.next().children('.tab-content').stop(true,true).hide().first().show();
        $this.children('li').first().addClass('active').stop(true,true).show();
      });
    
      $tabsNavLis.on('click', function(e) {
        var $this = jQuery(this);
        if( !$this.hasClass('active') ){
          $this.siblings().removeClass('active').end().addClass('active');
          var idx = $this.parent().children().index($this);
          $this.parent().next().children('.tab-content').stop(true,true).hide().eq(idx).fadeIn();
        }
        e.preventDefault();
      });
  
    }
  };
})(jQuery);