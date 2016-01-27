/**
 * @file
 * JavaScript functions for the full record display
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
(function ($) {
  $(document).ready(function() {
    /**
     * Show all authors
     */
    $('#xc-show-hidden-authors').click(function() {
      $('.xc-hidden-author').show();
      $('#xc-show-hidden-authors').hide();
      return false;
    });
  });
}(jQuery));