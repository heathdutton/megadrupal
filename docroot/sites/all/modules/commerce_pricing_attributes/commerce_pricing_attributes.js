(function($){

  Drupal.behaviors.commerce_pricing_attributes_widget = {
    attach: function (context, settings) {
      $('.commerce-pricing-attributes-set-details', context).once(function(){
        $('.commerce-pricing-attributes-set-details-item-show', $(this)).bind('click', function(){
          $(this).parent().siblings('.commerce-pricing-attributes-set-details-options').slideDown(300);
          $(this).hide();
          $(this).siblings('span.commerce-pricing-attributes-set-details-item-hide').show();
        });

        $('.commerce-pricing-attributes-set-details-item-hide', $(this)).bind('click', function(){
          $(this).parent().siblings('.commerce-pricing-attributes-set-details-options').slideUp(300);
          $(this).hide();
          $(this).siblings('span.commerce-pricing-attributes-set-details-item-show').show();
        });

        // This code handles the case when all attributes are enabled (checked).
        // In this case the select all/none checkbox must be set checked.
        $('.commerce-pricing-attributes-set-details-options').each(function(){
          if($(this).find('td input:checkbox:not(:checked)').length === 0){
            $(this).find('th.select-all input:checkbox').attr('checked', 'checked');
          }
        });
      });

      $('.commerce-pricing-attributes-set-details-options input.commerce-pricing-attributes-set-details-options-enabled:checkbox', context).once(function(){
        $(this).bind('click', function(){
          id = $(this).attr('id').replace('enabled', 'default');
          if($(this).is(':checked')){
            $('#'+id).removeAttr('disabled');
          }
          else{
            $('#'+id).attr('disabled', 'disabled');
          }
        });
      });
    }
  };

  Drupal.behaviors.commerce_pricing_attributes_admin_defaults = {
    attach: function (context, settings) {
      $('#commerce-pricing-attributes-default-setting-form', context).once(function(){
        if($(this).find('td input:checkbox:not(:checked)').length === 0){
          $(this).find('th.select-all input:checkbox').attr('checked', 'checked');
        }
      });
    }
  };

})(jQuery);
