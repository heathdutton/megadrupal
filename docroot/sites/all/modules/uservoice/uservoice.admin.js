/**
 * @file
 * Some helper javascript for admin settings form.
 */

(function ($) {

/**
 * Update gadget urls immediately based on entered sso key.
 */
Drupal.behaviors.adminUserVoiceGadgetURLs = {
  attach: function (context) {

    // Update gadget urls immediately when sso key value is changed.
    $('input[name="uservoice_sso_key"]').change(function() {
      var sso = $(this).val();
      var profileURL;
      var orderURL;
      if (sso) {
        profileURL = Drupal.settings.userVoice.userProfileGadgetURL + '?sso=' + sso;
        orderURL = Drupal.settings.userVoice.orderHistoryGadgetURL + '?sso=' + sso;
      }
      else {
        profileURL = 'To get the gadget URL, please enter your UserVoice SSO Key at the top of this form.';
        orderURL = 'To get the gadget URL, please enter your UserVoice SSO Key at the top of this form.';
      }
      $('#user-profile-gadget-url').text(profileURL);
      $('#order-history-gadget-url').text(orderURL);
    });

    // Update gadget urls immediately on page load.
    var sso = $('input[name="uservoice_sso_key"]').val();
    var profileURL;
    var orderURL;
    if (sso) {
      profileURL = Drupal.settings.userVoice.userProfileGadgetURL + '?sso=' + sso;
      orderURL = Drupal.settings.userVoice.orderHistoryGadgetURL + '?sso=' + sso;
    }
    else {
      profileURL = 'To get the gadget URL, please enter your UserVoice SSO Key at the top of this form.';
      orderURL = 'To get the gadget URL, please enter your UserVoice SSO Key at the top of this form.';
    }
    $('#user-profile-gadget-url').text(profileURL);
    $('#order-history-gadget-url').text(orderURL);
  }
};

})(jQuery);
