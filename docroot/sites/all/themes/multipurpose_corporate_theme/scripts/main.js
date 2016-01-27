(function ($) {
  // main slider
  Drupal.behaviors.mainOwlCarousel = {
    attach : function(context, settings) {
      var callbacks = {
        afterInit: mainSliderInit, 
        beforeMove: mainSliderBMove, 
        afterMove: mainSliderAMove, 
        addClassActive: true
      };
      console.log(settings.owlcarousel);
      for (var carousel in settings.owlcarousel) {
        if (carousel == 'owl-carousel-page') {
          $.extend(true, settings.owlcarousel[carousel].settings, callbacks);
        }
      }

      $sliderImg = $('.view-main-slider .views-field-field-background-image');

      $sliderImg.each(function() {
        if($(this).find('img').length == 0) {
          $(this).find('.field-content').css('background', '#1d374d');
        }
      });
    }
  };

  function mainSliderInit() {
    if(window.innerWidth >= 768) {
      $('.active .main-slider-text-wrapper').addClass('animated fadeInLeft');
      $('.active .main-slider-image img').addClass('animated zoomIn');
    } else {
      $('.active .main-slider-text-wrapper').addClass('animated fadeInUp');
    }
  }

  function mainSliderBMove() {
    if(window.innerWidth >= 768) {
      $('.active .main-slider-text-wrapper').removeClass('animated fadeInLeft');
      $('.active .main-slider-image img').removeClass('animated zoomIn');
    } else {
      $('.active .main-slider-text-wrapper').removeClass('animated fadeInUp');
    }
  }

  function mainSliderAMove() {
    if(window.innerWidth >= 768) {
      $('.active .main-slider-text-wrapper').addClass('animated fadeInLeft');
      $('.active .main-slider-image img').addClass('animated zoomIn');
    } else {
      $('.active .main-slider-text-wrapper').addClass('animated fadeInUp');
    }
  }

  // mobile menu
  Drupal.behaviors.mobileMenu = {
    attach : function(context, settings) {
      $(document).click(function(event) {
        if($(event.target).is('.btn-mobile-menu')) {
            if($('#block-superfish-1').is(':visible')) {
              $('#superfish-1').find('ul').removeClass('open').css('display', '');
            }
            $('#block-superfish-1').slideToggle();
        } else if($(event.target).closest('#block-superfish-1').length) {
          //script do nothing
        } else {
          if($('#block-superfish-1').is(":visible") && window.innerWidth <= 1024) {
            $('#block-superfish-1').slideUp();
            $('#superfish-1').find('ul').removeClass('open').css('display', '');
          }
        }
      });

      $('#superfish-1 a').on('click', function(event) {
        if(window.innerWidth <= 1024) {
          var $menuItems = $(this).closest('ul').find('ul');
          var $submenu = $(this).closest('li').children('ul');

          console.log($submenu.length);
          console.log(!$submenu.hasClass('open'));
          if($submenu.length && !$submenu.hasClass('open')) {
            event.preventDefault();
            event.stopPropagation();
            $menuItems.not($submenu).removeClass('open').slideUp();
            $submenu.addClass('open');
          } else {
            $(this).trigger('mouseover');
          }
        }
        
      });

      $(window).resize(function() {
        if(window.innerWidth >= 1024) {
          $('#block-superfish-1').css('display', '');
          $('#block-superfish-1').css('overflow', '');
        }
      });
    }
  };

  // match height(pricing tables)
  Drupal.behaviors.matchHeight = {
    attach : function(context, settings) {
      $('.pricing-table').matchHeight();
    }
  };

  $( document ).ready(function() {
    if (window.location.pathname == '/portfolio') {
      $('.view-portfolio-terms .view-header a').addClass('active');
    }
    else {
      $('.view-portfolio-terms .view-header a').removeClass('active');
    }
  });

})(jQuery);