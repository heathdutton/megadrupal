/**
 * @file
 * Client side encryption of user contact forms
 */
(function ($) {
  $(document).ready( function() {
    // Enable the encrypt button
    $('input#edit-encrypt').removeAttr("disabled");
    $('input#edit-encrypt').removeClass("form-button-disabled");
    // Bind the encryption behaviour to the Encrypt and Send button
    $('input#edit-encrypt').click( function(event) {
  
      // Initialize a form variable we'll use a couple of times
      var cryptForm = $(this).parents('form');
 
      // First thing is disable the form elements while we process this
      cryptForm.find('input#edit-submit').attr('disabled', 'disabled');
      cryptForm.find('input#edit-encrypt').attr('disabled', 'disabled');
      $('textarea#edit-message').attr('disabled', 'disabled');

      // Don't submit the form yet
      event.preventDefault();

      // Use a throbber
      progressElement = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
      $('.throbber', progressElement).after('<div class="message">' + Drupal.settings.pgpContact.message + '</div>')
      cryptForm.after(progressElement);

      // Fetch the public key from Drupal
      var pgpKey = $.post(
        Drupal.settings.basePath + 'user/' + Drupal.settings.pgpContact.user + '/ajax-key', 
        { token: $("input[name='pgp_contact_token']").val() }, 
        function (data) {

		console.log(pgpKey);
		console.log(data);

        // Once we have the key, perform the encryption
        pgpKey = openpgp.key.readArmored(data.publicKey);


        keyType = -1;
        if (pgpKey.keys[0].primaryKey.algorithm.toLowerCase().substring(0,3) == "rsa") keyType = 0;
        if (pgpKey.keys[0].primaryKey.algorithm.toLowerCase().substring(0,3) == "dsa") keyType = 1;
        if (keyType == -1) throw('Invalid key type: ' + pgpKey.keys[0].primaryKey.algorithm);
        var cryptedText = openpgp.encryptMessage(pgpKey.keys, $('textarea#edit-message').val());

        // Replace the text in the textarea with the encrypted text
        $('textarea#edit-message').removeAttr('disabled').val(cryptedText);
  
        // Submit the form
        cryptForm.submit();
      });
    });
  });
})(jQuery);
