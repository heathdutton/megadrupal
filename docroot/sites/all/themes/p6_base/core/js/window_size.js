/**
 * @file
 *
 * Displays window size at lower right corner of the browser viewport.
 *
 * @see https://gist.github.com/3158517
 */

jQuery(document).ready(function () {
  jQuery('<div id="WINDOW_SIZE">' + jQuery(window).width() + 'x' + jQuery(window).height() + '</div>')
    .attr('unselectable', 'on')
    .css({
      'position' : 'fixed',
      'bottom' : 0,
      'right' : 0,
      'background' : '#000000',
      'color' : '#FFFFFF',
      'font' : '16px/1 sans-serif',
      'padding' : '10px',
      'z-index' : 999999,
      'cursor' : 'default',
      '-moz-user-select' : 'none',
      '-webkit-user-select' : 'none',
      'user-select' : 'none',
      '-ms-user-select' : 'none'
    })
    .appendTo('body');
});
jQuery(window).resize(function () {
  jQuery("#WINDOW_SIZE").text(jQuery(window).width() + 'x' + jQuery(window).height());
});
