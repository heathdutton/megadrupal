// $Id: password_toggle.js,v 1.3.2.1 2010/08/15 10:54:01 stborchert Exp $
(function ($) {

/**
 * Add a "Show password" checkbox to each password field.
 */
Drupal.behaviors.showPassword = {
  attach: function (context) {
    // Create the checkbox.
    var showPassword = $('<label class="password-toggle"><input type="checkbox" />' + Drupal.t('Show password') + '</label>');
    // Add click handler to checkboxes.
    $(':checkbox', showPassword).click(function () {
      var orig;
      var copy;
      var wrap;
      if ($(this).is(':checked')) {
        // Copy original field and convert it to a simple textfield.
        orig = $(this).parent().parent().find(':password');
        copy = $('<input type="text" />');
      }
      else {
        // Copy original field and convert it to a password field.
        orig = $(this).parent().parent().find('.show-password');
        copy = $('<input type="password" />');
      }
      // Replace currently displayed field with the modified copy and re-assign
      // all attributes. Thanks to IE we have to go this way.
      $(copy).attr('id', $(orig).attr('id'));
      $(copy).attr('class', $(orig).attr('class'));
      $(copy).attr('size', $(orig).attr('size'));
      $(copy).attr('maxlength', $(orig).attr('maxlength'));
      $(copy).attr('name', $(orig).attr('name'));
      $(orig).replaceWith($(copy).toggleClass('show-password').val($(orig).val()));
    });
    // Add checkbox to all password field on the current page.
    showPassword.insertAfter($(':password', context));
  }
};

})(jQuery);
