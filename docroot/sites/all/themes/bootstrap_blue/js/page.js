/**
 * @file
 * Jquery for theme
 */
(function ($) {
  Drupal.behaviors.bootstrapblue = {
    attach: function (context, settings) {
      $('.show-search').click(function () {
        $('section#search').slideToggle("fast");
        $(this).toggleClass('active');
        $(this).parent().toggleClass('active');
      });
      var pageHeader = $('.main-inner h1.page-header');
      var pageExpand = "<div class='pull-right'><div class='btn-group'><button id='expand-contract' class='btn btn-expand' title='Expand'><i class='icon-resize-full'></i></button></div></div>";
      var mainWidth = $('section.main').width();
      if ($(window).width() > 764) {
        pageHeader.once('bootstrap-blue').prepend(pageExpand);
      }
      $('#expand-contract').toggle(function () {
        $('aside.sidebar-right').hide();
        $('section.main').css({"width": "100%"});
        $(this).addClass('btn-contract').removeClass('btn-expand');
        $('.main-inner').addClass('expand-main-content');
        $('.btn-contract i').removeClass('icon-resize-full').addClass('icon-resize-small');
      }, function () {
        $('section.main').width(mainWidth);
        $('aside.sidebar-right').show();
        $(this).addClass('btn-expand').removeClass('btn-contract');
        $('.main-inner').removeClass('expand-main-content');
        $('.btn-expand i').removeClass('icon-resize-small').addClass('icon-resize-full');
      });
    }
  }
})(jQuery)
