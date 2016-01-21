/**
 * Update, show, hide fields
 */

(function ($) {

Drupal.behaviors.validateconfFields = {
  attach: function (context) {
    $("#edit-obj-ref-man", context).click(function () {
      if($("#edit-obj-ref-man").val() === "TRUE"){
        $(".form-item-prefix-obj-ref-ext").addClass("price_none");
        $(".form-item-prefix-obj-ref-int").addClass("price_none");
        }
        if($("#edit-obj-ref-man").val() !== "TRUE"){
        $(".form-item-prefix-obj-ref-ext").removeClass("price_none");
        $(".form-item-prefix-obj-ref-int").removeClass("price_none");
        }
      });
    }
  }

})(jQuery);
