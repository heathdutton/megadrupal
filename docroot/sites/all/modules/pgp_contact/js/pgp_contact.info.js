/**
 * @file
 * Sets up the dynamic verification on key upload pages.
 */
(function ($) {

  $(document).ready(function(){
    if (getKey() != null)
      showKeyInfo(0);
    else
      init();

    $('input#edit-verify').click( function(event) {
      event.preventDefault();
      showKeyInfo();
    });

    $('textarea#edit-public-key').change( function() {
      init();
    });
  });

  /**
   * Prepare the environment for a new key
   */
  function init() {
    $('input#edit-submit').hide();
    $('div#pgp-key-dynamic-info').show();
    $('div#pgp-key-dynamic-info').find('input').val('');
    $('input#edit-verify').show();
  }

  /**
   * Get the key
   *
   * @return An openpgp.js key object, or null
   */
  function getKey() {
    keyData = openpgp.key.readArmored($('textarea#edit-public-key').val());
    console.log(keyData);
    if (typeof keyData.err == 'undefined')
      return keyData.keys[0];
    else
      return null;
  }

  /**
   * Display the key info
   */
  function showKeyInfo(fTime) {
    keyZero = getKey();
    if (keyZero==null) {
      alert('This key is invalid'); // TODO localize this string
    }
    else {
      fTime = typeof fTime !== 'undefined' ? fTime : 400;
      fp = keyZero.primaryKey.fingerprint.match(new RegExp('.{1,4}', 'g')).join(" ");
      $('input#edit-key-info-fingerprint').val(fp.toUpperCase());
      $('input#edit-key-info-algorithm').val(keyZero.primaryKey.algorithm.toUpperCase().substring(0,3));
      $('input#edit-key-info-user').val(keyZero.getPrimaryUser().user.userId.userid);
      if ($('div#pgp-ket-dynamic-info').is(':hidden'))
        $('div#pgp-ket-dynamic-info').fadeIn(fTime);
      if ($('input#edit-submit').is(':hidden'))
        $('input#edit-submit').fadeIn(fTime);
      $('div#pgp-key-dynamic-info fieldset.collapsed legend a').click();
    }
  }

})(jQuery);
