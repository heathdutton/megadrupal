(function ($) {

/**
 * Toggle masquerade_user_field / masquerade_role_field.
 */
Drupal.behaviors.masqueradeAsRoleToggle = {
  attach: function (context, settings) {

    /*
     * Empty masquerade_role_field when editing masquerade_user_field.
     */
    $('#edit-masquerade-user-field').click(function() {
      $('#edit-masquerade-role-field').val([]);
    });
    
    /*
     * Empty masquerade_user_field when editing masquerade_role_field.
     */
    $('#edit-masquerade-role-field').click(function() {
      $('#edit-masquerade-user-field').val('');
    });

  }
};

})(jQuery);
