/**
 * Rijkshuisstijl scripts.
 */

(function ($) {

  Drupal.behaviors.nojs = {
    attach: function(context, settings) {
      $('html',context).removeClass('no-js').addClass('js');
    }
  };

  /**
   * On document load...
   */
  $(function() {

    // Sets a menu button
    $('#main-bar').prepend('<a id="menu-button" name="menu-button">' + Drupal.t('Menu') + '</a>');

    $('#menu-button').click(function() {
      $(this).toggleClass('active');
      $(this).siblings('.nav').toggle();
    });
  });

  getAdminBarHeight();

  // Checks if the screen gets resized.
  $(window).bind('load resize', function(){
    if (isDesktop()) {
      $('#main-bar .nav').show();
    };
    getAdminBarHeight();
  });

  /**
   * Checks if the current browser has desktop dimensions or not.
   */
  function isDesktop(){
    // Actual viewport size
    var window_width = $(document).width();
    // breakpoint window / desktop size
    var window_break = 980;

    return (window_width >= window_break);
  };

  /**
   * Checks if the admin menu is present and adjest the body top margin to the height of the admin menu bar.
   */
   function getAdminBarHeight(){
      var adminMenuHeight = $('#admin-menu').innerHeight()+'px';
      var styleAdminMenuHeight = 'margin-top: '+adminMenuHeight+' !important';
      $('html body.admin-menu').attr('style', styleAdminMenuHeight);
   }

}(jQuery));
