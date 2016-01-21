jQuery(document).ready(function() {

  // Add a click handler for the 'cancel' button when adding new user details.
  // This need not be Drupal AJAX as it's only hiding page elements
  jQuery('#edit-newuser-cancel').click(function() {
    jQuery('#edit-newuser-fieldset').slideUp();
    jQuery('#edit-adduser-add').show().removeAttr('disabled').val('Add');
    jQuery('#edit-adduser-email').removeAttr('readonly');
    return false;
  });

});
