(function ($) {

Drupal.behaviors.ucAjaxCartAlt = {
  attach: function (context, settings) {

    $links = $(Drupal.settings.ucAjaxCartAlt.linkSelector).not('.uc-ajax-cart-alt-processed').addClass('uc-ajax-cart-alt-processed');
    // Store original link for when cart is empty
    $links.each(function() {
      $(this).data('uc_ajax_cart_alt_original', $(this).html());
    });

    // we refresh only one time
    $refresh = $('html').not('.uc-ajax-cart-alt-processed').addClass('uc-ajax-cart-alt-processed');
    element = $refresh[0];
    if (element) {
      var element_settings = {
        url: Drupal.settings.basePath + 'uc_ajax_cart_alt/ajax/refresh',
        event: 'ucAjaxCartAltOnLoad',
        progress: {
          type: 'none'
        },
      };
      var base = 'ucAjaxCartAltOnLoad';
      var ajax = new Drupal.ajax(base, element, element_settings);
      Drupal.ajax[base] = ajax;

      $(ajax.element).trigger('ucAjaxCartAltOnLoad');
    }
  }
}

/**
 * Custom defined Drupal AJAX command that will be called when the add to cart
 * ajax call returned no error.
 *
 * This function can be overriden on the theme if necessary.
 *
 * response.messages: Drupal status messages.
 * response.status_messages: Drupal themed status messages.
 */
Drupal.ajax.prototype.commands.ucAjaxCartAltAddItemSuccess = function(ajax, response, status) {
  if (!ajax.ucAjaxCartAltMessageElement) {
    ajax.ucAjaxCartAltMessageElement = $('<div class="uc-ajax-cart-alt-status-messages"></div>');
    $(ajax.element).before(ajax.ucAjaxCartAltMessageElement);
  }
  $(ajax.ucAjaxCartAltMessageElement).html(response.status_messages);
};

/**
 * Custom defined Drupal AJAX command that will be called when the add to cart
 * ajax call returned no error.
 * *
 * This function can be overriden on the theme if necessary.
 *
 * response.messages: Drupal status messages.
 * response.status_messages: Drupal themed status messages.
 */
Drupal.ajax.prototype.commands.ucAjaxCartAltAddItemError = function(ajax, response, status) {
  if (!ajax.ucAjaxCartAltMessageElement) {
    ajax.ucAjaxCartAltMessageElement = $('<div class="uc-ajax-cart-alt-status-messages"></div>');
    $(ajax.element).before(ajax.ucAjaxCartAltMessageElement);
  }
  $(ajax.ucAjaxCartAltMessageElement).html(response.status_messages);
};

/**
 * Custom defined Drupal AJAX command that will be called after refresh.
 *
 * This function can be overriden on the theme if necessary.
 */
Drupal.ajax.prototype.commands.ucAjaxCartAltRefresh = function(ajax, response, status) {
  if (response.empty) {
    $(response.selector).each(function() {
      $(this).html($(this).data('uc_ajax_cart_alt_original'));
    });
  }
};

/**
 * Custom defined Drupal AJAX command that will be called after cart view form is refreshed.
 *
 * This function can be overriden on the theme if necessary.
 */
Drupal.ajax.prototype.commands.ucAjaxCartAltViewForm = function(ajax, response, status) {
  // This probably will work just for garland and some themes, not all.
  // You might want to replicate this in your theme and update as necessary.
  // It will depend on how theme('status_messages') is output.
  $('#messages').remove();

  if ($("#uc-cart-view-form-table .messages").length) {
    var newScroll = $("#uc-cart-view-form-table .messages").offset().top - $("#uc-cart-view-form-table .messages").outerHeight();

    if ($('body').scrollTop() > newScroll) {
      $('body').animate({
        scrollTop :newScroll
      }, 100);
    }
  }
};

})(jQuery);
