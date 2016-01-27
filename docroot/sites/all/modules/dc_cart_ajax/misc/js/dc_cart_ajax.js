(function($){
  var $wrapper = null;

  Drupal.ajax = Drupal.ajax || {};
  Drupal.ajax.prototype.commands.dc_cart_ajax = function(ajax, response, status) {
    $wrapper = $wrapper || $('#' + response['form-id']).parents('.view.view-commerce-cart-form:eq(0)').parent();

    $wrapper
      .find('div.messages').remove().end()
      .prepend(response['message']);

    if (response.output != '') {
      $wrapper.find('#dc-cart-ajax-form-wrapper').html(response.output);
      return;
    }

    var fake_link = '#dc-cart-ajax-' + response['form-id'];
    $(fake_link).trigger('click');
  };

})(jQuery);