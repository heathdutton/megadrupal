(function ($) {
  Drupal.behaviors.osf_field = {
    attach: function (context, settings) {
      $('.field-widget-osf-scones-field input', context).bind("refreshCheck", function() {
        
        if($(this).val().match(/^http/)) {
          $(this).trigger('refresh');
        }
      }).keydown(function () { //after something is press lets see if we need to refresh
        $(this).trigger("refreshCheck");
      }).blur(function () { // when we loose focus lets see if we should refresh
        $(this).trigger("refreshCheck");
      });
      //Add controls for selecting and clearing all checkboxes
      $(".osf-sconesfield-select-all").click(function (e) {
        e.preventDefault();
        $(this).siblings(".form-type-checkboxes").find("input").attr("checked", "checked");
        $('.field-widget-osf-scones-field .form-type-checkbox').addClass('osf_field-selected');
      });
      $(".osf-sconesfield-clear-all").click(function (e) {
        e.preventDefault();
        $(this).siblings(".form-type-checkboxes").find("input").removeAttr("checked");
        $('.field-widget-osf-scones-field .form-type-checkbox').removeClass('osf_field-selected');
      });
      $('.field-widget-osf-scones-field .form-type-checkbox input', context).click(function () {
        /*  TODO make this only be on cardnality of one
        if($(this).attr("checked")) {
          $(this).parents(".field-widget-osf-scones-field").find("input").not(this).removeAttr("checked").removeClass('osf_field-selected');
        }
        */
      });
      $('.field-widget-osf-scones-field .form-type-checkbox', context).each(function () {
        uri = $(this).find('input').val();
        rate = 1*Drupal.settings.osf_field['field-name-field-test'][uri];
        size = 1 + (rate/2);
        if(rate > 0) {
          $(this).find('label').css("font-size", size + 'em');
          $(this).css("line-height", size + 'em');
          $(this).find('input').css("top", -rate/4 + 'em');
          $(this).find('input').css("position", "relative");
          $(this).find('input').attr('title', uri);
          $(this).find('label').attr('title', uri);
        }
        else {
          $(this).addClass("no-scones");
          $(this).find('input').attr('title', uri);
          $(this).find('label').attr('title', uri);
        }
      }).click(function (e) {
        if($(this).find('input').attr('checked')) {
          $(this).addClass('osf_field-selected');
        }
        else {
          $(this).removeClass('osf_field-selected');
        }

      }).click();

      $('.ctools-use-modal').click(function() {
        $('.open-popup').removeClass('open-popup');
        $(this).addClass('open-popup');
      });

    }
  };


})(jQuery); 
