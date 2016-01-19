(function($) {
  if (Drupal.ajax !== undefined) {
    Drupal.ajax.prototype.commands.node_compare_clear = function(ajax, response, status) {
      $('a.compare-toggle.remove')
        .html(response.text)
        .removeClass('remove')
        .addClass('add');
    };
  }

  Drupal.behaviors.nodeCompareBlock = {
    attach: function(context, settings) {
      // Processing of a status messages returned by ajax-request.
      messages = $('div.compare-messages-ajax');
      messages.css("left", ($(window).width() - messages.outerWidth()) / 2 + $(window).scrollLeft() + "px")
        .delay(2000)
        .fadeOut(1000);

      var itemsBlock = $('#node-compare-block-content');
      var itemsBlockContent = $('#node-compare-items');
      // Hiding empty block with nodes for comparison.
      if ($.trim(itemsBlockContent.text()) === "") {
        itemsBlock.addClass('empty');
      } else {
        itemsBlock.removeClass('empty');
      }
    }
  };
})(jQuery);
