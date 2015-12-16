/**
 * @file
 * Disable Field javascript functions.
 */

Drupal.behaviors.disable_field = {
    attach: function (context) {
     if (jQuery("#edit-field-add-disable-add-disable :checkbox:checked").length > 0) {
      jQuery(".form-item-field-roles-add").show();
    }
    if (jQuery("#edit-field-edit-disable-edit-disable :checkbox:checked").length > 0) {
      jQuery(".form-item-field-roles-edit").show();
    }
    jQuery("#edit-field-add-disable-add-disable").change(function () {
        if (this.checked) {
          jQuery(".form-item-field-roles-add").show();
          jQuery(".form-item-field-roles-add").css("padding-left", "30px");
        }
        else {
          jQuery(".form-item-field-roles-add").hide();
          jQuery("#edit-field-roles-add").val('');
        }
      });
    jQuery("#edit-field-edit-disable-edit-disable").change(function () {
        if (this.checked) {
          jQuery(".form-item-field-roles-edit").show();
          jQuery(".form-item-field-roles-edit").css('padding-left', '30px');
        }
        else {
          jQuery(".form-item-field-roles-edit").hide();
          jQuery("#edit-field-roles-edit").val('');
        }
    });
  }
};
