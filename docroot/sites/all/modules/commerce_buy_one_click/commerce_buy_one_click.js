Drupal.behaviors.commerce_buy_one_click = {
  attach: function() {
    jQuery(".commerce-buy-one-click-button").each(function(index) {
      var clicked_button = jQuery(this);
      
      if (Drupal.settings.commerce_buy_one_click.commerce_buy_one_click_respect_quantity_widget) {
        var selector = Drupal.settings.commerce_buy_one_click.commerce_buy_one_click_quantity_input_selector;
        
        // Refresh value of ".commerce-buy-one-click-button-quantity" element
        // when page is loaded.
        var initail_value = parseInt(clicked_button.closest('.commerce-add-to-cart').find(selector).first().val());
        clicked_button.closest('.commerce-buy-one-click-button-wrapper').find('.commerce-buy-one-click-button-quantity').first().html(initail_value);
        
        clicked_button.closest('.commerce-add-to-cart').find(selector).bind('change', function() {
          var new_quantity = parseInt(jQuery(this).val());
          if (!isNaN(new_quantity)) {
            clicked_button.closest('.commerce-buy-one-click-button-wrapper').find('.commerce-buy-one-click-button-quantity').first().html(new_quantity.toString());
            reinit_fancybox();
          }
        });
      }
      
      function reinit_fancybox() {
        clicked_button.fancybox({
            helpers: {
              overlay: {
                locked: false
              }
            },
            href : Drupal.settings.basePath + Drupal.settings.pathPrefix + 'commerce_buy_one_click',
            type : 'ajax',
            ajax : {
                type: 'GET',
                data: {
                    product_id: clicked_button.closest('.commerce-buy-one-click-button-wrapper').find('.commerce-buy-one-click-button-product-id').first().html(),
                    add_product_to_cart: clicked_button.closest('.commerce-buy-one-click-button-wrapper').find('.commerce-buy-one-click-button-add-product-to-cart').first().html(),
                    quantity: clicked_button.closest('.commerce-buy-one-click-button-wrapper').find('.commerce-buy-one-click-button-quantity').first().html()
                }
            },
            afterShow : function() {
              Drupal.attachBehaviors('#commerce_buy_one_click_form');
              jQuery('#commerce_buy_one_click_form').ajaxForm();
            }
          }
        );
      };
      reinit_fancybox();
    });
  }
};

jQuery.fn.commerceBuyOneClickOrderCompletedCallback = function(order, name, email) {
  // Do nothing.
};
