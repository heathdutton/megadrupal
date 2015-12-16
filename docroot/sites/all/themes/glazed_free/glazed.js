/**
 * @file
 * A JavaScript file that styles the page with bootstrap classes.
 *
 * @see sass/styles.scss for more info
 */

(function ($, Drupal, window, document, undefined) {

Drupal.behaviors.glazed = {
  attach: function(context, settings) {
    var windowHeight = $(window).height();
    // Bootstrap long dropdown multi-column helper
    if ($(window).width() > 768) {
        $('ul.dropdown-menu', context)
            .once('glazed').each(function() {
              var width = $(this).width();
              var columns = Math.floor($(this).find('>li').length / 15) + 1;
              if (columns > 1) {
                $(this).css({
                    'min-width' : width * columns + 2, // accounts for 1px border
                }).find('>li').css({
                    'width' : width
                })
              }
            });
    }

    // Helper classes
    $('.glazed-util-full-height', context).css('min-height', windowHeight);

    // User page
    $('.page-user .main-container', context).find('> .row > .col-sm-12')
        .once('glazed')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-2');

    // Main content layout
    $('.glazed-util-content-center-4-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed')
        .removeClass('col-sm-12')
        .addClass('col-sm-4 col-md-offset-4');

    $('.glazed-util-content-center-6-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed')
        .removeClass('col-sm-12')
        .addClass('col-sm-3 col-md-offset-3');

    $('.glazed-util-content-center-8-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-2');

    $('.glazed-util-content-center-10-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-1');

    // Breadcrumbs
    $('.breadcrumb a', context)
        .once('glazed')
        .after(' <span class="glazed-breadcrumb-spacer">/</span> ');

    // Comments
    $('#comments .links a', context)
        .once('glazed')
        .addClass('btn-sm');

    $('#comments .links .comment-reply a', context)
        .once('glazed')
        .addClass('btn-primary');

    // Sidebar nav blocks
    $('.region-sidebar-first .block .view ul, .region-sidebar-second .block .view ul', context)
        .once('glazed')
        .addClass('nav');

    // Portfolio content

    // Blog styling

    // Events styling
    $('.node-event [class^="field-event-"]', context)
        .once('glazed').each(function() {
          $(this).prev().andSelf().wrapAll('<div class="row">');
        });

    $('.node-event .field-label', context)
        .once('glazed')
        .addClass('col-sm-3');

    $('.node-event [class^="field-event-"]', context)
        .once('glazed')
        .addClass('col-sm-9');

    $('.node-event .field-event-location', context)
        .once('glazed')
        .wrapInner('<a href="https://maps.google.com/?q=' + $('.node-event .field-event-location').text() + '">');
  }
};

})(jQuery, Drupal, this, this.document);
