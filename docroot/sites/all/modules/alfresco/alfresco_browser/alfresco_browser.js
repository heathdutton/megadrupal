(function ($) {

Drupal.behaviors.attachBrowser = {
  attach: function (context) {
    $('div.alfresco-browser-container:not(.alfresco-browser-processed)', context).each(function() {
      $(this).addClass('alfresco-browser-processed');
      var inputRef = $(this).find('input.alfresco-browser-reference');
      var inputName = $(this).find('input.alfresco-browser-name');
      var btnBrowse = $(this).find('input.alfresco-browser-button');
      if (btnBrowse) {
        btnBrowse.after(Drupal.theme('alfrescoBrowserRemoveButton', this));
      }
      var btnRemove = $(this).find('input.alfresco-browser-remove');
      var divInfo = $(this).find('div.alfresco-browser-info');
      var attached = false;

      btnBrowse.click(function() {
        window.open(
          Drupal.settings.alfrescoBrowserUrl + '?r=' + inputRef.attr('id') + '&n=' + inputName.attr('id'),
          Drupal.settings.alfrescoBrowserName,
          Drupal.settings.alfrescoBrowserFeatures
        );
        attached = true;
      });
      
      btnRemove.click(function() {
        if (attached || confirm(Drupal.t("Are you sure you want to remove '!name' from this content?\n\nThe changes will not be saved until the Save node button is clicked.", {'!name' : inputName.val()}))) {
          inputRef.val('');
          inputName.val('');
          btnRemove.hide();
          btnBrowse.removeAttr('disabled').removeClass('form-button-disabled');
          divInfo.html('<em>' + Drupal.t('Changes will not be saved until the form is submitted.') + '</em>');
        }
      });
      
      inputRef.change(function() {
        if (!inputRef.val()) {
          btnRemove.hide();
          btnBrowse.removeAttr('disabled').removeClass('form-button-disabled');
        } else {
          btnRemove.show();
          btnBrowse.attr('disabled', 'disabled').addClass('form-button-disabled');
        }
      });
      
      if (!inputRef.val()) {
        btnRemove.attr('style', 'display:none;');
      } else {
        btnBrowse.attr('disabled', 'disabled').addClass('form-button-disabled');
      }
    });
  }
};

Drupal.theme.prototype.alfrescoBrowserRemoveButton = function () {
  return '<input type="button" class="alfresco-browser-remove form-submit" value="' + Drupal.t('Remove') + '" />';
};

})(jQuery);