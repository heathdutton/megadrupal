/**
 * @file
 * Contains the AJAX callback function for the Data URI Creator page.
 */

/**
 * Called by the Drupal AJAX response for a successful upload with URI creation.
 */
jQuery.fn.dataUriCreatorResultComplete = function () {
  // Obtain references to the AJAX-related elements.
  var buttonElementName = Drupal.settings.dataUriCreator.ajaxButtonName;
  var buttonElementID = jQuery('[name=' + buttonElementName + ']').attr('id');
  var dataElementName = Drupal.settings.dataUriCreator.dataUriName;
  var wrapElementID = Drupal.settings.dataUriCreator.resultWrapperId;
  var imageViewName = Drupal.settings.dataUriCreator.imageButtonName;

  // Ensure that the text box is enabled to allow easy copying of the Data URI.
  jQuery('[name=' + dataElementName + ']').each(function () { this.disabled = false; });

  // Display the image-view button, if it is available for the new Data URI.
  jQuery('[name=' + imageViewName + ']').show();

  // Attach a pre-serialize handler, if not yet initialized, which will
  // be executed when additional file uploads are requested by the user.
  if (typeof Drupal.settings.dataUriCreator.initialBeforeSerialize === 'undefined') {
    Drupal.settings.dataUriCreator.initialBeforeSerialize = Drupal.ajax[buttonElementID].options.beforeSerialize;
    Drupal.ajax[buttonElementID].options.beforeSerialize = function (element, options) {
      // Hide the old results while uploading.
      jQuery('#' + wrapElementID).hide();

      // Clear and disable the text box to prevent unnecessary data uploading.
      // This helps to avoid the current Data URI from being serialized again.
      jQuery('[name=' + dataElementName + ']').val('').each(function () { this.disabled = true; });

      // Execute the original pre-serialize handler for actual serialization.
      return Drupal.settings.dataUriCreator.initialBeforeSerialize(element, options);
    };
  }
}
