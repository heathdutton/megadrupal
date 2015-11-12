(function($){

Drupal.behaviors.atSlider = {};
Drupal.behaviors.atSlider.attach = function (context, settings) {

    $('.entity-bean.bean-slider').each(function(){
        // Find delta, options
        var $e = $(this);
        var delta = $(this).parents('.at-slider:eq(0)').data('delta').replace(/^slider-/, '');
        var options = settings.at_slider[delta];
        var apply = function(query) {
          $e.find(query).cycle(options);
        };

        apply('.field-collection-container .field-name-slider-item .field-items');
        apply('.field-collection-container .field--name-slider-item .field__items');
    });

};

})(jQuery);
