/**
 * @file
 * Da Vinci Custom Code of the javascript behaviour.
 */

(function ($) {
  Drupal.behaviors.da_vinciTheme = {
    attach: function (context) {
      // Checking mobile devices.
      var isMobile = {
        Android: function () {
          return navigator.userAgent.match(/Android/i);
        },
        BlackBerry: function () {
          return navigator.userAgent.match(/BlackBerry/i);
        },
        iOS: function () {
          return navigator.userAgent.match(/iPhone|iPad|iPod/i);
        },
        Opera: function () {
          return navigator.userAgent.match(/Opera Mini/i);
        },
        Windows: function () {
          return navigator.userAgent.match(/IEMobile/i);
        },
        any: function () {
          return (isMobile.Android() || isMobile.BlackBerry() || isMobile.iOS() || isMobile.Opera() || isMobile.Windows());
        }
      };
      // Mobile menu.
        slidingMenu = $('#navigation').html();
        $('body').append('<button type="button" class="js-menu-trigger sliding-menu-button"><img src="https://raw.githubusercontent.com/thoughtbot/refills/master/source/images/menu-white.png" alt="Menu Icon"></button><nav class="js-menu sliding-menu-content">' + slidingMenu + '</nav><div class="js-menu-screen menu-screen"></div>');
      $('.js-menu-trigger,.js-menu-screen', context).once('mainMenu', function () {
        $(this).click(function () {
          $('.js-menu,.js-menu-screen').toggleClass('is-visible');
        });
      });
      // Add Class Krumo-messages and remove styles for messages when dpm is active.
      if ($('.messages .krumo-root').parents('.messages').find('.container > ul > li').length > 1) {
        $('#main-content').prepend($('.messages .krumo-root').parents('li').addClass('krumo-messages'));
      } else {
        $('.messages .krumo-root').parents('.messages').removeClass('messages status').addClass('krumo-messages');
      }
      if($('.messages .container pre').length) {
        $('#main-content').prepend($('.messages .container pre').wrap('<div class="krumo-messages"></div>').closest('div.krumo-messages'));
      }
      // On click: add class 'hide' to hide message wrapper unless the user is admin.
      $('.messages').not($('.admin .messages')).click(function() {$(this).addClass('hide');});
      // Show back to top button.
      $(window).scroll(function() {
        if ($(window).scrollTop() < $(window).height() * 2) {
          $('.backtotop').removeClass('active');
        } else {
          $('.backtotop').addClass('active');
        }
      });
      // Back to top click event.
      $(".backtotop").click(function(e) {
        e.preventDefault();
        $('body').animate({
          scrollTop: $('body,html').offset().top
        }, 500);
        return false;
      });
      // Footer height.
      footerpush = function(){
        var footerHeight = $('.site-footer').outerHeight() + 50;
        $('#page #main-content').css('padding-bottom', footerHeight + 'px');
      };
      footerpush();
      $(window).resize(footerpush);
    }
  }
})(jQuery);
