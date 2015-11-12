(function ($) {
  $(function() {
    /*  Expand / Collapse for text format in CKeditor */
    $(".field-widget-text-textarea fieldset.filter-wrapper, .field-widget-text-textarea-with-summary fieldset.filter-wrapper").after('<div class="hide-text-format">Change Text format</div>');
    $(".field-widget-text-textarea fieldset.filter-wrapper, .field-widget-text-textarea-with-summary fieldset.filter-wrapper").hide();
    $(".hide-text-format").click( function(){
        $(this).parent().find("fieldset.filter-wrapper").toggle();
    });
  });
})(jQuery);
