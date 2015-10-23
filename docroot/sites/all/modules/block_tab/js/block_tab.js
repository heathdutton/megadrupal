// $Id$

(function($) {
  Drupal.behaviors.BlockTab = {
    attach: function() {
      $('.block_tab-block').hide();
      $('.block_tab-block:first', '.block_tab-blocks').show();

      $('.block_tab-title').click(function() {
        var region = $(this).parent().attr('region');
        var id  = $(this).attr('id').replace(/block_tab-title-/, '', 'gi');

        $('[region="' + region + '"] .block_tab-block').hide();
        $('[region="' + region + '"] .block_tab-title').removeClass('active');

        $('[region="' + region + '"] #block_tab-block-' + id).fadeIn('fast');
        $(this).addClass('active');
      });
      
      if (window.location.href.toString().match(/^.*(?:#tab){1}(.*)$/i)) {
        var selected_bid = window.location.href.toString().replace(/^.*(?:#tab){1}(.*)$/i, '$1');
        $('[bid="' + selected_bid + '"]').click();
      }
    }
  }
})(jQuery)
